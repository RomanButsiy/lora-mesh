
void gateway_init(void) {
  Serial.begin(115200);
  while (!Serial);
  lcd_init();
  S_log("Start FS", 0);
  FS_init();
  delay(900);
  S_log("...OK", 1, false);
  delay(300);
  S_log("Step FileConfig", 0);
  configSetup = readFile("config.json", 4096);
  jsonWrite(configJson, "SSDP", jsonRead(configSetup, "SSDP"));
  delay(900);
  S_log(configSetup.length() > 15 ? "...OK" : configSetup.c_str(), 1, false);
  delay(300);
  S_log("Start WIFI", 0);
  WIFIinit();
  S_log("Start Time", 0);
  Time_init();
  delay(900);
  S_log("...OK", 1, false);
  delay(300);
  S_log("Start SSDP", 0);
  SSDP_init();
  delay(900);
  S_log("...OK", 1, false);
  delay(300);
  S_log("Start WebServer", 0);
  HTTP_init();
  delay(900);
  S_log("...OK", 1, false);
  delay(300);
  MQTT_init();
  LANG_init();
  BUTTONS_init();
  S_log("Time Ready!", 0);
  S_log(GetTime().c_str(), 1, false);
  delay(1700);
  p_localIP();
}