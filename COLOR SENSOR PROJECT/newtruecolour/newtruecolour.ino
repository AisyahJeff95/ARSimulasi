#define s0  0
#define s1  21
#define s2  2
#define s3  22
#define out  4
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


void setup()
{
  Serial.begin(115200);
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
  color();

  if (red < blue && red < green && red < 25)
  {
    if (green - blue >= 10 && green - blue <= 25 && green - ( 2 * red ) >= 8 )
    {

      Serial.println("(Red Color)");
      delay(2000); //delay 2 saat untuk actuator
      digitalWrite(IN1, HIGH);
      digitalWrite(IN2, LOW);
      delay(3000);
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, HIGH);
      Serial.println("(Red Color OFF)");

    }
    else if (green - red <= 10 && green - red >= -3 && blue >= green)
    {

      Serial.println("(Yellow Color)");
      delay(4000);//delay 4 saat untuk actuator
      digitalWrite(IN3, HIGH);
      digitalWrite(IN4, LOW);
      delay(3000);
      digitalWrite(IN3, LOW);
      digitalWrite(IN4, HIGH);
      Serial.println("(Yellow Color OFF)");
    }
  }
  else if ((red > green &&  blue < green) && blue < 25 && red > 40)
  {

    Serial.println("(Blue Color)");
    delay(6000); //delay 6 saat untuk actuator
    digitalWrite(IN5, HIGH);
    digitalWrite(IN6, LOW);
    delay(3000);
    digitalWrite(IN5, LOW);
    digitalWrite(IN6, HIGH);
    Serial.println("(Blue Color 0FF)");

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
