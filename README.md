# OrcaSlicer-KX

**OrcaSlicer mit KX-Bridge-Patches für den Anycubic Kobra X**

Fertige Binaries von [OrcaSlicer](https://github.com/SoftFever/OrcaSlicer) mit integrierten KX-spezifischen Erweiterungen — empfohlen für den Einsatz mit [KX-Bridge](https://gitea.it-drui.de/viewit/KX-Bridge-Release).

[![Download](https://img.shields.io/badge/Download-Releases-2EA043?style=for-the-badge&logo=gitea&logoColor=white)](https://gitea.it-drui.de/viewit/OrcaSlicer-KX/releases)

---

## Download

Aktuelle Version: **2.4.0-kx1** (Basis: OrcaSlicer 2.4.0 stable)

| Plattform | Datei | Hinweis |
|-----------|-------|---------|
| Linux x86_64 | `OrcaSlicer-KX-Linux-x86_64.AppImage` | `chmod +x` setzen, dann starten |
| Windows x86_64 | `OrcaSlicer-KX-Windows-x86_64.zip` | Entpacken, `orca-slicer.exe` starten |

macOS wird nicht bereitgestellt — bitte aus dem Upstream selbst bauen.

---

## Enthaltene Patches

- **Moonraker-Bridge:** mehrstufiges Filament-Matching (Name, Sub-Brand, Vendor-Fallback) mit Vendor-Filter-Skip
- **AMS-Leerslot-Fix:** leere AMS-Slots werden korrekt grau dargestellt (kein Absturz / falsches Filament)
- **Filament-ID:** eindeutige `filament_id` für abgeleitete User-Presets (korrekte Bridge-Synchronisation)
- **Kobra X Profile:** Drucker- und G-Code-Profile für den Anycubic Kobra X

---

## Filament-Presets & KX-Bridge

📄 [Anleitung: eigene Filament-Presets erstellen, prüfen und importieren](https://gitea.it-drui.de/viewit/KX-Bridge-Release/src/branch/master/docs/filament-preset-bridge-guide.md)

---

## Branch-Struktur

| Branch | Version | Rolle |
|--------|---------|-------|
| `stable` / `kx-v2.4` | 2.4.0-kx1 | **aktuell stabil** |
| `kx-v2.3` | 2.3.2-kx4 | eingefroren (Altstand) |

---

## Warum ein eigenes Repo?

KX-Bridge selbst liegt unter [viewit/KX-Bridge-Release](https://gitea.it-drui.de/viewit/KX-Bridge-Release).
OrcaSlicer steht unter AGPL-3.0 — ein separates Repo hält Lizenzen, Quellherkunft und Update-Zyklen sauber getrennt.

---

## Lizenz & Quelle

Diese Builds sind Ableitungen von [OrcaSlicer](https://github.com/SoftFever/OrcaSlicer), lizenziert unter **GNU AGPL-3.0**.
Der KX-Patch-Quellcode liegt auf Branch [`kx-v2.4`](https://gitea.it-drui.de/viewit/OrcaSlicer-KX/src/branch/kx-v2.4) dieses Repos.
