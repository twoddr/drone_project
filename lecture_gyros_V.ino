// Include the necessary libraries
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

// Create an object for the MPU6050 sensor
Adafruit_MPU6050 mpu;
float corr_alx = -7.8;
float corr_aly = -1.7;
float corr_alz = 4.1;
float corr_aaz = 0;
float corr_aay = 0;
float corr_aax = 0;

unsigned long ti = 0; // initial time
float aX = 0; // actual acc
float aY = 0; // actual acc
float aZ = 0; // actual acc
float vX = 0; // actual speed
float vY = 0; // actual speed
float vZ = 0; // actual speed

// délai entre les mesures
int DELAI=50;


void setup() {
  // Initialize the serial communication
  Serial.begin(9600);

  // Check if the MPU6050 sensor is detected
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }
  Serial.println("MPU6050 Found!");

  // set accelerometer range to +-8G
  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);

  // set gyro range to +- 500 deg/s
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);

  // set filter bandwidth to 21 Hz
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);

  // Add a delay for stability
  delay(100);
}

void loop() {
  // Get new sensor events with the readings
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  // accélération actuelle
  float aXf = a.acceleration.x; // + corr_alx;
  float aYf = a.acceleration.y; // + corr_aly;
  float aZf = a.acceleration.z; // + corr_alz;

  // instant actuel
  unsigned long tf = millis();
  float dt;
  if (ti == 0){
    aX = aXf;
    aY = aYf;
    aZ = aZf;
    vX = 0;
    vY = 0;
    vZ = 0;// uniquement pour le démarrage des mesures
  } else {
    dt = tf-ti;
    // différence d'accélération
    float daX = aXf - aX;
    float daY = aYf - aY;
    float daZ = aZf - aZ;
    // sauver l'accélération actuelle
    aX = aXf;
    aY = aYf;
    aZ = aZf;

    vX += getIncrement(daX, dt);
    vY += getIncrement(daY, dt);
    vZ += getIncrement(daZ, dt);
  }
  ti = tf;



  // Print out the acceleration readings in m/s^2
  Serial.print("Vitesse :   X:");
  Serial.print(vX);
  Serial.print(", Y:");
  Serial.print(vY);
  Serial.print(", Z:");
  Serial.print(vZ);
  Serial.println(" (m/s)");

  // Print out the rotation readings in rad/s
  Serial.print("Rotation:       RX:");
  Serial.print(g.gyro.x + corr_aax);
  Serial.print(", RY:");
  Serial.print(g.gyro.y + corr_aay);
  Serial.print(", RZ:");
  Serial.print(g.gyro.z + corr_aaz);
  Serial.println(" (rad/s)");

  // Print out the temperature reading in degrees Celsius
  Serial.print("Temperature:    ");
  Serial.print(temp.temperature);
  Serial.println(" (degC)");

  // Add a blank line for readability
  Serial.println("");

  // Add a delay to avoid flooding the serial monitor
  delay(DELAI);
}

// définition de getSpeed
float getIncrement(float a, unsigned long deltaT){
  return a * deltaT;
}
