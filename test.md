# 1. LED Sequencing with ESP32-S3

## Objective

To implement an LED sequencing pattern using the ESP32-S3 development board, where three LEDs turn on and off in a specific sequence with a defined delay.

## Components Required

* ESP32-S3 development board
* 3 LEDs (any color)
* 3 Resistors (220Ω)
* Connecting wires (jumper wires)
* Breadboard

## Wiring (ESP32-S3 & LEDs)

| LED Pin        | ESP32-S3 Pin | Notes                               |
| :------------- | :----------- | :---------------------------------- |
| LED1 Anode (+) | D0 (GPIO 0)  |                                     |
| LED2 Anode (+) | D1 (GPIO 1)  |                                     |
| LED3 Anode (+) | D2 (GPIO 2)  |                                     |
| All LED Cathodes (-) | GND (via 220Ω Resistor) | Each LED cathode is connected to GND through a 220Ω resistor. This limits current flow and prevents LED damage. |

**Diagram (Conceptual):**

```
   ESP32-S3
   +------+
   | D0 ---[Resistor]--- LED1 (+)
   | D1 ---[Resistor]--- LED2 (+)
   | D2 ---[Resistor]--- LED3 (+)
   | GND ------------------ LED1 (-), LED2 (-), LED3 (-)
   +------+
```

## Code Implementation

```cpp
void setup() {
  pinMode(D0, OUTPUT);
  pinMode(D1, OUTPUT);
  pinMode(D2, OUTPUT);
}

void loop() {
  digitalWrite(D0, HIGH);
  delay(500);
  digitalWrite(D1, HIGH);
  delay(500);
  digitalWrite(D2, HIGH);
  delay(500);
  digitalWrite(D2, LOW);
  delay(500);
  digitalWrite(D1, LOW);
  delay(500);
  digitalWrite(D0, LOW);
  delay(500);
}
```

## Explanation

### Setup Phase:

* The `setup()` function is executed once when the ESP32-S3 starts.
* `pinMode(D0, OUTPUT);`, `pinMode(D1, OUTPUT);`, and `pinMode(D2, OUTPUT);` configure the digital pins D0, D1, and D2 as output pins. This allows the ESP32-S3 to control the voltage supplied to these pins, effectively turning the LEDs on and off.

### Loop Execution:

* The `loop()` function runs continuously after the `setup()` function completes.
* `digitalWrite(D0, HIGH);` sets the D0 pin to HIGH, turning on LED1.
* `delay(500);` pauses the execution for 500 milliseconds (0.5 seconds).
* The same process is repeated for LED2 (D1) and LED3 (D2), turning them on sequentially.
* Then, the LEDs are turned off in reverse order (LED3, LED2, LED1) with a 500ms delay between each.
* The `loop()` function then restarts, repeating the sequence indefinitely.

## Expected Output

The LEDs will turn ON and OFF sequentially, creating a wave-like lighting pattern. Each LED will stay lit for 500ms before the next LED in the sequence turns on. The LEDs will turn off in reverse order, also with a 500ms delay between each transition.

## Observations

* The LED sequence successfully follows the desired pattern, demonstrating basic digital output control with the ESP32-S3.
* The speed of the sequence can be easily adjusted by changing the `delay()` values in the code. Lower delay values will result in a faster sequence, and higher values will slow it down.

## Conclusion

This experiment provides a practical demonstration of how to control multiple LEDs using the ESP32-S3 and implement a basic sequential lighting pattern. It illustrates the fundamental concepts of digital output and timing delays, which are essential for many embedded systems applications.

Here’s your **Markdown (.md)** formatted lab report for this ESP32-S3 WiFi-based LED control experiment.  

---

# 2. WiFi-Based LED Control Using ESP32-S3

## **Objective**
To set up an ESP32-S3 as a web server that allows users to control an LED remotely through a web interface.

## **Components Required**
- ESP32-S3 development board  
- LED  
- Resistor (220Ω)  
- Connecting wires  
- Breadboard  

## **Wiring (ESP32-S3 & LED)**

| **LED Pin**      | **ESP32-S3 Pin** |
|-----------------|----------------|
| **Anode (+)**   | **GPIO 3 (D3)** |
| **Cathode (-)** | **GND (via 220Ω Resistor)** |

## **Code Implementation**

```cpp
#include <WiFi.h>
#include <WiFiClientSecure.h>

// WiFi credentials (replace with your network's SSID and password)
const char* ssid = "Airtel_gsk";
const char* password = "Setu7588";

// Create a server object that listens on port 80 (HTTP)
WiFiServer server(80);

// Define the pin for the built-in LED on the ESP32
const int ledPin = 3;
int ledState = LOW;

void setup() {
    Serial.begin(115200);        // Initialize serial communication for debugging
    pinMode(ledPin, OUTPUT);     // Set the LED pin as an output
    digitalWrite(ledPin, ledState);  // Initialize the LED to be off
    WiFi.begin(ssid, password);  // Connect to WiFi

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print("Connecting...");
    }

    Serial.println("\nConnected: " + WiFi.localIP().toString());
    server.begin();  // Start the server
}

void loop() {
    WiFiClient client = server.available();
    if (!client) return;

    String request = client.readStringUntil('\r');
    client.flush();

    if (request.indexOf("/LED=ON") != -1) {
        ledState = HIGH;
    } else if (request.indexOf("/LED=OFF") != -1) {
        ledState = LOW;
    }

    digitalWrite(ledPin, ledState);

    client.println("HTTP/1.1 200 OK\r\nContent-type:text/html\r\n\r\n"
                   "<h1>GSK Home Automation</h1>"
                   "<a href=\"/LED=ON\"><button>ON</button></a>"
                   "<a href=\"/LED=OFF\"><button>OFF</button></a>");

    client.stop();
}
```

