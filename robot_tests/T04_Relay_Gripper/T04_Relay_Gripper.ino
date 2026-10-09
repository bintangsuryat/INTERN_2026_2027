/*
 * T04 - Relay aktif LOW (solenoid gripper pneumatik)
 * Relay: pin 40. LOW = relay ON (gripper aktif), HIGH = OFF.
 * Saat boot pin HARUS HIGH -> diset HIGH sebelum pinMode(OUTPUT).
 * Serial 115200, kirim: '1' = ON, '0' = OFF
 */
const uint8_t PIN_RELAY = 40;

void relayOn()  { digitalWrite(PIN_RELAY, LOW);  Serial.println(F("Relay ON  (gripper aktif)")); }
void relayOff() { digitalWrite(PIN_RELAY, HIGH); Serial.println(F("Relay OFF")); }

void setup() {
  digitalWrite(PIN_RELAY, HIGH);   // aktifkan pull-up dulu agar tidak glitch
  pinMode(PIN_RELAY, OUTPUT);
  digitalWrite(PIN_RELAY, HIGH);
  Serial.begin(115200);
  Serial.println(F("T04 Relay siap. '1' = ON, '0' = OFF"));
}

void loop() {
  if (Serial.available()) {
    char c = Serial.read();
    if (c == '1') relayOn();
    else if (c == '0') relayOff();
  }
}
