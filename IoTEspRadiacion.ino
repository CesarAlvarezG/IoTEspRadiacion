#include "DHT.h"
#include <WiFi.h>
#include "secrets.h"
#include "ThingSpeak.h" // always include thingspeak header file after other header files and custom macros
#include <Adafruit_Sensor.h>
#include "Adafruit_TSL2591.h"

char ssid[] = SECRET_SSID;   // your network SSID (name) 
char pass[] = SECRET_PASS;   // your network password
int keyIndex = 0;            // your network key Index number (needed only for WEP)
WiFiClient  client;

unsigned long myChannelNumber = SECRET_CH_ID;
const char * myWriteAPIKey = SECRET_WRITE_APIKEY;

int red=0;//Estado de la red

#define DTHPIN 17
#define DTHTYPE DHT11

#define TACTULIZACION 60000

DHT dht(DTHPIN, DTHTYPE);
String consola;
float h=0;//Humedad
float t=0;//Temperatura
float Irr=0;//Irradiación solar
int paquetes=0;//Datos enviados

Adafruit_TSL2591 tsl = Adafruit_TSL2591(2591); // pass in a number for the sensor identifier (for your use later)


void setup() {
  Serial.begin(115200);
  Serial.println("Proyecto de investigación 1096");   
  Serial.println("Universidad del Quindío");
  Serial.println("Año 2024"); 
  Serial.println("Cesar Augusto Alvarez Gaspar");
  Serial.println("Auramaria Londoño Cano");   
  
  if(WiFi.status() != WL_CONNECTED){
    Serial.print("\nConectando a red SSID: ");
    Serial.println(SECRET_SSID);
    while(WiFi.status() != WL_CONNECTED){
      WiFi.begin(ssid, pass);  // Connect to WPA/WPA2 network. Change this line if using open or WEP network
      Serial.print(".");
      delay(5000);     
    } 
    Serial.println("\nConectado.");
  }
  
  consola="Estado General\n";
  Serial.println(consola);
  consola="DTH111\tTSL2591\tRed";
  Serial.println(consola);
  
  WiFi.mode(WIFI_STA);   
  ThingSpeak.begin(client);  
  
  dht.begin();
  t=dht.readTemperature();
  h=dht.readHumidity();
  if (isnan(h) || isnan(t)) {
    consola="dowm";
  }else{
      consola ="Up";
    }
  if (tsl.begin()) 
  {
    consola+="\t";
    consola+="Up";
  } 
  else 
  {
    consola+="\t";
    consola+="Down";
  }
  tsl.setGain(TSL2591_GAIN_MED);      // 25x gain
  tsl.setTiming(TSL2591_INTEGRATIONTIME_300MS);
  
  consola+="\t";
  if(WiFi.status() != WL_CONNECTED)
  {
    consola+="Down";
  }else{
        consola+="Up";
    }
  
  Serial.println(consola);
  
  Serial.println("\n");
  consola="[n]\t[°C]\t[%]\t[W/m2]\tStatus";
  Serial.println(consola);
}

// the loop function runs over and over again forever
void loop() {

   if(WiFi.status() != WL_CONNECTED){
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(SECRET_SSID);
    while(WiFi.status() != WL_CONNECTED){
      WiFi.begin(ssid, pass);  // Connect to WPA/WPA2 network. Change this line if using open or WEP network
      Serial.print(".");
      delay(5000);     
    } 
    Serial.println("\nConnected.");
  }

  t=dht.readTemperature();
  h=dht.readHumidity();
  uint16_t q = tsl.getLuminosity(TSL2591_FULLSPECTRUM);
  Irr=q/685;//Conversión de Lux a W/m2
  consola=paquetes;
  consola+="\t";
  consola+=t;
  consola+="\t";
  consola+=h;
  consola+="\t";
  consola+=Irr;
  
  
  // set the fields with the values
  ThingSpeak.setField(1, paquetes++);
  ThingSpeak.setField(2, t);
  ThingSpeak.setField(3, h);
  ThingSpeak.setField(4, Irr);
  int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
  if(x == 200){
    //Serial.println("Channel update successful.");
    consola+="\t";
    consola+="Up";
  }
  else{
    //Serial.println("Problem updating channel. HTTP error code " + String(x));
    consola+="\t";
    consola+="down:" + String(x);
  }

  Serial.println(consola);
  delay(TACTULIZACION);

}
