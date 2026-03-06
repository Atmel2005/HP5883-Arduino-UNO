# GY-271 mit HP5883 / QMC5883P (Arduino UNO)

## Was ist das für ein Modul

- Board: **GY-271**.
- Tatsächlicher Chip: **HP5883 / QMC5883P**.
- I2C-Adresse: **0x2C**.
- Register sind **nicht kompatibel** mit dem klassischen **HMC5883L** und den meisten Bibliotheken dafür.
- Im Projekt wird **direkte I2C-Steuerung (Wire)** ohne externe Kompass-Bibliotheken verwendet.

## Für welche Plattform

- Sketch geschrieben und getestet für **Arduino UNO**.
- Hardware-I2C-Pins am UNO:
  - SDA → A4
  - SCL → A5

## Anschluss des GY-271 an Arduino UNO

- `VCC` → **5V**
- `GND` → **GND**
- `SDA` → **A4**
- `SCL` → **A5**

## Wie der Code funktioniert

- In der Initialisierung werden die Register des HP5883/QMC5883P manuell gesetzt:
  - Umschalten in den **Continuous-Mode**,
  - Datenrate ~200 Hz,
  - Messbereich ±8G.
- In der `loop()`-Schleife:
  - werden **Rohwerte X/Y/Z** per I2C ausgelesen,
  - die Kalibrier-Offsets `offsetX`, `offsetY`, `offsetZ` abgezogen,
  - der **Azimut (Heading)** über X/Y berechnet,
  - und im Serial Monitor ausgegeben:
    - Rohwerte,
    - korrigierte Werte,
    - empfohlene neue Offsets,
    - Winkel `Heading` in Grad.

## Nutzung

1. Sketch `HMC5883L.ino` auf den Arduino UNO laden.
2. GY-271 wie oben beschrieben verbinden.
3. **Serial Monitor** öffnen mit:
   - Baudrate: **9600 Baud**.
4. Die Ausgabe hat etwa folgende Form:

   ```text
   RAW X: <rawX>  Y: <rawY>  Z: <rawZ>  | X: <X>  Y: <Y>  Z: <Z>  | RECOMMENDED new offsetX: <rawX> offsetY: <rawY> offsetZ: <rawZ>  Heading: <Winkel> deg
   ```

   Dabei gilt:
   - `RAW X/Y/Z` — echte, unveränderte Sensorwerte;
   - `X/Y/Z` — **nach Abzug der aktuellen `offsetX/Y/Z`** aus dem Sketchkopf;
   - `RECOMMENDED new offsetX/Y/Z` — Rohwerte, die als **neue Kalibrier-Offsets** für die aktuelle Lage genutzt werden können;
   - `Heading` — berechneter Kompasswinkel über X/Y.

## Kalibrierung

1. Am Sketch-Anfang stehen die Zeilen:

   ```cpp
   int16_t offsetX = ...;
   int16_t offsetY = ...;
   int16_t offsetZ = ...;
   ```

2. Wähle eine mechanische Lage der Platine (z. B. nivelliert im Gehäuse), die du als **Referenznull** annehmen willst.

3. In dieser Lage im Serial Monitor die Zeile mit `RAW X/Y/Z` und `RECOMMENDED new offsetX/Y/Z` ansehen.

4. Werte aus `RECOMMENDED new offsetX/Y/Z` in den Sketchkopf übernehmen:

   ```cpp
   int16_t offsetX = <RAW_X_in_dieser_Lage>;
   int16_t offsetY = <RAW_Y_in_dieser_Lage>;
   int16_t offsetZ = <RAW_Z_in_dieser_Lage>;
   ```

5. Sketch neu kompilieren und flashen.

6. In derselben physischen Lage sollten jetzt:
   - korrigierte `X`, `Y`, `Z` **nahe null** sein;
   - `Heading` stabil gegenüber der gewählten Richtung bleiben.

7. Bei Bedarf Kalibrierung wiederholen (erneut `RAW` ansehen und `offsetX/Y/Z` anpassen).

## Wichtig

