#include <ESP8266WiFi.h>
#include <PubSubClient.h>

const char* SSID = "iPhone de Roberto";
const char* PASSWORD = "OzbfiI7!lUTN2hZLmr!BHeh6uReIX&";
const char* BROKER_MQTT = "broker.hivemq.com";
int BROKER_PORT = 1883;
const char* TOPIC = "lewagon-iot-lamp";

WiFiClient espClient;
PubSubClient MQTT(espClient);


void initWiFi() {
  Serial.println("Conectando-se em: " + String(SSID));

  WiFi.begin(SSID, PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Serial.println("");
  Serial.println("WiFi conectado");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void initMQTT() {
  MQTT.setServer(BROKER_MQTT, BROKER_PORT);
  MQTT.setCallback(mqtt_callback);
}

void connectMQTT() {
  while (!MQTT.connected()) {
    Serial.println("Tentando se conectar ao Broker MQTT: " + String(BROKER_MQTT));
    if (MQTT.connect("clientId-lewagon-iot-lamp")) {
      Serial.println("Conectado");
      MQTT.subscribe(TOPIC);

    } else {
      Serial.println("Falha ao Conectar. Tentando novamente em 2 segundos");
      delay(2000);
    }
  }
}

void mqtt_callback(char* topic, byte* payload, unsigned int length) {

  String message;
  for (int i = 0; i < length; i++) {
    char c = (char)payload[i];
    message += c;
  }
  Serial.println("Tópico => " + String(topic) + " | Valor => " + String(message));
  Serial.flush();

  if (message == "on") {
    digitalWrite(D0, HIGH);
  } else {
    digitalWrite(D0, LOW);
  }

}

void setup() {
  Serial.begin(115200);
  pinMode(D0, OUTPUT);
  
  initWiFi();
  initMQTT();
}

void loop() {
 if (!MQTT.connected()) {
    connectMQTT();
  }
  MQTT.loop();
}
