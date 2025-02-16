#include <SPI.h>
#include <LoRa.h>
#include <ESP32Servo.h>



#define LORA1_SCK   18
#define LORA1_MISO  19
#define LORA1_MOSI  23
#define LORA1_SS    5
#define LORA1_RST   14
#define LORA1_DIO0  2
#define LORA1_FREQ  915E6 

#define LORA2_SCK   33
#define LORA2_MISO  25
#define LORA2_MOSI  32
#define LORA2_SS    15
#define LORA2_RST   27
#define LORA2_DIO0  4
#define LORA2_FREQ  915E6

#define SERVO_PIN1 13  
#define SERVO_PIN2 17



SPIClass vspi(VSPI);
SPIClass hspi(HSPI);

LoRaClass LoRa1;
LoRaClass LoRa2;

Servo serv1;
Servo serv2;

int dataMat[18][18];

void setup(){
  Serial.begin(115200);

  vspi.begin(LORA1_SCK, LORA1_MISO, LORA1_MOSI, LORA1_SS);
    LoRa1.setSPI(vspi);
    LoRa1.setPins(LORA1_SS, LORA1_RST, LORA1_DIO0);
    if (!LoRa1.begin(LORA1_FREQ)) {
        Serial.println("LoRa1 init failed!");
        while (1);
    }
    Serial.println("Datalink1 up!");

  hspi.begin(LORA2_SCK, LORA2_MISO, LORA2_MOSI, LORA2_SS);
    LoRa2.setSPI(hspi);
    LoRa2.setPins(LORA2_SS, LORA2_RST, LORA2_DIO0);
    if (!LoRa2.begin(LORA2_FREQ)) {
        Serial.println("LoRa2 init failed!");
        while (1);
    }
    Serial.println("Datalink2 up!");
    Serial.println("Datalink 1 and 2 are up!");

    serv1.attach(SERVO_PIN1);
    serv2.attach(SERVO_PIN2);
}



void loop(){
  LoRa1.beginPacket();
  LoRa1.print("Hello from LoRa1!");
  LoRa1.endPacket();

  int packetSize = LoRa2.parsePacket();

  if (packetSize) {
      int rssi = LoRa.packetRssi();
      if(rssi>-80){
        scan();
      }
    }
  
  delay(1000);
}

void scan(){
  int i,j,index1,index2;
  for(i=0;i<=180;i+=10){
    index1=i/10;
    for(j=0;j<=180;j+=10){
      serv2.write(j);
      index2=j/10;
      dataMat[i][j]=LoRa.packetRssi();
    }
    serv1.write(i);
  }

  int max=0;
  for(i=0;i<=17;i++){
    for(j=0;j<17;j++){
      if(max<dataMat[i][j]){
        max=dataMat[i][j];
        index1=i;
        index2=j;
      }
    }
  }

  serv1.write(i*10);
  serv2.write(j*10);

}