#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <ChronosESP32.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

// === Konfigurasi Pin I2C OLED ===
#define I2C_SDA 8
#define I2C_SCL 9

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
ChronosESP32 watch("SCOOPY SYSTEM");

// === Konfigurasi Pin Touchscreen ===
#define TOUCH_KANAN 3
#define TOUCH_KIRI 4

// === Class MultiButton ===
class MultiButton {
  private:
    uint8_t pin;
    unsigned long lastPressTime = 0;
    int clickCount = 0;
    bool lastState = LOW;
    unsigned long debounceTime = 40;
    unsigned long multiClickTime = 450;
    bool isWaiting = false;

  public:
    MultiButton(uint8_t p) : pin(p) {}
    
    void begin() {
      pinMode(pin, INPUT); 
    }

    int check() {
      bool currentState = digitalRead(pin);
      unsigned long currentMillis = millis();
      int result = 0;

      if (currentState == HIGH && lastState == LOW && (currentMillis - lastPressTime) > debounceTime) {
        clickCount++;
        lastPressTime = currentMillis;
        isWaiting = true;
      }

      if (isWaiting && (currentMillis - lastPressTime) > multiClickTime) {
        result = clickCount; 
        clickCount = 0;
        isWaiting = false;
      }

      lastState = currentState;
      return result;
    }
};

MultiButton btnKanan(TOUCH_KANAN);
MultiButton btnKiri(TOUCH_KIRI);

// === Variabel Sistem ===
bool isConnected = false;
String lastNotifApp = "";
String lastNotifTitle = "";
String lastNotifMessage = "";
unsigned long notifStart = 0;
unsigned long notifDuration = 0;
bool hasNotif = false;
bool isCallRinging = false;
int currentVolume = 50;

// === Efek Hujan (Rain Effect) ===
#define MAX_RAINDROPS 20
int rainX[MAX_RAINDROPS];
int rainY[MAX_RAINDROPS];

void initRain() {
  for (int i = 0; i < MAX_RAINDROPS; i++) {
    rainX[i] = random(0, SCREEN_WIDTH);
    rainY[i] = random(0, SCREEN_HEIGHT);
  }
}

void updateRain() {
  for (int i = 0; i < MAX_RAINDROPS; i++) {
    rainY[i] += 2; 
    if (rainY[i] > SCREEN_HEIGHT) {
      rainY[i] = 0;
      rainX[i] = random(0, SCREEN_WIDTH);
    }
    display.drawPixel(rainX[i], rainY[i], SSD1306_WHITE);
  }
}

// === FITUR BARU: Animasi Intro Teks Bertahap ===
void showIntro() {
  String introText = "SCOOPY SYSTEM";
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  // Menghitung titik tengah horizontal dan vertikal
  int textWidth = introText.length() * 6; // Lebar huruf size 1 = 6 pixel
  int startX = (SCREEN_WIDTH - textWidth) / 2;
  int startY = (SCREEN_HEIGHT - 8) / 2;

  String currentText = "";
  
  // Efek mengetik huruf demi huruf
  for (int i = 0; i < introText.length(); i++) {
    currentText += introText[i];
    display.clearDisplay();
    display.setCursor(startX, startY);
    display.print(currentText);
    display.display();
    delay(100); // Kecepatan munculnya per huruf (100 ms)
  }

  delay(1200); // Menahan tulisan selama 1.2 detik sebelum masuk ke dasbor utama
}

// === Tampilan Layar Utama ===
void showClock(String currentTimeStr) {
  int textSize = 2;
  int charWidth = 6 * textSize;
  int textWidth = currentTimeStr.length() * charWidth;
  int x = (SCREEN_WIDTH - textWidth) / 2;
  int y = (SCREEN_HEIGHT - 16) / 2;

  display.setTextSize(textSize);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(x, y);
  display.println(currentTimeStr);
}

