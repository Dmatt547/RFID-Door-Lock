#include <SPI.h>
#include <RFID.h>
#include <Wire.h>
#include <RTClib.h>
#include <SD.h>
#include <Servo.h>

#define SS_PIN 10
#define RST_PIN 9
RFID rfid(SS_PIN, RST_PIN);

RTC_PCF8523 rtc;
char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

String rfidCard;
Servo myservo;
File dataFile;

void setup() {
  Serial.begin(9600);
  Serial.println("Starting the RFID Reader...Ready to tap a card.");
  SPI.begin();
  rfid.init();
  myservo.attach(5);
  pinMode(8, OUTPUT);

  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    while (1) delay(10);
  }
  Serial.println("RTC has started."); // Added message for RTC start

  if (!rtc.initialized() || rtc.lostPower()) {
    Serial.println("RTC is NOT initialized, let's set the time!");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }

  rtc.start();

  if (!SD.begin(10)) {
    Serial.println("Card initialization failed!");
    while (1);
  }
  Serial.println("SD card initialized."); // Added message for SD card initialization

  dataFile = SD.open("RFID_Log.csv", FILE_WRITE);

  if (dataFile) {
    dataFile.println("Date,Day of Week,Time,RFID Card");
    dataFile.close();
  } else {
    Serial.println("Error opening file!");
  }
}

void loop() {
  if (rfid.isCard()) {
  Serial.println("Card detected!");
  if (rfid.readCardSerial()) {
    Serial.println("Card read successfully!");
      rfidCard = String(rfid.serNum[0]) + " " + String(rfid.serNum[1]) + " " + String(rfid.serNum[2]) + " " + String(rfid.serNum[3]);
      Serial.print("RFID Card Read: ");
      Serial.println(rfidCard);

      DateTime now = rtc.now();

      Serial.print("Access attempted at: ");
      Serial.print(now.year(), DEC);
      Serial.print('/');
      Serial.print(now.month(), DEC);
      Serial.print('/');
      Serial.print(now.day(), DEC);
      Serial.print(" (");
      Serial.print(daysOfTheWeek[now.dayOfTheWeek()]);
      Serial.print(") ");
      Serial.print(now.hour(), DEC);
      Serial.print(':');
      Serial.print(now.minute(), DEC);
      Serial.print(':');
      Serial.print(now.second(), DEC);
      Serial.println();

      dataFile = SD.open("RFID_Log.csv", FILE_WRITE);

      if (dataFile) {
        dataFile.print(now.year());
        dataFile.print('/');
        dataFile.print(now.month());
        dataFile.print('/');
        dataFile.print(now.day());
        dataFile.print(",");
        dataFile.print(daysOfTheWeek[now.dayOfTheWeek()]);
        dataFile.print(",");
        dataFile.print(now.hour());
        dataFile.print(':');
        dataFile.print(now.minute());
        dataFile.print(':');
        dataFile.print(now.second());
        dataFile.print(",");
        dataFile.println(rfidCard);

        dataFile.close();
      } else {
        Serial.println("Error opening file!");
      }

      if (rfidCard == "168 190 190 18") {
       Serial.println("Access Granted!");
        myservo.write(200);
        delay(3000);
        myservo.write(0);
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
        digitalWrite(8, HIGH);
        digitalWrite(7, HIGH);
        delay(100);
        digitalWrite(8, LOW);
        digitalWrite(7, LOW);
        delay(3000);
      } else {
        Serial.println("Access Denied!");
        digitalWrite(8, HIGH);
        digitalWrite(6, HIGH);
        delay(2000);
        digitalWrite(8, LOW);
        digitalWrite(6, LOW);
      }

      rfid.halt();
    }
  }
}
