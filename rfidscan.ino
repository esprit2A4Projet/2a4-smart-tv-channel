#include <SPI.h>
#include <MFRC522.h>

const int SS_PIN = 10; // SDA RFID
const int RST_PIN = 9;


MFRC522 module_rfid(SS_PIN, RST_PIN);

void setup() {
  
  SPI.begin();
  module_rfid.PCD_Init();
  Serial.begin(9600);
  
}

void loop() {
 
  if (!module_rfid.PICC_IsNewCardPresent()) {
    return;
  }

  if (!module_rfid.PICC_ReadCardSerial()) {
    return;
  }
  
  String UID = "";

  for (byte i = 0; i < module_rfid.uid.size; i++) {
    UID.concat(String(module_rfid.uid.uidByte[i] < 0x10 ? " 0" : " "));
    UID.concat(String(module_rfid.uid.uidByte[i], HEX));
  }

  UID.toUpperCase();

  Serial.println(UID);
  delay(1000); // Delay to ensure the complete tag is sent

  

  
}
