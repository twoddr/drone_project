#include <Servo.h>

Servo esc;   // Création de l'objet permettant le contrôle de l'ESC

int val = 0; //
int pot_val;

void setup() {
   esc.attach(9); // On attache l'ESC au port numérique 9 (port PWM obligatoire)
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


   // Quelques informations pour l'utilisateur
   /*Serial.println("Saisir un nombre entre 0 et 179");
   Serial.println("(0 = arret - 179 = vitesse maxi");
   Serial.println(" demarrage a partir de 20)");*/
   }

void loop() {
    pot_val=analogRead(A0);
   if (pot_val > 0) {
      //val = Serial.parseInt();   // lecture de la valeur passée par le port série
        if (val != pot_val){
          Serial.println("Valeur analogique : "+val);
        }
        val = pot_val;
        esc.write(val);            //
        delay(15);
      }
   }