- Dieser Code und das README sind **speziell für GY-271 mit HP5883/QMC5883P (Adresse 0x2C)** gedacht.
- Für Module mit echtem **HMC5883L (Adresse 0x1E)** oder Standard-**QMC5883L (0x0D)** werden andere Bibliotheken und ein anderes Register-Layout benötigt.
- Für einen Port auf ESP32 zuerst `Wire.begin()` auf `Wire.begin(21, 22)` setzen und die Serial-Geschwindigkeit anpassen (z. B. 115200).

---

## GY-271 з чипом HP5883 / QMC5883P (Arduino UNO)

## Що це за модуль

- Плата: **GY-271**.
- Фактичний чип: **HP5883 / QMC5883P**.
- I2C-адреса: **0x2C**.
- По реєстрах **не сумісний** із класичним **HMC5883L** і більшістю бібліотек для нього.
- У проєкті використовується **пряме керування I2C (Wire)** без зовнішніх бібліотек компаса.

## Для якої платформи

- Скетч написано й протестовано для **Arduino UNO**.
- Апаратні I2C-піни на UNO:
  - SDA → A4
  - SCL → A5

## Підключення GY-271 до Arduino UNO

- `VCC` → **5V**
- `GND` → **GND**
- `SDA` → **A4**
- `SCL` → **A5**

## Як працює код

- У функціях ініціалізації вручну налаштовуються регістри HP5883/QMC5883P:
  - перехід у **безперервний режим** вимірювань,
  - частота даних ~200 Гц,
  - діапазон ±8G.
- У циклі `loop()` код:
  - читає **сирі значення X/Y/Z** з регістрів датчика по I2C,
  - віднімає калібрувальні зсуви `offsetX`, `offsetY`, `offsetZ`,
  - рахує **азимут (heading)** по X/Y,
  - виводить у Serial Monitor:
    - сирі значення,
    - скориговані значення,
    - рекомендовані нові зсуви,
    - кут `Heading` у градусах.

## Як користуватися

1. Завантаж скетч `HMC5883L.ino` в Arduino UNO.
2. Підключи GY-271, як описано вище.
3. Відкрий **Serial Monitor** з параметрами:
   - швидкість: **9600 бод**.
4. Вивід матиме вигляд:

   ```text
   RAW X: <rawX>  Y: <rawY>  Z: <rawZ>  | X: <X>  Y: <Y>  Z: <Z>  | RECOMMENDED new offsetX: <rawX> offsetY: <rawY> offsetZ: <rawZ>  Heading: <кут> deg
   ```

   Де:
   - `RAW X/Y/Z` — реальні необроблені значення з датчика;
   - `X/Y/Z` — вже **після віднімання поточних `offsetX/Y/Z`** зі шапки скетча;
   - `RECOMMENDED new offsetX/Y/Z` — сирі значення, які можна записати як **нові калібрувальні зсуви** для поточного положення плати;
   - `Heading` — розрахований компасний кут по X/Y.

## Як калібрувати

1. На початку скетча є рядки:

   ```cpp
   int16_t offsetX = ...;
   int16_t offsetY = ...;
   int16_t offsetZ = ...;
   ```

2. Обери механічне положення плати (за рівнем, за корпусом), яке вважатимеш **еталонним нулем**.

3. У цьому положенні подивись у Serial Monitor рядок із `RAW X/Y/Z` і `RECOMMENDED new offsetX/Y/Z`.

4. Візьми значення з `RECOMMENDED new offsetX/Y/Z` і впиши їх у шапку скетча:

   ```cpp
   int16_t offsetX = <RAW_X_у_цій_позиції>;
   int16_t offsetY = <RAW_Y_у_цій_позиції>;
   int16_t offsetZ = <RAW_Z_у_цій_позиції>;
   ```

5. Перезбери й залий скетч знову.

6. У цьому ж фізичному положенні плати тепер:
   - скориговані `X`, `Y`, `Z` мають бути **близькі до нуля**;
   - `Heading` буде стабільним відносно обраного напрямку.

7. За потреби можна повторити калібрування для точнішої підгонки (знову дивитись `RAW` і переписувати `offsetX/Y/Z`).

