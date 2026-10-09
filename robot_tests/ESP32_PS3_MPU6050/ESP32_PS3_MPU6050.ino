/*
 * ESP32 (DevKit klasik) - PS3 Controller + MPU6050 -> UART ke Mega
 * Board   : "ESP32 Dev Module"
 * Library : Ps3Controller (jvpernis/esp32-ps3). Install manual / ZIP.
 * UART    : Serial2, RX2 = GPIO16, TX2 = GPIO17, 115200
 * MPU6050 : SDA = 21, SCL = 22, AD0 -> GND (0x68). Tanpa library tambahan.
 *
 * Langkah awal: upload, buka Serial Monitor (115200), catat MAC address
 * yang tercetak, tulis ke controller PS3 pakai SixaxisPairTool.
 * Format paket ke Mega: $lx,ly,rx,ry,btn,yaw10\n  (lihat T07)
 */
#include <Ps3Controller.h>
#include <Wire.h>

#define MPU_ADDR 0x68
#define RX2_PIN 16
#define TX2_PIN 17

float yaw = 0, gyroZoffset = 0;
unsigned long lastImu = 0, lastSend = 0;

bool mpuWrite(uint8_t reg, uint8_t val) {
  Wire.beginTransmission(MPU_ADDR); Wire.write(reg); Wire.write(val);
  return Wire.endTransmission() == 0;
}
int16_t readGyroZraw() {
  Wire.beginTransmission(MPU_ADDR); Wire.write(0x47); Wire.endTransmission(false);
  Wire.requestFrom((uint8_t)MPU_ADDR, (uint8_t)2);
  int16_t hi = Wire.read(), lo = Wire.read();
  return (hi << 8) | lo;
}

void setup() {
  Serial.begin(115200);
  Serial2.begin(115200, SERIAL_8N1, RX2_PIN, TX2_PIN);

  Wire.begin(21, 22);
  Wire.setClock(400000);
  if (!mpuWrite(0x6B, 0x00)) Serial.println(F("MPU6050 TIDAK TERDETEKSI"));  // wake up
  mpuWrite(0x1A, 0x03);   // DLPF ~44 Hz
  mpuWrite(0x1B, 0x00);   // gyro +-250 dps (131 LSB/dps)

  Serial.println(F("Kalibrasi gyro, jangan gerakkan robot..."));
  long sum = 0;
  for (int i = 0; i < 500; i++) { sum += readGyroZraw(); delay(2); }
  gyroZoffset = sum / 500.0f;
  Serial.println(F("Kalibrasi selesai"));

  Ps3.begin();
  Serial.print(F("MAC ESP32: ")); Serial.println(Ps3.getAddress());
  lastImu = micros();
}

void loop() {
  // Baca IMU, integrasi yaw
  unsigned long now = micros();
  float dt = (now - lastImu) / 1e6f;
  lastImu = now;
  float gz = (readGyroZraw() - gyroZoffset) / 131.0f;   // deg/s
  yaw += gz * dt;
  if (yaw > 180) yaw -= 360; else if (yaw < -180) yaw += 360;

  // Kirim paket 50 Hz
  if (millis() - lastSend >= 20) {
    lastSend = millis();
    int lx = 0, ly = 0, rx = 0, ry = 0, btn = 0;
    if (Ps3.isConnected()) {
      lx = Ps3.data.analog.stick.lx; ly = Ps3.data.analog.stick.ly;
      rx = Ps3.data.analog.stick.rx; ry = Ps3.data.analog.stick.ry;
      if (Ps3.data.button.cross)    btn |= 1 << 0;
      if (Ps3.data.button.circle)   btn |= 1 << 1;
      if (Ps3.data.button.square)   btn |= 1 << 2;
      if (Ps3.data.button.triangle) btn |= 1 << 3;
      if (Ps3.data.button.l1)       btn |= 1 << 4;
      if (Ps3.data.button.r1)       btn |= 1 << 5;
    }
    char out[64];
    snprintf(out, sizeof(out), "$%d,%d,%d,%d,%d,%d\n", lx, ly, rx, ry, btn, (int)(yaw * 10));
    Serial2.print(out);
    Serial.print(Ps3.isConnected() ? "PS3 OK  " : "PS3 --  ");
    Serial.print(out);
  }
}
