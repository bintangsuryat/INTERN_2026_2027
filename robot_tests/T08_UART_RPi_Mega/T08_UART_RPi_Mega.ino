/*
 * T08 - Mega <-> Raspberry Pi lewat Serial2 (115200)
 * Mega RX2 = 17 <- Pi TXD (GPIO14, pin 8)
 * Mega TX2 = 16 -> Pi RXD (GPIO15, pin 10)  !! lewat pembagi tegangan 5V->3.3V !!
 * GND disatukan. Pi hanya 3.3 V, jangan sambung TX2 Mega langsung.
 *
 * Paket dari Pi : OBJ,x,y,area\n
 * Balasan Mega  : ACK,x,y,area\n
 */
char buf[64];
uint8_t idx = 0;

void handleLine() {
  int x, y, area;
  if (sscanf(buf, "OBJ,%d,%d,%d", &x, &y, &area) == 3) {
    Serial.print(F("Dari Pi -> x=")); Serial.print(x);
    Serial.print(F(" y=")); Serial.print(y);
    Serial.print(F(" area=")); Serial.println(area);
    Serial2.print(F("ACK,")); Serial2.print(x); Serial2.print(',');
    Serial2.print(y); Serial2.print(','); Serial2.println(area);
  } else {
    Serial.print(F("Paket tidak dikenal: ")); Serial.println(buf);
  }
}

void setup() {
  Serial.begin(115200);
  Serial2.begin(115200);
  Serial.println(F("T08 UART Raspberry Pi siap"));
}

void loop() {
  while (Serial2.available()) {
    char c = Serial2.read();
    if (c == '\n') { buf[idx] = 0; handleLine(); idx = 0; }
    else if (c != '\r' && idx < sizeof(buf) - 1) buf[idx++] = c;
  }
}
