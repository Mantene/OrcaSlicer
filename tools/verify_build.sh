#!/usr/bin/env bash
# Prüft ob ein OrcaSlicer-KX Build-Artefakt die erwarteten Strings enthält.
# Verwendung:
#   ./tools/verify_build.sh linux  <path/to/OrcaSlicer.AppImage>
#   ./tools/verify_build.sh windows <path/to/OrcaSlicer-package.zip>
#
# Gibt Exit-Code 0 bei Erfolg, 1 bei Fehler zurück.

set -euo pipefail

PLATFORM="${1:-}"
ARTIFACT="${2:-}"

if [[ -z "$PLATFORM" || -z "$ARTIFACT" ]]; then
    echo "Verwendung: $0 <linux|windows> <artefakt-pfad>"
    exit 1
fi

if [[ ! -f "$ARTIFACT" ]]; then
    echo "FEHLER: Artefakt nicht gefunden: $ARTIFACT"
    exit 1
fi

# Versionstring aus version.inc lesen
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
REPO_ROOT="$(dirname "$SCRIPT_DIR")"
VERSION=$(grep 'set(SoftFever_VERSION' "$REPO_ROOT/version.inc" | head -1 | grep -oP 'SoftFever_VERSION\s+"\K[^"]+')

if [[ -z "$VERSION" ]]; then
    echo "FEHLER: Konnte VERSION nicht aus version.inc lesen"
    exit 1
fi

echo "=== OrcaSlicer-KX Build-Verifikation ==="
echo "Plattform : $PLATFORM"
echo "Artefakt  : $ARTIFACT"
echo "Erwartet  : $VERSION"
echo "========================================"

TMPDIR=$(mktemp -d)
trap "rm -rf $TMPDIR" EXIT

CHECKS=(
    "$VERSION"
    "KX-Bridge"
    "filament_sub_brands"
)

BINARY=""

if [[ "$PLATFORM" == "linux" ]]; then
    # Bevorzugt die unkomprimierte Binary aus dem build/-Baum prüfen (kein FUSE,
    # keine AppImage-Extraktion nötig). Fällt nur zurück auf --appimage-extract,
    # wenn keine lose Binary gefunden wird.
    # Echte ELF-Binary suchen, nicht die 4K-Shell-Wrapper im build/-Root.
    BUILD_ROOT="$(dirname "$ARTIFACT")"
    BINARY=""
    while IFS= read -r cand; do
        [[ -z "$cand" ]] && continue
        if file -b "$cand" 2>/dev/null | grep -q "ELF"; then
            BINARY="$cand"
            break
        fi
    done < <(find "$BUILD_ROOT" -name "orca-slicer" -type f -not -path "*/squashfs-root/*" 2>/dev/null)

    if [[ -z "$BINARY" ]]; then
        echo "Keine lose Binary gefunden — extrahiere AppImage..."
        chmod +x "$ARTIFACT"
        cd "$TMPDIR"
        if ! "$ARTIFACT" --appimage-extract bin/orca-slicer >/dev/null 2>&1; then
            "$ARTIFACT" --appimage-extract >/dev/null 2>&1 || true
        fi
        BINARY=$(find "$TMPDIR/squashfs-root" -name "orca-slicer" -type f 2>/dev/null | head -1)
    else
        echo "Prüfe lose Binary aus build/: $BINARY"
    fi

    if [[ -z "$BINARY" || ! -f "$BINARY" ]]; then
        echo "FEHLER: orca-slicer Binary weder im build/-Baum noch im AppImage gefunden"
        exit 1
    fi

elif [[ "$PLATFORM" == "windows" ]]; then
    echo "Extrahiere Windows ZIP..."
    unzip -q "$ARTIFACT" "OrcaSlicer.dll" -d "$TMPDIR" 2>/dev/null || true
    BINARY="$TMPDIR/OrcaSlicer.dll"
    if [[ ! -f "$BINARY" ]]; then
        echo "FEHLER: OrcaSlicer.dll nicht im ZIP gefunden"
        exit 1
    fi

else
    echo "FEHLER: Unbekannte Plattform '$PLATFORM' (erwartet: linux oder windows)"
    exit 1
fi

echo "Binary    : $BINARY ($(du -sh "$BINARY" | cut -f1))"
echo ""

FAILED=0
for needle in "${CHECKS[@]}"; do
    if grep -qaF "$needle" "$BINARY" 2>/dev/null; then
        echo "  [OK]  $needle"
    else
        echo "  [FAIL] $needle — NICHT GEFUNDEN"
        FAILED=1
    fi
done

echo ""
if [[ $FAILED -eq 0 ]]; then
    echo "=== VERIFIKATION ERFOLGREICH ==="
    exit 0
else
    echo "=== VERIFIKATION FEHLGESCHLAGEN — Upload abgebrochen ==="
    exit 1
fi
