#define PIR_SENSOR_PIN  4  // Connect HC-SR501 OUT to D3
#define LED_PIN         7  // Connect an LED to D8

void setup() {
  Serial.begin(115200);
  pinMode(PIR_SENSOR_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  
  Serial.println("HC-SR501 Motion Sensor Initialized!");
}

void loop() {
  int motionDetected = digitalRead(PIR_SENSOR_PIN);
  Serial.println(motionDetected);

  if (motionDetected == HIGH) {
    Serial.println("⚠️ Motion Detected!");
    digitalWrite(LED_PIN, HIGH);  // Turn on LED
    delay(5000);  // Keep LED ON for 5 seconds
  } else {
    digitalWrite(LED_PIN, LOW);   // Turn off LED
  }

  delay(500);  // Small delay to reduce false triggers
}