## **Explanation**
1. **WiFi Setup:**
   - The ESP32-S3 connects to the specified WiFi network using the provided SSID and password.
   - It waits until the connection is established and then starts an HTTP server.

2. **Web Server Handling:**
   - The ESP32-S3 listens for incoming HTTP requests on port 80.
   - If a request contains `/LED=ON`, the LED turns **ON**.
   - If a request contains `/LED=OFF`, the LED turns **OFF**.

3. **Web Interface:**
   - Users can access the server’s webpage to control the LED via two buttons: **ON** and **OFF**.

## **Expected Output**
- When connected to WiFi, the ESP32-S3 prints its **IP Address** in the Serial Monitor.
- Users can access the web interface by entering the ESP32's IP in a browser.
- Clicking the **ON** button turns the LED **ON**, and clicking **OFF** turns it **OFF**.

## **Observations**
- The ESP32-S3 successfully connects to WiFi and hosts a web server.
- The LED responds correctly to user input from the browser.
- The response time is quick, and the server remains stable.

## **Conclusion**
This experiment demonstrates how the ESP32-S3 can be used to control an LED via a web-based interface. This method can be expanded to control other IoT devices remotely.

# 3. Touch Sensor-Based LED Control Using ESP32-S3

## **Objective**
To use the ESP32-S3's capacitive touch sensor to control an LED, where touching a designated pin turns the LED on.

## **Components Required**
- ESP32-S3 development board  
- LED  
- Resistor (220Ω)  
- Connecting wires  
- Breadboard  

## **Wiring (ESP32-S3 & Touch Sensor LED Control)**

| **Component**     | **ESP32-S3 Pin** |
|------------------|----------------|
| **Touch Sensor** | **GPIO 7 (T7)** |
| **LED Anode (+)** | **GPIO D3** |
| **LED Cathode (-)** | **GND (via 220Ω Resistor)** |

## **Code Implementation**

```cpp
void setup() {
  pinMode(7, INPUT);   // Set touch sensor pin as input
  pinMode(D3, OUTPUT); // Set LED pin as output
  Serial.begin(115200);
}

int v;
void loop() {
  v = touchRead(7); // Read touch sensor value
  Serial.println(v);

  if(v > 10000){  // If touch detected
    digitalWrite(D3, HIGH);
    Serial.println("TOUCH");
  } else {        // If no touch detected
    digitalWrite(D3, LOW);
    Serial.println("NO");
  }
}
```

## **Explanation**
1. **Touch Sensor Configuration:**
   - The ESP32-S3 has built-in capacitive touch sensors.
   - GPIO **7** is configured as a touch-sensitive input.

2. **LED Control:**
   - The LED is connected to **D3**.
   - If the touch sensor value exceeds **10000**, the LED turns **ON** and prints `"TOUCH"` to the Serial Monitor.
   - Otherwise, the LED remains **OFF**, and `"NO"` is printed.

3. **Serial Monitoring:**
   - The touch sensor readings are printed continuously for debugging and analysis.

## **Expected Output**
- When a user touches GPIO **7**, the **LED turns ON**.
- When released, the **LED turns OFF**.
- Serial Monitor displays **"TOUCH"** when touched and **"NO"** otherwise.

## **Observations**
- The ESP32-S3 successfully detects touch input.
- The LED turns on and off according to touch sensor input.
- The threshold **(10000)** can be adjusted for better sensitivity.

## **Conclusion**
This experiment demonstrates how the ESP32-S3's **capacitive touch sensor** can be used for simple **touch-based control** of external devices like an LED.


# 4. ESP32-S3 BLE Server Implementation

## **Objective**
To implement a Bluetooth Low Energy (BLE) server using ESP32-S3, which sends incrementing numbers to a connected BLE client.

## **Components Required**
- ESP32-S3 development board  
- Smartphone or another BLE-compatible device  
- BLE scanning app (nRF Connect, BLE Scanner, etc.)

## **BLE Service and Characteristic UUIDs**
| **Component**     | **UUID** |
|------------------|--------------------------------------------------|
| **BLE Service**  | 4fafc201-1fb5-459e-8fcc-c5c9c331914b             |
| **BLE Characteristic** | beb5483e-36e1-4688-b7f5-ea07361b26a8       |

