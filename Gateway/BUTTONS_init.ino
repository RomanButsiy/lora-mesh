
void BUTTONS_init(void) {
  HTTP.on("/ButtonsControl", HTTP_GET, []() {
    HTTP.send(200, "text/plain", "OK"); // Відправляємо відповідь про виконання
  });
  HTTP.on("/setChartRefreshDHT", HTTP_GET, []() {
    uint16_t data = HTTP.arg("set").toInt();
    if ((data >= 1000) && (data <= 65000)) {
      jsonWrite(configSetup, "ChartRefreshDHT", data);
      saveConfig();                 // Функція збереження даних у Flash
      HTTP.send(200, "text/plain", "OK"); // Відправляємо відповідь про виконання
    } else {
      HTTP.send(400, "text/plain", "Out of range"); // Відправляємо відповідь про виконання
    }
  });
  
  }
