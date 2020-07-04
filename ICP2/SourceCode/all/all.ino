#include <SoftwareSerial.h>
#define DEBUG true
SoftwareSerial esp8266(9,10); 

#include <Adafruit_Sensor.h>

#define SSID "armour301"     // "SSID-WiFiname" 
#define PASS "snowman100"       // "password"
#define IP "184.106.153.149"// thingspeak.com ip
String msg = "GET /update?key=JXEV1FTF9RUQR2AU"; //change it with your api key like "GET /update?key=Your Api Key"

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
float altitude;
float concentration = 0;
int light;
float temp2;
float Vsig;

void setup() {
  Serial.begin(115200);

  pinMode(8,INPUT);
  starttime = millis(); 

  pinMode(A0,INPUT);

  dht.begin();

  esp8266.begin(115200);
  esp8266.println("AT");

  if(esp8266.find("OK")){
    connectWiFi();
    Serial.println("Connect to Internet");
  } else{
    Serial.println("Unable to connect to Internet");
  }
}

void loop() {
  run_light();
  run_temperature();
  run_uv();

  updatebeat();
  delay(5000);
}


void run_light() {
  Serial.println("Begin run_light...");
  light=analogRead(A0);
  Serial.println("Light = " + String(light));
  Serial.println("End run_light...");
  delay(2000);
}

void run_temperature() {
 
  Serial.println("Begin run_temperature...");
  //Read data and store it to variables hum and temp
  hum = dht.readHumidity();
  temp2= dht.readTemperature();
  //Print temp and humidity values to serial monitor
  Serial.print("Humidity: ");
  Serial.print(hum);
  Serial.print(" %, Temp: ");
  Serial.print(temp2);
  Serial.println(" Celsius");
  Serial.println("End run_temperature...");
  delay(2000);
}

void run_uv() {
    int sensorValue;
  long  sum=0;
  for(int i=0;i<1024;i++)
   {  
      sensorValue=analogRead(A1);
      sum=sensorValue+sum;
      delay(2);
   }   
 sum = sum >> 10;
 Vsig = sum*4980.0/1023.0; // Vsig is the value of voltage measured from the SIG pin of the Grove interface
 Serial.print("The voltage value: ");
 Serial.print(Vsig);
 Serial.print(" mV    --   ");
   delay(2000);
}

boolean connectWiFi(){
  Serial.println("AT+CWMODE=1");
  esp8266.println("AT+CWMODE=1");
  delay(2000);
  String cmd="AT+CWJAP=\"";
  cmd+=SSID;
  cmd+="\",\"";
  cmd+=PASS;
  cmd+="\"";
  Serial.println(cmd);
  esp8266.println(cmd);
  delay(5000);
  if(esp8266.find("OK")){
    Serial.println("OK");
    return true;    
  }else{
    return false;
  }
}

void updatebeat(){
  String cmd = "AT+CIPSTART=\"TCP\",\"";
  cmd += IP;
  cmd += "\",80";
  Serial.println(cmd);
  esp8266.println(cmd);
  delay(2000);
  if(esp8266.find("Error")){
    return;
  }
  cmd = msg ;
  cmd += "&field1=";   
  cmd += temp2;
  cmd += "&field2=";   
  cmd += hum;
  cmd += "&field3=";   
  cmd += Vsig;
  cmd += "&field4=";   
  cmd += light;
  cmd += "\r\n";
  Serial.print("AT+CIPSEND=");
  esp8266.print("AT+CIPSEND=");
  Serial.println(cmd.length());
  esp8266.println(cmd.length());
  Serial.print(cmd);
  esp8266.print(cmd);
}
