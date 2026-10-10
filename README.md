# PROJECT INTERN_2026_2027 
**SAMR**

**#Arsitektur System**

```
PS3 Controller --Bluetooth--> ESP32 (slave)
                              |  MPU6050 (I2C)
                              |  UART (joystick + yaw)
                              v
Raspberry Pi --UART--> ARDUINO MEGA 2560 (master) --> BTS7960 x2 --> Motor kiri/kanan
(webcam, OpenCV,        |   |   |   |                     
 layar 7 inch)          |   |   |   +--> Relay (aktif LOW) --> Solenoid gripper pneumatik
                        |   |   +------> Servo (arm)
                        |   +----------> LCD I2C 20x4
                        +--------------> 3x HC-SR04, 1x proximity
```

Pembagian peran:
• Mega (master): pengambil keputusan, mixing differential, kontrol motor, sensor jarak, state machine, aktuator, LCD.
• ESP32 (slave 1): Bluetooth PS3 + membaca MPU6050 dengan rate tinggi, kirim paket ke Mega. Alasan: pembacaan IMU tidak terganggu pembacaan ultrasonik/LCD di Mega.
• Raspberry Pi (slave 2): vision, kirim koordinat objek ke Mega, tampilkan status di layar 7 inch.


**# Daftar Komponen**

| Komponen | Jumlah | Fungsi |
| --- | --- | --- |
| Arduino Mega 2560 | 1 | Master controller |
| ESP32 DevKit (ESP32 klasik) | 1 | Bluetooth PS3 + MPU6050 |
| HC-SR04 | 3 | Jarak depan, kiri, kanan |
| Proximity sensor kuning (output HIGH/LOW) | 1 | Deteksi objek di gripper / marker (lihat asumsi) |
| MPU6050 | 1 | Yaw (heading) dan gyro |
| Servo | 1 | Menggerakkan arm |
| BTS7960 | 2 | Driver PWM motor kiri dan kanan |
| LCD I2C 20x4 | 1 | Status dan debug |
| Relay + optocoupler aktif LOW | 1 | Kontrol solenoid gripper pneumatik |
| Raspberry Pi + webcam + layar 7 inch | 1 | Object detection dan tampilan |


## Requirement Wiring dan Power
### 1. Pin map Arduino Mega 2560
| Fungsi | Pin Mega | Catatan |
| --- | --- | --- |
| UART ke ESP32 (Serial1) | TX1 = 18, RX1 = 19 | 115200 baud |
| UART ke Raspberry Pi (Serial2) | TX2 = 16, RX2 = 17 | 115200 baud |
| Debug USB (Serial0) | 0, 1 | Hanya untuk debug ke PC |
| LCD I2C 20x4 | SDA = 20, SCL = 21 | Alamat 0x27 atau 0x3F (cek dengan I2C scanner) |
| Motor kiri BTS7960 | RPWM = 5, LPWM = 6, R\_EN + L\_EN = vcc | RPWM maju, LPWM mundur; salah satu harus 0 |
| Motor kanan BTS7960 | RPWM = 8, LPWM = 9, R\_EN + L\_EN = vcc | Sama seperti kiri |
| Servo arm | 11 | Library Servo di Mega memakai Timer5, hindari PWM di pin 44-46 |
| Ultrasonik depan | TRIG = 30, ECHO = 31 |  |
| Ultrasonik kiri | TRIG = 32, ECHO = 33 |  |
| Ultrasonik kanan | TRIG = 34, ECHO = 35 |  |
| Proximity | 3 | INPUT\_PULLUP, lihat 5.5 |
| Relay gripper (aktif LOW) | 40 | Saat boot harus HIGH (relay OFF) |
| Buzzer (opsional) | 42 | Indikator state/fault |

### 2. Pin map ESP32 DevKit
| Fungsi | GPIO | Catatan |
| --- | --- | --- |
| UART ke Mega (Serial2) | RX2 = 16, TX2 = 17 | Gunakan ESP32 DevKit biasa; pada modul WROVER GPIO16/17 dipakai PSRAM |
| MPU6050 | SDA = 21, SCL = 22 | AD0 ke GND (alamat 0x68) |
| Daya | VIN = 5 V logika, GND | Bluetooth menarik arus puncak, jangan ambil dari USB laptop saat robot jalan |

Bluetooth PS3 memakai library Ps3Controller (khusus ESP32 klasik, bukan S3/C3). Alamat MAC Bluetooth ESP32 harus ditulis ke controller PS3 memakai SixaxisPairTool.

### 3. Raspberry Pi
- UART: GPIO14 (TXD, pin 8) dan GPIO15 (RXD, pin 10), perangkat `/dev/serial0`. Nonaktifkan serial console, aktifkan serial port via raspi-config.
- Alternatif tanpa level shifter: sambungkan Pi ke Mega lewat kabel USB (muncul sebagai `/dev/ttyACM0`), dengan konsekuensi Serial0 Mega dipakai Pi dan debug harus pindah ke LCD.
- Webcam USB, layar 7 inch (HDMI/DSI sesuai tipe).