## Важливо

- Цей код і README призначені **конкретно для GY-271 з чипом HP5883/QMC5883P (адреса 0x2C)**.
- Для модулів із справжнім **HMC5883L (адреса 0x1E)** або стандартним **QMC5883L (0x0D)** потрібні інші бібліотеки та інша карта регістрів.
- Якщо переносити код на ESP32, насамперед потрібно замінити `Wire.begin()` на `Wire.begin(21, 22)` і змінити швидкість Serial (наприклад, на 115200).

---

## GY-271 with HP5883 / QMC5883P (Arduino UNO)

## What this module is

- Board: **GY-271**.
- Actual chip: **HP5883 / QMC5883P**.
- I2C address: **0x2C**.
- Register map is **not compatible** with the classic **HMC5883L** and most libraries for it.
- The project uses **direct I2C control (Wire)** without external compass libraries.

## Target platform

- Sketch written and tested for **Arduino UNO**.
- Hardware I2C pins on UNO:
  - SDA → A4
  - SCL → A5

## Wiring GY-271 to Arduino UNO

- `VCC` → **5V**
- `GND` → **GND**
- `SDA` → **A4**
- `SCL` → **A5**

## How the code works

- In setup functions, HP5883/QMC5883P registers are configured manually:
  - switch to **continuous mode**,
  - data rate ~200 Hz,
  - range ±8G.
- In the `loop()` the code:
  - reads **raw X/Y/Z values** from sensor registers via I2C,
  - subtracts calibration offsets `offsetX`, `offsetY`, `offsetZ`,
  - computes **azimuth (heading)** from X/Y,
  - outputs to Serial Monitor:
    - raw values,
    - corrected values,
    - recommended new offsets,
    - `Heading` angle in degrees.

## How to use

1. Upload the sketch `HMC5883L.ino` to the Arduino UNO.
2. Wire the GY-271 as described above.
3. Open **Serial Monitor** with:
   - baud rate: **9600 baud**.
4. Output lines look like:

   ```text
   RAW X: <rawX>  Y: <rawY>  Z: <rawZ>  | X: <X>  Y: <Y>  Z: <Z>  | RECOMMENDED new offsetX: <rawX> offsetY: <rawY> offsetZ: <rawZ>  Heading: <angle> deg
   ```

   Where:
   - `RAW X/Y/Z` — actual unprocessed sensor values;
   - `X/Y/Z` — **after subtracting current `offsetX/Y/Z`** defined at the top of the sketch;
   - `RECOMMENDED new offsetX/Y/Z` — raw values you can use as **new calibration offsets** for the current board orientation;
   - `Heading` — calculated compass angle using X/Y.

## How to calibrate

1. At the top of the sketch you will see:

   ```cpp
   int16_t offsetX = ...;
   int16_t offsetY = ...;
   int16_t offsetZ = ...;
   ```

2. Choose a mechanical position of the board (leveled, aligned to the case) that you want to treat as **reference zero**.

3. In that position, check the Serial Monitor line with `RAW X/Y/Z` and `RECOMMENDED new offsetX/Y/Z`.

4. Take the values from `RECOMMENDED new offsetX/Y/Z` and put them into the sketch header:

   ```cpp
   int16_t offsetX = <RAW_X_in_this_position>;
   int16_t offsetY = <RAW_Y_in_this_position>;
   int16_t offsetZ = <RAW_Z_in_this_position>;
   ```

5. Rebuild and upload the sketch again.

6. In the same physical board position now:
   - corrected `X`, `Y`, `Z` should be **close to zero**;
   - `Heading` should be stable relative to the chosen direction.

7. If needed, repeat calibration for finer tuning (recheck `RAW` and update `offsetX/Y/Z`).

## Important

- This code and README are **specifically for GY-271 with HP5883/QMC5883P (address 0x2C)**.
- Modules with real **HMC5883L (address 0x1E)** or standard **QMC5883L (0x0D)** need different libraries and register maps.
- For porting to ESP32, first change `Wire.begin()` to `Wire.begin(21, 22)` and adjust Serial speed (e.g., 115200).