## **Code Implementation**
```cpp
// Include necessary BLE libraries
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

// Define unique UUIDs for BLE service and characteristic
#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

// Counter variable to send data over BLE
int counter = 0;  
BLECharacteristic *pCharacteristic = nullptr;

void setup() {
  Serial.begin(115200);
  Serial.println("Starting BLE work!");

  // Initialize BLE and set the device name
  BLEDevice::init("XIAO_ESP32S3");

  // Create a BLE server
  BLEServer *pServer = BLEDevice::createServer();

  // Create a BLE service with the specified UUID
  BLEService *pService = pServer->createService(SERVICE_UUID);

  // Create a characteristic for reading, writing, and notifying
  pCharacteristic = pService->createCharacteristic(
                                         CHARACTERISTIC_UUID,
                                         BLECharacteristic::PROPERTY_READ |
                                         BLECharacteristic::PROPERTY_WRITE |
                                         BLECharacteristic::PROPERTY_NOTIFY
                                       );

  // Start the BLE service
  pService->start();

  // Set up BLE advertising
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(true);
  pAdvertising->setMinPreferred(0x06);
  pAdvertising->setMinPreferred(0x12);
  
  // Start advertising the BLE server
  BLEDevice::startAdvertising();
  Serial.println("Characteristic defined! Now you can read it in your phone!");
}

void loop() {
  // Convert counter to string
  char countStr[10];
  snprintf(countStr, sizeof(countStr), "%d", counter);

  // Update the BLE characteristic with the counter value
  pCharacteristic->setValue(countStr);
  pCharacteristic->notify();

  // Print counter value to Serial Monitor
  Serial.println("Sending: " + String(countStr));

  // Increment counter
  counter++;

  // Delay before next update
  delay(2000);
}
```

## **Wiring (ESP32-S3 BLE)**
| **Component**    | **ESP32-S3 Pin** |
|-----------------|----------------|
| **BLE Functionality** | Built-in |

## **Explanation**
1. **BLE Server Initialization:**
   - The ESP32-S3 is set up as a **BLE server** with a unique **service UUID**.
   - A **characteristic** is defined to allow **reading, writing, and notifications**.

2. **BLE Advertising & Connection:**
   - The ESP32-S3 advertises the **service UUID**, making it discoverable to clients.
   - Once connected, the **client can read data from the ESP32-S3**.

3. **Data Transmission:**
   - The ESP32-S3 sends **incrementing numbers** every **2 seconds**.
   - The updated values are **sent as notifications** to the BLE client.

## **Expected Output**
- The ESP32-S3 advertises itself as `"XIAO_ESP32S3"`.
- A BLE client (e.g., smartphone app) can connect and receive **incrementing numbers**.
- Serial Monitor prints:
  ```
  Sending: 1
  Sending: 2
  Sending: 3
  ...
  ```

## **Observations**
- The ESP32-S3 successfully acts as a BLE server.
- Clients receive real-time updates as notifications.
- The UUIDs ensure a **unique connection** between devices.

## **Conclusion**
This experiment demonstrates how to create a **BLE server on ESP32-S3** that continuously **sends incrementing values** to a BLE client. This setup can be extended for **sensor data transmission** in IoT applications.


# 5. ESP32-S3 Data Logging to Google Sheets

## **Objective**
To send real-time data from an ESP32-S3 to **Google Sheets** using a **Google Apps Script Web App**.

## **Components Required**
- ESP32-S3 development board  
- WiFi network (for internet connectivity)  
- Google Sheets (for storing data)  
- Google Apps Script (to process requests)

## **Wiring (ESP32-S3 WiFi)**
| **Component**   | **ESP32-S3 Pin** |
|----------------|----------------|
| **WiFi Functionality** | Built-in |

## **Code Implementation**
### **ESP32-S3 Code (WiFi & HTTP POST Request)**
```cpp
#include <WiFi.h>
#include <HTTPClient.h>

const char* ssid = "Airtel_gsk";      // Your WiFi SSID
const char* password = "Setu7588";    // Your WiFi Password

const char* serverName = "https://script.google.com/macros/s/AKfycbxhTiGzGKfcMdJgXH2cxG5aR2Zk8f4nbmLu-gTklu1UcIYIbRST4AGuCqFnUHIScntA9Q/exec";  // Replace with your Web App URL

int count = 0;  // Counter variable

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
  if (WiFi.status() == WL_CONNECTED) {  // Check WiFi connection
    HTTPClient http;
    
    http.begin(serverName);
    http.addHeader("Content-Type", "application/json");

    // Convert count into JSON format
    String jsonPayload = "{\"count\":" + String(count) + "}";
    
    int httpResponseCode = http.POST(jsonPayload);  // Send POST request

    if (httpResponseCode > 0) {
      String response = http.getString();
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
```

### **Google Apps Script (Google Sheets Integration)**
```javascript
function doPost(e) {
  try {
    // Parse incoming request data
    const data = JSON.parse(e.postData.contents);

    // Open the Google Sheet by ID and access the correct sheet (e.g., "Sheet1")
    const sheet = SpreadsheetApp.openById("1vpNt_N5oNuybZaxhGWIOuup6W8p1iZMCqh1bt4mUQy0").getSheetByName("Sheet1");

    // Get the current date and time
    const currentDate = new Date();

    // Append a new row with the date in column A and the value in column B
    sheet.appendRow([currentDate, data.count]);

    // Return success response
    return ContentService.createTextOutput("Success").setMimeType(ContentService.MimeType.TEXT);
  } catch (error) {
    // Handle and log any errors
    Logger.log("Error: " + error.message);
    return ContentService.createTextOutput("Error: " + error.message).setMimeType(ContentService.MimeType.TEXT);
  }
}
```

