#include <DHT.h>

//Constants
#define DHTPIN 7     // what pin we're connected to
#define DHTTYPE DHT22   // DHT 22  (AM2302)
DHT dht(DHTPIN, DHTTYPE); //// Initialize DHT sensor for normal 16mhz Arduino

int sensorValue;
char caracter;
float temp = 0;
float hum = 0;
int ledpin1 = 12;
int ledpin2 = 13;

void setup() {
  Serial.begin(9600);
  dht.begin();
  pinMode(ledpin1, OUTPUT);
  pinMode(ledpin2, OUTPUT);
}

void loop() {
  if (Serial.available()) {
    caracter = Serial.read();

    if (caracter == 'T') {
      run_temperature_temp();
    }
    if (caracter == 'H') {
      run_temperature_humidity();
    }
    if (caracter == 'A') {
      run_gas_sensor();
    }
  }
}
void run_temperature_temp() {
  temp = dht.readTemperature();
  Serial.println(temp);
  if (temp > 690) {
    Serial.println("Temperature higher than normal, cool off!!");
    digitalWrite(ledpin1, HIGH);
  }
  else {
    digitalWrite(ledpin1, LOW);
  }
}
void run_temperature_humidity() {
  hum = dht.readHumidity();
  Serial.println(hum);
  if (hum > 1100) {
    Serial.println("Humidity higher than 70%");
    digitalWrite(ledpin1, HIGH);
  }
  else {
    digitalWrite(ledpin1, LOW);
  }
}

void run_gas_sensor() {
  sensorValue = analogRead(A0);
  Serial.println(sensorValue);
  if (sensorValue > 110) {
    Serial.println("Poor Air Quality");
    digitalWrite(ledpin2, HIGH);
  }
  else {
    digitalWrite(ledpin2, LOW);
  }
}
