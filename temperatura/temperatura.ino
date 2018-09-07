/*
 * Nombre:                  Sensor de Temperatura
 * Fecha de Actualización:  19/07/2018
 * Correo de contacto:      info@meteorito.mx
 * Descripción:             Este programa mide la temperatura ambiente
                            mediante el sensor de temperatura DHT22
 * Libreria DHT22:          https://github.com/adafruit/DHT-sensor-library
 * Libreria Adafruit:       https://github.com/adafruit/Adafruit_Sensor
*/

#include "DHT.h"            //Se incluye a la libreria del Sensor DHT22
float temperatura;          //Se declara la variable "temperatura" en tipo flotante para recibir valores con números decimales

DHT dht(2, DHT22);          //Asigna el PIN 2 Digital para la señal del sensor DHT22
 
void setup() {
  Serial.begin(9600);       // Inicia comunicación serial
  dht.begin();              // Inicia el sensor
}

int kelvin(){
  temperatura = dht.readTemperature();
  return temperatura+273;
  }

int celsius(){
  temperatura = dht.readTemperature();    //Se guarda la lectura de temperatura en la variable "temperatura"
  return temperatura;
  }
void loop() {
  String ReceivedInput;
  //Serial.print("Temperatura: ");          //Se imprime la palabra " Temperatura: "
  //Serial.print(temperatura);              //Se imprime el valor obtenido por el sensor    
  //Serial.println(" *C ");                 //en grados Celsius
  Serial.print("Kelvin = ");
  Serial.print(kelvin());
  Serial.println(" K");
  Serial.print("Celsius = ");
  Serial.print(celsius());
  Serial.println(" C");
  Serial.println("------------------");
  delay(500);                             //Retardo de medio segundo
  
}
