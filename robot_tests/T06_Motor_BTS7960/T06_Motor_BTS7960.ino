/*
 * T06 - Motor DC via 2x BTS7960
 * Kiri : RPWM 5, LPWM 6, R_EN+L_EN 22
 * Kanan: RPWM 7, LPWM 8, R_EN+L_EN 23
 * RPWM = maju, LPWM = mundur (salah satu harus 0).
 *
 * !! ANGKAT RODA dari lantai saat tes pertama !!
 *
 * Perintah Serial (115200, newline):
 *   L100  -> motor kiri  +100 (maju)    L-100 -> mundur
 *   R100  -> motor kanan +100           R-100 -> mundur
 *   B80   -> kedua motor +80            S     -> stop
 * PWM dibatasi MAX_PWM. Failsafe: stop otomatis jika 2 detik tanpa perintah.
 * Jika arah motor terbalik, tukar kabel motor atau ubah nilai INVERT_*.
 */
const uint8_t L_RPWM = 5, L_LPWM = 6, L_EN = 22;
const uint8_t R_RPWM = 7, R_LPWM = 8, R_EN = 23;
const int  MAX_PWM = 150;            // naikkan bertahap setelah aman
const bool INVERT_L = false, INVERT_R = false;
const unsigned long TIMEOUT_MS = 2000;

unsigned long lastCmd = 0;

void driveMotor(uint8_t rpwm, uint8_t lpwm, int speed, bool invert) {
  if (invert) speed = -speed;
  speed = constrain(speed, -MAX_PWM, MAX_PWM);
  if (speed >= 0) { analogWrite(lpwm, 0);      analogWrite(rpwm, speed); }
  else            { analogWrite(rpwm, 0);      analogWrite(lpwm, -speed); }
}
void motorL(int s) { driveMotor(L_RPWM, L_LPWM, s, INVERT_L); }
void motorR(int s) { driveMotor(R_RPWM, R_LPWM, s, INVERT_R); }
void stopAll()     { motorL(0); motorR(0); }

void setup() {
  Serial.begin(115200);
  uint8_t pins[] = {L_RPWM, L_LPWM, L_EN, R_RPWM, R_LPWM, R_EN};
  for (uint8_t p : pins) { pinMode(p, OUTPUT); digitalWrite(p, LOW); }
  digitalWrite(L_EN, HIGH);
  digitalWrite(R_EN, HIGH);
  Serial.println(F("T06 Motor siap. Contoh: L100, R-100, B80, S"));
}

void loop() {
  if (Serial.available()) {
    String s = Serial.readStringUntil('\n');
    s.trim(); s.toUpperCase();
    if (s.length()) {
      char c = s.charAt(0);
      int v = s.substring(1).toInt();
      if      (c == 'L') motorL(v);
      else if (c == 'R') motorR(v);
      else if (c == 'B') { motorL(v); motorR(v); }
      else if (c == 'S') stopAll();
      lastCmd = millis();
      Serial.print(F("OK: ")); Serial.println(s);
    }
  }
  if (millis() - lastCmd > TIMEOUT_MS) stopAll();   // failsafe
}
