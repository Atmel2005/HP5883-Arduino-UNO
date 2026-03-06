# GY-271 mit HP5883 / QMC5883P (Arduino UNO)

## Welches Modul?

- Board: **GY-271**
- Tatsächlicher Chip: **HP5883 / QMC5883P**
- I2C-Adresse: **0x2C**
- **Nicht** registerkompatibel mit klassischem **HMC5883L** und vielen Standardbibliotheken dafür.
- Im Projekt wird der Chip **direkt über I2C (Wire)** angesteuert, ohne spezielle Kompass-Bibliothek.

## Zielplattform

- Sketch ist für **Arduino UNO** geschrieben und getestet.
- I2C-Pins am UNO:
  - SDA → A4
  - SCL → A5

## Verdrahtung GY-271 ↔ Arduino UNO

- `VCC` → **5V**
- `GND` → **GND**
- `SDA` → **A4**
- `SCL` → **A5**

## Was macht der Sketch?

- Initialisiert den HP5883/QMC5883P:
  - kontinuierlicher Messmodus,
  - Datenrate ca. 200 Hz,
  - Messbereich ±8G.
- Im `loop()`:
  - werden **Rohdaten X/Y/Z** direkt aus den Registern des Chips gelesen,
  - von diesen Werten werden Kalibrier-Offsets `offsetX`, `offsetY`, `offsetZ` abgezogen,
  - aus den korrigierten X/Y wird der **Kompasswinkel (Heading)** berechnet,
  - alle Informationen werden im Serial Monitor ausgegeben:
    - Rohwerte,
    - korrigierte Werte,
    - empfohlene neue Offsets,
    - Heading in Grad.

## Verwendung

1. Sketch `HMC5883L.ino` auf den Arduino UNO laden.
2. GY-271 wie oben beschrieben anschließen.
3. **Serial Monitor** öffnen mit:
   - Baudrate **9600**.
4. Im Monitor erscheinen Zeilen ähnlich:

   ```
   RAW X: <rawX>  Y: <rawY>  Z: <rawZ>  | X: <X>  Y: <Y>  Z: <Z>  | RECOMMENDED new offsetX: <rawX> offsetY: <rawY> offsetZ: <rawZ>  Heading: <Winkel> deg
   ```

   Bedeutung:
   - `RAW X/Y/Z` – echte Rohdaten vom Sensor,
   - `X/Y/Z` – bereits **um die aktuellen Offsets `offsetX/Y/Z` korrigiert** (siehe Sketch-Kopf),
   - `RECOMMENDED new offsetX/Y/Z` – Rohdaten für diese Lage der Platine; diese Werte können als **neue Kalibrieroffsets** übernommen werden,
   - `Heading` – berechneter Kompasswinkel basierend auf den korrigierten X/Y.

## Kalibrierung

1. Im Kopf des Sketches stehen:

   ```cpp
   int16_t offsetX = ...;
   int16_t offsetY = ...;
   int16_t offsetZ = ...;
   ```

2. Wähle eine mechanische Lage der Platine (waagerecht, in Einbauposition), die als **Referenz-Null** dienen soll.

3. In dieser Lage im Serial Monitor eine der Zeilen anschauen und die Werte hinter `RECOMMENDED new offsetX/Y/Z` ablesen.

4. Diese drei Werte im Sketch-Kopf eintragen, z.B.:

   ```cpp
   int16_t offsetX = <RAW_X_in_dieser_Position>;
   int16_t offsetY = <RAW_Y_in_dieser_Position>;
   int16_t offsetZ = <RAW_Z_in_dieser_Position>;
   ```

5. Sketch neu kompilieren und auf den UNO laden.

6. In derselben Platinenlage sollten die korrigierten `X`, `Y`, `Z` nun **nahe 0** liegen, und `Heading` sollte für diese Richtung stabil sein.

7. Falls nötig, kann die Kalibrierung wiederholt werden (neue Rohdaten ablesen und `offsetX/Y/Z` entsprechend anpassen).

## Hinweise

- Dieser Sketch und diese README-Datei gelten **ausschließlich für GY-271 mit HP5883/QMC5883P (Adresse 0x2C)**.
- Für Module mit echtem **HMC5883L (0x1E)** oder mit Standard-**QMC5883L (0x0D)** werden andere Bibliotheken und andere Registerbelegungen benötigt.
- Für einen Port auf ESP32 muss u.a. `Wire.begin()` auf `Wire.begin(21, 22)` geändert und die Baudrate für Serial angepasst werden (z.B. 115200).