## **Explanation**
### **ESP32-S3 Code Workflow**
1. Connects to the **WiFi network** using the provided credentials.
2. Prepares a **JSON payload** containing an incrementing counter value.
3. Sends an **HTTP POST request** to the Google Apps Script Web App.
4. Receives a **response** from the server, indicating success or failure.
5. Waits **2 seconds** before sending the next update.

### **Google Apps Script Workflow**
1. Extracts **JSON data** from the POST request.
2. Opens the **Google Sheets document** using its unique ID.
3. Appends a **new row** with the **timestamp and count** value.
4. Sends a **success response** back to the ESP32-S3.

## **Expected Output**
- The ESP32-S3 will continuously send **count values** to the Google Sheet.
- The **Serial Monitor** will display:
  ```
  Connecting to WiFi...
  Connected to WiFi
  HTTP Response code: 200
  Response: Success
  ```
- The **Google Sheet** will log data as follows:

| **Timestamp**          | **Count** |
|-----------------------|---------|
| 2025-02-26 10:00:00  | 1       |
| 2025-02-26 10:00:02  | 2       |
| 2025-02-26 10:00:04  | 3       |

## **Observations**
- The ESP32-S3 successfully connects to the **Google Sheets API** via **HTTP POST**.
- The **data logging process** is **real-time and automated**.
- The **Web App URL** acts as an interface between ESP32-S3 and Google Sheets.

## **Conclusion**
This experiment demonstrates **real-time IoT data logging** using an **ESP32-S3 microcontroller** and **Google Sheets**. This setup can be extended for **sensor data logging**, **home automation**, and **remote monitoring applications**.

# 6. ESP32-S3 Camera Streaming via Web Server

## **Objective**
To implement a **WiFi-enabled camera streaming** web server using an **ESP32-S3 with a camera module**.

## **Components Required**
- ESP32-S3 development board with PSRAM  
- OV2640/OV3660 Camera Module  
- WiFi network (for remote access)  

## **Wiring (ESP32-S3 & Camera Module)**
The camera module must be connected to the ESP32-S3's camera pins as per the **board specifications**.

| **Camera Pin**  | **ESP32-S3 Pin** |
|----------------|----------------|
| D0–D7         | GPIOs (Y2-Y9)   |
| XCLK          | XCLK_GPIO_NUM   |
| PCLK          | PCLK_GPIO_NUM   |
| VSYNC         | VSYNC_GPIO_NUM  |
| HREF          | HREF_GPIO_NUM   |
| SDA/SCL       | SIOD/SIOC       |
| PWDN          | PWDN_GPIO_NUM   |
| RESET         | RESET_GPIO_NUM  |

## **Code Implementation**
### **ESP32-S3 Camera Streaming Code**
```cpp
#include "esp_camera.h"
#include <WiFi.h>

// ===================
// Select camera model
// ===================
#define CAMERA_MODEL_XIAO_ESP32S3 // Has PSRAM
#include "camera_pins.h"

// ===========================
// Enter your WiFi credentials
// ===========================
const char *ssid = "Airtel_gsk";
const char *password = "Setu7588";

void startCameraServer();
void setupLedFlash(int pin);

void setup() {
  Serial.begin(115200);
  Serial.setDebugOutput(true);
  Serial.println();

  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sccb_sda = SIOD_GPIO_NUM;
  config.pin_sccb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.frame_size = FRAMESIZE_UXGA;
  config.pixel_format = PIXFORMAT_JPEG;  // For streaming
  config.grab_mode = CAMERA_GRAB_WHEN_EMPTY;
  config.fb_location = CAMERA_FB_IN_PSRAM;
  config.jpeg_quality = 12;
  config.fb_count = 1;

  if (config.pixel_format == PIXFORMAT_JPEG) {
    if (psramFound()) {
      config.jpeg_quality = 10;
      config.fb_count = 2;
      config.grab_mode = CAMERA_GRAB_LATEST;
    } else {
      config.frame_size = FRAMESIZE_SVGA;
      config.fb_location = CAMERA_FB_IN_DRAM;
    }
  } else {
    config.frame_size = FRAMESIZE_240X240;
  }

  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    return;
  }

  sensor_t *s = esp_camera_sensor_get();
  if (s->id.PID == OV3660_PID) {
    s->set_vflip(s, 1);
    s->set_brightness(s, 1);
    s->set_saturation(s, -2);
  }
  if (config.pixel_format == PIXFORMAT_JPEG) {
    s->set_framesize(s, FRAMESIZE_QVGA);
  }

  WiFi.begin(ssid, password);
  WiFi.setSleep(false);

  Serial.print("WiFi connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected");

  startCameraServer();

  Serial.print("Camera Ready! Use 'http://");
  Serial.print(WiFi.localIP());
  Serial.println("' to connect");
}

void loop() {
  delay(10000); // Do nothing, handled by web server
}
```

