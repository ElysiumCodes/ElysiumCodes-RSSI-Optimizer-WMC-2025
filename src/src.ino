#include <SPI.h>
#include <LoRa.h>
#include <ESP32Servo.h>

#define LORA1_SCK   18
#define LORA1_MISO  19
#define LORA1_MOSI  23
#define LORA1_SS    5
#define LORA1_RST   14
#define LORA1_DIO0  2
#define LORA1_FREQ  433E6

#define LORA2_SCK   33
#define LORA2_MISO  25
#define LORA2_MOSI  32
#define LORA2_SS    15
#define LORA2_RST   27
#define LORA2_DIO0  4
#define LORA2_FREQ  433E6

#define SERVO_PIN1  22  
#define SERVO_PIN2  21


SPIClass vspi(VSPI);
SPIClass hspi(HSPI);

LoRaClass LoRa1;
LoRaClass LoRa2;

Servo serv1;
Servo serv2;

int dataMat[18][18];

void setup() {
    Serial.begin(115200);
    while (!Serial);
    delay(1500);
    
    Serial.println("Initializing LoRa modules...");

    vspi.begin(LORA1_SCK, LORA1_MISO, LORA1_MOSI, LORA1_SS);
    LoRa1.setSPI(vspi);
    LoRa1.setPins(LORA1_SS, LORA1_RST, LORA1_DIO0);
    if (!LoRa1.begin(LORA1_FREQ)) {
        Serial.println("LoRa1 init failed!");
        while (1);
    }
    Serial.println("LoRa1 initialized!");

    hspi.begin(LORA2_SCK, LORA2_MISO, LORA2_MOSI, LORA2_SS);
    LoRa2.setSPI(hspi);
    LoRa2.setPins(LORA2_SS, LORA2_RST, LORA2_DIO0);
    if (!LoRa2.begin(LORA2_FREQ)) {
        Serial.println("LoRa2 init failed!");
        while (1);
    }
    Serial.println("LoRa2 initialized!");

    serv1.attach(SERVO_PIN1);
    serv2.attach(SERVO_PIN2);

    Serial.println("Setup complete!");
}

void loop() {
    Serial.println("Sending packet from LoRa1...");

    LoRa1.beginPacket();
    LoRa1.print("Hello from LoRa1!");
    LoRa1.endPacket();

    Serial.println("Packet sent!");
    Serial.println("");

    delay(500); // Allow time for transmission

    int packetSize = LoRa2.parsePacket();
    if (packetSize) {

        int rssi = LoRa2.packetRssi();
        Serial.print("RSSI: ");
        Serial.println(rssi);
        Serial.println("");

        if (rssi < -80) {
            Serial.println("Weal signal detected, initiating scan...");
            scan();
        }
    } 
    else {
        Serial.println("No packet received.");
        Serial.println("");
        Serial.println("");
    }

    delay(1000);
}

void scan() {
    Serial.println("Scanning...");

    int maxSignal = -999;
    int bestIndex1 = 0, bestIndex2 = 0;

    for (int i = 0; i <= 180; i += 10) {
        serv1.write(i);
        int index1 = i / 10;

        for (int j = 0; j <= 180; j += 10) {
            serv2.write(j);
            int index2 = j / 10;
            
            delay(100);  // Give time for servos to stabilize
            
            dataMat[index1][index2] = LoRa2.packetRssi();
            
            if (dataMat[index1][index2] > maxSignal) {
                maxSignal = dataMat[index1][index2];
                bestIndex1 = index1;
                bestIndex2 = index2;
            }
        }
    }

    Serial.print("Best Signal Found at: ");
    Serial.print(bestIndex1 * 10);
    Serial.print("°, ");
    Serial.print(bestIndex2 * 10);
    Serial.print("° with RSSI ");
    Serial.println(maxSignal);

    serv1.write(bestIndex1 * 10);
    serv2.write(bestIndex2 * 10);
}