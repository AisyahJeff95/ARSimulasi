#define s0  0
#define s1  21
#define s2  2
#define s3  22
#define out  4

#include <WiFi.h>
#include <WiFiClient.h>

// WiFi network name and password:
const char * networkName = "iPhone (2)";
const char * networkPswd = "midoriaisyah951p";

WiFiServer server(8080);
WiFiClient client;


int IN1 = 14;
int IN2 = 27;
int IN3 = 26;
int IN4 = 25;
int IN5 = 12;
int IN6 = 13;

// Variables
int red = 0;
int green = 0;
int blue = 0;
bool colorCheck = false;

void setup()
{
  Serial.begin(115200);

  WiFi.begin(networkName,networkPswd);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }

  Serial.print("Connected to ");
  Serial.print(networkName);

  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  server.begin();
  
  
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
}

void loop()
{
  client = server.available();
  if(client){
    Serial.println("Client connected");
  }
  while(client){
       color();
      if (red < blue && red < green && red < 25)
      {
        if (green - blue >= 10 && green - blue <= 25 && green - ( 2 * red ) >= 8 )
        {
          Serial.println("(Red Color)");
          client.print("RED");
          client.print('\r');
          delay(2000); //Edit this delay for Sync with Red Actuator Open
          set_Red();
          delay(2000); //Edit this delay for Sync with Red Actuator Close
          reset_Red();
          
        }
        else if (green - red <= 10 && green - red >= -3 && blue >= green)
        {
          Serial.println("(Yellow Color)");
          client.print("YELLOW");
          client.print('\r');
          delay(4000);
          set_Yellow(); //Edit this delay for Sync with Yellow Actuator Open
          delay(2000);
          reset_Yellow(); //Edit this delay for Sync with Yellow Actuator Close
        }
        
      else if((red<30) && (blue<30) && (green<30)){
        client.print("(ERROR!) UNKNOWN CUBE");
        Serial.println("(ERROR!) UNKNOWN CUBE");
        client.print('\r');
        delay(10000);
      }
      }
      else if ((red > green &&  blue < green) && blue < 25 && red > 40)
      {
        Serial.println("(Blue Color)");
        client.print("BLUE");
        client.print('\r');
        delay(6000);//Edit this delay for Sync with Blue Actuator Open
          set_Blue();
          delay(2000); //Edit this delay for Sync with Blue Actuator Close
          reset_Blue();
      }
      else if((red<30) && (blue<30) && (green<30)){
        client.print("(ERROR!) UNKNOWN CUBE");
        Serial.println("(ERROR!) UNKNOWN CUBE");
        client.print('\r');
        delay(10000);
      }
    
  }
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
