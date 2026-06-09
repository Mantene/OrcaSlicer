# Eigene Filament-Presets anlegen, prüfen und mit KX-Bridge verknüpfen

> **Gilt für:** OrcaSlicer-KX v2.4.0-alpha-kx2 oder neuer

---

## Was ist die `filament_id` und warum ist sie wichtig?

Jedes Filament-Preset in OrcaSlicer hat eine interne `filament_id`. Diese ID wird von der KX-Bridge genutzt, um beim AMS-Sync das richtige Preset zuzuordnen.

- System-Presets (z.B. "Polymaker PolyTerra PLA") haben eine feste ID wie `GFL99` oder `OGFL04`.
- **Eigene (User-)Presets** bekommen in OrcaSlicer-KX automatisch eine eindeutige ID, die mit `P` beginnt (z.B. `P3a7f2c1`).

Ohne eindeutige ID zeigt OrcaSlicer beim Sync immer "Generic PLA" — auch wenn das Preset existiert.

> **Achtung — abgeleitete Presets (Issue #52):** Wenn du dein Preset von einem **Hersteller-Preset** ableitest (z.B. "Anycubic PLA Matte"), übernimmt es zunächst die feste Hersteller-ID (z.B. `GFA001`). Beim Sync wird dann fälschlicherweise das Hersteller-Preset statt deines eigenen ausgewählt.
> Ab OrcaSlicer-KX **v2.4.0-alpha-kx3** wird beim Speichern automatisch eine eigene `P...`-ID vergeben — auch für abgeleitete Presets. Hast du das Preset mit einer älteren Version angelegt, **öffne es einmal und speichere es erneut** (Save), damit die `P`-ID generiert wird.

---

## 1. Eigenes Filament-Preset anlegen

1. OrcaSlicer-KX starten
2. Rechts oben im **Filament-Dropdown** ein passendes Basis-Preset wählen (z.B. "Generic PLA" oder ein Hersteller-Preset)
3. Einstellungen nach Wunsch anpassen (Temperaturen, Kühlung, etc.)
4. Auf das **Speichern-Symbol** (Diskette) klicken → **"Save as new preset"**
5. Namen eingeben — z.B. `SUNLU PLA+ 2.0`
   > Der Name muss später exakt so in der Bridge eingetragen werden.
6. Drucker auswählen: **Anycubic Kobra X 0.4 nozzle** — wichtig für die Kompatibilität!
7. **Speichern** klicken
8. OrcaSlicer **einmal neu starten** — erst dann wird die `filament_id` dauerhaft gespeichert.

---

## 2. Eindeutige ID prüfen

Nach dem Neustart prüfen, ob die ID korrekt gesetzt wurde:

**Windows:**
```
%APPDATA%\OrcaSlicer\user\default\filament\SUNLU PLA+ 2.0.json
```

**Linux:**
```
~/.config/OrcaSlicer/user/default/filament/SUNLU PLA+ 2.0.json
```

Die Datei öffnen und nach `filament_id` suchen:

```json
{
  "filament_id": "P3a7f2c1",
  ...
}
```

✅ Korrekt: ID beginnt mit `P` gefolgt von 7 Hex-Zeichen  
❌ Fehlt oder leer: OrcaSlicer-KX zu alt — Update auf v2.4.0-alpha-kx2 oder neuer

---

## 3. Preset auf einen anderen PC übertragen (Import)

### Exportieren (Quell-PC)

Die Preset-Datei einfach kopieren:

**Windows:**
```
%APPDATA%\OrcaSlicer\user\default\filament\SUNLU PLA+ 2.0.json
```

**Linux:**
```
~/.config/OrcaSlicer/user/default/filament/SUNLU PLA+ 2.0.json
```

### Importieren (Ziel-PC)

**Methode A — Datei direkt kopieren:**
1. Die `.json`-Datei in das gleiche Verzeichnis auf dem Ziel-PC kopieren
2. OrcaSlicer neu starten → Preset erscheint im Dropdown

**Methode B — OrcaSlicer Import-Funktion:**
1. In OrcaSlicer: **File → Import → Import Configs...**
2. Die `.json`-Datei auswählen
3. OrcaSlicer neu starten

> **Wichtig:** Die `filament_id` in der Datei bleibt erhalten — das Preset wird auf dem Ziel-PC genauso erkannt wie auf dem Quell-PC.

---

## 4. Preset in KX-Bridge verknüpfen

1. KX-Bridge UI öffnen
2. **Filament-Verwaltung** → AMS-Slot auswählen
3. Im Feld **Filament-Name** exakt den OrcaSlicer-Preset-Namen eintragen:
   ```
   SUNLU PLA+ 2.0
   ```
4. Speichern

Die Bridge sendet beim Sync `filament_name: "SUNLU PLA+ 2.0"` → OrcaSlicer findet das Preset anhand von Name und `filament_id` → zeigt es korrekt an.

---

## Wichtige Hinweise

| Was | Warum |
|-----|-------|
| Name in OrcaSlicer und Bridge müssen **exakt** übereinstimmen | Groß-/Kleinschreibung und Sonderzeichen werden verglichen |
| Preset muss für **Anycubic Kobra X 0.4 nozzle** kompatibel sein | Beim Speichern den richtigen Drucker auswählen |
| Nach dem ersten Speichern OrcaSlicer **neu starten** | Erst dann wird die `filament_id` persistent geschrieben |
| **OrcaSlicer-KX v2.4.0-alpha-kx2** oder neuer verwenden | Ältere Versionen generieren keine eindeutige `filament_id` für User-Presets |
| Bei von Hersteller-Presets abgeleiteten Presets: **v2.4.0-alpha-kx3** oder neuer | Erst ab dieser Version wird die geerbte Hersteller-ID beim Speichern durch eine eigene `P`-ID ersetzt (Issue #52) |

---
---

# How to Create, Verify and Import Custom Filament Presets for KX-Bridge

> **Requires:** OrcaSlicer-KX v2.4.0-alpha-kx2 or newer

---

## What is the `filament_id` and why does it matter?

Every filament preset in OrcaSlicer has an internal `filament_id`. The KX-Bridge uses this ID to match the correct preset during AMS sync.

- System presets (e.g. "Polymaker PolyTerra PLA") have a fixed ID like `GFL99` or `OGFL04`.
- **Custom (user) presets** automatically receive a unique ID starting with `P` (e.g. `P3a7f2c1`) in OrcaSlicer-KX.

Without a unique ID, OrcaSlicer will always show "Generic PLA" during sync — even if the preset exists.

> **Caution — derived presets (Issue #52):** If you derive your preset from a **vendor preset** (e.g. "Anycubic PLA Matte"), it initially inherits the fixed vendor ID (e.g. `GFA001`). During sync the vendor preset is then incorrectly selected instead of your own.
> As of OrcaSlicer-KX **v2.4.0-alpha-kx3**, a unique `P...` ID is generated automatically on save — including for derived presets. If you created the preset with an older version, **open it once and save it again** so the `P` ID gets generated.

---

## 1. Create a Custom Filament Preset

1. Launch OrcaSlicer-KX
2. Select a suitable base preset from the **filament dropdown** (e.g. "Generic PLA" or a vendor preset)
3. Adjust settings as needed (temperatures, cooling, etc.)
4. Click the **save icon** (floppy disk) → **"Save as new preset"**
5. Enter a name — e.g. `SUNLU PLA+ 2.0`
   > This name must be entered in the bridge exactly as typed here.
6. Select printer: **Anycubic Kobra X 0.4 nozzle** — required for compatibility!
7. Click **Save**
8. **Restart OrcaSlicer once** — the `filament_id` is only written permanently after a restart.

---

## 2. Verify the Unique ID

After restarting, check that the ID was set correctly:

**Windows:**
```
%APPDATA%\OrcaSlicer\user\default\filament\SUNLU PLA+ 2.0.json
```

**Linux:**
```
~/.config/OrcaSlicer/user/default/filament/SUNLU PLA+ 2.0.json
```

Open the file and look for `filament_id`:

```json
{
  "filament_id": "P3a7f2c1",
  ...
}
```

✅ Correct: ID starts with `P` followed by 7 hex characters  
❌ Missing or empty: Your OrcaSlicer-KX version is too old — update to v2.4.0-alpha-kx2 or newer

---

## 3. Transfer a Preset to Another PC (Import)

### Export (source PC)

Simply copy the preset file:

**Windows:**
```
%APPDATA%\OrcaSlicer\user\default\filament\SUNLU PLA+ 2.0.json
```

**Linux:**
```
~/.config/OrcaSlicer/user/default/filament/SUNLU PLA+ 2.0.json
```

### Import (target PC)

**Method A — Copy file directly:**
1. Copy the `.json` file to the same directory on the target PC
2. Restart OrcaSlicer → preset appears in the dropdown

**Method B — OrcaSlicer import function:**
1. In OrcaSlicer: **File → Import → Import Configs...**
2. Select the `.json` file
3. Restart OrcaSlicer

> **Note:** The `filament_id` inside the file is preserved — the preset will be recognized on the target PC exactly as on the source PC.

---

## 4. Link the Preset in KX-Bridge

1. Open the KX-Bridge UI
2. Go to **Filament Management** → select the AMS slot
3. In the **Filament Name** field, enter the OrcaSlicer preset name exactly:
   ```
   SUNLU PLA+ 2.0
   ```
4. Save

The bridge sends `filament_name: "SUNLU PLA+ 2.0"` during sync → OrcaSlicer matches by name and `filament_id` → displays the preset correctly.

---

## Quick Reference

| What | Why |
|------|-----|
| Name in OrcaSlicer and Bridge must match **exactly** | Case and special characters are compared |
| Preset must be compatible with **Anycubic Kobra X 0.4 nozzle** | Select the correct printer when saving |
| **Restart OrcaSlicer** after saving for the first time | The `filament_id` is only written persistently after a restart |
| Use **OrcaSlicer-KX v2.4.0-alpha-kx2** or newer | Older versions do not generate a unique `filament_id` for user presets |
| For presets derived from vendor presets: **v2.4.0-alpha-kx3** or newer | Only from this version is the inherited vendor ID replaced with a unique `P` ID on save (Issue #52) |

---
---

# Cómo crear, verificar e importar perfiles de filamento personalizados para KX-Bridge

> **Requiere:** OrcaSlicer-KX v2.4.0-alpha-kx2 o superior

---

## ¿Qué es el `filament_id` y por qué es importante?

Cada perfil de filamento en OrcaSlicer tiene un `filament_id` interno. KX-Bridge usa este ID para asignar el perfil correcto durante la sincronización AMS.

- Los perfiles del sistema (p. ej. "Polymaker PolyTerra PLA") tienen un ID fijo como `GFL99` o `OGFL04`.
- Los **perfiles personalizados (usuario)** reciben automáticamente un ID único que empieza por `P` (p. ej. `P3a7f2c1`) en OrcaSlicer-KX.

Sin un ID único, OrcaSlicer mostrará siempre "Generic PLA" durante la sincronización, aunque el perfil exista.

> **Atención — perfiles derivados (Issue #52):** Si derivas tu perfil de un **perfil de fabricante** (p. ej. "Anycubic PLA Matte"), inicialmente hereda el ID fijo del fabricante (p. ej. `GFA001`). Durante la sincronización se selecciona entonces por error el perfil del fabricante en lugar del tuyo.
> A partir de OrcaSlicer-KX **v2.4.0-alpha-kx3**, se genera automáticamente un ID `P...` único al guardar — también para perfiles derivados. Si creaste el perfil con una versión anterior, **ábrelo una vez y vuelve a guardarlo** (Save) para que se genere el ID `P`.

---

## 1. Crear un perfil de filamento personalizado

1. Iniciar OrcaSlicer-KX
2. Seleccionar un perfil base adecuado en el **menú desplegable de filamento** (p. ej. "Generic PLA" o un perfil de fabricante)
3. Ajustar la configuración según sea necesario (temperaturas, refrigeración, etc.)
4. Hacer clic en el **icono de guardar** (disquete) → **"Save as new preset"**
5. Introducir un nombre — p. ej. `SUNLU PLA+ 2.0`
   > Este nombre debe introducirse en la bridge exactamente igual.
6. Seleccionar impresora: **Anycubic Kobra X 0.4 nozzle** — ¡necesario para la compatibilidad!
7. Hacer clic en **Guardar**
8. **Reiniciar OrcaSlicer una vez** — el `filament_id` solo se escribe de forma permanente tras un reinicio.

---

## 2. Verificar el ID único

Tras reiniciar, comprobar que el ID se ha establecido correctamente:

**Windows:**
```
%APPDATA%\OrcaSlicer\user\default\filament\SUNLU PLA+ 2.0.json
```

**Linux:**
```
~/.config/OrcaSlicer/user/default/filament/SUNLU PLA+ 2.0.json
```

Abrir el archivo y buscar `filament_id`:

```json
{
  "filament_id": "P3a7f2c1",
  ...
}
```

✅ Correcto: el ID empieza por `P` seguido de 7 caracteres hexadecimales  
❌ Falta o está vacío: la versión de OrcaSlicer-KX es demasiado antigua — actualizar a v2.4.0-alpha-kx2 o superior

---

## 3. Transferir un perfil a otro PC (importar)

### Exportar (PC de origen)

Simplemente copiar el archivo del perfil:

**Windows:**
```
%APPDATA%\OrcaSlicer\user\default\filament\SUNLU PLA+ 2.0.json
```

**Linux:**
```
~/.config/OrcaSlicer/user/default/filament/SUNLU PLA+ 2.0.json
```

### Importar (PC de destino)

**Método A — Copiar el archivo directamente:**
1. Copiar el archivo `.json` al mismo directorio en el PC de destino
2. Reiniciar OrcaSlicer → el perfil aparece en el menú desplegable

**Método B — Función de importación de OrcaSlicer:**
1. En OrcaSlicer: **File → Import → Import Configs...**
2. Seleccionar el archivo `.json`
3. Reiniciar OrcaSlicer

> **Nota:** El `filament_id` dentro del archivo se conserva — el perfil se reconocerá en el PC de destino exactamente igual que en el de origen.

---

## 4. Vincular el perfil en KX-Bridge

1. Abrir la interfaz de KX-Bridge
2. Ir a **Gestión de filamentos** → seleccionar la ranura AMS
3. En el campo **Nombre de filamento**, introducir el nombre exacto del perfil de OrcaSlicer:
   ```
   SUNLU PLA+ 2.0
   ```
4. Guardar

La bridge envía `filament_name: "SUNLU PLA+ 2.0"` durante la sincronización → OrcaSlicer busca por nombre y `filament_id` → muestra el perfil correctamente.

---

## Referencia rápida

| Qué | Por qué |
|-----|---------|
| El nombre en OrcaSlicer y en Bridge debe coincidir **exactamente** | Se comparan mayúsculas, minúsculas y caracteres especiales |
| El perfil debe ser compatible con **Anycubic Kobra X 0.4 nozzle** | Seleccionar la impresora correcta al guardar |
| **Reiniciar OrcaSlicer** tras guardar por primera vez | El `filament_id` solo se escribe de forma permanente tras un reinicio |
| Usar **OrcaSlicer-KX v2.4.0-alpha-kx2** o superior | Las versiones anteriores no generan un `filament_id` único para perfiles de usuario |
| Para perfiles derivados de perfiles de fabricante: **v2.4.0-alpha-kx3** o superior | Solo a partir de esta versión se reemplaza el ID heredado del fabricante por un ID `P` único al guardar (Issue #52) |
