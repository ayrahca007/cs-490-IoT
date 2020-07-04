// Run this Program using a 3-pin light sensor; Connect the info pin to Analog Pin A0 on the Arduino
int light;

void setup(){
  Serial.begin(9600);
  pinMode(A0,INPUT);
}

void loop(){
  light = analogRead(A0);
  Serial.println("Light Sensor Reading = " + String(light));
  delay(5000);
}
