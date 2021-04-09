#include <SPI.h>
#include <MFRC522.h>

constexpr uint8_t RST_PIN = D3;     // Configurable, see typical pin layout above
constexpr uint8_t SS_PIN = D4;     // Configurable, see typical pin layout above

MFRC522 rfid(SS_PIN, RST_PIN); // Instance of the class
MFRC522::MIFARE_Key key;

String tag;

void setup() {
  Serial.begin(9600);
  SPI.begin(); // Init SPI bus
  rfid.PCD_Init(); // Init MFRC522

  Serial.println("Put your card to the reader...");
  Serial.println();
}

void loop() {
  // Look for new cards
  if ( ! rfid.PICC_IsNewCardPresent())
    return;

  // Select one of the cards
  if ( ! rfid.PICC_ReadCardSerial()) 
  {
    return;
  }

  //Show UID on serial monitor
  Serial.print("UID tag :");
  String content= "";
  byte letter;
  for (byte i = 0; i < rfid.uid.size; i++) 
  {
     Serial.print(rfid.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(rfid.uid.uidByte[i], HEX);
     content.concat(String(rfid.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(rfid.uid.uidByte[i], HEX));
  }
  Serial.println();
  Serial.print("Message : ");
  content.toUpperCase();

  if (content.substring(1) == "17 2B 03 D9") //change here the UID of the card/cards that you want to give access
  {
    Serial.println("Authorized access");
    Serial.println();
    delay(1000);

    
//    delay(500);
//    digitalWrite(LED_G, HIGH);
//    tone(BUZZER, 500);
//    delay(300);
//    noTone(BUZZER);
//    myServo.write(180);
//    delay(5000);
//    myServo.write(0);
//    digitalWrite(LED_G, LOW);
  }
 
 else   {
    Serial.println(" Access denied");
    delay(1000);

//    digitalWrite(LED_R, HIGH);
//    tone(BUZZER, 300);
//    delay(1000);
//    digitalWrite(LED_R, LOW);
//    noTone(BUZZER);
  }
} 
    
//  if (rfid.PICC_ReadCardSerial()) {
//    for (byte i = 0; i < 4; i++) {
//      tag += rfid.uid.uidByte[i];
//    }
//    Serial.println(tag);
//    tag = "";
//    rfid.PICC_HaltA();
//    rfid.PCD_StopCrypto1();
//  }
//} 
