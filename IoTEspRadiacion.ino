#include "DHT.h"

#define DTHPIN 17
#define DTHTYPE DHT11

#define TACTULIZACION 1000

DHT dht(DTHPIN, DTHTYPE);
String consola;
float h=0;//Humedad
float t=0;//Temperatura

void setup() {
  Serial.begin(115200);
  Serial.println("Proyecto de investigación 1096");   
  Serial.println("Universidad del Quindío");
  Serial.println("Año 2024"); 
  Serial.println("Cesar Augusto Alvarez Gaspar");
  Serial.println("Auramaria Londoño Cano");   

  dht.begin();
  consola="[°C]\t[%]";
  Serial.println(consola);
}

// the loop function runs over and over again forever
void loop() {

  
  t=dht.readTemperature();
  h=dht.readHumidity();
  consola=t;
  consola+="\t";
  consola+=h;
  
  Serial.println(consola);
  delay(TACTULIZACION);

}
