#define TIME_MSG_LEN  11   // Longueur du message à être reçu du sketch Processing
#define TIME_HEADER  'T'   // En-tête qui déclare que le message reçu du sketch Processing est pour synchroniser le temps
#define TIME_REQUEST  7    // Requête envoyée au sketch Processing pour demander une synchroniser du temps

#include <Time.h>     // Module pour les fonctions de temps
#include <TimeLib.h>  // Module pour les fonctions de temps

int hours_red_reg[] = {9, 10, 12, 14};            // Les heures de commencement des périodes sur une journée normale
int minutes_red_reg[] = {15, 40, 50, 15};         // Les minutes de commencement des périodes sur une journée normale
int hours_green_reg[] = {10, 11, 14, 15};         // Les heures de fin des périodes sur une journée normale
int minutes_green_reg[] = {30, 55, 5, 30};        // Les minutes de fin des périodes sur une journée normale
int hours_yellow_reg[] = {9, 10, 12, 14};         // Les heures d'alerte au commencement des périodes sur une journée normale
int minutes_yellow_reg[] = {12, 37, 47, 12};      // Les minutes d'alerte au commencement des périodes sur une journée normale (3 minutes avant la cloche)
int hours_red_cap[] = {9, 10, 11, 13, 14};        // Les heures de commencement des périodes sur une journée CAP
int minutes_red_cap[] = {15, 22, 30, 22, 30};     // Les minutes de commencement des périodes sur une journée CAP
int hours_green_cap[] = {10, 11, 12, 12, 15};     // Les heures de fin des périodes sur une journée CAP
int minutes_green_cap[] = {15, 22, 30, 22, 30};   // Les minutes de fin des périodes sur une journée CAP
int hours_yellow_cap[] = {9, 10, 11, 13, 14};     // Les heures d'alerte au commencement des périodes sur une journée CAP
int minutes_yellow_cap[] = {13, 20, 28, 20, 28};  // Les minutes d'alerte au commencement des périodes sur une journée CAP (2 minutes avant la cloche)
int del[] = {11, 12, 13};                         // DELs: Rouge, Jaune, Vert
int btn = 10;                                     // Bouton pour activer le mode pour l'alarme à feu
boolean wed = false;                              // Est-ce que c'est une journée CAP (mercredi)?

void setup() {
  Serial.begin(9600); // Ouvrir la communication en série entre l'ordinateur et l'Arduino

  for(int i = 0; i < 3; i++) {  // Pour chacune des 3 DELs,
    pinMode(del[i], OUTPUT);    //// Définir en tant que sortie
  }
  pinMode(btn, INPUT);          // Définir bouton d'alarme à feu en tant qu'entrée

  digitalWrite(del[2], HIGH); // Commencer sketch avec DEL verte allumée
}

