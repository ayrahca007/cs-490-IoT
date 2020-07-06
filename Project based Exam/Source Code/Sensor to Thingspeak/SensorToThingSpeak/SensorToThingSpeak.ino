#include <SoftwareSerial.h>
#define DEBUG true
SoftwareSerial esp8266(9,10);

#include <Adafruit_Sensor.h>

#define SSID "ATThuNq222"     // "SSID-WiFiname" 
#define PASS "af?qsth%g46n"       // "password"
#define IP "184.106.153.149"// thingspeak.com ip 184.106.153.149
String msg = "GET /update?key=XQ4GYSZ09C3W2CYC"; //change it with your api key like "GET /update?key=Your Api Key"

int pin = 8;
unsigned long duration;
unsigned long starttime;
unsigned long sampletime_ms = 2000;
unsigned long lowpulseoccupancy = 0;
float ratio = 0;

//Libraries
#include <DHT.h>

//Constants
#define DHTPIN 7     // what pin we're connected to
#define DHTTYPE DHT22   // DHT 22  (AM2302)
DHT dht(DHTPIN, DHTTYPE); //// Initialize DHT sensor for normal 16mhz Arduino

//Variables
int chk;
float hum;  //Stores humidity value
float temp; //Stores temperature value
float pressure;
float concentration = 0;
int light;
int temp2;
//MQ-7 wiring
#define analogMQ7 A0      // Signal 
int MQ7sensorValue = 0;   // value read from the sensor

void setup() {
  Serial.begin(115200);

  pinMode(8, INPUT);
  starttime = millis();

  //pinMode(A0, INPUT);
  pinMode(analogMQ7, INPUT);

  dht.begin();

  esp8266.begin(115200);
  
  esp8266.println("AT");
  connectWiFi();     //moved to outside if statement below
  if (esp8266.find("OK")) {
    //connectWiFi();
    Serial.println("Connect to Internet");
  } else {
    Serial.println("Unable to connect to Internet");
  }
 analogWrite(analogMQ7, HIGH); // HIGH = 255
    // heat for 1 min
 delay(10000);
    // now reducing the heating power: turn the heater to approx 1,4V
 analogWrite(analogMQ7, 71.4); // 255x1400/5000
    // heat for 90 sec
 delay(3000);
}

void loop() {
  run_temperature();


  analogWrite(analogMQ7, HIGH); 
  delay(50); // Getting an analog read apparently takes 100uSec
  MQ7sensorValue = analogRead(analogMQ7); 
  Serial.print("CO Levels: ");
  Serial.print(MQ7sensorValue);
  Serial.print("\n");


  updatebeat();
  delay(5000);
}


void run_light() {
  Serial.println("Begin run_light...");
  light = analogRead(A0);
  Serial.println("Light = " + String(light));
  Serial.println("End run_light...");
  delay(2000);
}

void run_temperature() {

  Serial.println("Begin run_temperature...");
  //Read data and store it to variables hum and temp
  hum = dht.readHumidity();
  temp2 = dht.readTemperature();
  //Print temp and humidity values to serial monitor
  Serial.print("Humidity: ");
  Serial.print(hum);
  Serial.print(" %, Temp: ");
  Serial.print(temp2);
  Serial.println(" Celsius");
  Serial.println("End run_temperature...");
  delay(2000);
}

boolean connectWiFi() {
  Serial.println("AT+CWMODE=1");
  esp8266.println("AT+CWMODE=1");
  delay(2000);
  String cmd = "AT+CWJAP=\"";
  cmd += SSID;
  cmd += "\",\"";
  cmd += PASS;
  cmd += "\"";
  Serial.println(cmd);
  esp8266.println(cmd);
  delay(5000);
  if (esp8266.find("OK")) {
    Serial.println("OK");
    return true;
  } else {
    return false;
  }
}

void updatebeat() {
  String cmd = "AT+CIPSTART=\"TCP\",\"";
  cmd += IP;
  cmd += "\",80";
  Serial.println(cmd);
  esp8266.println(cmd);
  delay(2000);
  if (esp8266.find("Error")) {
    return;
  }
  cmd = msg ;
  cmd += "&field1=";
  cmd += temp2;
  cmd += "&field2=";
  cmd += hum;
  cmd += "&field3=";
  //cmd += Vsig;
  //cmd += "&field4=";
  cmd += MQ7sensorValue;
  cmd += "\r\n";
  Serial.print("AT+CIPSEND=");
  esp8266.print("AT+CIPSEND=");
  Serial.println(cmd.length());
  esp8266.println(cmd.length());
  Serial.print(cmd);
  esp8266.print(cmd);
}
