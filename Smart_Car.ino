#include <IRremote.h>
#include "SR04.h"
#include <Servo.h>
Servo steer;

int m1cspeed = 5;
int m1d1 = 4;
int m1d2 = 3;
int m1speed = 255;

int m2cspeed = 10;
int m2d1 = 9;
int m2d2 = 8;
int m2speed = 255;

int receiver = 7;
IRrecv irrecv(receiver);
uint32_t last_decodedRawData = 0;

int trig = 13;
int echo = 12;
SR04 sensor = SR04(echo, trig);
long distance;

int beep = 2;

void horn(){
  digitalWrite(beep, HIGH);
  delay(80);
  digitalWrite(beep, LOW);
  delay(80);
  digitalWrite(beep, HIGH);
  delay(80);
  digitalWrite(beep, LOW);
}

void right(){
  steer.write(135);
}
void left(){
  steer.write(65);
}
void straight(){
  steer.write(100);
}

void emergency_stop(){
  distance = sensor.Distance();
  if (distance < 20 && distance > 5){
    Serial.println(distance);
    stop();
    delay(1000);
    backward();
    delay(1000);
    stop();
  }
}

void forward(){
  digitalWrite(m1d1, LOW);
  digitalWrite(m1d2, HIGH);
  analogWrite(m1cspeed, m1speed);

  digitalWrite(m2d1, HIGH);
  digitalWrite(m2d2, LOW);
  analogWrite(m2cspeed, m2speed);
}

void backward(){
  digitalWrite(m1d1, HIGH);
  digitalWrite(m1d2, LOW);
  analogWrite(m1cspeed, m1speed);

  digitalWrite(m2d1, LOW);
  digitalWrite(m2d2, HIGH);
  analogWrite(m2cspeed, m2speed);
}
void stop(){
  analogWrite(m1cspeed, 0);
  analogWrite(m2cspeed, 0);
}
void detect(){

  //if (irrecv.decodedIRData.flags)
  //{

  //  irrecv.decodedIRData.decodedRawData = last_decodedRawData;
  //  Serial.println("REPEAT!");
  //} 
  //else{
    //output the IR code on the serial monitor
    Serial.print("IR code:0x");
    Serial.println(irrecv.decodedIRData.decodedRawData, HEX);
  //}
  switch (irrecv.decodedIRData.decodedRawData){
    case 0xE0: Serial.println("horn"); horn(); break;
    case 0x4B9B: Serial.println("left"); left();   break;
    case 0x4B9A: Serial.println("straight"); straight();   break;
    case 0x4B9C: Serial.println("right");  right(); break;
    case 0xF4: Serial.println("forward");  forward();  break;
    case 0xE5: Serial.println("stop");  stop();  break;
    case 0xF5: Serial.println("backward");  backward();  break;
    default:
      Serial.println(" other button   ");
  last_decodedRawData = irrecv.decodedIRData.decodedRawData;
  delay(500);
  }
}
void setup() {
  pinMode(m1d1, OUTPUT);
  pinMode(m1d2, OUTPUT);
  pinMode(m1cspeed, OUTPUT);

  pinMode(m2d1, OUTPUT);
  pinMode(m2d2, OUTPUT);
  pinMode(m2cspeed, OUTPUT);


  pinMode(beep, OUTPUT);
  steer.attach(11);

  Serial.begin(9600);
  irrecv.enableIRIn();
}

void loop() {
  if (irrecv.decode()){
    detect();
    irrecv.resume(); // receive the next value
  }
  emergency_stop();
}