---

## GY-271 с чипом HP5883 / QMC5883P (Arduino UNO)

## Что за модуль

- Плата: **GY-271**.
- Фактический чип: **HP5883 / QMC5883P**.
- I2C-адрес: **0x2C**.
- Не совместим по регистрам с классическим **HMC5883L** и большинством библиотек под него.
- В проекте используется **прямое управление по I2C (Wire)**, без внешних библиотек компаса.

## Для какой платформы

- Скетч написан и протестирован для **Arduino UNO**.
- Аппаратные пины I2C на UNO:
  - SDA → A4
  - SCL → A5

## Подключение GY-271 к Arduino UNO

- `VCC` → **5V**
- `GND` → **GND**
- `SDA` → **A4**
- `SCL` → **A5**

## Как работает код

- В функциях инициализации вручную настраиваются регистры HP5883/QMC5883P:
  - перевод в **непрерывный режим** измерений,
  - частота данных ~200 Гц,
  - диапазон ±8G.
- В цикле `loop()` код:
  - читает **сырые значения X/Y/Z** из регистров датчика по I2C,
  - вычитает калибровочные смещения `offsetX`, `offsetY`, `offsetZ`,
  - считает **азимут (heading)** по X/Y,
  - выводит в Serial Monitor:
    - сырые значения,
    - скорректированные значения,
    - рекомендованные новые смещения,
    - угол `Heading` в градусах.

## Как пользоваться

1. Залей скетч `HMC5883L.ino` в Arduino UNO.
2. Подключи GY-271 как указано выше.
3. Открой **Serial Monitor** с параметрами:
   - скорость: **9600 бод**.
4. В выводе будут строки вида:

   ```text
   RAW X: <rawX>  Y: <rawY>  Z: <rawZ>  | X: <X>  Y: <Y>  Z: <Z>  | RECOMMENDED new offsetX: <rawX> offsetY: <rawY> offsetZ: <rawZ>  Heading: <угол> deg
   ```

   Где:
   - `RAW X/Y/Z` — реальные необработанные значения с датчика;
   - `X/Y/Z` — уже **после вычитания текущих `offsetX/Y/Z`** из шапки скетча;
   - `RECOMMENDED new offsetX/Y/Z` — сырые значения, которые можно записать как **новые калибровочные смещения** для текущего положения платы;
   - `Heading` — рассчитанный компасный угол по X/Y.

## Как калибровать

1. В начале скетча есть строки:

   ```cpp
   int16_t offsetX = ...;
   int16_t offsetY = ...;
   int16_t offsetZ = ...;
   ```

2. Выбери механическое положение платы (по уровню, по корпусу), которое хочешь считать **эталонным нулём**.

3. При этом положении посмотри в Serial Monitor строку с `RAW X/Y/Z` и `RECOMMENDED new offsetX/Y/Z`.

4. Возьми значения из `RECOMMENDED new offsetX/Y/Z` и впиши их в шапку скетча:

   ```cpp
   int16_t offsetX = <RAW_X_в_этой_позиции>;
   int16_t offsetY = <RAW_Y_в_этой_позиции>;
   int16_t offsetZ = <RAW_Z_в_этой_позиции>;
   ```

5. Пересобери и залей скетч заново.

6. В том же физическом положении платы теперь:
   - скорректированные `X`, `Y`, `Z` должны быть **близки к нулю**;
   - `Heading` будет стабилен относительно выбранного направления.

7. При необходимости можешь повторить калибровку для более точной подстройки (снова смотреть `RAW` и переписывать `offsetX/Y/Z`).

## Важно

- Этот код и README предназначены **конкретно для GY-271 с чипом HP5883/QMC5883P (адрес 0x2C)**.
- Для модулей с настоящим **HMC5883L (адрес 0x1E)** или стандартным **QMC5883L (0x0D)** нужны другие библиотеки и другая карта регистров.
- Если будешь переносить код на ESP32, в первую очередь нужно заменить `Wire.begin()` на `Wire.begin(21, 22)` и изменить скорость Serial (например, на 115200).
