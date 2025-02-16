#include <SPI.h>
#include <LoRa.h>

// Define SPI objects for two LoRa modules
SPIClass vspi(VSPI);  // First LoRa module
SPIClass hspi(HSPI);  // Second LoRa module

// Create two LoRa objects
LoRaClass LoRa1;
LoRaClass LoRa2;

// Define pins for first LoRa (connected to VSPI)
#define LORA1_SCK   18
#define LORA1_MISO  19
#define LORA1_MOSI  23
#define LORA1_SS    5
#define LORA1_RST   14
#define LORA1_DIO0  2
#define LORA1_FREQ  915E6  // Frequency in Hz

// Define pins for second LoRa (connected to HSPI)
#define LORA2_SCK   33
#define LORA2_MISO  25
#define LORA2_MOSI  32
#define LORA2_SS    15
#define LORA2_RST   27
#define LORA2_DIO0  4
#define LORA2_FREQ  868E6  // Frequency in Hz

void setup() {
    Serial.begin(115200);

    // Initialize first LoRa module (VSPI)
    vspi.begin(LORA1_SCK, LORA1_MISO, LORA1_MOSI, LORA1_SS);
    LoRa1.setSPI(vspi);
    LoRa1.setPins(LORA1_SS, LORA1_RST, LORA1_DIO0);
    if (!LoRa1.begin(LORA1_FREQ)) {
        Serial.println("LoRa1 init failed!");
        while (1);
    }
    Serial.println("LoRa1 initialized!");

    // Initialize second LoRa module (HSPI)
    hspi.begin(LORA2_SCK, LORA2_MISO, LORA2_MOSI, LORA2_SS);
    LoRa2.setSPI(hspi);
    LoRa2.setPins(LORA2_SS, LORA2_RST, LORA2_DIO0);
    if (!LoRa2.begin(LORA2_FREQ)) {
        Serial.println("LoRa2 init failed!");
        while (1);
    }
    Serial.println("LoRa2 initialized!");
}

void loop() {
    // LoRa1 Transmitting
    LoRa1.beginPacket();
    LoRa1.print("Hello from LoRa1!");
    LoRa1.endPacket();
    Serial.println("Sent from LoRa1");

    // LoRa2 Receiving
    int packetSize = LoRa2.parsePacket();
    
    if (packetSize) {
        Serial.print("LoRa2 received: ");
        while (LoRa2.available()) {
            Serial.print((char)LoRa2.read());
        }
        Serial.println();
    }

    delay(1000);
}
