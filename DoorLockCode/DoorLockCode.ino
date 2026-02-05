#include <SPI.h>
#include <RFID.h>
#include <Wire.h>
#include <RTClib.h>
#include <SD.h>
#include <Servo.h>

#define RFID_SS_PIN 2
#define RST_PIN 9
#define SD_SS_PIN 10

RFID rfid(RFID_SS_PIN, RST_PIN);
RTC_PCF8523 rtc;

const char* filename = "RFID_Log.csv";

char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

String rfidCard;
bool isRFIDTagDetected = false;
File dataFile;

String accessStatus;

Servo myservo;

void setup() {
  Serial.begin(9600);
  Serial.println("Starting the RFID Reader...");
  myservo.attach(5);
  myservo.write(10);
  pinMode(8, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(6, OUTPUT);
  SPI.begin();
  rfid.init();

  pinMode(SD_SS_PIN, OUTPUT);   
  pinMode(RFID_SS_PIN, OUTPUT); 

  digitalWrite(SD_SS_PIN, HIGH);   
  digitalWrite(RFID_SS_PIN, HIGH); 

  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }

  Serial.println("RTC has started.");

  if (!rtc.initialized() || rtc.lostPower()) {
    Serial.println("RTC is NOT initialized, let's set the time!");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }

  rtc.start();

  if (!SD.begin(SD_SS_PIN)) {
    Serial.println("Card initialization failed!");
    while (1);
  }

  Serial.println("SD card initialized.");

  

  if (!SD.exists(filename)) {
    dataFile = SD.open(filename, FILE_WRITE);
    if (dataFile) {
      dataFile.println("Date,Day of Week,Time,RFID Card,Access Status");
      dataFile.close();
    } else {
      Serial.println("Error creating new file!");
    }
  }
}

void logData() {
  if (isRFIDTagDetected) {
    digitalWrite(RFID_SS_PIN, HIGH);
    digitalWrite(SD_SS_PIN, LOW);

    DateTime now = rtc.now();

    dataFile = SD.open("RFID_Log.csv", FILE_WRITE);

    if (dataFile) {
      dataFile.print(now.year()); dataFile.print('/');
      dataFile.print(now.month()); dataFile.print('/');
      dataFile.print(now.day()); dataFile.print(",");
      dataFile.print(daysOfTheWeek[now.dayOfTheWeek()]); dataFile.print(",");
      dataFile.print(now.hour()); dataFile.print(':');
      dataFile.print(now.minute()); dataFile.print(':');
      dataFile.print(now.second()); dataFile.print(",");
      dataFile.print(rfidCard); dataFile.print(",");
      dataFile.println(accessStatus);

      dataFile.close();
    } else {
      Serial.println("Error opening file for writing!");
    }

    digitalWrite(SD_SS_PIN, HIGH);
    digitalWrite(RFID_SS_PIN, LOW);

    isRFIDTagDetected = false;
  }
}

void loop() {
  if (rfid.isCard()) {
    if (rfid.readCardSerial()) {
      delay(1000); 
      rfidCard = String(rfid.serNum[0]) + " " + String(rfid.serNum[1]) + " " + String(rfid.serNum[2]) + " " + String(rfid.serNum[3]);
      Serial.println(rfidCard);

      if (rfidCard == "168 190 190 18") {
        Serial.println("Access Granted!");
        accessStatus = ("Access Granted!");
        digitalWrite(8, HIGH);
        digitalWrite(7, HIGH);
        delay(100);
        digitalWrite(8, LOW);
        digitalWrite(7, LOW);
        delay(100);
        digitalWrite(8, HIGH);
        digitalWrite(7, HIGH);
        delay(100);
        digitalWrite(8, LOW);
        digitalWrite(7, LOW);
        delay(100);
        myservo.write(70);
        delay(2000);
        myservo.write(10);
        // delay(1000);
        // myservo.write(80);
        
      } else {
        Serial.println("Access Denied!");
        accessStatus = ("Access Denied!");
        digitalWrite(8, HIGH);
        digitalWrite(6, HIGH);
        delay(500);
        digitalWrite(8, LOW);
        digitalWrite(6, LOW);
      }

      rfid.halt();
      isRFIDTagDetected = true;
    }
  }

  logData();
}