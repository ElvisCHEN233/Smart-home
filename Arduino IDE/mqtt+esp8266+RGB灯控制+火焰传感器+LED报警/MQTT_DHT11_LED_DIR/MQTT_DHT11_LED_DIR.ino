#include <PubSubClient.h>
#include <ESP8266WiFi.h>
#define LED 2
WiFiClient espClient;
PubSubClient client(espClient);
int redpin = 14;
int bluepin =12;
int greenpin =13;
char val=100;
int Led=2;
int buttonpin=16;
int val2;
const char* ssid = "惟丶";
const char* password = "kl1314...";
const char* mqtt_server = "47.107.84.138";
long lastMsg = 0;
int value = 0;

void setup_wifi() {

  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
  
if ((char)payload[0] == '0') {
   val=0;
   analogWrite(14, val);
   analogWrite(12, val);
   analogWrite(13, val);  
  }
if ((char)payload[0] == '1') {
   val=28.4;
   analogWrite(14, val);
   analogWrite(12, val);
   analogWrite(13, val);  
  }

   if ((char)payload[0] == '2') {
   val=56.8;
   analogWrite(14, val);
   analogWrite(12, val);
   analogWrite(13, val);  
  }
  
  if ((char)payload[0] == '3') {
   val=85.2;
   analogWrite(14, val);
   analogWrite(12, val);
   analogWrite(13, val);  
  }
  if ((char)payload[0] == '4') {
   val=113.6;
   analogWrite(14, val);
   analogWrite(12, val);
   analogWrite(13, val);  
  }
  if ((char)payload[0] == '5') {
   val=142;
   analogWrite(14, val);
   analogWrite(12, val);
   analogWrite(13, val);  
  }  
  if ((char)payload[0] == '6') {
   val=170.4;
   analogWrite(14, val);
   analogWrite(12, val);
   analogWrite(13, val);  
  } 
   
  if ((char)payload[0] == '7') {
   val=198.8;
   analogWrite(14, val);
   analogWrite(12, val);
   analogWrite(13, val);  
  }

   if ((char)payload[0] == '8') {
   val=204;
   analogWrite(14, val);
   analogWrite(12, val);
   analogWrite(13, val);  
  }

   if ((char)payload[0] == '9') {
   val=255;
   analogWrite(14, val);
   analogWrite(12, val);
   analogWrite(13, val);  
  } 

void reconnect() {//回调函数
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      client.publish("outTopic", "hello world");
      client.subscribe("inTopic");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void setup() 
{
  Serial.begin(115200);
  pinMode(Led,OUTPUT);
  pinMode(buttonpin,INPUT);
   setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  reconnect();
  while(!client.subscribe("RGB",0))
  {Serial.println("订阅失败;尝试重新订阅！");
   client.subscribe("RGB",0);                 
   delay(300);
   }
   Serial.println("订阅成功~~~");
}

void loop() 
{
  if (!client.connected()) {
    reconnect();
  }
    client.loop();

  val2=digitalRead(buttonpin);
  if(val2==LOW)
  {
    digitalWrite(Led,HIGH);
    delay(100);
    digitalWrite(Led,LOW);
    delay(100);
  }
  else
  {
    digitalWrite(Led,HIGH);
  }
}
