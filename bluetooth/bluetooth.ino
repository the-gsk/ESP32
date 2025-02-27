// Server Code
#include <BLEDevice.h>       // Library for BLE device setup
#include <BLEUtils.h>        // Utility functions for BLE
#include <BLEServer.h>       // Library for creating BLE server

// Define the unique UUIDs for the BLE service and characteristic
#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

// Initialize a counter to send incrementing numbers over BLE
int counter = 0;  
BLECharacteristic *pCharacteristic = nullptr;  // Pointer to the characteristic

void setup() {
  // Initialize the serial monitor for debugging
  Serial.begin(115200);
  Serial.println("Starting BLE work!");

  // Initialize BLE and set the device name
  BLEDevice::init("XIAO_ESP32S3");

  // Create a BLE server
  BLEServer *pServer = BLEDevice::createServer();

  // Create a BLE service with the specified UUID
  BLEService *pService = pServer->createService(SERVICE_UUID);

  // Create a characteristic within the service, allowing read, write, and notify operations
  pCharacteristic = pService->createCharacteristic(
                                         CHARACTERISTIC_UUID,
                                         BLECharacteristic::PROPERTY_READ |
                                         BLECharacteristic::PROPERTY_WRITE |
                                         BLECharacteristic::PROPERTY_NOTIFY
                                       );

  // Start the BLE service
  pService->start();

  // Set up advertising for the BLE server
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);  // Advertise the service UUID
  pAdvertising->setScanResponse(true);         // Include response to scanning requests
  pAdvertising->setMinPreferred(0x06);         // Helps with iPhone connection issues
  pAdvertising->setMinPreferred(0x12);         // Additional compatibility setting
  
  // Start advertising the BLE server
  BLEDevice::startAdvertising();
  Serial.println("Characteristic defined! Now you can read it in your phone!");
}

void loop() {
  // Convert the counter to a string and set it as the characteristic value
  char countStr[10];  // Buffer to store the counter as a string
  snprintf(countStr, sizeof(countStr), "%d", counter);  // Convert integer to string
  
  // Update the BLE characteristic with the counter value
  pCharacteristic->setValue(countStr);

  // Send a notification to the client with the updated characteristic value
  pCharacteristic->notify();

  // Print the current counter value to the Serial Monitor for debugging
  Serial.println("Sending: " + String(countStr));

  // Increment the counter for the next notification
  counter++;

  // Delay to control the notification frequency (e.g., every 2 seconds)
  delay(2000);
}


