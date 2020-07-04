#include <SoftwareSerial.h>
#define DEBUG true
SoftwareSerial esp8266(9,10); 

#define SSID "armour301"     // "SSID-WiFiname" 
#define PASS "snowman100"       // "password"
#define IP "184.106.153.149"// thingspeak.com ip
String msg = "GET /update?key=J4ZAMTJZ7K0I746R";

//Libraries
#include <DHT.h>

//Constants
#define DHTPIN 7     // what pin we're connected to
#define DHTTYPE DHT22   // DHT 22  (AM2302)
DHT dht(DHTPIN, DHTTYPE); //// Initialize DHT sensor for normal 16mhz Arduino

//Variables
int chk;
float hum;  //Stores humidity value
float temp2; //Stores temperature value
float gcd;
float gcm;
float gal;
float gam;
float gt;
float ga;


/************************MQ135sensor************************************/
/************************Hardware Related Macros************************************/
#define         MQ135PIN                       (5)     //define which analog input channel you are going to use
#define         RL_VALUE_MQ135                 (1)     //define the load resistance on the board, in kilo ohms
#define         RO_CLEAN_AIR_FACTOR_MQ135      (3.59)  //RO_CLEAR_AIR_FACTOR=(Sensor resistance in clean air)/RO,
                                                       //which is derived from the chart in datasheet

/***********************Software Related Macros************************************/
#define         CALIBARAION_SAMPLE_TIMES     (50)    //define how many samples you are going to take in the calibration phase
#define         CALIBRATION_SAMPLE_INTERVAL  (500)   //define the time interal(in milisecond) between each samples in the
                                                     //cablibration phase
#define         READ_SAMPLE_INTERVAL         (50)    //define how many samples you are going to take in normal operation
#define         READ_SAMPLE_TIMES            (5)     //define the time interal(in milisecond) between each samples in 
                                                     //normal operation

/**********************Application Related Macros**********************************/
#define         GAS_CARBON_DIOXIDE           (9)
#define         GAS_CARBON_MONOXIDE          (3)
#define         GAS_ALCOHOL                  (4)
#define         GAS_AMMONIUM                 (10)
#define         GAS_TOLUENE                  (11)
#define         GAS_ACETONE                  (12)
//#define         accuracy                     (0)   //for linearcurves
#define         accuracy                   (1)   //for nonlinearcurves, un comment this line and comment the above line if calculations 
                                                   //are to be done using non linear curve equations
/*****************************Globals***********************************************/
float           Ro = 10;                            //Ro is initialized to 10 kilo ohms

void setup() {
  Serial.begin(115200);                               //UART setup, baudrate = 9600bps
  dht.begin();

  esp8266.begin(115200);
  esp8266.println("AT");

  if(esp8266.find("OK")){
    connectWiFi();
    Serial.println("Connect to Internet");
  } else{
    Serial.println("Unable to connect to Internet");
  }
  
  Serial.print("Calibrating...\n");                
  Ro = MQCalibration(MQ135PIN);                     //Calibrating the sensor. Please make sure the sensor is in clean air 
                                                    //when you perform the calibration                    
  Serial.print("Calibration is done...\n"); 
  Serial.print("Ro=");
  Serial.print(Ro);
  Serial.print("kohm");
  Serial.print("\n");
}

void loop() {

   Serial.print("CARBON_DIOXIDE:"); 
   Serial.print(MQGetGasPercentage(MQRead(MQ135PIN)/Ro,GAS_CARBON_DIOXIDE) );
   Serial.print( "ppm" );
   Serial.print("    ");   
   Serial.print("CARBON_MONOXIDE:"); 
   Serial.print(MQGetGasPercentage(MQRead(MQ135PIN)/Ro,GAS_CARBON_MONOXIDE) );
   Serial.print( "ppm" );
   Serial.print("    ");   
   Serial.print("ALCOHOL:"); 
   Serial.print(MQGetGasPercentage(MQRead(MQ135PIN)/Ro,GAS_ALCOHOL) );
   Serial.print( "ppm" );
   Serial.print("    ");   
   Serial.print("AMMONIUM:"); 
   Serial.print(MQGetGasPercentage(MQRead(MQ135PIN)/Ro,GAS_AMMONIUM) );
   Serial.print( "ppm" );
   Serial.print("    ");   
   Serial.print("TOLUENE:"); 
   Serial.print(MQGetGasPercentage(MQRead(MQ135PIN)/Ro,GAS_TOLUENE) );
   Serial.print( "ppm" );
   Serial.print("    ");   
   Serial.print("ACETONE:"); 
   Serial.print(MQGetGasPercentage(MQRead(MQ135PIN)/Ro,GAS_ACETONE) );
   Serial.print( "ppm" );
   Serial.print("\n");
   delay(200);
   run_temperature();
   updatebeat();
  delay(5000);
}

/****************** MQResistanceCalculation ****************************************
Input:   raw_adc - raw value read from adc, which represents the voltage
Output:  the calculated sensor resistance
Remarks: The sensor and the load resistor forms a voltage divider. Given the voltage
         across the load resistor and its resistance, the resistance of the sensor
         could be derived.
************************************************************************************/ 
float MQResistanceCalculation(int raw_adc)
{
  return ( ((float)RL_VALUE_MQ135*(1023-raw_adc)/raw_adc));
}

