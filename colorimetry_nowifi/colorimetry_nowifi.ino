#include <Wire.h>
#include "Adafruit_TCS34725.h"

// Initialize variables
Adafruit_TCS34725 tcs;
uint16_t r, g, b, c, colorTemp, lux = 0;  // define values for color sensor readings, initialize to 0
int turbiditySensor = 12;  // pin 12 = Turbidity color sensor VDD
int throughputSensor = 14;  // pin 14 = Throughput color sensor VDD
int NUM_DATA = 20; // number of data points to collect

void setup(void) {
  Serial.begin(115200);
  pinMode(throughputSensor, OUTPUT);
  pinMode(turbiditySensor, OUTPUT);
}

void loop(void) {
    // THROUGHPUT READINGS
    tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_700MS, TCS34725_GAIN_1X);
    Serial.println("Initializing Throughput Readings...");
    digitalWrite(throughputSensor, HIGH);
    delay(2000); // Need to delay to wait for sensor to initialize itself

    if (tcs.begin()) {
      Serial.println("Found throughput color sensor!");
    } else {
      Serial.println("Throughput sensor could not be found ... check your connections");
      while (1);
    }

    for(int i = 0; i < NUM_DATA; i++){
      sensorON(throughputSensor);
      delay(750);
    }

    digitalWrite(throughputSensor, LOW);
    Serial.println("Finished throughput readings. Initializing Turbidity readings...");
    delay(2000);  // Extra delay just in case

    // TURBIDITY READINGS
    tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_700MS, TCS34725_GAIN_1X);  // initialize sensor values each time the function gets called
    digitalWrite(turbiditySensor, HIGH);
    delay(2000); // Need to wair for sensor to initialize itself

    if (tcs.begin()) {
      Serial.println("Found turbidity color sensor!");
    } else {
      Serial.println("Turbidity sensor could not be found ... check your connections");
      while (1);
    }

    for(int j = 0; j < NUM_DATA; j++){
      sensorON(turbiditySensor);
      delay(750);
    }

    digitalWrite(turbiditySensor, LOW);
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
