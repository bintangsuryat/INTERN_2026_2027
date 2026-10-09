/*
 * T02 - 3x HC-SR04 (depan, kiri, kanan)
 * Depan : TRIG 30, ECHO 31
 * Kiri  : TRIG 32, ECHO 33
 * Kanan : TRIG 34, ECHO 35
 * HC-SR04 butuh 5 V. Serial: 115200
 * Hasil -1 = tidak ada echo (di luar jangkauan / kabel salah)
 */
const uint8_t TRIG_F = 30, ECHO_F = 31;
const uint8_t TRIG_L = 32, ECHO_L = 33;
const uint8_t TRIG_R = 34, ECHO_R = 35;

float readCm(uint8_t trig, uint8_t echo) {
  digitalWrite(trig, LOW);  delayMicroseconds(2);
  digitalWrite(trig, HIGH); delayMicroseconds(10);
  digitalWrite(trig, LOW);
  unsigned long d = pulseIn(echo, HIGH, 30000UL);  // timeout 30 ms (~5 m)
  if (d == 0) return -1;
  return d * 0.0343f / 2.0f;
}

void setup() {
  Serial.begin(115200);
  pinMode(TRIG_F, OUTPUT); pinMode(ECHO_F, INPUT);
  pinMode(TRIG_L, OUTPUT); pinMode(ECHO_L, INPUT);
  pinMode(TRIG_R, OUTPUT); pinMode(ECHO_R, INPUT);
  Serial.println(F("T02 Ultrasonik siap"));
}

void loop() {
  float f = readCm(TRIG_F, ECHO_F); delay(60);  // jeda agar tidak saling gema
  float l = readCm(TRIG_L, ECHO_L); delay(60);
  float r = readCm(TRIG_R, ECHO_R); delay(60);
  Serial.print(F("Depan: ")); Serial.print(f, 1);
  Serial.print(F(" cm | Kiri: ")); Serial.print(l, 1);
  Serial.print(F(" cm | Kanan: ")); Serial.print(r, 1);
  Serial.println(F(" cm"));
  delay(100);
}
