#include <DHT.h>

//Constants
#define DHTPIN 2     // what pin we're connected to
#define DHTTYPE DHT11   // DHT 11  (AM2302)
#include <Wire.h>
#include <SPI.h>
#include "Adafruit_VEML6070.h"
Adafruit_VEML6070 uv = Adafruit_VEML6070();
#define VEML6070_ADDR_L     (0x38) ///< Low address

// Initialize DHT sensor for normal 16mhz Arduino
DHT dht(DHTPIN, DHTTYPE);

//Variables
int chk;
float hum;  //Stores humidity value
float temp; //Stores temperature value

int smokeA0 = A0;
// Your threshold value
int sensorThres = 400;

void setup()
{
    Serial.begin(9600);
  dht.begin();
    
}

String hum1;
String temp1;

void loop()
{
    //Read data and store it to variables hum and temp
    hum = dht.readHumidity();
    temp= dht.readTemperature();
    //hum1 = String(hum);
    temp1 = String(temp);
    //Print temp and humidity values to serial monitor
   // Serial.print("Humidity: ");
    Serial.print(temp);
    Serial.print(",");
    Serial.print(hum);
   // Serial.println(" Celsius");




   //////////////////////////////////////////////////
///////////////////MQ SENSOR//////////////////////
//////////////////////////////////////////////////
 int analogSensor = analogRead(smokeA0);
{
  //Serial.print("Air Test Value: ");
  //Serial.print("\n");
  Serial.print(",");
  Serial.print(analogSensor);
  Serial.print(",");
    // Checks if it has reached the threshold value
}  
  

    Serial. print('\n');  
    delay(2000); //Delay 2 sec.





    
}