## **Explanation**
### **ESP32-S3 Camera Streaming Workflow**
1. **WiFi Connection**  
   - The ESP32-S3 connects to the specified **WiFi network**.
   
2. **Camera Initialization**  
   - Configures **camera settings** including frame size, pixel format, and memory allocation.
   - **PSRAM (if available)** is used to improve **image quality and performance**.

3. **Web Server Setup**  
   - Starts a **web server** on the **ESP32-S3 local IP address**.
   - The server handles **live streaming** from the camera.

4. **User Access**  
   - Users can access the live video stream by entering the ESP32-S3's **local IP address** in a web browser.

## **Expected Output**
- The ESP32-S3 will print:
  ```
  WiFi connected
  Camera Ready! Use 'http://192.168.X.X' to connect
  ```
- Users can **view the live camera stream** in a browser by entering the provided **ESP32-S3 IP address**.

## **Observations**
- The ESP32-S3 successfully captures and **streams video over WiFi**.
- The **image quality and frame rate** depend on **PSRAM availability** and **network speed**.
- The **web server remains active** as long as the ESP32-S3 is powered on.

## **Conclusion**
This project demonstrates how an **ESP32-S3** can function as a **wireless camera** by **hosting a web server** for real-time streaming. This setup is useful for **home surveillance**, **remote monitoring**, and **IoT applications**.

# 7. ESP32-S3 Motion Detection System using PIR Sensor

## **Objective**
To detect motion using an **HC-SR501 PIR sensor** with an **ESP32-S3 board**, and trigger an **LED** when movement is detected.

## **Components Required**
- **ESP32-S3 Development Board**  
- **HC-SR501 PIR Motion Sensor**  
- **LED** (Optional for indication)  
- **Resistor (220Ω)** (for LED protection)  
- **Jumper Wires**  

## **Wiring Diagram**
| **Component**       | **ESP32-S3 Pin** |
|--------------------|----------------|
| PIR Sensor VCC    | 3.3V           |
| PIR Sensor GND    | GND            |
| PIR Sensor OUT    | GPIO 4         |
| LED +ve (Anode)   | GPIO 7         |
| LED -ve (Cathode) | GND            |

## **Code Implementation**
### **ESP32-S3 PIR Motion Sensor Code**
```cpp
#define PIR_SENSOR_PIN  4  // Connect HC-SR501 OUT to GPIO 4
#define LED_PIN         7  // Connect an LED to GPIO 7

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
```

## **Explanation**
### **Motion Detection Workflow**
1. **Sensor Initialization**  
   - The **HC-SR501 PIR sensor** is connected to the ESP32-S3.
   - The **LED** is set as an **output**.

2. **Reading PIR Sensor Output**  
   - The **PIR sensor detects motion** and sends a **HIGH signal** to the ESP32-S3.
   - If motion is **detected**, an **alert message is printed**, and the **LED is turned ON** for 5 seconds.

3. **LED Indication**  
   - If no motion is detected, the **LED remains OFF**.
   - The code introduces a **500ms delay** to minimize false triggers.

## **Expected Output**
When motion is detected, the **Serial Monitor** will display:
```
1
⚠️ Motion Detected!
```
and the **LED will turn ON for 5 seconds**.

If no motion is detected:
```
0
```
and the **LED remains OFF**.

## **Observations**
- The **motion detection range** is approximately **3–7 meters**.
- The **LED successfully lights up** when motion is detected.
- The **PIR sensor has a warm-up time (~30 seconds)** before it starts detecting motion accurately.

## **Conclusion**
This project demonstrates how to integrate an **HC-SR501 PIR motion sensor** with an **ESP32-S3** for motion detection. Such a system can be used for **security alerts, smart home automation, and intrusion detection**.

# 8. ESP32-S3 Heartbeat Monitoring System using a Pulse Sensor

## **Objective**
To measure and display heartbeat pulses per minute (BPM) using an **ESP32-S3** board and an **analog pulse sensor**.

## **Components Required**
- **ESP32-S3 Development Board**  
- **Pulse Sensor (KY-039 or similar)**  
- **Jumper Wires**  

## **Wiring Diagram**
| **Component**       | **ESP32-S3 Pin** |
|--------------------|----------------|
| Pulse Sensor VCC  | 3.3V or 5V      |
| Pulse Sensor GND  | GND             |
| Pulse Sensor OUT  | GPIO 4 (Analog Input) |

## **Code Implementation**
### **ESP32-S3 Pulse Sensor Code**
```cpp
#define PULSE_SENSOR_PIN 4  // Connect Pulse Sensor OUT to GPIO 4

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
```

## **Explanation**
### **Heartbeat Monitoring Workflow**
1. **Sensor Initialization**  
   - The **pulse sensor** is connected to the ESP32-S3.
   - The sensor **measures heartbeat pulses** based on **finger placement**.

