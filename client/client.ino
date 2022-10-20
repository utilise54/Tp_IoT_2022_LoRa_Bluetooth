#include <WiFi.h>
#include <MQTT.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
String ipaddr = "";

WiFiClient net;
MQTTClient client;
unsigned long lastMillis = 0;

void waitConnect() {
  Serial.print("checking wifi...");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print("."); delay(1000);
  }
  Serial.print("\nconnecting...");
  while (!client.connect("IoT.DevKit")) {
    Serial.print("."); delay(1000);
  }
  Serial.println("\nconnected!");
}

void printMessage(String &topic, String &payload) {
  Serial.println("incoming: " + topic + " - " + payload);
}

// ---------------------------- GET IP
void setIp(String &topic, String &payload) {
  ipaddr = payload;
}

void getIpaddr() {
  Serial.println("getIpAddr...");
  WiFi.begin("tpmqtt", "........");
  client.begin("test.mosquitto.org", net);
  client.onMessage(setIp);
  waitConnect();
  client.subscribe("perrinekylian");
  Serial.println("waiting info");
  while (ipaddr == "") {
    client.loop();
    delay(10);
  }
  Serial.println("Server IP : " + ipaddr);
}

void sendcOk() {
  Serial.println("send Ok...");
  client.publish("perrinekylian/cOK", "OK");
  Serial.println("sended");
}



/// ---------------
void getLoRaData(){

  HTTPClient http;
  WiFi.disconnect();
  WiFi.begin("perrinekylian","perrinekylian");
  Serial.print("checking wifi...");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print("."); delay(1000);
  }
  Serial.print("\nconnecting...");
  http.begin(ipaddr);
  Serial.print("\nConnected...");
  int httpResponseCode = http.GET();

  if(httpResponseCode != 200){
    Serial.print(httpResponseCode);
  }
  else{
    Serial.println(http.getString());
    /*DynamicJsonDocument datas(24);
    deserializeJson(datas,http.getString());
    Serial.println("f");
    Serial.println((char *)datas["f"]);
    Serial.println("sf");
    Serial.println((char *)datas["sf"]);
    Serial.println("sb");
    Serial.println((char *)datas["sb"]);*/
    
  }



}
// Send C ok
void setup() {
  Serial.begin(9600);
  getIpaddr();
  sendcOk();
  getLoRaData();
}

void loop() {

}
