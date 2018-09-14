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
//TEMPERATURA
float celsius(){
  temperatura = dht.readTemperature();    //Se guarda la lectura de temperatura en la variable "temperatura"
  return temperatura;
  }
float kelvin(float celsius){
  return celsius+273; //añadir 273 para pasar a kelvin
  }

float fahrenheit(float celsius){
  float f = dht.readTemperature(true);
  return f;
  }
void temp_sens(float fahrenheit){
  float fi = dht.readTemperature(true);
  float hi = dht.computeHeatIndex(fahrenheit, humedad);
  float hiDegC = dht.convertFtoC(hi);
  Serial.print("Sensación térmica: ");
  Serial.print(hiDegC);
  Serial.println(" C");
  }
void temperature(){ //metodo de temperatura
  Serial.print("Celsius : ");
  Serial.print(celsius());
  Serial.println(" C");
  Serial.print("Fahrenheit :");
  Serial.print(fahrenheit(celsius()));
  Serial.println("F");
  Serial.print("Kelvin : ");
  Serial.print(kelvin(celsius()));
  Serial.println(" K");
  temp_sens(fahrenheit(celsius()));
  Serial.print("Punto de rocío: ");
  Serial.print(dewPoint(fahrenheit(celsius()), humedad));
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
//extra
double dewPoint(double celsius, double humidity) //funcion para el punto de rocio
{
  // (1) Saturation Vapor Pressure = ESGG(T)
  double RATIO = 373.15 / (273.15 + celsius);
  double RHS = -7.90298 * (RATIO - 1);
  RHS += 5.02808 * log10(RATIO);
  RHS += -1.3816e-7 * (pow(10, (11.344 * (1 - 1 / RATIO ))) - 1) ;
  RHS += 8.1328e-3 * (pow(10, (-3.49149 * (RATIO - 1))) - 1) ;
  RHS += log10(1013.246);

  // factor -3 is to adjust units - Vapor Pressure SVP * humidity
  double VP = pow(10, RHS - 3) * humidity;

  // (2) DEWPOINT = F(Vapor Pressure)
  double T = log(VP / 0.61078); // temp var
  return (241.88 * T) / (17.558 - T);
}