/***************************** MQCalibration ****************************************
Input:   mq_pin - analog channel
Output:  Ro of the sensor
Remarks: This function assumes that the sensor is in clean air. It use  
         MQResistanceCalculation to calculates the sensor resistance in clean air 
         and then divides it with RO_CLEAN_AIR_FACTOR. RO_CLEAN_AIR_FACTOR is about 
         10, which differs slightly between different sensors.
************************************************************************************/ 
float MQCalibration(int mq_pin)
{
  int i;
  float RS_AIR_val=0,r0;

  for (i=0;i<CALIBARAION_SAMPLE_TIMES;i++) {                     //take multiple samples
    RS_AIR_val += MQResistanceCalculation(analogRead(mq_pin));
    delay(CALIBRATION_SAMPLE_INTERVAL);
  }
  RS_AIR_val = RS_AIR_val/CALIBARAION_SAMPLE_TIMES;              //calculate the average value

  r0 = RS_AIR_val/RO_CLEAN_AIR_FACTOR_MQ135;                      //RS_AIR_val divided by RO_CLEAN_AIR_FACTOR yields the Ro 
                                                                 //according to the chart in the datasheet 

  return r0; 
}

/*****************************  MQRead *********************************************
Input:   mq_pin - analog channel
Output:  Rs of the sensor
Remarks: This function use MQResistanceCalculation to caculate the sensor resistenc (Rs).
         The Rs changes as the sensor is in the different consentration of the target
         gas. The sample times and the time interval between samples could be configured
         by changing the definition of the macros.
************************************************************************************/ 
float MQRead(int mq_pin)
{
  int i;
  float rs=0;

  for (i=0;i<READ_SAMPLE_TIMES;i++) {
    rs += MQResistanceCalculation(analogRead(mq_pin));
    delay(READ_SAMPLE_INTERVAL);
  }

  rs = rs/READ_SAMPLE_TIMES;

  return rs;  
}

/*****************************  MQGetGasPercentage **********************************
Input:   rs_ro_ratio - Rs divided by Ro
         gas_id      - target gas type
Output:  ppm of the target gas
Remarks: This function uses different equations representing curves of each gas to 
         calculate the ppm (parts per million) of the target gas.
************************************************************************************/ 
int MQGetGasPercentage(float rs_ro_ratio, int gas_id)
{ 
  if ( accuracy == 0 ) {
  if ( gas_id == GAS_CARBON_DIOXIDE ) {
    gcd = pow(10,((-2.890*(log10(rs_ro_ratio))) + 2.055));
    return (gcd);
  } else if ( gas_id == GAS_CARBON_MONOXIDE ) {
    gcm = pow(10,((-3.891*(log10(rs_ro_ratio))) + 2.750));
    return (gcm);
  } else if ( gas_id == GAS_ALCOHOL ) {
    gal = pow(10,((-3.181*(log10(rs_ro_ratio))) + 1.895));
    return (gal);
  } else if ( gas_id == GAS_AMMONIUM ) {
    gam = pow(10,((-2.469*(log10(rs_ro_ratio))) + 2.005));
    return (gam);
  } else if ( gas_id == GAS_TOLUENE ) {
    gt = pow(10,((-3.479*(log10(rs_ro_ratio))) + 1.658));
    return (gt);
  } else if ( gas_id == GAS_ACETONE ) {
    ga = pow(10,((-3.452*(log10(rs_ro_ratio))) + 1.542));
    return (ga);
  }   
} 

  else if ( accuracy == 1 ) {
    if ( gas_id == GAS_CARBON_DIOXIDE ) {
      gcd = pow(10,((-2.890*(log10(rs_ro_ratio))) + 2.055));
    return (gcd);
  } else if ( gas_id == GAS_CARBON_MONOXIDE ) {
    gcm = pow(10,(1.457*pow((log10(rs_ro_ratio)), 2) - 4.725*(log10(rs_ro_ratio)) + 2.855));
    return (gcm);
  } else if ( gas_id == GAS_ALCOHOL ) {
    gal = pow(10,((-3.181*(log10(rs_ro_ratio))) + 1.895));
    return (gal);
  } else if ( gas_id == GAS_AMMONIUM ) {
    gam = pow(10,((-2.469*(log10(rs_ro_ratio))) + 2.005));
    return (gam);
  } else if ( gas_id == GAS_TOLUENE ) {
    gt = pow(10,((-3.479*(log10(rs_ro_ratio))) + 1.658));
    return (gt);
  } else if ( gas_id == GAS_ACETONE ) {
    ga = pow(10,(-1.004*pow((log10(rs_ro_ratio)), 2) - 3.525*(log10(rs_ro_ratio)) + 1.553));
    return (ga);
  }
}    
  return 0;
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
  cmd += gcd;
  cmd += "&field3=";   
  cmd += gcm;
  cmd += "&field4=";   
  cmd += gal;
  cmd += "&field5=";   
  cmd += gam;
  cmd += "&field6=";   
  cmd += gt;
  cmd += "&field7=";   
  cmd += ga;
  cmd += "\r\n";
  Serial.print("AT+CIPSEND=");
  esp8266.print("AT+CIPSEND=");
  Serial.println(cmd.length());
  esp8266.println(cmd.length());
  Serial.print(cmd);
  esp8266.print(cmd);
}
