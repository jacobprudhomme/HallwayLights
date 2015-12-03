#define TIME_MSG_LEN  11   // time sync to PC is HEADER followed by Unix time_t as ten ASCII digits
#define TIME_HEADER  'T'   // Header tag for serial time sync message
#define TIME_REQUEST  7    // ASCII bell character requests a time sync message

#include <Time.h>
#include <TimeLib.h>

int hours_red_reg[] = {9, 10, 12, 14};
int minutes_red_reg[] = {15, 40, 55, 15};
int hours_green_reg[] = {10, 11, 14};
int minutes_green_reg[] = {30, 55, 5};
int hours_red_cap[] = {9, 10, 11, 13, 14};
int minutes_red_cap[] = {15, 22, 30, 22, 30};
int hours_green_cap[] = {10, 11, 12, 12};
int minutes_green_cap[] = {15, 22, 30, 22};
int hours_off[] = {15};
int minutes_off[] = {30};
int del[] = {11, 12, 13}; // RYG
boolean wed = false;   // Is it Wednesday?

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
  switch(weekday()) {
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

  Serial.println(minute());
  Serial.println(hour());

  if(wed == false) {
    for(int i = 0; i < 4; i++) {
      if(hour() == hours_red_reg[i]) {
        Serial.println("The hours align");
        if(minute() == minutes_red_reg[i]) {
          Serial.println("The minutes align");
          for(int j = 0; j < 3; j++) {
            digitalWrite(del[j], LOW);
          }
          digitalWrite(del[0], HIGH);
        }
      }
    }
    for(int i = 0; i < 3; i++) {
      if(hour() == hours_green_reg[i]) {
        Serial.println("The hours align");
        if(minute() == minutes_green_reg[i]) {
          Serial.println("The minutes align");
          for(int j = 0; j < 3; j++) {
            digitalWrite(del[j], LOW);
          }
          digitalWrite(del[2], HIGH);
        }
      }
    }
  } else {
    for(int i = 0; i < 5; i++) {
      if(hour() == hours_red_cap[i]) {
        Serial.println("The hours align");
        if(minute() == minutes_red_cap[i]) {
          Serial.println("The minutes align");
          for(int j = 0; j < 3; j++) {
            digitalWrite(del[j], LOW);
          }
          digitalWrite(del[0], HIGH);
        }
      }
    }
    for(int i = 0; i < 4; i++) {
      if(hour() == hours_green_cap[i]) {
        Serial.println("The hours align");
        if(minute() == minutes_green_cap[i]) {
          Serial.println("The minutes align");
          for(int j = 0; j < 3; j++) {
            digitalWrite(del[j], LOW);
          }
          digitalWrite(del[2], HIGH);
        }
      }
    }
  }

  delay(30000); // Check every 30 seconds
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
