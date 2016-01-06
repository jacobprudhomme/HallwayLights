/**
 * SyncArduinoClock.
 *
 * portIndex doit être le port auquel est connecté l'Arduino
 *
 * Le temps actuel est envoyé en réponse à une requête envoyée par l'Arduino
 * ou en cliquant n'importe où dans la fenêtre du logiciel
 *
 * Le message pour le temps est 11 caractères ASCII; une en-tête (la lettre 'T')
 * suivi des 10 chiffres représentant le temps de l'ordinateur (temps comme défini sur Unix)
 */

import java.util.*;          // Module pour outils et fonctions standards
import processing.serial.*;  // Module pour traitement des données envoyées par connexion en série

public static final short portIndex = 0;      // Le port en série auquel est connecté l'Arduino
public static final char TIME_HEADER = 'T';   // Caractère pour l'en-tête du message de synchronisation du temps
public static final char TIME_REQUEST = '7';  // Caractère indiquant une requête de l'Arduino pour synchroniser le temps
public static final char LF = 10;             // ASCII pour saut de ligne
public static final char CR = 13;             // ASCII pour retour de chariot
Serial myPort;                                // Objet pour la connexion en série

void setup() {
  size(200, 200);                                            // Grandeur de la fenêtre du logiciel (L x l: 200px x 200px)
  println(Serial.list());                                    // Afficher tous les ports en série disponibles
  println(" Connecting to -> " + Serial.list()[portIndex]);  // Afficher un message de connexion au port défini par portIndex
  myPort = new Serial(this,Serial.list()[portIndex], 9600);  // Commencer la connexion en série
}

void draw() {
  if (myPort.available() > 0) {          // Si le port en série est disponible, alors
    char val = char(myPort.read());      //// Lire ce qui est envoyé
    if(val == TIME_REQUEST) {            //// Si le message reçu est une requête de synchronisation envoyée par l'Arduino, alors
       long t = getTimeNow();            ////// Obtenir temps de l'ordinateur et l'assigner à une variable
       sendTimeMessage(TIME_HEADER, t);  ////// VOIR FONCTION sendTimeMessage
    } else {                             //// Sinon, alors
       if(val == LF)                     ////// Si le message obtenu est vide, alors
           ;                             //////// Ne rien faire
       else if(val == CR)                ////// Si le message obtenu est un retour à la ligne, alors
         println();                      //////// Afficher un retour à la ligne
       else                              ////// Sinon, alors
         print(val);                     //////// Afficher le message quelconque
    }
  }
}

void mousePressed() {                          // Si n'importe où dans la fenêtre du logiciel est cliqué, alors
  sendTimeMessage(TIME_HEADER, getTimeNow());  //// Envoie un message contenant le temps à l'Arduino (VOIR FONCTION getTimeNow)
}


void sendTimeMessage(char header, long time) {
  String timeStr = String.valueOf(time);        // Convertir le paramètre de temps (time) en une chaîne de caractères
  myPort.write(header);                         // Envoyer l'en-tête indiquant que ceci est un message de synchronisation à l'Arduino
  myPort.write(timeStr);                        // Envoyer le temps à être synchronisé
}

long getTimeNow() {
  GregorianCalendar cal = new GregorianCalendar();  // Objet pour la date
  cal.setTime(new Date());                          // Obtenir date actuelle
  int tzo = cal.get(Calendar.ZONE_OFFSET);          // Obtenir fuseau horaire actuel
  int dst = cal.get(Calendar.DST_OFFSET);           // Obtenir si heure d'été ou non
  long now = (cal.getTimeInMillis() / 1000);        // Obtenir temps actuel, convertir en secondes
  now = now + (tzo / 1000) + (dst / 1000);          // Calculer le temps actuel en prenant compte du fuseau horaire et du statut de l'heure d'été
  return now;                                       // Retourner cette valeur de temps
}