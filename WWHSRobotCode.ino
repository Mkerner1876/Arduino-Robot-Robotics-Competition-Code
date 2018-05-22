#include <Servo.h>
long duration;
int distance;
const int trigPin = 12;
const int echoPin = 13;
int motor1Pin1 = 3; // pin 2 on L293D IC
int motor1Pin2 = 4; // pin 7 on L293D IC
int enable1Pin = 6; // pin 1 on L293D IC
int motor2Pin1 = 8; // pin 10 on L293D IC
int motor2Pin2 = 9; // pin 15 on L293D IC
int enable2Pin = 11; // pin 9 on L293D IC
int state;
int flag = 0;      //makes sure that the serial only prints once the state
int stateStop = 0;
Servo servo1;
Servo servo2;
void setup() {
  // sets the pins as outputs:
  pinMode(motor1Pin1, OUTPUT);
  pinMode(motor1Pin2, OUTPUT);
  pinMode(enable1Pin, OUTPUT);
  pinMode(motor2Pin1, OUTPUT);
  pinMode(motor2Pin2, OUTPUT);
  pinMode(enable2Pin, OUTPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  // sets enable1Pin and enable2Pin high so that motor can turn on:
  digitalWrite(enable1Pin, HIGH);
  digitalWrite(enable2Pin, HIGH);
  // initialize serial communication at 9600 bits per second:
  servo1.attach(5); //left
  servo2.attach(7); //right
  Serial.begin(9600);
}

void loop() {
  //ultrasonic sensor bootup
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;
  Serial.print("Distance: ");
  Serial.println(distance);

  if (distance < 13) {
    servo1.write(0);
    servo2.write(179);
    delay(2000);
      digitalWrite(motor1Pin1, LOW);
    digitalWrite(motor1Pin2, HIGH);
    digitalWrite(motor2Pin1, HIGH);
    digitalWrite(motor2Pin2, LOW);
    delay(500);
    digitalWrite(motor1Pin1, LOW);
    digitalWrite(motor1Pin2, LOW);
    digitalWrite(motor2Pin1, LOW);
    digitalWrite(motor2Pin2, LOW);
    delay(500);
    servo1.write(179);
    servo2.write(0);
    delay(500);
    servo1.write(90);
    servo2.write(50);
  }


  //bluetooth control
  //if some date is sent, reads it and saves in state
  if (Serial.available() > 0) {
    state = Serial.read();
    flag = 0;
  }
  // if the state is 'F' the DC motor will go forward
  if (state == 'F') {
    digitalWrite(motor1Pin1, HIGH);
    digitalWrite(motor1Pin2, LOW);
    digitalWrite(motor2Pin1, LOW);
    digitalWrite(motor2Pin2, HIGH);
    if (flag == 0) {
      Serial.println("Go Forward!");
      flag = 1;
    }
  }

  // if the state is 'R' the motor will turn left
  else if (state == 'R') {
    digitalWrite(motor1Pin1, HIGH);
    digitalWrite(motor1Pin2, LOW);
    digitalWrite(motor2Pin1, LOW);
    digitalWrite(motor2Pin2, LOW);
    if (flag == 0) {
      Serial.println("Turn LEFT");
      flag = 1;
    }
    delay(1500);
    state = 3;
    stateStop = 1;
  }
  // if the state is 'S' the motor will Stop
  else if (state == 'S' || stateStop == 1) {
    digitalWrite(motor1Pin1, LOW);
    digitalWrite(motor1Pin2, LOW);
    digitalWrite(motor2Pin1, LOW);
    digitalWrite(motor2Pin2, LOW);
    if (flag == 0) {
      Serial.println("STOP!");
      flag = 1;
    }
    stateStop = 0;
  }
  // if the state is 'L' the motor will turn right
  else if (state == 'L') {
    digitalWrite(motor1Pin1, LOW);
    digitalWrite(motor1Pin2, LOW);
    digitalWrite(motor2Pin1, LOW);
    digitalWrite(motor2Pin2, HIGH);
    if (flag == 0) {
      Serial.println("Turn RIGHT");
      flag = 1;
    }
    delay(1500);
    state = 3;
    stateStop = 1;
  }
  // if the state is 'B' the motor will Reverse
  else if (state == 'B') {
    digitalWrite(motor1Pin1, LOW);
    digitalWrite(motor1Pin2, HIGH);
    digitalWrite(motor2Pin1, HIGH);
    digitalWrite(motor2Pin2, LOW);
    if (flag == 0) {
      Serial.println("Reverse!");
    }
  }
}
// In void loop, delete the distance if statement to test bluetooth. 
//For running the complete code, the serial monitor will be interfered with when tx and rx for bluetooth is connected.
