# Smart-home

## 智联家居

**一、需求分析**

​        随着科技科技地不断发展，物联网作为通信行业新兴应用，在万物互联的大趋势下，市场规模将进一步扩大。随着国家政策扶持，国家的物联网产业将延续良好的发展势头。本产品本着想要实现家中的各种家居或产品都能连上网，可以发生数据交互，达到物物相联的目的，同时通过手机与用户进行交互来操作物件，从而实现智能生活。

​        本产品在阿里云服务器上搭建 MQTT 服务器，让手机端和开发板端通过 WiFi 进行无线连接，摆脱数据线的控制。土壤湿度显示可以让用户更加直观地知道对于植物的浇水时。RGB 灯可以通过 APP 端的调节达到期望的亮度，摆脱了常用灯光组件只能根据档位选择亮度的方式。温湿度情况的显示也可以帮助用户对居室环境进行改善。火焰检测器也为厨卫环境提供了一定的安全预警保障。同时，用户可以通过设置自定义的语音指令如：“开灯”、“关灯”等，在 APP 端进行语音识别后对灯光进行远程控制。 

***

### 二、技术方案

​        硬件方面的组成有 ESP8266nodemcu 开发板、DHT11 温湿度传感器、RGB 灯土壤湿度传感器以及火焰传感器。产品的各个硬件均来源与网络购买，利用杜邦线将各传感器和开发板进行连接。软件开发环境方面选择了 Arduino IDE 对 ESP8266nodemcu 开发板进行开发，APP由一款由谷歌工作室开发的可在线安卓编程的软件--APP inventor 进行开发设计。服务器方面选择用阿里云服务器搭载 MQTT 服务器作为数据传输的中枢。代码为自学 Arduino 语言后利用 Arduino IDE 进行撰写。 

***

### 三、功能说明

​        在阿里云服务器上搭建 MQTT 服务器，让手机端和开发板端通过 WiFi 进行无线连接，摆脱数据线的控制。土壤湿度显示可以让用户更加直观地知道对于植物的浇水时机。RGB灯可以通过 APP 端的调节达到期望的亮度，摆脱了常用灯光组件只能根据档位选择亮度的方式。温湿度情况的显示也可以帮助用户对居室环境进行改善。火焰检测器也为厨卫环境提供了一定的安全预警保障。同时，用户可以通过设置自定义的语音指令如：“开灯”、“关灯”等，在 APP 端进行语音识别后对灯光进行远程控制。 

***

###  四、应用前景

​        产品可以应用于普通的家庭中。用户可以通过手机 APP 端对室内的任意一盏灯进行控制。可以任意设置分组、分批定时控制。建立安防报警系统，通过设立火灾探测器、烟雾探测器等进行 24 小时设防，连接蜂鸣器等报警器件实现自动报警功能。今后可以通过连接电动机对抽水泵进行控制，实现水和喷壶管理与花草自动浇灌。

****

### 五、主要开发过程代码（ESP8266）

Arduino创建MQTT客户端更加便捷，编写了ESP8266MQTT库，由开源库PubSubClient（PubSubClient）二次开发而来，相对于PubSubClient重新封装了一些函数，方便使用。



client.subscribe

用来订阅主题并将该主题内收到的消息通过串口发送

client.publis

用来向某个主题内发送消息

 

/*单独测土壤湿度*/

\#include<Ticker.h>

Ticker timer;

\#define humi 4

\#define LED 2

\#define mol A0

int val=1; 

int mo=0;

 

void tick_humidity_control()

{

 val=digitalRead(humi);//读出脚位4的值并指定给val

 mo=(analogRead(mol))*5;//读出类比脚位a0的值*5并指定给mo

 mo=map(mo, 0, 1023*5, 100, 0);//将mo所读取到的讯号对等装换至0-100之间数值

}

 

void setup() 

{

 Serial.begin(115200);

 pinMode(humi, INPUT);

 timer.attach(0.1, tick_humidity_control);

}

 

void loop() 

{

 if(val==0)

 {

  digitalWrite(LED, LOW);

  Serial.print("soi_moisture: ");

  Serial.print(mo);

  Serial.println(" %");

 }

 else

 {

  digitalWrite(LED, HIGH);

  Serial.print("soi_moisture: ");

  Serial.print(mo);

  Serial.println(" %");

 }

 delay(500);

}

 

/*

连接上一个MQTT服务器，每2秒发布一次"hello world"消息到主题“outTopic”

客户端监听主题“inTopic”，并判断负载内容来控制灯亮灭

*/

\#include <ESP8266WiFi.h>

\#include <PubSubClient.h>

\#include <SimpleDHT.h>

const char* ssid = "CMCC-DNJ7";

const char* password = "13411542468";

const char* mqtt_server = "47.107.84.138";

SimpleDHT11 dht11(5);              //DHT11接D1引脚

WiFiClient espClient;            //初始化以太网客户端对象 --WiFIEspClient.h

PubSubClient client(espClient);         //初始化MQTT库PubSubClient.h的对象，然后传进去

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

 

void callback(char* topic, byte* payload, unsigned int length) {//有效载荷（Payload）,携带的数据信息

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

//断开连接

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

 pinMode(2, OUTPUT);   // Initialize the BUILTIN_LED pin as an output

 Serial.begin(115200);

 setup_wifi();                //连接WiFi

 //配置mqtt服务器地址和端口

 client.setServer(mqtt_server, 1883);     //连接WiFi后，连接MQTT服务器，服务器+端口

 //设置订阅消息回调

 client.setCallback(callback);

 reconnect();

 while(!client.subscribe("led",0))

 {Serial.println("订阅失败;尝试重新订阅！");

  client.subscribe("led",0);         

  //订阅请求，客户端到服务端，主题名+消息等级（topic+qos（0/1））QoS 0

  delay(300);

  }

  Serial.println("订阅成功~~~");

}

 

void loop() {//重连机制

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

  //不断监听信息

 client.loop();

 

 long now = millis();

 if (now - lastMsg > 5000) {

  //每2s发布一次信息

  lastMsg = now;

  ++value;

 

  //读取温湿度

//   snprintf (temp, 50, "当前温度为 :%ld", (int)temperature);

//   snprintf (hum, 50, "当前湿度为 :%ld", (int)humidity);

  snprintf (temp, 50, "%ld", (int)temperature);

  snprintf (hum, 50, "%ld", (int)humidity);

  

  //Serial.print("Publish message: ");

  Serial.println(temp);

  //Serial.print("\n");

  Serial.println(hum);

  Serial.print("\n");

  client.publish("temp", temp);     //发布消息，两个方向都允许

  client.publish("hum", hum);     //发布消息，两个方向都允许

 }

}

 

 
