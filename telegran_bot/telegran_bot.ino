#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>

const char* ssid = "Airtel_gsk";
const char* password = "Setu7588";

#define BOTtoken "7750654810:AAF47kE_wf_ZgOe6rttjtfSlXTd3YLrlSyA"
#define CHAT_ID "1496040749"

WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);

int botRequestDelay = 1000;
unsigned long lastTimeBotRan;

const int ledPin = 21;
bool ledState = HIGH;

void NewMessagesHandle(int NewMessages) {
  Serial.println("NewMessagesHandle");
  Serial.println(String(NewMessages));

  for (int i=0; i<NewMessages; i++) {
    String chat_id = String(bot.messages[i].chat_id);
    // if (chat_id != CHAT_ID){
    //   bot.sendMessage(chat_id, "Unauthorized user", "");
    //   continue;
    // }
    
    String text = bot.messages[i].text;
    Serial.println(text);

    String from_name = bot.messages[i].from_name;

    if (text == "/start") {
      String welcome = "Welcome!\n";
      welcome += "Use following commands to control LED.\n\n";
      welcome += "/led_on to turn ON LED\n\n";
      welcome += "/led_off to turn OFF LED\n";
      bot.sendMessage(chat_id, welcome, "");
    }

    if (text == "/led_on") {
      bot.sendMessage(chat_id, "The LED is turned ON", "");
      ledState = LOW;
      digitalWrite(ledPin, ledState);
    }
    
    if (text == "/led_off") {
      bot.sendMessage(chat_id, "The LED is turned OFF", "");
      ledState = HIGH;
      digitalWrite(ledPin, ledState);
    }
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, ledState);
  WiFi.mode(WIFI_STA);                /*Set the WiFi in STA Mode*/
  WiFi.begin(ssid, password);
  Serial.print("Connecting...");
  Serial.println(ssid);
  delay(1000);                       /*Wait for 1000mS*/
  while(WiFi.waitForConnectResult() != WL_CONNECTED){Serial.print(".");}
  Serial.print("Connected!");
  client.setCACert(TELEGRAM_CERTIFICATE_ROOT);
}

void loop() {
  if (millis() > lastTimeBotRan + botRequestDelay)  {
    int NewMessages = bot.getUpdates(bot.last_message_received + 1);

    while(NewMessages) {
      NewMessagesHandle(NewMessages);
      NewMessages = bot.getUpdates(bot.last_message_received + 1);
    }
    lastTimeBotRan = millis();
  }
}