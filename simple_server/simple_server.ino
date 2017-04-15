#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <Wire.h>
#include "Adafruit_TCS34725.h"
#include "Adafruit_ADS1015.h"

// Initialize variables
Adafruit_TCS34725 tcs;
Adafruit_ADS1115 ads;  /* Use this for the 16-bit version */
uint16_t r, g, b, c, colorTemp, lux = 0;  // define values for color sensor readings, initialize to 0
int turbiditySensor = 12;  // pin 12 = Turbidity color sensor VDD
int throughputSensor = 14;  // pin 14 = Throughput color sensor VDD
int gpio0_pin = 0; // pin 0 = ESP8266 Red LED
int NUM_DATA = 50; // number of data points to collect
int pH_DATA = 300;
int currDataSet = 1;

String webPage = "";
String currDataPage = "";

MDNSResponder mdns;

// Replace with your network credentials
const char* ssid = "visitor";
const char* password = "";
//const char* ssid = "ECE449GROUP4";
//const char* password = "66808558";

ESP8266WebServer server(80);

void setup(void){
  webPage += "<h1>ECE 458 Group 4 Web Server</h1><p>LED Sanity Check <a href=\"LEDOn\"><button>LED ON</button></a>&nbsp;<a href=\"LEDOff\"><button>LED OFF</button></a></p>";
  webPage += "<p>Collect Data <a href=\"collect\"><button>Start Collection</button></a>&nbsp;<a href=\"retrieve\"><button>Retrieve Data</button></a>&nbsp;<a href=\"clear\"><button>Clear Data</button></a></p>";
  currDataPage = webPage;

  // preparing GPIOs
  pinMode(gpio0_pin, OUTPUT);
  pinMode(throughputSensor, OUTPUT);
  pinMode(turbiditySensor, OUTPUT);
  digitalWrite(gpio0_pin, HIGH);
  ads.begin();
  delay(1000);

  // Initiate serial and WIFI
  Serial.begin(115200);
  WiFi.setAutoConnect(true);
  WiFi.begin(ssid, password);
  Serial.print("Attemping to connect to: ");
  Serial.println(ssid);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  // Initiate MDNS responder
  if (mdns.begin("esp8266", WiFi.localIP())) {
    Serial.println("MDNS responder started");
  }

  // Home Page
  server.on("/", [](){
    server.send(200, "text/html", webPage);
  });

  // LED Off
  server.on("/LEDOff", [](){
    server.send(200, "text/html", webPage);
    digitalWrite(gpio0_pin, HIGH);
    delay(1000);
  });

  // LED On
  server.on("/LEDOn", [](){
    server.send(200, "text/html", webPage);
    digitalWrite(gpio0_pin, LOW);
    delay(1000);
  });

  // Collect Data
  server.on("/collect", [](){
    // Sends 200 OK immediately to avoid ERR_CONNECTION_TIMED_OUT
    String modified_page = webPage;
    modified_page += "<p>Waiting 30 seconds to collect data. Data will have finished collecting when red LED turns off.</p>";
    server.send(200, "text/html", modified_page);
    delay(30000);
    digitalWrite(gpio0_pin, LOW);

    currDataPage += "<p><b>Test " + String(currDataSet, DEC) + ":</b></p>";

    // THROUGHPUT READINGS
    tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_700MS, TCS34725_GAIN_1X);
    Serial.println("Initializing Throughput Readings...");
    currDataPage += "<p>Initializing Throughput Readings...</p>";
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
    currDataPage += "<p>Finished throughput readings. Initializing turbidity readings...</p>";
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
    currDataPage +=  "<p>Finished turbidity readings. Initializing pH readings...</p>";

    int16_t results;
    for(int k = 0; k< pH_DATA; k++){
      float multiplier = 0.1875F;
      results = ads.readADC_Differential_0_1();
      Serial.println(results * multiplier);
      currDataPage +=  "<p>"+String(results * multiplier, 2)+"</p>";
      delay(1000);
    }
    
    digitalWrite(turbiditySensor, LOW);
    delay(1000);
    
    currDataSet++;
    digitalWrite(gpio0_pin, HIGH);
    delay(1000);
  });

  // Retrieve data
  server.on("/retrieve", [](){
    server.send(200, "text/html", currDataPage);
    delay(1000);
  });

  // Clear data
  server.on("/clear", [](){
    currDataPage = webPage;
    currDataSet = 1;
    server.send(200, "text/html", webPage);
    delay(1000);
  });
  server.begin();
  Serial.println("HTTP server started");
}

void loop(void){
  server.handleClient();
}

// create function to call the sensor depending on the pin desired
void sensorON(int pin){  // "pin" is determined in the main loop, and is the pin for the sensor that is getting powered on

  // collect data from color sensor
  tcs.getRawData(&r, &g, &b, &c);
  colorTemp = tcs.calculateColorTemperature(r, g, b);
  lux = tcs.calculateLux(r, g, b);

  currDataPage += "<p>" + String(g, DEC) + "</p>";

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
