/*
 * T05 - Servo arm
 * Servo signal: pin 11 (Servo library Mega memakai Timer5,
 * hindari analogWrite di pin 44-46).
 * Daya servo dari supply 5-6 V TERPISAH, GND disatukan dengan Mega.
 * Serial 115200: ketik sudut 0-180 lalu Enter, atau "sweep".
 */
#include <Servo.h>

const uint8_t PIN_SERVO = 11;
Servo arm;

void sweep() {
  for (int a = 0; a <= 180; a += 2) { arm.write(a); delay(15); }
  for (int a = 180; a >= 0; a -= 2) { arm.write(a); delay(15); }
}

void setup() {
  Serial.begin(115200);
  arm.attach(PIN_SERVO);
  arm.write(90);
  Serial.println(F("T05 Servo siap. Ketik sudut 0-180 atau 'sweep'"));
}

void loop() {
  if (Serial.available()) {
    String s = Serial.readStringUntil('\n');
    s.trim();
    if (s.equalsIgnoreCase("sweep")) {
      sweep();
    } else if (s.length()) {
      int a = constrain(s.toInt(), 0, 180);
      arm.write(a);
      Serial.print(F("Sudut: ")); Serial.println(a);
    }
  }
}
