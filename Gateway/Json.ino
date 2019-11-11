// ------------- Читання значення json
String jsonRead(String &json, String name) {
  DynamicJsonBuffer jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(json);
  return root[name].as<String>();
}

// ------------- Читання значення json
int jsonReadtoInt(String &json, String name) {
  DynamicJsonBuffer jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(json);
  return root[name];
}

// ------------- Запис значення json String
String jsonWrite(String &json, String name, String volume) {
  DynamicJsonBuffer jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(json);
  root[name] = volume;
  json = "";
  root.printTo(json);
  return json;
}

// ------------- Запис значення json int
String jsonWrite(String &json, String name, int volume) {
  DynamicJsonBuffer jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(json);
  root[name] = volume;
  json = "";
  root.printTo(json);
  return json;
}

void saveConfig (){
  writeFile("config.json", configSetup );
}

// ------------- Читання файлу у рядок
String readFile(String fileName, size_t len ) {
  File configFile = SPIFFS.open("/" + fileName, "r");
  if (!configFile) {
    return "Failed";
  }
  size_t size = configFile.size();
  if (size > len) {
    configFile.close();
    return "Large";
  }
  String temp = configFile.readString();
  configFile.close();
  return temp;
}

// ------------- Запис рядка у файл
String writeFile(String fileName, String strings ) {
  File configFile = SPIFFS.open("/" + fileName, "w");
  if (!configFile) {
    return "Failed to open config file";
  }
  configFile.print(strings);
  //strings.printTo(configFile);
  configFile.close();
  return "Write sucsses";
}

// ------------- Створення даних для графіка
String graf(int datas) {
  String root = "{}";
  DynamicJsonBuffer jsonBuffer;
  JsonObject& json = jsonBuffer.parseObject(root);
  JsonArray& data = json.createNestedArray("data");
  data.add(datas);
  root = "";
  json.printTo(root);
  return root;
}
// ------------- Створення даних для графіка
String graf(float datas) {
  String root = "{}";
  DynamicJsonBuffer jsonBuffer;
  JsonObject& json = jsonBuffer.parseObject(root);
  JsonArray& data = json.createNestedArray("data");
  data.add(datas);
  root = "";
  json.printTo(root);
  return root;
}
// ------------- Створення даних для графіка
String graf(int datas, int datas1) {
  String root = "{}";
  DynamicJsonBuffer jsonBuffer;
  JsonObject& json = jsonBuffer.parseObject(root);
  JsonArray& data = json.createNestedArray("data");
  JsonArray& data1 = json.createNestedArray("data1");
  data.add(datas);
  data1.add(datas1);
  root = "";
  json.printTo(root);
  return root;
}
// ------------- Створення даних для графіка
String graf(float datas, float datas1) {
  String root = "{}";
  DynamicJsonBuffer jsonBuffer;
  JsonObject& json = jsonBuffer.parseObject(root);
  JsonArray& data = json.createNestedArray("data");
  JsonArray& data1 = json.createNestedArray("data1");
  data.add(datas);
  data1.add(datas1);
  root = "";
  json.printTo(root);
  return root;
}
// ------------- Створення даних для графіка
String graf(float datas, float datas1, float datas2) {
  String root = "{}";
  DynamicJsonBuffer jsonBuffer;
  JsonObject& json = jsonBuffer.parseObject(root);
  // Заполняем поля json
  JsonArray& data = json.createNestedArray("data");
  JsonArray& data1 = json.createNestedArray("data1");
  JsonArray& data2 = json.createNestedArray("data2");
  data.add(datas);
  data1.add(datas1);
  data2.add(datas2);
  root = "";
  json.printTo(root);
  return root;
}
// ------------- Створення даних для графіка
String graf(int datas, int datas1, int datas2) {
  String root = "{}";
  DynamicJsonBuffer jsonBuffer;
  JsonObject& json = jsonBuffer.parseObject(root);
  JsonArray& data = json.createNestedArray("data");
  JsonArray& data1 = json.createNestedArray("data1");
  JsonArray& data2 = json.createNestedArray("data2");
  data.add(datas);
  data1.add(datas1);
  data2.add(datas2);
  root = "";
  json.printTo(root);
  return root;
}
