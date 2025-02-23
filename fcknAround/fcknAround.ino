#include <ESP32Servo.h>

Servo servo1;
Servo servo2;
Servo servo3;
Servo servo4;

int servo1Pin = 22; // Connect to the control pin of servo 1
int servo2Pin = 21; // Connect to the control pin of servo 2
int servo3Pin = 13;
int servo4Pin = 26;


void setup() {
    servo1.attach(servo1Pin);
    servo2.attach(servo2Pin);
    servo1.write(180);
    delay(1000);
    servo1.write(0);
    servo2.write(180);
    delay(1000);
    servo2.write(0);
    servo3.write(180);
    delay(1000);
    servo3.write(0);
    servo4.write(180);
    delay(1000);
    servo4.write(0);
}

void loop() {

    for(int i=0;i<=180;i=i+15){
      servo2.write(i);
      servo3.write(i);
      for(int j=0;j<180;j=j+15){
        servo1.write(j);
        servo4.write(j);
        delay(500);
      } 
    }

    servo1.write(0);
    servo3.write(0);
    servo2.write(0);
    servo4.write(0);
    delay(2000);

}
