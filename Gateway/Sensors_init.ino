void DHT_init() {
  HTTP.on("/getTemperature.json", HTTP_GET, []() {
    String data = graf((float)(jsonReadtoInt(configJson, "Temperature")/100.0));
    jsonWrite(data, "refresh", getTimeChartRefresh("ChartRefreshDHT"));
    HTTP.send(200, "application/json", data);
  });
  HTTP.on("/getHumidity.json", HTTP_GET, []() {
    String data = graf(jsonReadtoInt(configJson, "Humidity"));
    jsonWrite(data, "refresh", getTimeChartRefresh("ChartRefreshDHT"));
    HTTP.send(200, "application/json", data);
  });
}

uint16_t getTimeChartRefresh(String name) {
  uint16_t _time = jsonReadtoInt(configSetup, name);
  if (_time >= 1000) {
    return _time;
  } else {
    return 10000;  
  }
}
