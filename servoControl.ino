#include <ESP32Servo.h>
#define SERVO_PIN1 13  
#define SERVO_PIN2 14

Servo serv1;
Servo serv2;

void setup() {
    serv1.attach(SERVO_PIN1);
    serv2.attach(SERVO_PIN2);
}

void loop() {
  int i,j;
  for(i=0;i<=180;i+=5){
    for(j=0;j<=180;j+=5){
      serv2.write(j);
    }
    serv1.write(i);
  }
}
