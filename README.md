# 🛵 SCOOPY SYSTEM - Smart Motorcycle Dashboard

SCOOPY SYSTEM adalah proyek *Internet of Things* (IoT) yang dirancang khusus untuk memodifikasi dasbor/speedometer sepeda motor menjadi *smart dashboard*. Proyek ini ditenagai oleh mikrokontroler **ESP32-C3 Super Mini** dan terintegrasi dengan aplikasi *smartphone* melalui koneksi Bluetooth Low Energy (BLE).

Desain kelistrikan proyek ini difokuskan pada efisiensi daya (menggunakan baterai LiPo/TP4056 eksternal) dan estetika. Menggunakan sensor sentuh kapasitif TTP223, tombol kontrol dapat disembunyikan tepat di balik mika speedometer tanpa merusak tampilan asli motor.

## ✨ Fitur Utama
*   **Custom Boot Intro:** Animasi efek mengetik bertuliskan "SCOOPY" berukuran besar saat perangkat pertama kali dinyalakan.
*   **Real-time Clock & Rain Animation:** Tampilan *standby* berupa jam digital presisi tinggi (disinkronkan dari *smartphone*) dipadukan dengan animasi hujan bergulir yang estetis di layar OLED.
*   **Smart Notifications:** Otomatis menampilkan nama pengirim dan isi pesan secara *text-wrapping* (mendukung WhatsApp, SMS, dan aplikasi lainnya).
*   **Incoming Call Alert:** Peringatan visual khusus saat ada panggilan telepon masuk.
*   **Invisible Media Controller:** Kontrol musik *smartphone* (*Play/Pause, Next/Prev, Volume*) langsung dari dasbor motor melalui multi-ketukan (*multi-tap*) pada permukaan mika tanpa tombol fisik.

## 🛠️ Komponen Hardware
1.  **ESP32-C3 Super Mini** (Mikrokontroler utama dengan fitur BLE)
2.  **Layar OLED SSD1306 128x64** (Komunikasi I2C)
3.  **2x Sensor Sentuh Kapasitif TTP223** (Untuk kontrol navigasi kanan & kiri)
4.  Baterai LiPo + Modul TP4056 (Untuk suplai daya *standalone*)
5.  Kabel Jumper, *Heat Shrink*, atau *Solder Seal Wire Connector* untuk percabangan kabel 3.3V/GND.

## 🔌 Skema Kelistrikan (Wiring)

| Komponen | Pin ESP32-C3 | Keterangan |
| :--- | :--- | :--- |
| **OLED SDA** | `GPIO 8` | Jalur Data I2C |
| **OLED SCL** | `GPIO 9` | Jalur Clock I2C |
| **Sensor Kanan (I/O)** | `GPIO 3` | Sinyal ketukan kanan |
| **Sensor Kiri (I/O)** | `GPIO 4` | Sinyal ketukan kiri |
| **Semua VCC/VDD** | `3.3V` | **Jangan** gunakan 5V agar aman bagi ESP32 |
| **Semua GND** | `GND` | Jalur Ground |

## 📚 Library yang Dibutuhkan
Sebelum melakukan *compile* dan *upload* kode melalui Arduino IDE, pastikan Anda telah menginstal *library* berikut melalui **Library Manager**:
*   `Adafruit GFX Library`
*   `Adafruit SSD1306`
*   `ChronosESP32` (Untuk komunikasi BLE dan manajemen data notifikasi)

## 🎮 Panduan Kontrol Sentuh
Sistem ini menggunakan logika *MultiButton* dengan batas jeda (*debounce*) sekitar 450ms.

| Aksi Sentuhan | Sensor Kanan | Sensor Kiri |
| :--- | :--- | :--- |
| **1x Tap** | Play / Pause Musik (Tutup Notifikasi Telepon) | Play / Pause Musik (Tutup Notifikasi Telepon) |
| **2x Tap** | Lagu Selanjutnya (*Next Track*) | Lagu Sebelumnya (*Previous Track*) |
| **3x Tap** | Naikkan Volume (+10%) | Turunkan Volume (-10%) |

## 🚀 Cara Instalasi & Penggunaan
1.  **Persiapan Hardware:** Rangkai komponen sesuai dengan tabel skema *wiring* di atas. Pastikan penempatan sensor TTP223 menempel rapat dari bagian dalam mika pelindung dasbor motor.
2.  **Upload Firmware:** *Compile* dan *upload* kode `ScoopySystem.ino` menggunakan Arduino IDE ke papan ESP32-C3 Super Mini.
3.  **Koneksi Smartphone:** 
    * Unduh dan instal aplikasi pendamping **Chronos** di *smartphone* Android.
    * Nyalakan Bluetooth di HP, buka aplikasi Chronos, dan *scan* perangkat bernama **"SCOOPY SYSTEM"**.
    * Berikan izin (*permissions*) untuk membaca Notifikasi dan Kontrol Media pada aplikasi Chronos.
4.  **Selesai!** Dasbor pintar siap digunakan saat berkendara.

---
**Pengembang:** Damar Arief Witjaksono  
*Dikembangkan untuk eksperimen sistem IoT dan modifikasi otomotif terintegrasi.*