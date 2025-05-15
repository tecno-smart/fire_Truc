
#define en 5   // speed
#define in1 9  // R
#define in2 8  // R
#define in3 7  // L
#define in4 6  // L
int speed = 160;

void setup() {
  Serial.begin(9600);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
}


void loop() {
  if (Serial.available() > 0) {
    char incom = Serial.read();

    if (incom == 'u') {
      forward();
    } else if (incom == 'U') {
      stop();
    }
    //--------------------------------

    if (incom == 'r') {
      right();
    } else if (incom == 'R') {
      stop();
    }
    //--------------------------------

    if (incom == 'd') {
      back();
    } else if (incom == 'D') {
      stop();
    }
    //--------------------------------

    if (incom == 'l') {
      left();
    } else if (incom == 'L') {
      stop();
    }
    //--------------------------------


  }  // end serial
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