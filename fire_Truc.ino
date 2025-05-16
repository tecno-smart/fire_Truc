#include <Servo.h>
Servo p_servo;



#include <SoftwareSerial.h>
SoftwareSerial gsm(3, 2);

#define en 5   // speed
#define en2 11   // speed
#define in1 9  // R
#define in2 8  // R
#define in3 7  // L
#define in4 6  // L
#define buzzer 13
#define pumb 4

#define f_L A2
#define f_C A1
#define f_R A0



int speed = 160;


int sens_R, sens_C, sens_L;

char incom = 'z';

uint8_t detict = LOW;

uint8_t no_detict = HIGH;

uint8_t ON = LOW;

uint8_t OFF = HIGH;

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
  pinMode(en2, OUTPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(pumb, OUTPUT);

  pinMode(f_L, INPUT);
  pinMode(f_C, INPUT);
  pinMode(f_R, INPUT);




}


void loop() {

  
  
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
    forward();
    if (!msg) {

      msg = true;
    }
  }

}  // end loop

void read_sens() {
  sens_R = digitalRead(f_R);
  sens_C = digitalRead(f_C);
  sens_L = digitalRead(f_L);
 
}


void forward() {
  analogWrite(en, speed);
  analogWrite(en2, speed);
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
}
void back() {
  analogWrite(en, speed);
  analogWrite(en2, speed);
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
}
void right() {
  analogWrite(en, speed);
  analogWrite(en2, speed);
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
  analogWrite(en, 0);
  analogWrite(en2, 0);
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

