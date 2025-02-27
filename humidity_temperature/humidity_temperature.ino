#include <Adafruit_Sensor.h>
#include <DHT.h>

// Define sensor type and pin
#define DHTPIN 4       // Connect data pin of DHT sensor to GPIO4
#define DHTTYPE DHT11  // Change to DHT11 if using DHT11 sensor

DHT dht(DHTPIN, DHTTYPE);

void setup() {
    Serial.begin(115200);
    Serial.println("DHT Sensor Test");
    
    dht.begin();
}

void loop() {
    float humidity = dht.readHumidity();
    float temperature = dht.readTemperature(); // Read temperature in Celsius

    if (isnan(humidity) || isnan(temperature)) {
        Serial.println("Failed to read from DHT sensor!");
        return;
    }

    Serial.print("Humidity: ");
    Serial.print(humidity);
    Serial.print("%  Temperature: ");
    Serial.print(temperature);
    Serial.println("°C");

    delay(2000); // Wait 2 seconds before next reading
}
