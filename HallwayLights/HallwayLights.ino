#include <Time.h>
#include <TimeLib.h>

int del[] = {11, 12, 13}; // RYG
boolean wed = false;   // Is it Wednesday?

void setup() {
  Serial.begin(9600);

  for(int i = 0; i < 3; i++) {
    pinMode(del[i], OUTPUT);
  }
}

void loop() {
  switch(weekday()) {
    case 1:
      Serial.println("Saturday");
      wed = false;
      break;
    case 2:
      Serial.println("Sunday");
      wed = false;
      break;
    case 3:
      Serial.println("Monday");
      wed = false;
      break;
    case 4:
      Serial.println("Tuesday");
      wed = false;
      break;
    case 5:
      Serial.println("Wednesday");
      wed = true;
      break;
    case 6:
      Serial.println("Thursday");
      wed = false;
      break;
    case 7:
      Serial.println("Friday");
      wed = false;
      break;
  }

  for(int i = 0; i < 3; i++) {
    digitalWrite(del[i], LOW);
  }

  if(wed == true) {
    digitalWrite(del[0], HIGH);
  } else {
    digitalWrite(del[2], HIGH);
  }

  delay(3600000); // Check every hour
}
