#include "PluginHostBindings.hpp"

#include <libslic3r/Format/bbs_3mf.hpp>
#include <slic3r/GUI/GUI.hpp>
#include <slic3r/GUI/GUI_App.hpp>
#include <slic3r/GUI/Plater.hpp>

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include <boost/filesystem.hpp>

#include <wx/app.h>
#include <wx/defs.h>

#include <future>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <vector>

namespace py = pybind11;

namespace Slic3r {
namespace {

GUI::Plater* current_plater()
{
    if (wxTheApp == nullptr)
        throw std::runtime_error("OrcaSlicer application is not initialized");

    GUI::Plater* plater = GUI::wxGetApp().plater();
    if (plater == nullptr)
        throw std::runtime_error("Plater is not available");

    return plater;
}

// Run a (pure C++/wx) callable on the main/UI thread, blocking the caller until
// it completes, with the GIL released across the wait. If already on the main
// thread, run inline (also with the GIL released so other Python threads run).
// Mirrors the helper in PluginHostUi.cpp (both live in anonymous namespaces).
template<typename Fn>
auto run_on_ui_blocking(Fn&& fn) -> std::invoke_result_t<Fn&>
{
    using R = std::invoke_result_t<Fn&>;
    if (wxTheApp == nullptr)
        throw std::runtime_error("OrcaSlicer application is not initialized");

    if (wxIsMainThread()) {
        py::gil_scoped_release nogil;
        return fn();
    }

    std::promise<R> prom;
    std::future<R>  fut = prom.get_future();

    py::gil_scoped_release nogil;
    GUI::wxGetApp().CallAfter([&prom, &fn]() {
        try {
            if constexpr (std::is_void_v<R>) {
                fn();
                prom.set_value();
            } else {
                prom.set_value(fn());
            }
        } catch (...) {
            prom.set_exception(std::current_exception());
        }
    });
    return fut.get();
}

} // namespace

// KX addition: `orca.host.actions` — commands into the running application,
// each marshalled onto the UI thread (plugins may call these from any thread,
// e.g. a plugin-hosted local automation server). The rest of `orca.host` is
// deliberately read-only; this submodule is the deliberate exception.
namespace host_bindings {

void register_actions(py::module_& host)
{
    auto actions = host.def_submodule(
        "actions",
        "Commands into the running application (KX). Every call is marshalled "
        "onto the UI thread and blocks the calling Python thread (GIL "
        "released) until the command has been issued.");

    actions.def(
        "load_files",
        [](const std::vector<std::string>& paths) -> std::vector<size_t> {
            std::vector<boost::filesystem::path> fs_paths;
            fs_paths.reserve(paths.size());
            for (const std::string& p : paths) {
                boost::filesystem::path fp(p);
                if (!boost::filesystem::exists(fp))
                    throw std::runtime_error("load_files: no such file: " + p);
                fs_paths.emplace_back(std::move(fp));
            }
            return run_on_ui_blocking([&fs_paths]() -> std::vector<size_t> {
                return current_plater()->load_files(
                    fs_paths,
                    LoadStrategy::LoadModel | LoadStrategy::LoadConfig,
                    /*ask_multi=*/false);
            });
        },
        py::arg("paths"),
        "Load model/project files (3mf, stl, step, ...) onto the plate as the "
        "File->Open flow would, without the multi-object prompt. A .3mf is "
        "loaded WITH its embedded presets (model + config), so a project "
        "prepared elsewhere keeps its settings. Returns the loaded object "
        "indices. Raises if a path does not exist.");

    actions.def(
        "reslice",
        []() {
            run_on_ui_blocking([]() { current_plater()->reslice(); });
        },
        "Start (or restart) slicing of the current plate, exactly as the "
        "Slice button does. Returns as soon as the background job is "
        "scheduled -- it does NOT wait for slicing to finish. Poll "
        "is_slicing() for completion, or register a slicing-pipeline "
        "capability at Step.psGCodePostProcess to receive the finished "
        "G-code path.");

    actions.def(
        "is_slicing",
        []() -> bool {
            return run_on_ui_blocking(
                []() -> bool { return current_plater()->is_background_process_slicing(); });
        },
        "True while the background process is slicing. Note the scheduling "
        "gap: immediately after reslice() this may still read False for a "
        "moment before the job starts.");

    actions.def(
        "export_3mf",
        [](const std::string& output_path) -> int {
            boost::filesystem::path out(output_path);
            return run_on_ui_blocking(
                [&out]() -> int { return current_plater()->export_3mf(out); });
        },
        py::arg("path"),
        "Save the current plate as a project .3mf (model + presets) to the "
        "given path, no dialog. Returns the exporter's status code (0 on "
        "success).");
}

} // namespace host_bindings
} // namespace Slic3r