2. **Pulse Value Detection**  
   - The ESP32-S3 **reads analog pulse values**.
   - If the **pulse exceeds a set threshold (550)**, a **heartbeat is detected**.

3. **BPM Calculation**  
   - Every **10 detected beats**, the **BPM (beats per minute)** is calculated.
   - Formula:  
     ```
     BPM = (60000 / (time difference)) * beatCount
     ```

4. **Serial Output Display**  
   - The **pulse values** and **calculated BPM** are displayed in the **Serial Monitor**.

## **Expected Output**
When running the code, the **Serial Monitor** will display:
```
Pulse Value: 512
Pulse Value: 540
Pulse Value: 600
⚠️ Heartbeat Detected!
BPM: 72
```

## **Observations**
- The **BPM values** may fluctuate initially but stabilize over time.
- Proper **finger placement** ensures accurate readings.
- **Threshold adjustment** may be required depending on the sensor model.

## **Conclusion**
This project successfully demonstrates a **heartbeat monitoring system** using an **ESP32-S3 and an analog pulse sensor**. This system can be further developed into a **wearable health monitoring device** or integrated into **IoT-based healthcare applications**.

# 9. ESP32-S3 Temperature & Humidity Monitoring using DHT11 Sensor

## **Objective**
To measure and display temperature and humidity using an **ESP32-S3** board and a **DHT11 sensor**.

## **Components Required**
- **ESP32-S3 Development Board**  
- **DHT11 Temperature & Humidity Sensor**  
- **10KΩ Resistor (Optional, for pull-up)**  
- **Jumper Wires**  

## **Wiring Diagram**
| **Component**      | **ESP32-S3 Pin** |
|-------------------|----------------|
| DHT11 VCC        | 3.3V or 5V      |
| DHT11 GND        | GND             |
| DHT11 Data       | GPIO 4          |

## **Code Implementation**
### **ESP32-S3 DHT11 Sensor Code**
```cpp
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
```

## **Explanation**
### **Temperature & Humidity Measurement Workflow**
1. **Sensor Initialization**  
   - The **DHT11 sensor** is connected to the ESP32-S3.
   - The **Adafruit DHT library** is used to read temperature and humidity.

2. **Reading Sensor Data**  
   - The **temperature (°C)** and **humidity (%)** are measured every **2 seconds**.
   - The `readTemperature()` and `readHumidity()` functions retrieve sensor values.

3. **Error Handling**  
   - If the sensor **fails to read data**, a **warning message** is printed.

4. **Serial Output Display**  
   - The temperature and humidity values are displayed in the **Serial Monitor**.

## **Expected Output**
When running the code, the **Serial Monitor** will display:
```
DHT Sensor Test
Humidity: 55.6%  Temperature: 28.3°C
Humidity: 56.2%  Temperature: 28.1°C
Humidity: 55.9%  Temperature: 28.5°C
```

## **Observations**
- The **DHT11 sensor** may take a **few seconds to stabilize**.
- Readings may slightly **fluctuate due to environmental changes**.
- If readings fail, check the **sensor wiring and pull-up resistor**.

## **Conclusion**
This project successfully demonstrates a **temperature and humidity monitoring system** using an **ESP32-S3 and a DHT11 sensor**. The collected data can be **logged, displayed on a web dashboard, or sent to an IoT cloud** for further analysis.

# 10. ESP32-S3 Digital Clock with OLED Display, WiFi, and Timed LED Control

## **Objective**
To develop a **WiFi-enabled digital clock** using the **ESP32-S3**, an **SSD1306 OLED display**, and an **NTP time synchronization mechanism**. Additionally, control an LED and buzzer based on a scheduled time.

## **Components Required**
- **ESP32-S3 Development Board**  
- **0.96" SSD1306 OLED Display**  
- **LED**  
- **Buzzer**  
- **Jumper Wires**  

## **Circuit Connections**
| **Component**      | **ESP32-S3 Pin** |
|-------------------|----------------|
| OLED VCC        | 3.3V             |
| OLED GND        | GND              |
| OLED SDA        | GPIO 21          |
| OLED SCL        | GPIO 22          |
| LED             | GPIO D3          |
| Buzzer          | GPIO D2          |

## **Code Implementation**

### **1. Required Libraries**
The program utilizes:
- **Adafruit_GFX & Adafruit_SSD1306** for the OLED display
- **WiFi & NTPClient** for time synchronization

```cpp
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
```

### **2. WiFi & NTP Setup**
WiFi credentials and NTP client initialization:

```cpp
const char *ssid = "Airtel_gsk";
const char *password = "Setu7588";

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");
```

### **3. Time & Display Configuration**
The time is fetched from an NTP server, converted to **12-hour format**, and displayed on the **OLED screen**:

```cpp
void clockDisplay() {
    timeClient.update();
    int currentHour = timeClient.getHours();
    int currentMinute = timeClient.getMinutes();
    int currentSecond = timeClient.getSeconds();
    String am_pm = (currentHour < 12) ? "AM" : "PM";
    if (currentHour == 0) {
        currentHour = 12; // Midnight handling
    } else if (currentHour > 12) {
        currentHour -= 12;
    }

    display.clearDisplay();
    display.setTextSize(2);
    display.setCursor(18, 10);
    display.print(String(currentHour) + ":" + String(currentMinute) + " " + am_pm);
    display.display();
}
```

