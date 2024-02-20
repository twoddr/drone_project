#include <Servo.h>

Servo esc;   // Création de l'objet permettant le contrôle de l'ESC

int val; //
int pot_val;

const float amplitude = 79.5; // (179 - 20) / 2
const float offset = 99.5;    // (179 + 20) / 2

// Fréquence de la sinusoïde
const float frequency = 0.05;


void setup() {
   esc.attach(9,1000,2000); // On attache l'ESC au port numérique 9 (port PWM obligatoire); min pulse width et max pulse width
   delay(15);
   Serial.begin(9600);

   // Initialisation de l'ESC
   //  (certains ESC ont besoin d'une "procédure d'initialisation"
   //   pour devenir opérationnels - voir notice)
   esc.write(0);
   delay(1000);
   esc.write(1023);
   delay(1000);
   esc.write(0);
   val = 0;
   //minimun 45 pour soulever avec 1 moteur 


    pinMode(LED_BUILTIN, OUTPUT);
   // Quelques informations pour l'utilisateur
   Serial.println("Saisir un nombre entre 0 et 179");
   Serial.println("(0 = arret - 179 = vitesse maxi");
   Serial.println(" demarrage a partir de 20)");
   }

void loop() {

  float sinValue = sin(millis() * frequency / 1000.0); // Utilisez millis() pour créer une variation dans le temps
  
  // Mise à l'échelle de la valeur de la sinusoïde dans la plage souhaitée
  int scaledValue = int(sinValue * amplitude + offset);
  
  // Affichage du résultat
  Serial.println(scaledValue);
  //val = scaledValue;


    //pot_val=analogRead(A0);
    //pot_val=map(pot_val, 0, 1023, 0 , 180);
   //if (pot_val > 0) {
  pot_val = Serial.parseInt();   // lecture de la valeur passée par le port série
  if (pot_val < 180 && pot_val > 0){
    Serial.print("Valeur lue : ");
    Serial.println(pot_val);
    digitalWrite(LED_BUILTIN, HIGH);
    val = pot_val; //map(pot_val, 0, 1023, 0 , 180);
  } else {
    digitalWrite(LED_BUILTIN, LOW);
  }
  esc.write(val);            //
  Serial.print("Valeur envoyée vers l'ESC : ");
  Serial.println(val);
  //delay(50);
//}
}