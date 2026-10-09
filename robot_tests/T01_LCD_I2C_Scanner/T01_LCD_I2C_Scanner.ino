/*
 * T01 - I2C Scanner + LCD 20x4
 * Board  : Arduino Mega 2560
 * Pin    : SDA = 20, SCL = 21
 * Library: "LiquidCrystal I2C" (Frank de Brabander) via Library Manager
 * Serial : 115200
 */
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

uint8_t lcdAddr = 0;

uint8_t scanI2C() {
  uint8_t found = 0, first = 0;
  Serial.println(F("Scanning I2C..."));
  for (uint8_t a = 1; a < 127; a++) {
    Wire.beginTransmission(a);
    if (Wire.endTransmission() == 0) {
      Serial.print(F("  Device di 0x")); Serial.println(a, HEX);
      if (!first && (a == 0x27 || a == 0x3F)) first = a;
      found++;
    }
  }
  if (!found) Serial.println(F("  Tidak ada device. Cek SDA/SCL/VCC/GND."));
  return first;
}

void setup() {
  Serial.begin(115200);
  Wire.begin();
  lcdAddr = scanI2C();
  if (!lcdAddr) {
    Serial.println(F("LCD (0x27/0x3F) tidak ditemukan."));
    while (true) {}
  }
  LiquidCrystal_I2C lcd(lcdAddr, 20, 4);
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0); lcd.print(F("ABU Robocon 2027"));
  lcd.setCursor(0, 1); lcd.print(F("LCD OK, addr 0x"));
  lcd.print(lcdAddr, HEX);
  lcd.setCursor(0, 2); lcd.print(F("Mega 2560 Master"));
  lcd.setCursor(0, 3); lcd.print(F("Tes baris 4 ........"));
  Serial.println(F("LCD tampil. Jika kosong, putar potensio kontras di belakang LCD."));
}

void loop() {}