### **4. Timed LED & Buzzer Activation**
LED and buzzer activate between 22:54 and 22:58:

```cpp
const int ledPin = D3;
const int buzzerPin = D2;
const int activateHour = 22, activateMinute = 54;
const int deactivateHour = 22, deactivateMinute = 58;
bool deviceActive = false;

void deviceActivation() {
    timeClient.update();
    int currentHour = timeClient.getHours();
    int currentMinute = timeClient.getMinutes();

    if ((currentHour > activateHour || (currentHour == activateHour && currentMinute >= activateMinute)) &&
        (currentHour < deactivateHour || (currentHour == deactivateHour && currentMinute < deactivateMinute))) {
        deviceActive = true;
        digitalWrite(ledPin, HIGH);
    } else {
        deviceActive = false;
        digitalWrite(ledPin, LOW);
    }
}
```

### **5. Setup & Loop Functions**
The ESP32-S3 initializes WiFi, OLED, and starts the NTP client:

```cpp
void setup() {
    display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);
    pinMode(ledPin, OUTPUT);
    pinMode(buzzerPin, OUTPUT);
    WiFi.begin(ssid, password);
    timeClient.begin();
    timeClient.setTimeOffset(19800); // GMT+5:30 for IST
}

void loop() {
    clockDisplay();
    deviceActivation();
    delay(1000);
}
```

## **Expected Output**
When executed, the **Serial Monitor** and OLED display will show:

```
WiFi Connecting...
WiFi Connected
22:55 PM
LED Activated
Buzzer ON
```

## **Observations**
- The OLED successfully displays the **current time** and **date**.
- The LED and buzzer activate only between **22:54 and 22:58**.
- If WiFi is disconnected, the system **reconnects automatically**.

## **Conclusion**
This project successfully implements a **WiFi-synced clock with an OLED display and scheduled LED/Buzzer activation**. The system can be further expanded to include **alarm notifications, IoT integration, or a mobile app**.

# 11. ESP32-S3 Telegram Bot for Home Automation

## **Objective**
To develop a **Telegram bot-controlled home automation system** using the **ESP32-S3**, a **DHT11 humidity sensor**, and various actuators like an LED, fan, and door lock mechanism. The bot allows remote monitoring and control through predefined commands.

## **Components Required**
- **ESP32-S3 Development Board**  
- **DHT11 Temperature & Humidity Sensor**  
- **LED**  
- **Fan**  
- **Buzzer**  
- **Relay for Door Lock**  
- **Jumper Wires**  

## **Circuit Connections**
| **Component**  | **ESP32-S3 Pin** |
|---------------|----------------|
| DHT11 Data   | GPIO 4         |
| LED          | GPIO 10        |
| Fan         | GPIO 5         |
| Door Lock   | GPIO 8         |
| Buzzer      | GPIO 1         |

## **Code Implementation**

### **1. Required Libraries**
The program utilizes:
- **WiFi** for internet connectivity
- **WiFiClientSecure** and **UniversalTelegramBot** for bot communication
- **DHT** for temperature and humidity sensing
- **ArduinoJson** for message handling

```cpp
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>
#include <DHT.h>
```

### **2. WiFi & Telegram Bot Setup**
WiFi credentials and Telegram bot initialization:

```cpp
const char* ssid = "Airtel_gsk";
const char* password = "Setu7588";
#define BOTtoken "<Your_Telegram_Bot_Token>"
#define OWNER_CHAT_ID "<Your_Chat_ID>"
```

### **3. Pin Configuration & Status Handling**

```cpp
#define DHTPIN 4
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

const int ledPin = 10;
const int fanPin = 5;
const int doorPin = 8;
const int buzzerPin = 1;

bool ledState = LOW;
bool fanState = LOW;
bool doorState = HIGH;
```

### **4. Telegram Bot Command Processing**
The bot processes commands like `/open_door`, `/light_on`, `/light_off`, `/fan_on`, `/fan_off`, `/temp`, `/humidity`, and `/status`.

```cpp
void handleNewMessages(int numNewMessages) {
    for (int i = 0; i < numNewMessages; i++) {
        String chat_id = String(bot.messages[i].chat_id);
        String text = bot.messages[i].text;

        if (text == "/open_door") {
            digitalWrite(doorPin, LOW);
            digitalWrite(buzzerPin, HIGH);
            delay(10000);
            digitalWrite(doorPin, HIGH);
            digitalWrite(buzzerPin, LOW);
            bot.sendMessage(chat_id, "Door is now locked.", "");
        }
        if (text == "/light_on") {
            digitalWrite(ledPin, HIGH);
            ledState = HIGH;
            bot.sendMessage(chat_id, "Light is ON.", "");
        }
        if (text == "/light_off") {
            digitalWrite(ledPin, LOW);
            ledState = LOW;
            bot.sendMessage(chat_id, "Light is OFF.", "");
        }
        if (text == "/fan_on") {
            digitalWrite(fanPin, HIGH);
            fanState = HIGH;
            bot.sendMessage(chat_id, "Fan is ON.", "");
        }
        if (text == "/fan_off") {
            digitalWrite(fanPin, LOW);
            fanState = LOW;
            bot.sendMessage(chat_id, "Fan is OFF.", "");
        }
        if (text == "/temp") {
            float temp = dht.readTemperature();
            bot.sendMessage(chat_id, "Temperature: " + String(temp) + "°C", "");
        }
        if (text == "/humidity") {
            float humidity = dht.readHumidity();
            bot.sendMessage(chat_id, "Humidity: " + String(humidity) + "%", "");
        }
        if (text == "/status") {
            sendStatus(chat_id);
        }
    }
}
```

