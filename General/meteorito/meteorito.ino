#include <SFE_BMP180.h>
#include <Wire.h>
#include "DHT.h"    

char status;
double tem,presion;
float temperatura,humedad;          //Se declara la variable "temperatura" y "humedad" en tipo flotante para recibir valores con números decimales

DHT dht(2, DHT22);          //Asigna el PIN 2 Digital para la señal del sensor DHT22
SFE_BMP180 sensorPresion;   //sensor de presion

void setup() {
  Serial.begin(9600);
  dht.begin(); //inicio del sensor de temperatura
  sensorPresion.begin(); //inicio del sensor de presion
}

float kelvin(){
  temperatura = dht.readTemperature(); //leer temperatura
  return temperatura+273; //añadir 273 para pasar a kelvin
  }

float celsius(){
  temperatura = dht.readTemperature();    //Se guarda la lectura de temperatura en la variable "temperatura"
  return temperatura;
  }
void temperature(){ //metodo de temperatura
  Serial.print("Kelvin : ");
  Serial.print(kelvin());
  Serial.println(" K");
  Serial.print("Celsius : ");
  Serial.print(celsius());
  Serial.println(" C");
  }
void pressure(){ //metodo de presion
  status = sensorPresion.startTemperature(); 
  delay(status);
  sensorPresion.getTemperature(tem);
  status = sensorPresion.startPressure(3);
  delay(status);
  sensorPresion.getPressure(presion,tem);
  Serial.print("Presion absoluta: ");
  Serial.print(presion*0.1,2);
  Serial.println( " KPa");
  }
void humidity(){
  humedad = dht.readHumidity();
  Serial.print("Humedad: ");
  Serial.print(humedad);
  Serial.println(" %");
  }

void loop(){
  temperature();
  Serial.println();
  pressure();
  Serial.println();
  humidity();
  Serial.println("-----------------------------");
  delay(1500);
}
