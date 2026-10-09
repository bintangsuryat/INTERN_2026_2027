/*
 * T07 - Mega menerima paket dari ESP32 lewat Serial1 (115200)
 * Mega RX1 = 19  <-  ESP32 TX2 = GPIO17
 * Mega TX1 = 18  ->  ESP32 RX2 = GPIO16 (lewat pembagi tegangan 5V->3.3V,
 *                    atau biarkan tidak tersambung karena hanya terima)
 * GND harus disatukan.
 *
 * Format paket (ASCII, 1 baris):  $lx,ly,rx,ry,btn,yaw10\n
 *   lx..ry : -128..127 (stick PS3)
 *   btn    : bitmask (bit0 X, bit1 O, bit2 Kotak, bit3 Segitiga, bit4 L1, bit5 R1)
 *   yaw10  : yaw dalam 0.1 derajat (integer)
 */
int lx, ly, rx, ry, btn, yaw10;
char buf[64];
uint8_t idx = 0;
unsigned long lastPacket = 0, lastPrint = 0;
bool timeoutShown = false;

void parseLine() {
  if (buf[0] != '$') return;
  int n = sscanf(buf + 1, "%d,%d,%d,%d,%d,%d", &lx, &ly, &rx, &ry, &btn, &yaw10);
  if (n == 6) { lastPacket = millis(); timeoutShown = false; }
}

void setup() {
  Serial.begin(115200);
  Serial1.begin(115200);
  Serial.println(F("T07 UART ESP32 siap"));
}

void loop() {
  while (Serial1.available()) {
    char c = Serial1.read();
    if (c == '\n') { buf[idx] = 0; parseLine(); idx = 0; }
    else if (c != '\r' && idx < sizeof(buf) - 1) buf[idx++] = c;
  }

  if (millis() - lastPacket > 500) {          // komunikasi putus
    if (!timeoutShown) { Serial.println(F("TIMEOUT: tidak ada paket dari ESP32")); timeoutShown = true; }
  } else if (millis() - lastPrint > 100) {
    lastPrint = millis();
    Serial.print(F("LX=")); Serial.print(lx);
    Serial.print(F(" LY=")); Serial.print(ly);
    Serial.print(F(" RX=")); Serial.print(rx);
    Serial.print(F(" RY=")); Serial.print(ry);
    Serial.print(F(" BTN=")); Serial.print(btn, BIN);
    Serial.print(F(" YAW=")); Serial.print(yaw10 / 10.0, 1);
    Serial.println();
  }
}