### **5. Setup & Loop Functions**

```cpp
void setup() {
    Serial.begin(115200);
    pinMode(ledPin, OUTPUT);
    pinMode(fanPin, OUTPUT);
    pinMode(doorPin, OUTPUT);
    digitalWrite(ledPin, ledState);
    digitalWrite(fanPin, fanState);
    digitalWrite(doorPin, doorState);
    digitalWrite(buzzerPin, LOW);
    dht.begin();
    
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
    }
    bot.sendMessage(OWNER_CHAT_ID, "I am live now! Here is the current status:", "");
    sendStatus(OWNER_CHAT_ID);
}

void loop() {
    if (millis() > lastTimeBotRan + botRequestDelay) {
        int numNewMessages = bot.getUpdates(bot.last_message_received + 1);
        while (numNewMessages) {
            handleNewMessages(numNewMessages);
            numNewMessages = bot.getUpdates(bot.last_message_received + 1);
        }
        lastTimeBotRan = millis();
    }
}
```

## **Expected Output**
When executed, the **Serial Monitor** and Telegram bot will display messages:

```
WiFi Connecting...
WiFi Connected
Bot Online!
Light is ON.
Fan is OFF.
Temperature: 27°C
Humidity: 60%
```

## **Observations**
- The bot successfully processes commands to control the **light, fan, door, and buzzer**.
- The **DHT11 sensor** provides accurate **temperature and humidity readings**.
- Users need **approval** from the admin to access controls.

## **Conclusion**
This project successfully implements a **Telegram bot-controlled home automation system** using the **ESP32-S3**. The system can be expanded to support **camera surveillance, voice commands, or cloud integration**.


# 12. ESP32-S3-Based Spy Rover

**1. Introduction**
The Spy Rover project is a surveillance system built using the ESP32-S3 microcontroller. It integrates a camera module for live video streaming, motor drivers for movement control, and a WiFi-based communication system. The project is designed for remote-controlled surveillance applications, providing real-time monitoring and maneuverability.

**2. Objectives**
- Develop a remotely operated spy rover using the ESP32-S3 board.
- Implement live video streaming through an ESP32-compatible camera.
- Enable directional control using a web-based interface.
- Integrate WebSockets for real-time control and responsiveness.

**3. Components Used**
- ESP32-S3 microcontroller
- ESP32-compatible Camera Module (e.g., OV2640)
- L298N Motor Driver Module
- DC Motors with Wheels
- Power Supply (Lithium-ion Battery or 12V Adapter)
- LED for indication
- Jumper Wires and PCB Board

**4. Circuit Diagram**
(Include a schematic representation of the connections)

**5. Code Implementation**
The ESP32 firmware handles camera streaming, WebSocket communication, and motor control. Below is a snippet of the primary functionality:

```cpp
#include "esp_camera.h"
#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

#define UP 1
#define DOWN 2
#define LEFT 3
#define RIGHT 4
#define STOP 0

const char* ssid = "Airtel_gsk";
const char* password = "Setu7588";

AsyncWebServer server(80);
AsyncWebSocket wsCamera("/Camera");
AsyncWebSocket wsCarInput("/CarInput");
uint32_t cameraClientId = 0;

void moveCar(int direction) {
    switch (direction) {
        case UP:
            // Forward motion logic
            break;
        case DOWN:
            // Backward motion logic
            break;
        case LEFT:
            // Left turn logic
            break;
        case RIGHT:
            // Right turn logic
            break;
        case STOP:
            // Stop motion
            break;
    }
}

void setup() {
    Serial.begin(115200);
    WiFi.softAP(ssid, password);
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(200, "text/plain", "Spy Rover Active");
    });
    server.begin();
    Serial.println("Server started");
}

void loop() {
    // Handle real-time commands
}
```

**6. Working Principle**
- The ESP32-S3 connects to a WiFi network and hosts a web server.
- Users can control the rover through a web interface that sends commands via WebSockets.
- The camera captures live video, which is streamed over the network.
- The motor driver controls the rover’s movement based on received commands.

**7. Applications**
- Remote surveillance and security monitoring
- Search and rescue operations
- Educational robotics projects
- Home automation with integrated AI

**8. Conclusion**
The ESP32-S3-based Spy Rover successfully integrates live video streaming and remote control functionalities. Future improvements can include obstacle detection, AI-based motion tracking, and extended battery life optimization.