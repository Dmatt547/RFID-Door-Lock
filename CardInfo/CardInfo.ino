#include <SPI.h>
#include <RFID.h>
#include <Servo.h>
#include <SD.h> // Include the SD library
#include <RTClib.h> // Include the RTC library for date and time

#define SS_PIN 10
#define RST_PIN 9
RFID rfid(SS_PIN, RST_PIN);

Servo Servo1;

String rfidCard;

File dataFile; // File object for data logging
RTC_DS1307 rtc; // RTC object for date and time

void setup() {
  Serial.begin(9600);
  Serial.println("Starting the RFID Reader...");
  SPI.begin();
  rfid.init();
  Servo1.attach(5);
  Servo1.write(0);
  pinMode(8, OUTPUT);
  

  // Initialize the SD card
  if (!SD.begin(10)) {
    Serial.println("SD Card initialization failed!");
    while (1); // Halt the program
  }
  Serial.print("Initializing SD card...");

  // Initialize the RTC
  if (!rtc.begin()) {
    Serial.println("RTC initialization failed!");
    while (1); // Halt the program
  }

  // Uncomment the line below to set the RTC date and time (if not already set)
  // rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));

  // Open the data log file
  dataFile = SD.open("log.csv", FILE_WRITE);

  if (dataFile) {
    // Write header to the CSV file
    dataFile.println("Date,Time,RFID Card");
    dataFile.close();
  } else {
    Serial.println("Error opening log.csv");
  }
}


void loop() {
  if (rfid.isCard()) {
    if (rfid.readCardSerial()) {
      rfidCard = String(rfid.serNum[0]) + " " + String(rfid.serNum[1]) + " " + String(rfid.serNum[2]) + " " + String(rfid.serNum[3]);
      Serial.println(rfidCard);

      // Get the current date and time from the RTC
      DateTime now = rtc.now();

      Serial.print("Date: ");
      Serial.print(now.year(), DEC);
      Serial.print('/');
      Serial.print(now.month(), DEC);
      Serial.print('/');
      Serial.print(now.day(), DEC);
      Serial.print(" Time: ");
      Serial.print(now.hour(), DEC);
      Serial.print(':');
      Serial.print(now.minute(), DEC);
      Serial.print(':');
      Serial.println(now.second(), DEC);


      // Open the data log file for appending
      dataFile = SD.open("log.csv", FILE_WRITE);

      if (dataFile) {
        // Write the date, time, and RFID card to the CSV file
        dataFile.print(now.year(), DEC);
        dataFile.print('/');
        dataFile.print(now.month(), DEC);
        dataFile.print('/');
        dataFile.print(now.day(), DEC);
        dataFile.print(',');
        dataFile.print(now.hour(), DEC);
        dataFile.print(':');
        dataFile.print(now.minute(), DEC);
        dataFile.print(':');
        dataFile.print(now.second(), DEC);
        dataFile.print(',');
        dataFile.println(rfidCard);
        dataFile.close();
      } else {
        Serial.println("Error opening log.csv");
      }

      if (rfidCard == "168 190 190 18") {
        Serial.println("Access Granted!");
        digitalWrite(8, HIGH);
        digitalWrite(7, HIGH);
        delay(100);
        digitalWrite(8, LOW);
        digitalWrite(7, LOW);
        delay(100);
        // Rest of your code for granting access
      } else {
        Serial.println("Access Denied!");
        digitalWrite(8, HIGH);
        digitalWrite(6, HIGH);
        delay(2000);
        digitalWrite(8, LOW);
        digitalWrite(6, LOW);
      }
    }
    rfid.halt();
  }
}
