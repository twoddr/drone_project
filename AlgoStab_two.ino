#include <Servo.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

Servo esc[4];   // Tableau d'objets permettant le contrôle des ESC pour les quatre moteurs
int val[4];     // Tableau pour les valeurs de commande de chaque moteur
int pot_val;    // Valeur lue à partir du port série

const float amplitude = 79.5; // Amplitude de la sinusoïde (179 - 20) / 2
const float offset = 99.5;    // Offset de la sinusoïde (179 + 20) / 2
const float frequency = 0.05; // Fréquence de la sinusoïde

//value of rotor
int p1;
int p2;
int p3;
int p4;
//value of rotate
int ax;
int ay;
int az;
//value of speed
int gr;
int gt;
int gl;
//value of variation
int min = 0.2;
int max = 0.2;


Adafruit_MPU6050 mpu;
float corr_alx = -8.8;
float corr_aly = -1.7;
float corr_alz = 4.1;
void setup() {
  Serial.begin(9600);

  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }
  Serial.println("MPU6050 Found!");
  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
  //=================================Motor Setup=================================
  for (int i = 0; i < 4; i++) {
       esc[i].attach(9 + i, 1000, 2000); // Attache des ESC aux ports numériques 9, 10, 11 et 12 (port PWM obligatoire) avec des valeurs min et max pour les impulsions
       delay(15);
   }



   for (int i = 0; i < 4; i++) {
       // Initialisation des ESC
       esc[i].writeMicroseconds(1000); // Envoi d'un signal minimum à chaque ESC
       delay(1000);
       esc[i].writeMicroseconds(2000); // Envoi d'un signal maximum à chaque ESC
       delay(1000);
       esc[i].writeMicroseconds(1000); // Envoi d'un signal minimum à chaque ESC
       val[i] = 50;//27
   }

   pinMode(LED_BUILTIN, OUTPUT); // Configuration de la broche de la LED intégrée en tant que sortie
   // Informations pour l'utilisateur
   Serial.println("Enter a number between 0 and 179");
   Serial.println("(0 = stop - 179 = maximum speed)");
   Serial.println("Starting from 20)");
  delay(20);
}

void loop() {
  //===============================Capteur=================================
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  //tres foireux 
  //Serial.print(a.acceleration.x + corr_alx);
  //filtrage
  Serial.print(", Y:");
  int y = a.acceleration.y +corr_aly;
  if(y <0.20 && y > -0.2){
    y=0;
  }
  Serial.println(y);
  Serial.print(", X:");

  int x = a.acceleration.x+ corr_alx;
  if(x <0.20 && x > -0.2){
    x=0;
  }
  Serial.println(x);
  // z et y sont les deux pour stabilier le drone 
  //=================================Algo=================================
  //Si ça ne fonctionne pas c'est de la faute de trillet ;)
  ax = x;
  ay = y;
  az = a.acceleration.z + corr_alz;
  gr = g.gyro.x;
  gt = g.gyro.z;
  calc();
  
  //=================================Motor Control=================================
 
  // Envoi de la valeur de commande à chaque ESC
  for (int i = 0; i < 4; i++) {
      esc[i].write(val[i]);            
      Serial.print("Value sent to ESC ");
      Serial.print(i);
      Serial.print(": ");
      Serial.println(val[i]);
  }
  //===================================================================================
  delay(100);
}


void calc(){
  if(min<ax){
    p1--;
    p2--;
    p3++;
    p4++;
  }
  if (max>ax){
    p1++;
    p2++;
    p3--;
    p4--;
  }
  if(min<ay){
    p1++;
    p2--;
    p3++;
    p4--;
  }
  if (max>ay){
    p1--;
    p2++;
    p3--;
    p4++;
  }

  if(min<az){
    p1++;
    p2++;
    p3++;
    p4++;
  }
  if (max>az){
    p1--;
    p2--;
    p3--;
    p4--;
  }

  if (min<gr)
  {
    p1++;
    p2--;
    p3++;
    p4--;
  }
  if (max>gr)
  {
    p1--;
    p2++;
    p3--;
    p4++;
  }

  if (min<gt)
  {
    p1--;
    p2--;
    p3++;
    p4++;
  }
  if (max>gt)
  {
    p1++;
    p2++;
    p3--;
    p4--;
  }
  
  
  //don't forget to set [p1,p2,p3,p4] with rotation values
  val[0] = val[0] + p1;
  val[1] = val[1] + p2;
  val[2] = val[2] + p3;
  val[3] = val[3] + p4;

  //reset values
  p1 = 0;
  p2 = 0;
  p3 = 0;
  p4 = 0;
}
