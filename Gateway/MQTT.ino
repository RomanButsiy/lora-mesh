
void MQTT_init(void) {
   //--------------------Для збереження даних MQTT
  HTTP.on("/mqtt", HTTP_GET, []() {
  jsonWrite(configSetup, "mqttState", HTTP.arg("mqtt-state").toInt());
  jsonWrite(configSetup, "mqttPort", HTTP.arg("mqtt-port").toInt());
  jsonWrite(configSetup, "mqttServer", HTTP.arg("mqtt-server"));
  jsonWrite(configSetup, "mqttUser", HTTP.arg("mqtt-user"));
  jsonWrite(configSetup, "mqttPass", HTTP.arg("mqtt-pass"));
  saveConfig();                 // Функція збереження даних у Flash
    HTTP.send(200, "text/plain", "OK"); // Відправляємо відповідь про виконання
  });
   //--------------------Перепідключити MQTT
  HTTP.on("/mqtt_reconnect", HTTP_GET, []() {
    mqtt_State = true;
    if (mqtt_start()) {
      HTTP.send(200, "text/plain", "OK"); // Відправляємо відповідь про виконання
    } else {
      HTTP.send(503, "text/plain", "Service Unavailable"); // Відправляємо відповідь про помилку
    }
    p_localIP();
  });
  mqtt_State = jsonReadtoInt(configSetup, "mqttState");
  if (mqtt_State) {
    mqtt_start();
  }
}

bool mqtt_start(void) {
  String _server = jsonRead(configSetup, "mqttServer");
  int _port = jsonReadtoInt(configSetup, "mqttPort");
  mqtt_client.setServer(_server.c_str(), _port);
  return mqtt_connect();
}

void mqtt_listener(void) {
  if (mqtt_State) {
    if (!mqtt_client.connected()) {
      if (!mqtt_connect()) {
        return;
      }
    }
    mqtt_client.loop();
    if (Serial.available()) {
      Serial.setTimeout(100);
      String _serial = Serial.readStringUntil('\n');
      if (_serial.startsWith("node: ")) {
        bool node = (_serial.indexOf("M") == -1);
        node = node && (_serial.indexOf("r") != -1);
        node = node && (_serial.indexOf("n") != -1);
        if (node) {
          char msg[BUFSIZE];
          int end = _serial.indexOf('\r');
          if (end > 0) {
            snprintf (msg, BUFSIZE, _serial.substring(6, end).c_str());
          } else {
            snprintf (msg, BUFSIZE, _serial.substring(6).c_str());
          }
          mqtt_client.publish("mesh_gateway/data", msg);
        }
      } else {
        readDHT(_serial);
      }
    }
  } else {
    if (Serial.available()) {
      Serial.setTimeout(100);
      String _serial = Serial.readStringUntil('\n');
      if (!_serial.startsWith("node: ")) {
        readDHT(_serial);
      }
    }
  }
}

void readDHT(String &_serial) {
  // ->1 :{"M":"DHT11","t":2060,"h":66}
  if (_serial.indexOf("DHT11") != -1) {
    uint8_t found = _serial.indexOf(':') + 1;
    _serial = _serial.substring(found);
    jsonWrite(configJson, "Temperature", jsonReadtoInt(_serial, "t"));
    jsonWrite(configJson, "Humidity", jsonReadtoInt(_serial, "h"));
  }
}

// Connect to MQTT broker
bool mqtt_connect() {
  byte tries = 5;
  String _clientId = jsonRead(configSetup, "SSDP");
  String _username = jsonRead(configSetup, "mqttUser");
  String _password = jsonRead(configSetup, "mqttPass");
  // Loop until we're reconnected
  S_log("MQTT connecting", 0);
  S_log("", 1, false);
  while (!mqtt_client.connected() && --tries) {
    // Attempt to connect
    if (mqtt_client.connect(_clientId.c_str(), _username.c_str(), _password.c_str())) {
      Q_log("connected");
      delay(1000);
      mqtt_State = true;
      return true;
    } else {
      Q_log(".");
    }
  }
  Q_log("failed!!!");
  delay(1000);
  mqtt_State = false;
  return false;
}
