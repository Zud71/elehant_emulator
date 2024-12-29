#include <ArduinoBLE.h>
#include "esp_system.h"

const uint8_t newMAC1[] = {0xB0,0x03,0x02,0x00,0x18,0xFA};
const uint8_t newMAC2[] = {0xB0,0x04,0x02,0x00,0x18,0xFA};


bool swch= true;

BLEService myService("fff0");
BLEIntCharacteristic myCharacteristic("fff1", BLERead | BLEBroadcast);

// Advertising parameters should have a global scope. Do NOT define them in 'setup' or in 'loop'
//const uint8_t completeRawAdvertisingData[] = {0x02,0x01,0x06,0x09,0xff,0x01,0x01,0x00,0x01,0x02,0x03,0x04,0x06};   
//80 08 96 01 02 03 FA 18 00 88 B8 01 00 58 4E 07 20
//80 13 96 01 02 04 FA 18 00 01 24 00 00 58 4E 07 20
uint8_t completeRawAdvertisingData03[] = {0x14,0xFF,0xFF,0xFF,0x80,0x08,0x96,0x01,0x02,0x03,0xFA,0x18,0x00,0x88,0xB8,0x01,0x00,0x58,0x4E,0x07,0x20}; 

//uint8_t completeRawAdvertisingData04[] = {0x14,0xFF,0xFF,0xFF,0x80,0x13,0x96,0x01,0x02,0x04,0xFA,0x18,0x00,0x01,0x24,0x00,0x00,0x58,0x4E,0x07,0x20};

uint8_t count = 1;

void SendBle();

void setup() {
  Serial.begin(9600);
  while (!Serial);
  
 
}

void loop() {

  if(swch){
  
  Serial.println("Set MAC1");
  esp_base_mac_addr_set(newMAC1);
  completeRawAdvertisingData03[20]=0x03;
  completeRawAdvertisingData03[9]=0x03;
  SendBle();
  swch =false;

  }else { 

  Serial.println("Set MAC2");
  esp_base_mac_addr_set(newMAC2);
  completeRawAdvertisingData03[20]=0x04;
  completeRawAdvertisingData03[9]=0x04;
  SendBle();
  swch =true;

  }

   delay(30000);

}

void SendBle()
{

  Serial.println("Start ble");

  if (!BLE.begin()) {
    Serial.println("failed to initialize BLE!");
    while (1);
  }


  myService.addCharacteristic(myCharacteristic);
  BLE.addService(myService);

  // Build advertising data packet
  BLEAdvertisingData advData;

  if(count<254)
  {
    count++;

  }else 
  {
    count=1;

  }
  
  completeRawAdvertisingData03[13]=count;


  // If a packet has a raw data parameter, then all the other parameters of the packet will be ignored
  advData.setRawData(completeRawAdvertisingData03, sizeof(completeRawAdvertisingData03)); 
  //advData.setRawData(&completeRawAdvertisingData, 21);  
  // Copy set parameters in the actual advertising packet
  BLE.setAdvertisingData(advData);

  BLE.advertise();

  Serial.println("advertising ...");

  BLE.poll();

  delay(30000);

  BLE.end();

  Serial.println("Stop ble");
}

