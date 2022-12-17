#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <SimpleDHT.h>
const char* ssid = "惟丶";
const char* password = "kl1314...";
const char* mqtt_server = "47.107.84.138";
SimpleDHT11 dht11(5);
WiFiClient espClient；
PubSubClient client(espClient);
long lastMsg = 0;
char temp[50];
char hum[50];
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
  if ((char)payload[0] == '1') {
    digitalWrite(2, LOW);   
   
  }
  if ((char)payload[0] == '0'){
    digitalWrite(2, HIGH);  
  }

}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("outTopic", "hello world");
      // ... and resubscribe
      client.subscribe("inTopic");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup() {
  pinMode(2, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  reconnect();
  while(!client.subscribe("led",0))
  {Serial.println("订阅失败;尝试重新订阅！");
   client.subscribe("led",0);                 
   delay(300);
   }
   Serial.println("订阅成功~~~");
}

void loop() {
  byte temperature = 0;
  byte humidity = 0;
  int err = SimpleDHTErrSuccess;
  if((err = dht11.read(&temperature,&humidity, NULL))!= SimpleDHTErrSuccess){
    Serial.print("Read DHT11 failed, err=");
    Serial.println(err);
    delay(1000);
    return;
  }

  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  long now = millis();
  if (now - lastMsg > 5000) {
    lastMsg = now;
    ++value;
    snprintf (temp, 50, "%ld", (int)temperature);
    snprintf (hum, 50, "%ld", (int)humidity);
    Serial.println(temp);
    Serial.println(hum);
    Serial.print("\n");
    client.publish("temp", temp);
    client.publish("hum", hum);
  }
}
