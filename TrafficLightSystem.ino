#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 10
#define RST_PIN 9
MFRC522 rfid(SS_PIN, RST_PIN);

// Lane 1 LEDs
#define R1 2
#define G1 3

// Lane 2 LEDs
#define R2 6
#define G2 5

// Emergency Blue
#define BLUE_LIGHT 7

String amb1 = "443F94";

void setup() {

  Serial.begin(9600);
  SPI.begin();
  rfid.PCD_Init();

  pinMode(R1,OUTPUT);
  pinMode(G1,OUTPUT);
  pinMode(R2,OUTPUT);
  pinMode(G2,OUTPUT);
  pinMode(BLUE_LIGHT,OUTPUT);

  Serial.println("Traffic System Ready");
}

void loop() {

  // Lane1 GREEN Lane2 RED
  digitalWrite(G1,HIGH);
  digitalWrite(R1,LOW);
  digitalWrite(R2,HIGH);
  digitalWrite(G2,LOW);

  delay(3000);

  checkRFID();

  // Lane1 RED Lane2 GREEN
  digitalWrite(G1,LOW);
  digitalWrite(R1,HIGH);
  digitalWrite(R2,LOW);
  digitalWrite(G2,HIGH);

  delay(3000);

  checkRFID();
}

void checkRFID(){

  if (rfid.PICC_IsNewCardPresent() && rfid.PICC_ReadCardSerial()) {

    String tagID="";

    for(byte i=0;i<rfid.uid.size;i++){
      tagID += String(rfid.uid.uidByte[i],HEX);
    }

    tagID.toUpperCase();

    Serial.print("Detected RFID Tag: ");
    Serial.println(tagID);

    if(tagID == amb1){
      activateEmergency();
    }

    rfid.PICC_HaltA();
    rfid.PCD_StopCrypto1();
  }
}

void activateEmergency(){

  Serial.println("🚑 Emergency Vehicle Detected");

  // Both lanes RED
  digitalWrite(R1,HIGH);
  digitalWrite(R2,HIGH);
  digitalWrite(G1,LOW);
  digitalWrite(G2,LOW);

  // Blue light ON
  digitalWrite(BLUE_LIGHT,HIGH);

  delay(4000);

  digitalWrite(BLUE_LIGHT,LOW);

  Serial.println("Returning to normal traffic");
}