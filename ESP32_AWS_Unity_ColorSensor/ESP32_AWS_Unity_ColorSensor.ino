//#define s0  0
//#define s1  21
//#define s2  2
//#define s3  22
//#define out  4

#include <WiFi.h>
#include <WiFiClient.h>

#include <ArduinoJson.h>
#include <EEPROM.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include "secrets.h"
#include <WiFiClientSecure.h>
#include <MQTTClient.h>

// The MQTT topics that this device should publish/subscribe
#define AWS_IOT_PUBLISH_TOPIC   "unityCommands"
#define AWS_IOT_SUBSCRIBE_TOPIC "unityCommands_2"

// WiFi network name and password:

const char * red_color = "RED";
const char * blue_color = "BLUE";
const char * yellow_color = "YELLOW";
const char * unknown_color = "UNKNOWN";

//WiFiServer server(8080);
//WiFiClient client;

WiFiClientSecure net = WiFiClientSecure();
MQTTClient client = MQTTClient(256);


int IN1 = 14;
int IN2 = 27;
int IN3 = 26;
int IN4 = 25;
int IN5 = 12;
int IN6 = 13;
int s0 = 0;
int s1 = 21;
int s2 = 2;
int s3 = 22;
int out = 4;

// Variables
int red = 0;
int green = 0;
int blue = 0;
bool colorCheck = false;
void messageHandler(String &topic, String &payload) {
  Serial.println("incoming: " + topic + " - " + payload);

//  StaticJsonDocument<200> doc;
//  deserializeJson(doc, payload);
//  const char* message = doc["message"];
}

void connectAWS()
{
  // Configure WiFiClientSecure to use the AWS IoT device credentials
  net.setCACert(AWS_CERT_CA);
  net.setCertificate(AWS_CERT_CRT);
  net.setPrivateKey(AWS_CERT_PRIVATE);

  // Connect to the MQTT broker on the AWS endpoint we defined earlier
  client.begin(AWS_IOT_ENDPOINT, 8883, net);

  // Create a message handler
  client.onMessage(messageHandler);

  Serial.print("Connecting to AWS IOT");

  while (!client.connect(THINGNAME)) {
    Serial.print(".");
    delay(100);
  }

  if(!client.connected()){
    Serial.println("AWS IoT Timeout!");
    return;
  }

  // Subscribe to a topic
  client.subscribe(AWS_IOT_SUBSCRIBE_TOPIC);

  Serial.println("AWS IoT Connected!");
}


void publishMessage(int colorCode)
{
  Serial.print("Sending Message: ");
  Serial.println(colorCode);
   StaticJsonDocument<200> doc;
 
  switch(colorCode){
    case 1:
      doc["Color"] = red_color;
      break;
    case 2:
      doc["Color"] = blue_color;
      break;
    case 3:
      doc["Color"] = yellow_color;
      break;
    case 5:
      doc["Color"] = "N/A";
    default:
      doc["Color"] = unknown_color;
  }
    
  char jsonBuffer[512];
  serializeJson(doc, jsonBuffer); // print to client
  client.publish(AWS_IOT_PUBLISH_TOPIC, "Hello from ESP32");
  client.publish(AWS_IOT_PUBLISH_TOPIC, jsonBuffer);
  client.loop();
}


void setup()
{
  Serial.begin(115200);

  WiFi.begin(WIFI_SSID,WIFI_PASSWORD);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }

  Serial.print("Connected to ");
  Serial.print(WIFI_SSID);

  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
  
  Serial.println("color sensor detection");

  pinMode(s0, OUTPUT);
  pinMode(s1, OUTPUT);
  pinMode(s2, OUTPUT);
  pinMode(s3, OUTPUT);
  pinMode(out, INPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(IN5, OUTPUT);
  pinMode(IN6, OUTPUT);

  digitalWrite(s0, HIGH);
  digitalWrite(s1, HIGH);
  connectAWS();
}

void loop()
{
//  publishMessage(1);
//  delay(5000);
       color();
      if (red < blue && red < green && red < 25)
      {
        if (green - blue >= 10 && green - blue <= 25 && green - ( 2 * red ) >= 8 )
        {
          Serial.println("(Red Color)");
          publishMessage(1);
          delay(2000); //Edit this delay for Sync with Red Actuator Open
          set_Red();
          delay(2000); //Edit this delay for Sync with Red Actuator Close
          reset_Red();
          
        }
        else if (green - red <= 10 && green - red >= -3 && blue >= green)
        {
          Serial.println("(Yellow Color)");
          publishMessage(3);
          delay(4000);
          set_Yellow(); //Edit this delay for Sync with Yellow Actuator Open
          delay(2000);
          reset_Yellow(); //Edit this delay for Sync with Yellow Actuator Close
        }
        
      else if((red<30) && (blue<30) && (green<30)){
        Serial.println("(ERROR!) UNKNOWN CUBE");
        publishMessage(0);
        delay(10000);
      }
      }
      else if ((red > green &&  blue < green) && blue < 25 && red > 40)
      {
        Serial.println("(Blue Color)");
        publishMessage(2);
        delay(6000);//Edit this delay for Sync with Blue Actuator Open
          set_Blue();
          delay(2000); //Edit this delay for Sync with Blue Actuator Close
          reset_Blue();
      }
      else if((red<30) && (blue<30) && (green<30)){
        Serial.println("(ERROR!) UNKNOWN CUBE");
        publishMessage(0);
        delay(10000);
      }
//      else{
//        publishMessage(5);
//      }
    
}

void color()
{
  digitalWrite(s2, LOW);
  digitalWrite(s3, LOW);
  //count OUT, pRed, RED
  red = pulseIn(out, digitalRead(out) == HIGH ? LOW : HIGH);
  digitalWrite(s3, HIGH);
  //count OUT, pBLUE, BLUE
  blue = pulseIn(out, digitalRead(out) == HIGH ? LOW : HIGH);
  digitalWrite(s2, HIGH);
  //count OUT, pGreen, GREEN
  green = pulseIn(out, digitalRead(out) == HIGH ? LOW : HIGH);
}
void set_Red(){
  //Move RED Actuator
      digitalWrite(IN1, HIGH);
      digitalWrite(IN2, LOW);
      Serial.println("(Red Color ON)");
}
void reset_Red(){
  //Reset RED Actuator
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, HIGH);
      Serial.println("(Red Color OFF)");
}
void set_Blue(){
  //Move Blue Actuator
    digitalWrite(IN5, HIGH);
    digitalWrite(IN6, LOW);
    Serial.println("(Blue Color ON)");
}
void reset_Blue(){
  //Reset Red Actuator
    digitalWrite(IN5, LOW);
    digitalWrite(IN6, HIGH);
    Serial.println("(Blue Color OFF)");
}
void set_Yellow(){
  //Move Yellow Actuator
      digitalWrite(IN3, HIGH);
      digitalWrite(IN4, LOW);
      Serial.println("(Yellow Color ON)");
}
void reset_Yellow(){
  //Reset Yellow Actuator
      digitalWrite(IN3, LOW);
      digitalWrite(IN4, HIGH);
      Serial.println("(Yellow Color OFF)");
}
