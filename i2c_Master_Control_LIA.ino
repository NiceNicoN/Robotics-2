/*
  I2C Master Control Demo
  i2c-master-demo-control.ino
  Demonstrate use of I2C bus
  Master sends potentimeter position data
  DroneBot Workshop 2019
  https://dronebotworkshop.com
*/

// Include these libraries for RFID card reader and LCDisplay
#include <SPI.h>                 //Serial Peripheral Interface
#include <MFRC522.h>             //RFID library
#include <LiquidCrystal.h>       //LCDisplay library 
// Include Arduino Wire library for I2C
#include <Wire.h>

const int rs = 7, en = 6, d4 = 5, d5 = 4, d6 = 3, d7 = 2;   //Defining a constant for the needed LCD interface pins
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);                  //Renaming the constant LCD

// Define Slave I2C Address
#define SLAVE_ADDR 9

int SDA_PIN = A4;

int RST_PIN = 9;          // Configurable, see typical pin layout above
int SS_PIN = 10;          // Slave Select Pin
// Analog pin for potentiometer
//int analogPin = 0;
// Integer to hold potentiometer value
int val = 0;
int notval = 900;


MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance
byte accessUID[4] = {0x64, 0xB2, 0x88, 0x2A};


void setup() {
  Serial.begin(9600);		// Initialize serial communications with the PC
	while (!Serial);		  // Do nothing if no serial port is opened (added for Arduinos based on ATMEGA32U4)
	SPI.begin();			    // Init SPI bus
	mfrc522.PCD_Init();		// Init MFRC522
	delay(4);				      // Optional delay. Some board do need more time after init to be ready, see Readme
	mfrc522.PCD_DumpVersionToSerial();	// Show details of PCD - MFRC522 Card Reader details
	Serial.println(F("Scan PICC to see UID, SAK, type, and data blocks..."));

  pinMode(SDA_PIN, INPUT);  // sets the digital pin 13 as output
  //pinMode(RST_PIN, INPUT);    // sets the digital pin 7 as input

  // Initialize I2C communications as Master
  Wire.begin();
}

void loop() {

  lcd.setCursor(0, 0);   
  lcd.print("Hello"); 

  val = analogRead(SDA_PIN);

  Serial.println(val);
  
  // Reset the loop if no new card present on the sensor/reader. This saves the entire process when idle.
	if ( ! mfrc522.PICC_IsNewCardPresent()) {
		return;
	}

	// Select one of the cards
	if ( ! mfrc522.PICC_ReadCardSerial()) {
		return;
	}

	// Dump debug info about the card; PICC_HaltA() is automatically called
	if(mfrc522.uid.uidByte[0] == accessUID[0] && mfrc522.uid.uidByte[1] ==accessUID[1] && mfrc522.uid.uidByte[2] ==accessUID[2] && mfrc522.uid.uidByte[3] ==accessUID[3]){
    Serial.println("Access Granted");
  
    // Write a charatre to the Slave
    Wire.beginTransmission(SLAVE_ADDR);
    Wire.write(val);
    Wire.endTransmission(); 

    lcd.setCursor(0, 1);   
    lcd.print("Nicoleta!");
    delay(2000);
    
    
  }else{
    Serial.println("Access Denied");

    //Wire.beginTransmission(SLAVE_ADDR);
    Wire.write(notval);
    //Wire.endTransmission(); 

    lcd.setCursor(0, 1);
    lcd.print("no no! stranger!");
    delay(2000);
  }
  mfrc522.PICC_HaltA();
  lcd.clear();
 
  

  delay(50);

}