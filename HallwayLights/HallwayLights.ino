#define TIME_MSG_LEN  11   // time sync to PC is HEADER followed by Unix time_t as ten ASCII digits
#define TIME_HEADER  'T'   // Header tag for serial time sync message
#define TIME_REQUEST  7    // ASCII bell character requests a time sync message

#include <Time.h>     // Time library
#include <TimeLib.h>  // Time library

int hours_red_reg[] = {9, 10, 13, 14};        // Les heures de commencement des périodes
int minutes_red_reg[] = {15, 40, 2, 15};     // Les minutes de commencement des périodes
int hours_green_reg[] = {10, 11, 14, 15};         // Les heures de fin des périodes
int minutes_green_reg[] = {30, 55, 5, 30};        // Les minutes de fin des périodes
int hours_red_cap[] = {9, 10, 11, 13, 14};    // Les heures de commencement des périodes sur une journée CAP
int minutes_red_cap[] = {15, 22, 30, 22, 30}; // Les minutes de commencement des périodes sur une journée CAP
int hours_green_cap[] = {10, 11, 12, 12, 15};     // Les heures de fin des périodes sur une journée CAP
int minutes_green_cap[] = {15, 22, 30, 22, 30};   // Les minutes de fin des périodes sur une journée CAP
int del[] = {11, 12, 13};                     // DELs: Rouge, Jaune, Vert
boolean wed = false;                          // Est-ce que c'est une journée CAP (mercredi)?

void setup() {
  Serial.begin(9600);

  for(int i = 0; i < 3; i++) {
    pinMode(del[i], OUTPUT);
  }

  digitalWrite(del[2], HIGH);
}

void loop() {
  Serial.println('7');
  processSyncMessage();

  time_t t = now();
  
  switch(weekday(t)) {
    case 0:
      Serial.println("Saturday");
      wed = false;
      break;
    case 1:
      Serial.println("Sunday");
      wed = false;
      break;
    case 2:
      Serial.println("Monday");
      wed = false;
      break;
    case 3:
      Serial.println("Tuesday");
      wed = false;
      break;
    case 4:
      Serial.println("Wednesday");
      wed = true;
      break;
    case 5:
      Serial.println("Thursday");
      wed = false;
      break;
    case 6:
      Serial.println("Friday");
      wed = false;
      break;
  }

  Serial.println(hour(t));
  Serial.println(minute(t));

  if(wed == false) {
    for(int i = 0; i < 4; i++) {
      if((hour(t) >= hours_red_reg[i]) && (hour(t) <= hours_green_reg[i])) {
        Serial.println("The hours align red reg");
        if((minute(t) >= minutes_red_reg[i]) && (minute(t) < (minutes_red_reg[i] + 75))) {
          Serial.println("The minutes align red reg");
          for(int j = 0; j < 3; j++) {
            digitalWrite(del[j], LOW);
          }
          digitalWrite(del[0], HIGH);
        }
      }
    }
    for(int i = 0; i < 4; i++) {
      if((hour(t) >= hours_green_reg[i]) && (hour(t) <= hours_red_reg[i + 1])) {
        Serial.println("The hours align green reg");
        if(minute(t) >= minutes_green_reg[i]) {
          Serial.println("The minutes align green reg");
          for(int j = 0; j < 3; j++) {
            digitalWrite(del[j], LOW);
          }
          digitalWrite(del[2], HIGH);
        }
      }
    }
  } else {
    for(int i = 0; i < 5; i++) {
      if((hour(t) >= hours_red_cap[i]) && (hour(t) <= hours_green_cap[i])) {
        Serial.println("The hours align red cap");
        if((minute(t) >= minutes_red_cap[i]) && (minute(t) < (minutes_red_cap[i] + 60))) {
          Serial.println("The minutes align red cap");
          for(int j = 0; j < 3; j++) {
            digitalWrite(del[j], LOW);
          }
          digitalWrite(del[0], HIGH);
        }
      }
    }
    for(int i = 0; i < 5; i++) {
      if((hour(t) >= hours_green_cap[i]) && (hour(t) <= hours_red_cap[i + 1])) {
        Serial.println("The hours align for green cap");
        if(minute(t) >= minutes_green_cap[i]) {
          Serial.println("The minutes align green cap");
          for(int j = 0; j < 3; j++) {
            digitalWrite(del[j], LOW);
          }
          digitalWrite(del[2], HIGH);
        }
      }
    }
  }

  delay(3000); // Check every 30 seconds
}

void processSyncMessage() {
  // if time sync available from serial port, update time and return true
  while(Serial.available() >=  TIME_MSG_LEN ){  // time message consists of header & 10 ASCII digits
    char c = Serial.read() ;
    Serial.print(c);
    if( c == TIME_HEADER ) {
      time_t pctime = 0;
      for(int i=0; i < TIME_MSG_LEN -1; i++){
        c = Serial.read();
        if( c >= '0' && c <= '9'){
          pctime = (10 * pctime) + (c - '0') ; // convert digits to a number
        }
      }
      setTime(pctime);   // Sync Arduino clock to the time received on the serial port
    }
  }
}
