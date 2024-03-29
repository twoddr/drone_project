#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

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
  delay(100);
}

void loop() {
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
 

  // fait varier le temps de mesure 
  delay(100);
}