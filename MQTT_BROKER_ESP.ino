#include <ESP8266WiFi.h>
#include <PubSubClient.h> //Biblioteca MQTT

#define MSG_Buffer_Size 50

const char* ssid = "Isabela";
const char* password = "230278isa";
const char* Broker_MQTT = "broker.mqtt-dashboard.com";
char Message[MSG_Buffer_Size];
int value = 0;

WiFiClient ESPClient; //Declaração do Objeto WiFi
PubSubClient client(ESPClient); //Declaração do objeto MQTT

//Declaração das funções
void Setup_WiFi();
void Callback(char* topic, byte* payload, unsigned int length);
void reconnect();

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200);
  Setup_WiFi();
  client.setServer(Broker_MQTT, 1883);
  client.setCallback(Callback);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  snprintf (Message, MSG_Buffer_Size, "hello world #%ld", value);
  Serial.print("Publish message: ");
  Serial.println(Message);
  client.publish("outTopic", Message);
  delay(5000);
  value++;
}

void Setup_WiFi(){
  delay(10);
  Serial.println("\nConectando ao WiFi...");
  Serial.print("SSID: ");
  Serial.println(ssid);
  WiFi.mode(WIFI_STA); //Modo Station
  WiFi.begin(ssid, password);
  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  Serial.println("\n WiFi conectado!");
  Serial.print("Endereço IP do ESP8266: ");
  Serial.println(WiFi.localIP());
}

void Callback(char* topic, byte* payload, unsigned int length){
  Serial.print("Mensagem[");
  Serial.print(topic);
  Serial.print("]");
  for (int i = 0; i < length; i++){
    Serial.print((char)payload[i]);
  }
  Serial.println();
  if ((char)payload[0] == 1) digitalWrite(LED_BUILTIN, HIGH);
  else digitalWrite(LED_BUILTIN, LOW);
}

void reconnect(){
  while(!client.connected()){
    Serial.print("Tentando Reconectar ao Broker...");
    String clientID = "ESP8266-";
    clientID += String(random(0xffff,HEX));
    if (client.connect(clientID.c_str())) {
      Serial.println("Conectado!");
      client.publish("outTopic", "hello world"); //Publica um tópico, depois te conectado
      client.subscribe("inTopic"); //Subscrevendo-se no tópico InTopic
    } else {
      Serial.print("falha, rc=");
      Serial.print(client.state());
      Serial.println(" Tentando Novamente em 2 segundos...");
      delay(2000);
    }
  }
}

