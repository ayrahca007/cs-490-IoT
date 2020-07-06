#include <DHT.h>
#define DHTPIN 13     // what pin we're connected to
#define DHTTYPE DHT11   // DHT 22  (AM2302)
DHT dht(DHTPIN, DHTTYPE); //// Initialize DHT sensor for normal 16mhz Arduino
#include <Wire.h>
#include <SPI.h>
#include "Adafruit_VEML6070.h"
Adafruit_VEML6070 uv = Adafruit_VEML6070();
#define VEML6070_ADDR_L     (0x38) ///< Low address



int Rpin3 = 10; //MQ
int Gpin3 = 9; //MQ

int Rpin = 8; //Temp
int Gpin = 7; //Temp


int Bpin = 6; //Humidity
int Gpin2 = 5; //Humidity
int Rpin2 = 4; //Humidity

int pin;
float hum; //Humidity
float temp; //Temperature 

int smokeA0 = A0;
// Your threshold value
int sensorThres = 400;



void setup() {
  //Serial Port begin
  Serial.begin (9600);
  dht.begin();

  //Define inputs and outputs
    pinMode(smokeA0, INPUT);

  
}
 
void loop() {

digitalWrite(Bpin, LOW);
digitalWrite(Gpin2, LOW);
digitalWrite(Rpin2, LOW);
digitalWrite(Rpin, LOW);
digitalWrite(Gpin, LOW);
digitalWrite(Gpin3, LOW);
digitalWrite(Rpin3, LOW);


//Serial.print("\n");  
//Serial.print("\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\");  
//Serial.print("\n");  
//Serial.print("\\\\\\\\\\WEATHER STATION\\\\\\\\");  
//Serial.print("\n");  
//Serial.print("\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\");  
//Serial.print("\n"); 

 

//////////////////////////////////////////////////
///////////////////DHT11//////////////////////////
//////////////////////////////////////////////////

    delay(2000);
    //Read data and store it to variables hum and temp
    hum = dht.readHumidity();
    temp= dht.readTemperature();

// put your main code here, to run repeatedly:
Serial.print("Humidity: ");

Serial.print("\n");
Serial.println(hum);//data that is being Sent

  if (hum >= 60)
{
 Serial.print("Wet - Take Precaution");
  digitalWrite(Bpin, HIGH);
  delay(500);
  digitalWrite(Bpin, LOW);
  delay(500);
  digitalWrite(Bpin, HIGH);
  delay(500);
  digitalWrite(Bpin, LOW);
  delay(500);
  digitalWrite(Gpin2, HIGH);
  digitalWrite(Gpin2, LOW);
  digitalWrite(Rpin2, LOW);
}
  else if (hum >= 20)
{
 Serial.print("Comfortable - Enjoy the Day!");
  digitalWrite(Bpin, LOW);
  digitalWrite(Gpin2, HIGH);
  delay(500);
  digitalWrite(Gpin2, LOW);
  delay(500);
  digitalWrite(Gpin2, HIGH);
  delay(500);
  digitalWrite(Gpin2, LOW);
  delay(500);
  digitalWrite(Gpin2, HIGH);
  delay(500);
  digitalWrite(Rpin2, LOW);
}
  else
{
  Serial.print("Dry - Buy a humidifier!");
  digitalWrite(Bpin, LOW);
  digitalWrite(Gpin2, LOW);
  digitalWrite(Rpin2, HIGH);
  delay(500);
  digitalWrite(Rpin2, LOW);
  delay(500);
  digitalWrite(Rpin2, HIGH);
  delay(500);
  digitalWrite(Rpin2, LOW);
  delay(500);
  digitalWrite(Rpin2, HIGH);
}
delay(5000);
Serial.print("\n");

Serial.print("Temperature: ");
Serial.print("\n");
Serial.println(temp);
delay(200);

  if (temp >= 32)
{
 Serial.print("Hot - Stay Hydrated! ");
  digitalWrite(Rpin, HIGH);
  delay(500);
  digitalWrite(Rpin, LOW);
  delay(500);
  digitalWrite(Rpin, HIGH);
  delay(500);
  digitalWrite(Rpin, LOW);
  delay(500);
  digitalWrite(Rpin, HIGH);
  delay(500);
  digitalWrite(Gpin, LOW);
}
  else if (temp >=22)
{
 Serial.print("Comfortable - Wear Sunscreen!");
  digitalWrite(Rpin, LOW);
  digitalWrite(Gpin, HIGH);
  digitalWrite(Gpin, LOW);
  delay(500);
  digitalWrite(Gpin, HIGH);
  delay(500);
  digitalWrite(Gpin, LOW);
  delay(500);
  digitalWrite(Gpin, HIGH);
  delay(500);
}
  else 
{
 Serial.print("Cool");
  digitalWrite(Rpin, LOW);
  digitalWrite(Gpin, HIGH);
  digitalWrite(Gpin, LOW);
  delay(500);
  digitalWrite(Gpin, HIGH);
  delay(500);
  digitalWrite(Gpin, LOW);
  delay(500);
  digitalWrite(Gpin, HIGH);
  delay(500);
}
 Serial.print("\n");

//////////////////////////////////////////////////
///////////////////MQ SENSOR//////////////////////
//////////////////////////////////////////////////
delay(5000);
 int analogSensor = analogRead(smokeA0);
{
  Serial.print("Air Test Value: ");
  Serial.print("\n");
  Serial.print(analogSensor);
  // Checks if it has reached the threshold value
  Serial.print("\n");
  
  if (analogSensor > sensorThres)
  {
    Serial.print("DANGER");
    digitalWrite(Rpin3, HIGH);
    delay(500);
    digitalWrite(Rpin3, LOW);
    delay(500);
    digitalWrite(Rpin3, HIGH);
    delay(500);
    digitalWrite(Rpin3, LOW);
    delay(500);
    digitalWrite(Rpin3, HIGH);
    delay(500);
    digitalWrite(Gpin3, LOW);
 }
  
  else
  {
    Serial.print("SAFE");
    digitalWrite(Rpin3, LOW);
    digitalWrite(Gpin3, HIGH);
    digitalWrite(Gpin3, LOW);
    delay(500);
    digitalWrite(Gpin3, HIGH);
    delay(500);
    digitalWrite(Gpin3, LOW);
    delay(500);
    digitalWrite(Gpin3, HIGH);
    delay(500);
  }
  delay(100);
}




Serial.print("\n");  
//Serial.print("\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\");  
//Serial.print("\n");  
//Serial.print("\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\");  
//Serial.print("\n");  
//Serial.print("\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\");  
//Serial.print("\n"); 
delay(5000);




  
}
