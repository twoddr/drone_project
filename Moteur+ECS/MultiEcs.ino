#include <Servo.h>

Servo esc[4];   // Tableau d'objets permettant le contrôle des ESC pour les quatre moteurs
int val[4];     // Tableau pour les valeurs de commande de chaque moteur
int pot_val;    // Valeur lue à partir du port série

const float amplitude = 79.5; // Amplitude de la sinusoïde (179 - 20) / 2
const float offset = 99.5;    // Offset de la sinusoïde (179 + 20) / 2
const float frequency = 0.05; // Fréquence de la sinusoïde

void setup() {
   for (int i = 0; i < 4; i++) {
       esc[i].attach(9 + i, 1000, 2000); // Attache des ESC aux ports numériques 9, 10, 11 et 12 (port PWM obligatoire) avec des valeurs min et max pour les impulsions
       delay(15);
   }

   Serial.begin(9600);        // Initialisation du port série

   for (int i = 0; i < 4; i++) {
       // Initialisation des ESC
       esc[i].writeMicroseconds(1000); // Envoi d'un signal minimum à chaque ESC
       delay(1000);
       esc[i].writeMicroseconds(2000); // Envoi d'un signal maximum à chaque ESC
       delay(1000);
       esc[i].writeMicroseconds(1000); // Envoi d'un signal minimum à chaque ESC
       val[i] = 0;
   }

   pinMode(LED_BUILTIN, OUTPUT); // Configuration de la broche de la LED intégrée en tant que sortie
   // Informations pour l'utilisateur
   Serial.println("Enter a number between 0 and 179");
   Serial.println("(0 = stop - 179 = maximum speed)");
   Serial.println("Starting from 20)");
}

void loop() {
  float sinValue = sin(millis() * frequency / 1000.0); // Calcul de la valeur de la sinusoïde en fonction du temps
  
  // Mise à l'échelle de la valeur de la sinusoïde dans la plage souhaitée
  int scaledValue = int(sinValue * amplitude + offset);
  
  // Affichage du résultat
  Serial.println(scaledValue);
  
  // Lecture de la valeur depuis le port série
  pot_val = Serial.parseInt();   
  // Vérification si la valeur est valide
  if (pot_val < 180 && pot_val > 0) {
    Serial.print("Read value: ");
    Serial.println(pot_val);
    digitalWrite(LED_BUILTIN, HIGH); // Allumage de la LED intégrée
    // Attribution de la valeur lue à chaque moteur
    for (int i = 0; i < 4; i++) {
        val[i] = pot_val;
    }
  } else {
    digitalWrite(LED_BUILTIN, LOW); // Extinction de la LED intégrée
  }
  // Envoi de la valeur de commande à chaque ESC
  for (int i = 0; i < 4; i++) {
      esc[i].write(val[i]);            
      Serial.print("Value sent to ESC ");
      Serial.print(i);
      Serial.print(": ");
      Serial.println(val[i]);
  }
}
