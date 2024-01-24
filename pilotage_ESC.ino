#include <Servo.h>

Servo esc;  // Create an object to control the ESC
int val = 0;  // Variable to store the potentiometer value
int pot_val;  // Variable to store the previous potentiometer value

void setup() {
  esc.attach(9);  // Attach the ESC to digital pin 9 (PWM pin required)
  delay(15);
  Serial.begin(9600);

  // Initialize the ESC
  // Some ESCs need an "initialization procedure" to become operational - see manual
  esc.write(0);
  delay(1000);
  esc.write(1023);
  delay(1000);
  esc.write(0);
  /*Serial.println("Saisir un nombre entre 0 et 179");
   Serial.println("(0 = arret - 179 = vitesse maxi");
   Serial.println(" demarrage a partir de 20)");*/
}

void loop() {
  pot_val = analogRead(A0);  // Read the value from the potentiometer
  if (pot_val > 0) {
    if (val != pot_val) {
        Serial.println("Analog value: " + String(val));  // Print the value if it has changed
    }
    val = pot_val;  // Update the stored value
    esc.write(val);  // Send the value to the ESC
    delay(15);
  }
}