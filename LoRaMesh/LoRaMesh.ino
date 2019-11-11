#include "DHT.h"
#include <EEPROM.h>
#include <RHRouter.h>
#include <RHMesh.h>
#include <RH_RF95.h>
#define RH_HAVE_SERIAL
#define N_NODES 4
#define DHTPIN 3 // Вивід підключення давача

uint8_t nodeId;
uint8_t routes[N_NODES]; // full routing table for mesh
int16_t rssi[N_NODES]; // signal strength info

// Singleton instance of the radio manager
RH_RF95 driver;

// Class to manage message delivery and receipt, using the manager declared above
RHMesh *manager;

// message buffer
char buf[RH_MESH_MAX_MESSAGE_LEN];

DHT dht(DHTPIN, DHT11);

int freeMem() {
  extern int __heap_start, *__brkval;
  int v;
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
}

void setup() {
  randomSeed(analogRead(0));
  Serial.begin(115200);
  while (!Serial) ; // Wait for serial port to be available

  nodeId = EEPROM.read(0);
  if (nodeId > 10) {
    Serial.print(F("EEPROM nodeId invalid: "));
    Serial.println(nodeId);
    nodeId = 1;
  }
  Serial.print(F("initializing node "));

  manager = new RHMesh(driver, nodeId);

  if (!manager->init()) {
    Serial.println(F("init failed"));
  } else {
    Serial.println("done");
  }
  driver.setTxPower(23, false);
  driver.setFrequency(915.0);
  driver.setCADTimeout(500);
  Serial.println("RF95 ready");

  for(uint8_t n=1;n<=N_NODES;n++) {
    routes[n-1] = 0;
    rssi[n-1] = 0;
  }
  dht.begin();
  Serial.print(F("mem = "));
  Serial.println(freeMem());
}

const __FlashStringHelper* getErrorString(uint8_t error) {
  switch(error) {
    case 1: return F("invalid length");
    break;
    case 2: return F("no route");
    break;
    case 3: return F("timeout");
    break;
    case 4: return F("no reply");
    break;
    case 5: return F("unable to deliver");
    break;
  }
  return F("unknown");
}

void updateRoutingTable() {
  for(uint8_t n=1;n<=N_NODES;n++) {
    RHRouter::RoutingTableEntry *route = manager->getRouteTo(n);
    if (n == nodeId) {
      routes[n-1] = 255; // self
    } else {
      routes[n-1] = route->next_hop;
      if (routes[n-1] == 0) {
        // if we have no route to the node, reset the received signal strength
        rssi[n-1] = 0;
      }
    }
  }
}

// Create a JSON string with the routing info to each node
void getRouteInfoString(char *p, size_t len) {
  p[0] = '\0';
  strcat(p, "[");
  for(uint8_t n=1;n<=N_NODES;n++) {
    strcat(p, "{\"n\":");
    sprintf(p+strlen(p), "%d", routes[n-1]);
    strcat(p, ",");
    strcat(p, "\"r\":");
    sprintf(p+strlen(p), "%d", rssi[n-1]);
    strcat(p, "}");
    if (n<N_NODES) {
      strcat(p, ",");
    }
  }
  strcat(p, "]");
}

void printNodeInfo(uint8_t node, char *s) {
  Serial.print(F("node: "));
  Serial.print(F("{"));
  Serial.print(F("\""));
  Serial.print(node);
  Serial.print(F("\""));
  Serial.print(F(": "));
  Serial.print(s);
  Serial.println(F("}"));
}

void sendDataFromDHT(){
  getDataFromDHT(buf, RH_MESH_MAX_MESSAGE_LEN);
  sendAck(1);
}

bool getDataFromDHT(char *p, size_t len) {
  int16_t t = dht.readTemperature()*100;
  uint8_t h = dht.readHumidity();
  p[0] = '\0';
    strcat(p, "{\"M\":");
    strcat(p, "\"DHT11\",\"t\":");
    sprintf(p+strlen(p), "%d", t);
    strcat(p, ",");
    strcat(p, "\"h\":");
    sprintf(p+strlen(p), "%d", h);
    strcat(p, "}");
  return true;
}

void receiveAckNextHop(uint8_t n) {
  Serial.println(F(" OK"));
  // we received an acknowledgement from the next hop for the node we tried to send to.
  RHRouter::RoutingTableEntry *route = manager->getRouteTo(n);
  if (route->next_hop != 0) {
    rssi[route->next_hop-1] = driver.lastRssi();
  }
}

void sendAck(uint8_t n) {
  Serial.print(F("->"));
  Serial.print(n);
  Serial.print(F(" :"));
  Serial.print(buf);
  if (nodeId == 1) printNodeInfo(nodeId, buf); // debugging
  // send an acknowledged message to the target node
  uint8_t error = manager->sendtoWait((uint8_t *)buf, strlen(buf), n);
  if (error != RH_ROUTER_ERROR_NONE) {
    Serial.println();
    Serial.print(F(" ! "));
    Serial.println(getErrorString(error));
  } else {
    receiveAckNextHop(n);
  }
}

void loop() {
    
  for(uint8_t n=1;n<=N_NODES;n++) {
    if (n == nodeId) continue; // self

    updateRoutingTable();
    getRouteInfoString(buf, RH_MESH_MAX_MESSAGE_LEN);
    sendAck(n);
    // listen for incoming messages. Wait a random amount of time before we transmit
    // again to the next node
    unsigned long nextTransmit = millis() + random(2000, 3000);
    while (nextTransmit > millis()) {
      int waitTime = nextTransmit - millis();
      uint8_t len = sizeof(buf);
      uint8_t from;
      if (manager->recvfromAckTimeout((uint8_t *)buf, &len, waitTime, &from)) {
        buf[len] = '\0'; // null terminate string
        Serial.print(from);
        Serial.print(F("->"));
        Serial.print(F(" :"));
        Serial.println(buf);
        if (nodeId == 1) printNodeInfo(from, buf); // debugging
        // we received data from node 'from', but it may have actually come from an intermediate node
        RHRouter::RoutingTableEntry *route = manager->getRouteTo(from);
        if (route->next_hop != 0) {
          rssi[route->next_hop-1] = driver.lastRssi();
        }
      }
    }
  }
  //sendDataFromDHT(); // comment for main and other nodes
}
