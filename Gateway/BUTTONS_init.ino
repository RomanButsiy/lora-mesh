
void BUTTONS_init(void) {
  HTTP.on("/ButtonsControl", HTTP_GET, []() {
    HTTP.send(200, "text/plain", "OK"); // Відправляємо відповідь про виконання
  });
  }
