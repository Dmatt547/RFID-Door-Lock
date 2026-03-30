# RFID Door Lock System

## Overview
This project is an RFID-based door locking system built using Arduino, designed to provide secure access control with data logging capabilities. The system records each access attempt, including timestamp, card ID, and access status (granted or denied), enabling improved security monitoring and analysis.

Unlike basic RFID systems, this implementation integrates real-time data capture to provide insights into access patterns and potential unauthorized attempts. 

---

## Features
- RFID-based access control
- Real-time timestamp logging using RTC module
- Data logging to SD card (CSV format)
- Access validation (authorised vs unauthorised cards)
- Visual feedback using LEDs
- Audible feedback using buzzer
- Servo motor-controlled locking mechanism

---

## Technologies Used
- Arduino Uno
- RFID module
- Real-Time Clock (RTC)
- Data logging shield (SD card)
- Servo motor
- Breadboard and jumper wires
- C++ (Arduino)

---

## System Architecture

### Hardware Components
- RFID reader – detects and reads card IDs  
- Arduino Uno – processes data and controls system  
- RTC module – timestamps each access event  
- Data logging shield – stores data on SD card  
- Servo motor – controls door lock mechanism  
- LEDs – indicate access status  
- Buzzer – provides audio feedback  

The system connects all components through the Arduino, which acts as the central controller. 

---

## How It Works
1. An RFID card is scanned by the reader  
2. The system checks the card ID against authorised entries  
3. If authorised → access granted:
   - Servo unlocks door  
   - LED and buzzer provide feedback  
4. If unauthorised → access denied:
   - Door remains locked  
   - Alert feedback is triggered  
5. The system logs:
   - Date and time  
   - Card ID  
   - Access status  

All data is stored on an SD card for later analysis. 

---

## Data Collection
- Each scan is recorded with timestamp and access status  
- Data is stored in CSV format on the SD card  
- Duplicate entries are reduced using scan delay logic  
- Data can be analysed to identify access patterns and security risks  

---

## Results
- Successful logging of both authorised and unauthorised access attempts  
- Majority of scans were "Access Granted" due to valid users  
- System effectively identified and flagged unauthorised attempts  
- Data enabled analysis of access frequency and usage patterns  

---

## Challenges
- Integrating multiple hardware components  
- Configuring SD card and data logging correctly  
- Managing power supply for servo motor and Arduino  
- Ensuring reliable communication between modules  

---

## Ethical Considerations
- Privacy concerns from tracking user activity  
- Data security risks if logs are accessed without protection  
- Need for user consent and transparency in data collection  
- Importance of secure data storage and retention policies  

---

## Future Improvements
- Integration with smart home systems  
- Predictive automation based on user behaviour  
- LCD display for real-time feedback  
- Improved data security and encryption  
- Advanced analytics for access patterns  

---

## Key Learnings
- Hardware and software integration in IoT systems  
- Data logging and real-time processing  
- Working with sensors and microcontrollers  
- Importance of security and ethical considerations in data systems  
