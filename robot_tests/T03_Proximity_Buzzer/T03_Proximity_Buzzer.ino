/*
 * T03 - Proximity sensor + Buzzer
 * Proximity : pin 36, INPUT_PULLUP (asumsi output NPN open-collector:
 *             LOW = objek terdeteksi). Ubah DETECT_LEVEL jika berlawanan.
 * Buzzer    : pin 42 (opsional)
 * PERINGATAN: jika sensor tipe PNP (output = tegangan supply 6-36 V),
 *             JANGAN langsung ke pin Mega. Pakai pembagi tegangan/optocoupler.
 */
const uint8_t PIN_PROX   = 36;
const uint8_t PIN_BUZZER = 42;
const uint8_t DETECT_LEVEL = LOW;

void setup() {
  Serial.begin(115200);
  pinMode(PIN_PROX, INPUT_PULLUP);
  pinMode(PIN_BUZZER, OUTPUT);
  digitalWrite(PIN_BUZZER, LOW);
  Serial.println(F("T03 Proximity + Buzzer siap"));
}

void loop() {
  static bool last = false;
  bool detected = (digitalRead(PIN_PROX) == DETECT_LEVEL);
  if (detected != last) {
    last = detected;
    Serial.println(detected ? F("OBJEK TERDETEKSI") : F("Tidak ada objek"));
    digitalWrite(PIN_BUZZER, detected ? HIGH : LOW);
  }
}
