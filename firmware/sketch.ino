#include <WiFi.h>
#include <MQTT.h>
#include "DHTesp.h"

const char ssid[] = "Wokwi-GUEST";
const char pass[] = "";

WiFiClient net;
MQTTClient client;
DHTesp dhtSensor;

const int PIN_RED = 25;
const int PIN_GREEN = 26;
const int PIN_BLUE = 27;
const int PIN_API = 14;
const int PIN_ALARM = 12;
const int PIN_DHT = 15;

bool api, dataApiLama = 0;
String suhu, dataSuhuLama = "0";

void connect(){
  rgb(1,0,0);
  while(WiFi.status() != WL_CONNECTED){
    delay(500);
  }
  rgb(0,1,0);

  while(!client.connect("arsip-tamu", "kelasiot", "passwordkelasiot")){
    delay(500);
  }
  rgb(0,0,1);
}

void setup() {
  pinMode(PIN_RED, OUTPUT);
  pinMode(PIN_GREEN, OUTPUT);
  pinMode(PIN_BLUE, OUTPUT);
  pinMode(PIN_API, INPUT_PULLUP);
  pinMode(PIN_ALARM, OUTPUT);
  dhtSensor.setup(PIN_DHT, DHTesp::DHT22);

  WiFi.begin(ssid, pass);
  client.begin("kelasiot.cloud.shiftr.io", net);

  connect();
}

void loop() {
  TempAndHumidity data = dhtSensor.getTempAndHumidity();

  api = digitalRead(PIN_API);
  suhu = String(data.temperature, 2);

  if(suhu != dataSuhuLama){
    client.publish("kelasiot/kebakaran/arsip/tamu/suhu", suhu, true, 1);
    dataSuhuLama = suhu;
  }

  if(api != dataApiLama){
    client.publish("kelasiot/kebakaran/arsip/tamu/api", String(api), true, 1);
    dataApiLama = api;
  }

  alarm(api);

  delay(10);
}

void rgb(bool red, bool green, bool blue){
  digitalWrite(PIN_RED, red);
  digitalWrite(PIN_GREEN, green);
  digitalWrite(PIN_BLUE, blue);
}

void alarm(bool api){
  if(api == 0){
    digitalWrite(PIN_ALARM, 1);
  } else {
    digitalWrite(PIN_ALARM, 0);
  }
}