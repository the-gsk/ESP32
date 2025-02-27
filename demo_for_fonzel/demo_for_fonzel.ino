#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>
#include <DHT.h>

// const char* ssid = "Airtel_gsk";
// const char* password = "Setu7588";

const char* ssid = "ESOFS-Corporate";
const char* password = "Compactor@987$";

#define BOTtoken "7750654810:AAF47kE_wf_ZgOe6rttjtfSlXTd3YLrlSyA"
#define OWNER_CHAT_ID "1496040749"

#define DHTPIN 4
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);

int botRequestDelay = 1000;
unsigned long lastTimeBotRan;

const int ledPin = D10;
const int fanPin = D5;
const int doorPin = D8;
const int buzzerPin = D1;

bool ledState = LOW;
bool fanState = LOW;
bool doorState = HIGH;

struct User {
  String chat_id;
  bool approved;
};

std::vector<User> pendingRequests;
std::vector<String> approvedUsers;

void sendStatus(String chat_id) {
  float temp = dht.readTemperature();
  float humidity = dht.readHumidity();
  String status = "Device Status:\n";
  status += "\nDoor: " + String(doorState ? "Locked" : "Open");
  status += "\nLight: " + String(ledState ? "On" : "Off");
  status += "\nFan: " + String(fanState ? "On" : "Off");
  status += "\nTemperature: " + String(temp) + "°C";
  status += "\nHumidity: " + String(humidity) + "%";
  bot.sendMessage(chat_id, status, "");
}

void welcomeMessage(String chat_id) {
  String welcome_msg = "Welcome! to `GSK` World!\n";
  welcome_msg += "Use the following commands:\n\n";
  
  welcome_msg += "/help - Display a list of available commands and their descriptions.\n";
  welcome_msg += "/open\\_door - Activate the door opening mechanism.\n";
  welcome_msg += "/light\\_on - Turn on the connected light.\n";
  welcome_msg += "/light\\_off - Turn off the connected light.\n";
  welcome_msg += "/fan\\_on - Turn on the connected fan.\n";
  welcome_msg += "/fan\\_off - Turn off the connected fan.\n";
  welcome_msg += "/temp - Retrieve and display the current temperature reading.\n";
  welcome_msg += "/humidity - Retrieve and display the current humidity reading.\n";
  welcome_msg += "/status - Display the current status of all connected devices (door, lights, fan, sensor readings).\n";
  welcome_msg += "/log\\_off - Terminate the current session or disconnect from the system.\n\n";

  welcome_msg += "For support, contact: [@gauravshankarkumar](https://t.me/gauravshankarkumar)";
  
  bot.sendMessage(chat_id, welcome_msg, "Markdown");
}

void handleNewMessages(int numNewMessages) {
  for (int i = 0; i < numNewMessages; i++) {
    String chat_id = String(bot.messages[i].chat_id);
    String text = bot.messages[i].text;
    String from_name = bot.messages[i].from_name;

    if (chat_id != OWNER_CHAT_ID && std::find(approvedUsers.begin(), approvedUsers.end(), chat_id) == approvedUsers.end()) {
      pendingRequests.push_back({chat_id, false});
      bot.sendMessage(OWNER_CHAT_ID, "New user request: " + from_name + " (ID: " + chat_id + ")\nUse /approve_" + chat_id + " to approve or /reject_" + chat_id + " to reject.", "");
      bot.sendMessage(chat_id, "Your request is pending approval.", "");
      continue;
    }

    if (text.startsWith("/approve_") && chat_id == OWNER_CHAT_ID) {
      String user_id = text.substring(9);
      approvedUsers.push_back(user_id);
      bot.sendMessage(user_id, "You are now approved.", "");
      welcomeMessage(user_id);
      continue;
    }

    if (text.startsWith("/reject_") && chat_id == OWNER_CHAT_ID) {
      String user_id = text.substring(8);
      bot.sendMessage(user_id, "Your request was rejected. You may request again.", "");
      continue;
    }

    if (text == "/start" || text == "/help") {
      welcomeMessage(chat_id);
    }

    if (text == "/open_door") {
      bot.sendMessage(chat_id, "Door is opening for 10 seconds...", "");
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
    if (text == "/log_off") {
      approvedUsers.erase(std::remove(approvedUsers.begin(), approvedUsers.end(), chat_id), approvedUsers.end());
      bot.sendMessage(chat_id, "You have been logged out.", "");
    }
    if (text == "/ApprovalList" && chat_id == OWNER_CHAT_ID) {
      String list = "Pending Approval Users:\n";
      for (User u : pendingRequests) {
        list += "ID: " + u.chat_id + "\n";
      }
      bot.sendMessage(chat_id, list, "");
    }
  }
}

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
  Serial.print("Connecting...");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }
  Serial.println("Connected!");
  client.setCACert(TELEGRAM_CERTIFICATE_ROOT);
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
