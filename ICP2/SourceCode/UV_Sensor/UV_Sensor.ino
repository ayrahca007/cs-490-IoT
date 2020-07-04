#include <SoftwareSerial.h>
#define RX 2
#define TX 3
String AP = "armour301";       // AP NAME
String PASS = "snowman100"; // AP PASSWORD
String API = "JXEV1FTF9RUQR2AU";   // Write API KEY
String HOST = "api.thingspeak.com";
String PORT = "80";
int countTrueCommand;
int countTimeCommand;
boolean found = false;
int valSensor = 1;

SoftwareSerial esp8266(RX, TX);

float Vsig;

void setup() {
  Serial.begin(9600);

  esp8266.begin(115200);
  sendCommand("AT", 5, "OK");
  sendCommand("AT+CWMODE=1", 5, "OK");
  sendCommand("AT+CWJAP=\"" + AP + "\",\"" + PASS + "\"", 20, "OK");
}

void loop()
{
  int sensorValue;
  long  sum = 0;
  for (int i = 0; i < 1024; i++)
  {
    sensorValue = analogRead(A0);
    sum = sensorValue + sum;
    delay(2);
  }
  sum = sum >> 10;
  Vsig = sum * 4980.0 / 1023.0; // Vsig is the value of voltage measured from the SIG pin of the Grove interface
  Serial.print("The voltage value: ");
  Serial.print(Vsig);
  Serial.print(" mV    --   ");

  if (Vsig < 50) {
    Serial.print("UV Index: 0 "); Serial.println("   Exposure level - NONE (You're probably at home!) ");
  }
  if (Vsig > 50 && Vsig < 227) {
    Serial.print("UV Index: 1 "); Serial.println("   Exposure level - LOW (You're probably at home!) ");
  }
  if (Vsig > 227 && Vsig < 318) {
    Serial.print("UV Index: 2 "); Serial.println("   Exposure level - LOW (You can go outside and have fun!) ");
  }
  if (Vsig > 318 && Vsig < 408) {
    Serial.print("UV Index: 3 "); Serial.println("   Exposure level - MODERATE (Sun starts to annoy you) ");
  }
  if (Vsig > 408 && Vsig < 503) {
    Serial.print("UV Index: 4 "); Serial.println("   Exposure level - MODERATE (Sun starts to annoy you) ");
  }
  if (Vsig > 503 && Vsig < 606) {
    Serial.print("UV Index: 5 "); Serial.println("   Exposure level - MODERATE (Sun starts to annoy you) ");
  }
  if (Vsig > 606 && Vsig < 696) {
    Serial.print("UV Index: 6 "); Serial.println("   Exposure level - HIGH (Get out from the sunlight! get out now!) ");
  }
  if (Vsig > 696 && Vsig < 795) {
    Serial.print("UV Index: 7 "); Serial.println("   Exposure level - HIGH (Get out from the sunlight! get out now!) ");
  }
  if (Vsig > 795 && Vsig < 881) {
    Serial.print("UV Index: 8 "); Serial.println("   Exposure level - VERY HIGH (Get out from the sunlight! get out now!) ");
  }
  if (Vsig > 881 && Vsig < 976) {
    Serial.print("UV Index: 9 "); Serial.println("   Exposure level - VERY HIGH (If you value your health, don't go outside, just stay at home!) ");
  }
  if (Vsig > 976 && Vsig < 1079) {
    Serial.print("UV Index: 10 "); Serial.println("   Exposure level - VERY HIGH (If you value your health, don't go outside, just stay at home!) ");
  }
  if (Vsig > 1079 && Vsig < 1170) {
    Serial.print("UV Index: 11 "); Serial.println("   Exposure level - EXTREME (If you value your health, don't go outside, just stay at home!) ");
  }
  if (Vsig > 1170) {
    Serial.print("UV Index: 11+ "); Serial.println("   Exposure level - EXTREME (You will probably die in 3, 2, 1... Just JOKING, don't be scared...) - intensity of sunlight is really at maximum ");
  }

  String getData = "GET /update?api_key=" + API + "&field3=" + sensorValue;
  sendCommand("AT+CIPMUX=1", 5, "OK");
  sendCommand("AT+CIPSTART=0,\"TCP\",\"" + HOST + "\"," + PORT, 15, "OK");
  sendCommand("AT+CIPSEND=0," + String(getData.length() + 4), 4, ">");
  esp8266.println(getData); delay(1500); countTrueCommand++;
  sendCommand("AT+CIPCLOSE=0", 5, "OK");
}

void sendCommand(String command, int maxTime, char readReplay[]) {
  Serial.print(countTrueCommand);
  Serial.print(". at command => ");
  Serial.print(command);
  Serial.print(" ");
  while (countTimeCommand < (maxTime * 1))
  {
    esp8266.println(command);//at+cipsend
    if (esp8266.find(readReplay)) //ok
    {
      found = true;
      break;
    }

    countTimeCommand++;
  }

  if (found == true)
  {
    Serial.println("OYI");
    countTrueCommand++;
    countTimeCommand = 0;
  }

  if (found == false)
  {
    Serial.println("Fail");
    countTrueCommand = 0;
    countTimeCommand = 0;
  }

  found = false;
}
