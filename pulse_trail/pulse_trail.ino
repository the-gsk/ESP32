#define PULSE_SENSOR_PIN 4  // Connect HC-SR501 OUT to D3

int pulseValue = 0;
unsigned long lastBeatTime = 0;
int beatCount = 0;
float bpm = 0.0;

void setup() {
  Serial.begin(115200);
  pinMode(PULSE_SENSOR_PIN, INPUT);
  Serial.println("Heartbeat Pulse Sensor Initialized!");
}

void loop() {
  pulseValue = analogRead(PULSE_SENSOR_PIN);  // Read pulse sensor data
  Serial.print("Pulse Value: ");
  Serial.println(pulseValue);

  if (pulseValue > 550) { // Adjust threshold based on your sensor
    unsigned long currentTime = millis();
    if (currentTime - lastBeatTime > 600) {  // Prevent false triggers
      beatCount++;
      lastBeatTime = currentTime;
    }
  }

  // Calculate BPM every 10 beats
  if (beatCount >= 10) {
    bpm = (60000.0 / (millis() - lastBeatTime)) * beatCount;
    Serial.print("BPM: ");
    Serial.println(bpm);
    beatCount = 0;  // Reset beat count
  }

  delay(100);  // Small delay to smooth readings
}