void loop() {
  Serial.println('7');  // Envoyer la requête de synchronisation au sketch Processing
  processSyncMessage(); // VOIR FONCTION processSyncMessage

  time_t t = now(); // Pour éviter des ambiguités si le temps actuel change de minute durant la boucle
  
  switch(weekday(t)) {              // Si le jour de la semaine est...
    case 0:                         //// ...samedi, alors
      Serial.println("Saturday");   ////// Envoyer un message au sketch Processing
      wed = false;                  ////// Ce n'est pas mercredi (journée normale)
      break;
    case 1:                         //// ...dimanche, alors
      Serial.println("Sunday");     ////// Envoyer un message au sketch Processing
      wed = false;                  ////// Ce n'est pas mercredi (journée normale)
      break;
    case 2:                         //// ...lundi, alors
      Serial.println("Monday");     ////// Envoyer un message au sketch Processing
      wed = false;                  ////// Ce n'est pas mercredi (journée normale)
      break;
    case 3:                         //// ...mardi, alors
      Serial.println("Tuesday");    ////// Envoyer un message au sketch Processing
      wed = false;                  ////// Ce n'est pas mercredi (journée normale)
      break;
    case 4:                         //// ...mercredi, alors
      Serial.println("Wednesday");  ////// Envoyer un message au sketch Processing
      wed = true;                   ////// C'est mercredi (journée CAP)
      break;
    case 5:                         //// ...jeudi, alors
      Serial.println("Thursday");   ////// Envoyer un message au sketch Processing
      wed = false;                  ////// Ce n'est pas mercredi (journée normale)
      break;
    case 6:                         //// ...vendredi, alors
      Serial.println("Friday");     ////// Envoyer un message au sketch Processing
      wed = false;                  ////// Ce n'est pas mercredi (journée normale)
      break;
  }

  Serial.println(hour(t));    // Envoyer l'heure au sketch Processing
  Serial.println(minute(t));  // Envoyer la minute au sketch Processing

  if(wed == false) {                                                                                                                                                            // Si c'est une journée normale, alors
    if(digitalRead(btn) != 1) {                                                                                                                                                 //// Si le bouton d'alarme à feu n'a pas été pesé, alors
      for(int i = 0; i < 4; i++) {                                                                                                                                              ////// Pour chaque élément dans les 3 listes des heures de classe pour une journée normale,
        if(((hour(t) * 60 + minute(t)) >= (hours_yellow_reg[i] * 60 + minutes_yellow_reg[i])) && ((hour(t) * 60 + minute(t)) < (hours_red_reg[i] * 60 + minutes_red_reg[i]))) { //////// Si le temps actuel est égal ou entre le temps de la presque-fin de la pause et le commencement de la période, alors
          Serial.println("Fin de pause");                                                                                                                                       ////////// Envoyer un message que c'est presque la fin de la pause
          for(int j = 0; j < 3; j++) {                                                                                                                                          ////////// Pour chacune des 3 DELs,
            digitalWrite(del[j], LOW);                                                                                                                                          //////////// Éteindre
          }
          digitalWrite(del[1], HIGH);                                                                                                                                           ////////// Clignoter DEL jaune
          delay(500);
          digitalWrite(del[1], LOW);
          delay(500);
        }
        if(((hour(t) * 60 + minute(t)) >= (hours_red_reg[i] * 60 + minutes_red_reg[i])) && ((hour(t) * 60 + minute(t)) < (hours_green_reg[i] * 60 + minutes_green_reg[i]))) { //////// Si le temps actuel est égal ou entre le temps de commencement de la période et de la pause, alors
          Serial.println("En classe");                                                                                                                                        ////////// Envoyer un message que c'est le temps des classes
          for(int j = 0; j < 3; j++) {                                                                                                                                        ////////// Pour chacune des 3 DELs,
            digitalWrite(del[j], LOW);                                                                                                                                        //////////// Éteindre
          }
          digitalWrite(del[0], HIGH);                                                                                                                                         ////////// Allumer DEL rouge
        }
        if(((hour(t) * 60 + minute(t)) >= (hours_green_reg[i] * 60 + minutes_green_reg[i])) && ((hour(t) * 60 + minute(t)) < (hours_yellow_reg[i + 1] * 60 + minutes_yellow_reg[i + 1]))) { //////// Si le temps actuel est égal ou entre le temps de commencement de la pause et de la presque-fin de la pause, alors
          Serial.println("En pause");                                                                                                                                                 ////////// Envoyer un message que c'est le temps de la pause
          for(int j = 0; j < 3; j++) {                                                                                                                                                ////////// Pour chacune des 3 DELs,
            digitalWrite(del[j], LOW);                                                                                                                                                //////////// Éteindre
          }
          digitalWrite(del[2], HIGH);                                                                                                                                                 ////////// Allumer DEL verte
        }
      }
    } else {                        //// Sinon, alors
      for(int j = 0; j < 3; j++) {  ////// Pour chacune des 3 DELs,
        digitalWrite(del[j], LOW);  //////// Éteindre
      }
      while(1) {                    ////// Boucler infiniment (jusqu'à ce que le bouton RESET est pesé)
        // Faire clignoter DEL rouge
        digitalWrite(del[0], HIGH);
        delay(500);
        digitalWrite(del[0], LOW);
        delay(500);
      }
    }
  } else {                                                                                                                                                                      // Si c'est une journée CAP, alors
    if(digitalRead(btn) != 1) {                                                                                                                                                 //// Si le bouton d'alarme à feu n'a pas été pesé, alors
      for(int i = 0; i < 5; i++) {                                                                                                                                              ////// Pour chaque élément dans les 3 listes des heures de classe pour une journée CAP,
        if(((hour(t) * 60 + minute(t)) >= (hours_yellow_cap[i] * 60 + minutes_yellow_cap[i])) && ((hour(t) * 60 + minute(t)) < (hours_red_cap[i] * 60 + minutes_red_cap[i]))) { //////// Si le temps actuel est égal ou entre le temps de la presque-fin de la pause et le commencement de la période, alors
          Serial.println("Fin de pause");                                                                                                                                       ////////// Envoyer un message que c'est presque la fin de la pause
          for(int j = 0; j < 3; j++) {                                                                                                                                          ////////// Pour chacune des 3 DELs,
            digitalWrite(del[j], LOW);                                                                                                                                          //////////// Éteindre
          }
          digitalWrite(del[1], HIGH);                                                                                                                                           ////////// Clignoter DEL jaune
          delay(500);
          digitalWrite(del[1], LOW);
          delay(500);
        }
        if(((hour(t) * 60 + minute(t)) >= (hours_red_cap[i] * 60 + minutes_red_cap[i])) && ((hour(t) * 60 + minute(t)) < (hours_green_cap[i] * 60 + minutes_green_cap[i]))) { //////// Si le temps actuel est égal ou entre le temps de commencement de la période et de la pause, alors
          Serial.println("En classe");                                                                                                                                        ////////// Envoyer un message que c'est le temps des classes
          for(int j = 0; j < 3; j++) {                                                                                                                                        ////////// Pour chacune des 3 DELs,
            digitalWrite(del[j], LOW);                                                                                                                                        //////////// Éteindre
          }
          digitalWrite(del[0], HIGH);                                                                                                                                         ////////// Allumer DEL rouge
        }
        if(((hour(t) * 60 + minute(t)) >= (hours_green_cap[i] * 60 + minutes_green_cap[i])) && ((hour(t) * 60 + minute(t)) < (hours_yellow_cap[i + 1] * 60 + minutes_yellow_cap[i + 1]))) { //////// Si le temps actuel est égal ou entre le temps de commencement de la pause et de la presque-fin de la pause, alors
          Serial.println("En pause");                                                                                                                                                       ////////// Envoyer un message que c'est le temps de la pause
          for(int j = 0; j < 3; j++) {                                                                                                                                                      ////////// Pour chacune des 3 DELs,
            digitalWrite(del[j], LOW);                                                                                                                                                      //////////// Éteindre
          }
          digitalWrite(del[2], HIGH);                                                                                                                                                       ////////// Allumer DEL verte
        }
      }
    } else {                        //// Sinon, alors
      for(int j = 0; j < 3; j++) {  ////// Pour chacune des 3 DELs,
        digitalWrite(del[j], LOW);  //////// Éteindre
      }
      while(1) {                    ////// Boucler infiniment (jusqu'à ce que le bouton RESET est pesé)
        // Faire clignoter DEL rouge
        digitalWrite(del[0], HIGH);
        delay(500);
        digitalWrite(del[0], LOW);
        delay(500);
      }
    }
  }

  delay(5000); // Attendre 5 secondes avant de répéter la boucle
}

void processSyncMessage() {
  // Si le sketch Processing a envoyé un message de synchronisation du temps, mettre à jour l'horloge de l'Arduino
  while(Serial.available() >=  TIME_MSG_LEN ){  // Tant que le message reçu est de la bonne longueur, alors
    char c = Serial.read() ;                    //// Lire le message, un caractère à la fois
    Serial.print(c);                            //// Afficher le message sur le moniteur en série, un caractère à la fois
    if( c == TIME_HEADER ) {                    //// Si le caractère est l'en-tête du message qui démarque une synchronisation du temps, alors
      time_t pctime = 0;                        ////// Créer nouvelle variable de temps vide
      for(int i=0; i < TIME_MSG_LEN -1; i++){   ////// Pour chaque caractère dans le message de synchronisation,
        c = Serial.read();                      //////// Lire le message, un caractère à la fois
        if( c >= '0' && c <= '9'){              //////// Si le caractère est un nombre (indiquant le temps), alors
          pctime = (10 * pctime) + (c - '0');   ////////// La variable de temps vide prend le nouveau temps envoyé par le sketch Processing
        }
      }
      setTime(pctime);                          ////// Synchroniser l'horloge interne de l'Arduino avec le nouveau temps reçu
    }
  }
}
