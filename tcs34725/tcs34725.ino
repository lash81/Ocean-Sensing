#include <Wire.h>
#include "Adafruit_TCS34725.h"

/* Example code for the Adafruit TCS34725 breakout library */

/* Connect SCL    to analog 5
   Connect SDA    to analog 4
   Connect VDD    to 3.3V DC
   Connect GROUND to common ground */
   
/* Initialise with default values (int time = 2.4ms, gain = 1x) */
// Adafruit_TCS34725 tcs = Adafruit_TCS34725();

/* Initialise with specific int time and gain values */
//Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_700MS, TCS34725_GAIN_1X);

Adafruit_TCS34725 tcs;

uint16_t r, g, b, c, colorTemp, lux = 0;  // define values for color sensor readings, initialize to 0

int sensorA = 12;  // digital pin 12 = turbidity
int sensorB = 14;  // digital pin 14 = throughput

void setup(void) {
  Serial.begin(115200);
  pinMode(sensorA, OUTPUT);  // set pin to power sensorA
  pinMode(sensorB, OUTPUT);  // set pin to power sensorB
}

void loop(void) {
  
  // Using two FOR loops to collect samples from independent color sensors.  Could probably condense into further functions, maybe in next revision.  

  // loop to sample sensorB 5 times. NOTE: the first 2 samples (i = 0, 1) will be incorrect; samples i = 2,3,4 give proper readings.
  tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_700MS, TCS34725_GAIN_1X);  // initialize sensor values each time the function gets called

  Serial.println("Initializing Turbidity Readings...");
  digitalWrite(sensorB, HIGH);  // set "pin" to high, powering the sensor
  delay(2000);
  
   if (tcs.begin()) {
    Serial.println("Found turbidity color sensor!");
  } else {
    Serial.println("Turbidity sensor could not be found ... check your connections");
    while (1);
  }
  
  for(int i = 0; i < 10; i++){
    sensorON(sensorB);
    delay(1000);
  }
  
  digitalWrite(sensorB, LOW);
  Serial.println("Finished turbidity readings. Initializing Throughput readings...");
  
  delay(2000);
  tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_700MS, TCS34725_GAIN_1X);  // initialize sensor values each time the function gets called

  digitalWrite(sensorA, HIGH);  // set "pin" to high, powering the sensor

  delay(2000);
  
   if (tcs.begin()) {
    Serial.println("Found throughput color sensor!");
  } else {
    Serial.println("Throughput sensor could not be found ... check your connections");
    while (1);
  }
  
  for(int j = 0; j < 10; j++){
    sensorON(sensorA);
    delay(1000);
  }
  
  digitalWrite(sensorA, LOW);
  
  delay(2000);
  
}

// create function to call the sensor depending on the pin desired
void sensorON(int pin){  // "pin" is determined in the main loop, and is the pin for the sensor that is getting powered on
    
  // collect data from color sensor
  tcs.getRawData(&r, &g, &b, &c);
  colorTemp = tcs.calculateColorTemperature(r, g, b);
  lux = tcs.calculateLux(r, g, b);
  
  // display data
  Serial.print("Pin: "); Serial.print(pin, DEC);
  Serial.print(" Color Temp: "); Serial.print(colorTemp, DEC); Serial.print(" K - ");
  Serial.print("Lux: "); Serial.print(lux, DEC); Serial.print(" - ");
  Serial.print("R: "); Serial.print(r, DEC); Serial.print(" ");
  Serial.print("G: "); Serial.print(g, DEC); Serial.print(" ");
  Serial.print("B: "); Serial.print(b, DEC); Serial.print(" ");
  Serial.print("C: "); Serial.print(c, DEC); Serial.print(" ");
  Serial.println(" ");
}
