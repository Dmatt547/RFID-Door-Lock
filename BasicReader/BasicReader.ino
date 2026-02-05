#include <SPI.h>
#include <RFID.h>
#include <Wire.h>
#include <RTClib.h>
#include <Servo.h>

#define SS_PIN 10
#define RST_PIN 9
RFID rfid(SS_PIN, RST_PIN);

RTC_PCF8523 rtc;
char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

String rfidCard;

Servo myservo;

void setup() {
  Serial.begin(9600);
  Serial.println("Starting the RFID Reader...");
  SPI.begin();
  rfid.init();
  myservo.attach(5);
  pinMode(8, OUTPUT);

  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    while (1) delay(10);
  }

  if (!rtc.initialized() || rtc.lostPower()) {
    Serial.println("RTC is NOT initialized, let's set the time!");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }

  rtc.start();  // Restart the RTC if it was stopped
}

void loop() {
  if (rfid.isCard()) {
    if (rfid.readCardSerial()) {
      rfidCard = String(rfid.serNum[0]) + " " + String(rfid.serNum[1]) + " " + String(rfid.serNum[2]) + " " + String(rfid.serNum[3]);
      Serial.println(rfidCard);

      DateTime now = rtc.now();  // Get the current date and time from the RTC

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

      if (rfidCard == "168 190 190 18") {
        Serial.println("Access Granted!");
        myservo.write(180);
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