void printWrappedText(String text, int x, int y, int textSize) {
  display.setTextSize(textSize);
  display.setTextColor(SSD1306_WHITE);
  int maxCharsPerLine = SCREEN_WIDTH / (6 * textSize);
  int start = 0;

  while (start < text.length()) {
    int end = start + maxCharsPerLine;
    if (end > text.length()) end = text.length();
    String line = text.substring(start, end);
    
    int lineWidth = line.length() * (6 * textSize);
    int centeredX = (SCREEN_WIDTH - lineWidth) / 2;
    if (centeredX < 0) centeredX = x;

    display.setCursor(centeredX, y);
    display.println(line);

    y += 8 * textSize;
    start = end;
  }
}

// === Callback Bluetooth Chronos ===
void connectionCallback(bool state) {
  isConnected = state;
}

void notificationCallback(Notification notification) {
  lastNotifApp = notification.app;
  lastNotifTitle = notification.title;
  lastNotifMessage = notification.message;
  hasNotif = true;
  notifStart = millis();
  notifDuration = (lastNotifApp == "Phone" || lastNotifApp == "Call") ? 10000 : 5000;
}

void setup() {
  Wire.begin(I2C_SDA, I2C_SCL);
  
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3D)) {
      for(;;);
    }
  }

  // Panggil animasi intro di sini saat awal dinyalakan
  showIntro();
  
  btnKanan.begin();
  btnKiri.begin();

  watch.setConnectionCallback(connectionCallback);
  watch.setNotificationCallback(notificationCallback);
  watch.begin();
  
  initRain();
}

void loop() {
  watch.loop();
  String currentTimeStr = watch.getTime();
  
  int klikKanan = btnKanan.check();
  int klikKiri = btnKiri.check();

  if (hasNotif && (millis() - notifStart < notifDuration)) {
    if (lastNotifApp == "Phone" || lastNotifApp == "Call") {
      isCallRinging = true;
    } else {
      isCallRinging = false;
    }
  } else {
    hasNotif = false;
    isCallRinging = false;
  }

  // Kontrol Tombol Kanan
  if (klikKanan == 1) {
    if (isCallRinging) {
      hasNotif = false;   
      isCallRinging = false;
    } else {
      watch.musicControl(MUSIC_TOGGLE); 
    }
  } else if (klikKanan == 2) {
    if (!isCallRinging) watch.musicControl(MUSIC_NEXT); 
  } else if (klikKanan == 3) {
    if (!isCallRinging) {
      currentVolume += 10; 
      if (currentVolume > 100) currentVolume = 100; 
      watch.setVolume(currentVolume);
    }
  }

  // Kontrol Tombol Kiri
  if (klikKiri == 1) {
    if (isCallRinging) {
      hasNotif = false;   
      isCallRinging = false;
    } else {
      watch.musicControl(MUSIC_TOGGLE); 
    }
  } else if (klikKiri == 2) {
    if (!isCallRinging) watch.musicControl(MUSIC_PREVIOUS); 
  } else if (klikKiri == 3) {
    if (!isCallRinging) {
      currentVolume -= 10; 
      if (currentVolume < 0) currentVolume = 0; 
      watch.setVolume(currentVolume);
    }
  }

  display.clearDisplay();

  if (hasNotif && (millis() - notifStart < notifDuration)) {
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0,0);

    if (lastNotifApp == "Phone" || lastNotifApp == "Call") {
      display.println("Incoming Call:");
      printWrappedText(lastNotifTitle, 0, 16, 1);
    } else {
      display.println("Notification:");
      display.println(lastNotifApp);
      printWrappedText(lastNotifTitle, 0, 16, 1);
      printWrappedText(lastNotifMessage, 0, 32, 1);
    }
  } else {
    showClock(currentTimeStr);
    updateRain();
    
    if (isConnected) {
      display.drawPixel(SCREEN_WIDTH - 5, 5, SSD1306_WHITE);
      display.drawPixel(SCREEN_WIDTH - 6, 6, SSD1306_WHITE);
      display.drawPixel(SCREEN_WIDTH - 4, 6, SSD1306_WHITE);
      display.drawPixel(SCREEN_WIDTH - 5, 7, SSD1306_WHITE);
    }
  }

  display.display();
}