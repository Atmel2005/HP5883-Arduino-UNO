#include <Wire.h>

// QMC5883P I2C адрес
const int QMC5883P_ADDR = 0x2C;

// Регистры (по твоему примеру)
const int MODE_REG   = 0x0A;
const int CONFIG_REG = 0x0B;

// Данные (LSB/MSB)
const int X_LSB_REG = 0x01;

// Калибровочные смещения (заполняются вручную по результатам измерений)
int16_t offsetX = 0;
int16_t offsetY = 0;
int16_t offsetZ = 0;

// Инициализация QMC5883P / HP5883
void initQMC5883P() {
  // UNO: SDA=A4, SCL=A5
  Wire.begin();

  // Режим: непрерывный, 200 Гц, диапазон ±8G (по найденному решению)
  Wire.beginTransmission(QMC5883P_ADDR);
  Wire.write(MODE_REG);
  Wire.write(0xCF);   // continuous mode, 200Hz
  Wire.endTransmission();

  Wire.beginTransmission(QMC5883P_ADDR);
  Wire.write(CONFIG_REG);
  Wire.write(0x08);   // set/reset on, ±8G
  Wire.endTransmission();
}

// Чтение сырых данных X/Y/Z
bool readQMC5883PData(int16_t &x, int16_t &y, int16_t &z) {
  Wire.beginTransmission(QMC5883P_ADDR);
  Wire.write(X_LSB_REG);
  Wire.endTransmission(false);

  Wire.requestFrom(QMC5883P_ADDR, (uint8_t)6);
  if (Wire.available() != 6) {
    return false;
  }

  uint8_t x_lsb = Wire.read();
  uint8_t x_msb = Wire.read();
  uint8_t y_lsb = Wire.read();
  uint8_t y_msb = Wire.read();
  uint8_t z_lsb = Wire.read();
  uint8_t z_msb = Wire.read();

  x = (int16_t)((x_msb << 8) | x_lsb);
  y = (int16_t)((y_msb << 8) | y_lsb);
  z = (int16_t)((z_msb << 8) | z_lsb);

  return true;
}

void setup() {
  Serial.begin(9600);
  Serial.println("QMC5883P / HP5883 compass test on UNO");

  initQMC5883P();
}

void loop() {
  int16_t rawX = 0, rawY = 0, rawZ = 0;
  if (!readQMC5883PData(rawX, rawY, rawZ)) {
    Serial.println("Read error");
    delay(200);
    return;
  }
  
  // Применяем калибровочные смещения
  int16_t x = rawX - offsetX;
  int16_t y = rawY - offsetY;
  int16_t z = rawZ - offsetZ;

  // Вывод: сырые значения, скорректированные и рекомендуемые оффсеты
  Serial.print("RAW X: "); Serial.print(rawX);
  Serial.print("  Y: "); Serial.print(rawY);
  Serial.print("  Z: "); Serial.print(rawZ);

  Serial.print("  | X: "); Serial.print(x);
  Serial.print("  Y: "); Serial.print(y);
  Serial.print("  Z: "); Serial.print(z);

  // Рекомендованные НОВЫЕ оффсеты (без дельт): запиши эти значения в offsetX/Y/Z в шапке
  Serial.print("  | RECOMMENDED new offsetX: "); Serial.print(rawX);
  Serial.print("  offsetY: "); Serial.print(rawY);
  Serial.print("  offsetZ: "); Serial.print(rawZ);

  // Расчёт азимута (компасного угла) по X/Y (уже после применения offset)
  float fx = (float)x;
  float fy = (float)y;

  float heading = atan2(fy, fx);

  if (heading < 0) heading += 2 * PI;
  if (heading > 2 * PI) heading -= 2 * PI;

  float headingDegrees = heading * 180.0f / PI;

  Serial.print("  Heading: ");
  Serial.print(headingDegrees);
  Serial.println(" deg");

  delay(100);
}