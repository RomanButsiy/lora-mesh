/*
    Розробив Roman
    Канал на YouTube: https://goo.gl/x8FL2o
    2019 Roman
*/

#include <ESP8266HTTPUpdateServer.h>
#include <LiquidCrystal_I2C.h>
#include <ESP8266WebServer.h>
#include <PubSubClient.h>
#include <ESP8266WiFi.h> 
#include <ESP8266SSDP.h>     
#include <ArduinoJson.h>  // Ставимо через менеджер бібліотек
#include <RH_RF95.h>
#include <Wire.h>
#include <time.h>    
#include <FS.h> 

#define BUFSIZE RH_RF95_MAX_MESSAGE_LEN

//--------------------------------------------------------------------
//  Default data:
//--------------------------------------------------------------------
// Для виводу інформації на LCD дисплей
LiquidCrystal_I2C lcd(0x27, 16, 2);
// Об'єкт для оновлення web сторінки
ESP8266HTTPUpdateServer httpUpdater;
// Web інтерфейс для пристрою
ESP8266WebServer HTTP(80);
// Для файлової системи
File fsUploadFile;
// Для MQTT сервера
WiFiClient espClient;
PubSubClient mqtt_client(espClient);

String configSetup = "{}";
String configJson = "{}";
bool mqtt_State = false;
void S_log(const char *s, int line, bool lclear = true);
void setup() {
  gateway_init();
}

void loop() {
  HTTP.handleClient();
  mqtt_listener();
}
