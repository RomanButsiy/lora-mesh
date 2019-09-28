
void WIFIinit() {
  //--------------------Для збереження даних WIFI
  HTTP.on("/ssid", HTTP_GET, []() {
  jsonWrite(configSetup, "ssid", HTTP.arg("ssid"));
  jsonWrite(configSetup, "password", HTTP.arg("password"));
  saveConfig();                 // Функція збереження даних у Flash
  HTTP.send(200, "text/plain", "OK"); // Відправляємо відповідь про виконання
  });
  //--------------------Для збереження даних точки достопу
  HTTP.on("/ssidap", HTTP_GET, []() {
  jsonWrite(configSetup, "ssidAP", HTTP.arg("ssidAP"));
  jsonWrite(configSetup, "passwordAP", HTTP.arg("passwordAP"));
  saveConfig();                 // Функція збереження даних у Flash
  HTTP.send(200, "text/plain", "OK"); // Відправляємо відповідь про виконання
  });


  // Спроба встановити з'єднання з точкою доступу
   WiFi.mode(WIFI_STA);
  byte tries = 11;
  String _ssid = jsonRead(configSetup, "ssid");
  String _password = jsonRead(configSetup, "password");
  if (_ssid == "" && _password == "") {
    WiFi.begin();
  }
  else {
    WiFi.begin(_ssid.c_str(), _password.c_str());
  }
  S_log(".", 1, false);
  // Робимо перевірку підключення до тих пір поки лічильник tries
  while (--tries && WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Q_log(".");
  }
  if (WiFi.status() != WL_CONNECTED) {
    // Якщо не вдалося підключитися запускаємо в режимі AP
    StartAPMode();
  } else {
    Q_log("OK");
    delay(300);
    S_log(_ssid.c_str(), 0);
    S_log("is connected", 1, false);
  }
  delay(700);
}

void p_localIP(void) {
  S_log("IP address:", 0);
  S_log(WiFi.localIP().toString().c_str(), 1, false);
}

bool StartAPMode() {
  IPAddress apIP(192, 168, 0, 1);
  // Відключаємо WIFI
  WiFi.disconnect();
  // Міняємо режим на режим точки доступу
  WiFi.mode(WIFI_AP);
  // Задаем настройки сети
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
  // Включаємо WIFI в режимі точки доступу з ім'ям і паролем
  // які зберігаються у змінних _ssidAP _passwordAP
  String _ssidAP = jsonRead(configSetup, "ssidAP");
  String _passwordAP = jsonRead(configSetup, "passwordAP");
  WiFi.softAP(_ssidAP.c_str(), _passwordAP.c_str());
  S_log(".......FAILED!!!", 1, false);
  delay(500);
  S_log("Created an AP:", 0);
  S_log(_ssidAP.c_str(), 1, false);
  delay(700);
  return true;
}
