# PROJECT INTERN_2026_2027 
**SAMR**

**#Arsitektur System**

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

