#include "DHT.h"
#include <WiFi.h>
#include "secrets.h"
#include "ThingSpeak.h" // always include thingspeak header file after other header files and custom macros

char ssid[] = SECRET_SSID;   // your network SSID (name) 
char pass[] = SECRET_PASS;   // your network password
int keyIndex = 0;            // your network key Index number (needed only for WEP)
WiFiClient  client;

unsigned long myChannelNumber = SECRET_CH_ID;
const char * myWriteAPIKey = SECRET_WRITE_APIKEY;



#define DTHPIN 17
#define DTHTYPE DHT11

#define TACTULIZACION 60000

DHT dht(DTHPIN, DTHTYPE);
String consola;
float h=0;//Humedad
float t=0;//Temperatura
int paquetes=0;//Datos enviados


void setup() {
  Serial.begin(115200);
  Serial.println("Proyecto de investigación 1096");   
  Serial.println("Universidad del Quindío");
  Serial.println("Año 2024"); 
  Serial.println("Cesar Augusto Alvarez Gaspar");
  Serial.println("Auramaria Londoño Cano");   

  WiFi.mode(WIFI_STA);   
  ThingSpeak.begin(client);  

  dht.begin();
  consola="[°C]\t[%]";
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
  consola=t;
  consola+="\t";
  consola+=h;
  
  // set the fields with the values
  ThingSpeak.setField(1, paquetes++);
  ThingSpeak.setField(2, t);
  ThingSpeak.setField(3, h);
  int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
  if(x == 200){
    Serial.println("Channel update successful.");
  }
  else{
    Serial.println("Problem updating channel. HTTP error code " + String(x));
  }

  Serial.println(consola);
  delay(TACTULIZACION);

}
