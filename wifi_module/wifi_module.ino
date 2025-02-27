#include <WiFi.h>
#include <WiFiClientSecure.h>
// #include <UniversalTelegramBot.h>
// #include <ArduinoJson.h>

// WiFi credentials (replace with your network's SSID and password)
const char* ssid = "Airtel_gsk";
const char* password = "Setu7588";

// Create a server object that listens on port 80 (HTTP)
WiFiServer server(80);
// Define the pin for the built-in LED on the ESP32
const int ledPin = 3;
// Variable to store the LED state (LOW = off, HIGH = on)
int ledState = LOW;

void setup() {
    Serial.begin(115200);        // Initialize serial communication at 115200 baud rate for debugging
    pinMode(ledPin, OUTPUT);     // Set the LED pin as an output
    digitalWrite(ledPin, ledState);  // Initialize the LED to be off
    WiFi.begin(ssid, password);  // Start connecting to WiFi with the specified credentials
    // Wait until the ESP32 is connected to the WiFi network
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);             // Wait for half a second
        Serial.print("Connecting...");      // Print dots in Serial Monitor as it tries to connect
    }
    // When connected, print the IP address assigned by the WiFi network
    Serial.println("\nConnected: " + WiFi.localIP().toString());
    server.begin();  // Start the server so it can listen for incoming clients
}

void loop() {
    // Check for an incoming client (e.g., browser or app sending HTTP request)
    WiFiClient client = server.available();
    if (!client) return;  // If no client is available, exit the loop and check again later

    // Read the client's request and store it in the `request` variable
    String request = client.readStringUntil('\r');
    client.flush();  // Clear the client's buffer to prepare for the next request

    // Check if the request contains "/LED=ON" or "/LED=OFF" and update the LED state accordingly
    if (request.indexOf("/LED=ON") != -1) {
        ledState = HIGH;  // Update the LED state to on
    } else if (request.indexOf("/LED=OFF") != -1) {
        ledState = LOW;   // Update the LED state to off
    }

    // Set the LED to the current state
    digitalWrite(ledPin, ledState);

    // Send an HTTP response back to the client with HTML content for a simple web interface
    client.println("HTTP/1.1 200 OK\r\nContent-type:text/html\r\n\r\n"   // HTTP headers
                   "<h1>GSK Home Automation</h1>"                          // HTML title on the page
                   "<a href=\"/LED=ON\"><button>ON</button></a>"         // HTML link to turn the LED on
                   "<a href=\"/LED=OFF\"><button>OFF</button></a>");     // HTML link to turn the LED off

    client.stop();  // Disconnect the client to free up the connection
}
