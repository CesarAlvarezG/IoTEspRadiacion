#include "DHT.h"
//#include <Wire.h>
//#include "SSD1306Wire.h"   

//SSD1306Wire display(0x3c, SDA, SCL); 

#define DTHPIN 17
#define DTHTYPE DHT11

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
  consola="[%]\t[°C]";
  Serial.println(consola);
}

// the loop function runs over and over again forever
void loop() {

  h=dht.readHumidity();
  t=dht.readTemperature();
  consola=h;
  consola+="\t";
  consola+=t;
  Serial.println(consola);
  delay(1000);

}
