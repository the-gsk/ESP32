#include <WiFi.h>
#include <HTTPClient.h>

const char* ssid = "Airtel_gsk";          // Your WiFi SSID
const char* password = "Setu7588";  // Your WiFi Password

const char* serverName = "https://script.google.com/macros/s/AKfycbxhTiGzGKfcMdJgXH2cxG5aR2Zk8f4nbmLu-gTklu1UcIYIbRST4AGuCqFnUHIScntA9Q/exec";  // Replace with your Web App URL

int count = 0;

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  Serial.print("Connecting to WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected to WiFi");
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {  // Check WiFi connection status
    HTTPClient http;
    
    http.begin(serverName);
    http.addHeader("Content-Type", "application/json");

    // Convert the count into JSON format
    String jsonPayload = "{\"count\":" + String(count) + "}";
    
    int httpResponseCode = http.POST(jsonPayload);  // Send POST request

    if (httpResponseCode > 0) {
      String response = http.getString();  // Get response from server
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
      Serial.print("Response: ");
      Serial.println(response);
    } else {
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
    }

    http.end();  // Free resources

    count++;     // Increment count
    delay(2000); // Wait for 2 seconds
  } else {
    Serial.println("WiFi Disconnected. Reconnecting...");
    WiFi.reconnect();
  }
}
