#include <Servo.h>
Servo p_servo;

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x20, 16, 2);  // by frank de Brabander
#include <DHT11.h>                   // by Dhruba saha
DHT11 dht11(11);
#include <SoftwareSerial.h>
SoftwareSerial gsm(3, 2);

#define en 5   // speed
#define in1 9  // R
#define in2 8  // R
#define in3 7  // L
#define in4 6  // L
#define buzzer 13
#define pumb 4

#define f_L A2
#define f_C A1
#define f_R A0
#define gas A3


int speed = 160;
unsigned long last;
int temperature = 0;
int humidity = 0;

int sens_R, sens_C, sens_L;
char incom = 'z';
uint8_t detict = LOW;
uint8_t no_detict = HIGH;
uint8_t ON = HIGH;
uint8_t OFF = LOW;
bool msg = true;
int gas_r=0;
void setup() {
  Serial.begin(9600);
  gsm.begin(9600);
  p_servo.attach(10);
  p_servo.write(90);

  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  pinMode(en, OUTPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(pumb, OUTPUT);

  pinMode(f_L, INPUT);
  pinMode(f_C, INPUT);
  pinMode(f_R, INPUT);
  pinMode(gas, INPUT);


  lcd.init();
  lcd.init();
  lcd.backlight();
  lcd.setCursor(3, 0);
  lcd.print("FIRE TRUCK");
  delay(1000);
  lcd.clear();
}


void loop() {

  if (Serial.available() > 0) {
    incom = Serial.read();



  }  // end serial
  if (incom == 'u') {
    forward();
  }
  //--------------------------------

  else if (incom == 'r') {
    right();
  }
  //--------------------------------

  else if (incom == 'd') {
    back();
  }
  //--------------------------------

  else if (incom == 'l') {
    left();
  } else if (incom == 's') {
    stop();
  }
  //--------------------------------

  temp_hum();  // ead temp and humid
  read_sens();
  if (sens_C == detict) {

    stop();
    digitalWrite(buzzer, HIGH);
    p_servo.write(90);
    digitalWrite(pumb, ON);
    if (msg) {
      send_message();
      msg = false;
    }
    delay(1000);
  }
  if (sens_R == detict) {
    stop();
    p_servo.write(10);
    digitalWrite(buzzer, HIGH);
    digitalWrite(pumb, ON);
    if (msg) {
      send_message();
      msg = false;
    }
    delay(1000);
  }
  if (sens_L == detict) {
    stop();
    digitalWrite(buzzer, HIGH);
    p_servo.write(180);
    digitalWrite(pumb, ON);
    if (msg) {
      send_message();
      msg = false;
    }
    delay(1000);
  }
  if (sens_C == no_detict && sens_R == no_detict && sens_L == no_detict) {
    digitalWrite(buzzer, LOW);
    digitalWrite(pumb, OFF);
    if (!msg) {

      msg = true;
    }
  }

}  // end loop

void read_sens() {
  sens_R = digitalRead(f_R);
  sens_C = digitalRead(f_C);
  sens_L = digitalRead(f_L);
  gas_r= digitalRead(gas);
}
void temp_hum() {

  int result = dht11.readTemperatureHumidity(temperature, humidity);
  if (result == 0) {
    // lcd
    if (millis() - last >= 1000) {
      last = millis();
      screen(temperature, humidity);
    }
  }
}

void forward() {
  analogWrite(en, speed);
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
}
void back() {
  analogWrite(en, speed);
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
}
void right() {
  analogWrite(en, speed);
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
}
void left() {
  analogWrite(en, speed);
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
}
void stop() {
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
}

void send_message() {

  gsm.println("AT+CMGF=1");
  delay(500);
  gsm.println("AT+CMGS=\"+201093978618\"\r");
  delay(500);
  gsm.print("Warning! Fire detected!");
  delay(500);
  gsm.write(26);
}

void screen(int t, int h) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Temp:");
  lcd.setCursor(5, 0);
  lcd.print(t);
  lcd.write(223);
  lcd.setCursor(10, 0);
  lcd.print("GAS:");
  lcd.print(gas_r);
  lcd.setCursor(0, 1);
  lcd.print("Humidity:");
  lcd.setCursor(10, 1);
  lcd.print(h);
  lcd.print(" %");
}