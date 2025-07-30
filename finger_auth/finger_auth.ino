#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_Fingerprint.h>

// Pin configs based on your setup:
#define R307_RX D1        // ESP32 RX <- Sensor TX
#define R307_TX D2        // ESP32 TX -> Sensor RX
#define OLED_SDA D4
#define OLED_SCL D5
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1


// User button for enroll (optional)
#define ENROLL_PIN D6      // D1

// Set number of fingerprint slots (adjust as needed)
#define MAX_FINGERPRINTS  10

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

HardwareSerial fingerSerial(1);    // UART1
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&fingerSerial);

void setup() {
  Serial.begin(115200);

  pinMode(ENROLL_PIN, INPUT);  // Button HIGH unless pressed

  // OLED
  Wire.begin(OLED_SDA, OLED_SCL);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay(); display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE); display.setCursor(0,0);
  display.println("Fingerprint Init");
  display.display();

  // Fingerprint serial
  fingerSerial.begin(57600, SERIAL_8N1, R307_RX, R307_TX);
  finger.begin(57600);
  delay(1500);

  if (finger.verifyPassword()) {
    display.clearDisplay(); display.setCursor(0,0);
    display.println("Sensor OK");/private/var/folders/14/dvrk7l894glbf9pqcqv3s9zc0000gn/T/.arduinoIDE-unsaved2025630-93348-1ebwbmb.ivkkk/sketch_jul30b/sketch_jul30b.ino
    Serial.println("Fingerprint sensor detected.");
    display.display();
  } else {
    display.clearDisplay(); display.setCursor(0,0);
    display.println("SENSOR FAIL"); Serial.println("Can't find sensor!");
    display.display();
    while (1) { delay(1000); }
  }
  delay(1000);
}

void loop() {
  // Serial.println("ENROLL_PIN");
  Serial.println(digitalRead(ENROLL_PIN));
  if (digitalRead(ENROLL_PIN) != LOW) {
    enrollFingerprint();
    delay(1000);
    return;
  }
  searchFingerprint();
  delay(300);
}

//-- Enroll finger routine --
void enrollFingerprint() {
  display.clearDisplay(); display.setCursor(0,0);
  display.println("Enroll mode"); display.display();
  Serial.println("Enter enroll mode. Place a finger...");

  int enroll_id = getFirstEmptyID();
  if (enroll_id < 1 || enroll_id > MAX_FINGERPRINTS) enroll_id = 1;    // fallback

  // 3-scan enroll process (per Adafruit example)
  int p = -1;
  while (p != FINGERPRINT_OK) {
    display.clearDisplay(); display.setCursor(0,0);
    display.print("Put finger #"); display.println(enroll_id); display.display();
    p = finger.getImage();
  }
  Serial.println("Image taken");

  p = finger.image2Tz(1);
  if (p != FINGERPRINT_OK) { showError(p, "Image2Tz 1"); return; }
  display.clearDisplay(); display.setCursor(0,0); display.println("Remove finger");
  display.display(); delay(1500);

  p = 0;
  while (p != FINGERPRINT_NOFINGER) p = finger.getImage();
  Serial.println("Ready for 2nd scan");

  p = -1;
  while (p != FINGERPRINT_OK) {
    display.clearDisplay(); display.setCursor(0,0);
    display.println("Place again...");
    display.display();
    p = finger.getImage();
  }
  Serial.println("Image 2 taken");

  p = finger.image2Tz(2);
  if (p != FINGERPRINT_OK) { showError(p, "Image2Tz 2"); return; }

  p = finger.createModel();
  if (p != FINGERPRINT_OK) { showError(p, "createModel"); return; }

  p = finger.storeModel(enroll_id);
  if (p == FINGERPRINT_OK) {
    display.clearDisplay(); display.setCursor(0,0);
    display.println("Enroll done!");
    display.print("Slot: "); display.println(enroll_id);
    display.display();
    Serial.print("Enrolled at ID "); Serial.println(enroll_id);
    delay(2000);
  } else {
    showError(p, "storeModel");
  }
}


//-- Search/Match Routine --
void searchFingerprint() {
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK) return;  // No finger or error
  display.clearDisplay(); display.setCursor(0,0);
  display.println("Image captured"); display.display();

  p = finger.image2Tz(1);
  if (p != FINGERPRINT_OK) { showError(p, "Image2Tz"); return; }

  p = finger.fingerFastSearch();
  if (p == FINGERPRINT_OK) {
    Serial.print("Match ID: "); Serial.println(finger.fingerID);
    Serial.print("Confidence: "); Serial.println(finger.confidence);

    display.clearDisplay(); display.setCursor(0,0);
    display.print("MATCH! ID: "); display.println(finger.fingerID);
    display.print("Conf: "); display.println(finger.confidence);
    display.display();
    delay(1500);
  } else if (p == FINGERPRINT_NOTFOUND) {
    display.clearDisplay(); display.setCursor(0,0);
    display.println("No match"); display.display();
    Serial.println("No match found.");
    delay(1000);
  } else {
    showError(p, "Search");
  }
}

//-- Helper: Find first empty slot for enrollment --
int getFirstEmptyID() {
  finger.getTemplateCount();
  for (int i = 1; i <= MAX_FINGERPRINTS; i++) {
    int r = finger.loadModel(i);
    if (r != FINGERPRINT_OK) return i;
  }
  return -1;
}

//-- Helper: Show error on display/serial --
void showError(uint8_t code, const char* msg) {
  display.clearDisplay(); display.setCursor(0,0);
  display.print(msg); display.print(" err:");
  display.println(code);
  display.display();
  Serial.print(msg); Serial.print(" error: "); Serial.println(code);
  delay(1500);
}
