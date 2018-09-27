#include <SFE_BMP180.h>
#include <Wire.h>
#include "DHT.h"    

#define LDR_PIN                   0   // Here Analog pin 0
#define MAX_ADC_READING           1023  // 10bit adc 2^10 == 1023
#define ADC_REF_VOLTAGE           5.0 // 5 volts
#define REF_RESISTANCE            10030 // 10k resistor 
#define LUX_CALC_SCALAR           12518931 // Formula 
#define LUX_CALC_EXPONENT         -1.405  // exponent first calculated with calculator 
#define HALL_SEN_PIN 3
#define HALL_SEN_ANALOG_PIN A0
unsigned long tiempoAntesDos;
unsigned long  tiempoDos=0;
unsigned long sumaTiempoDos=0;
byte contadorDos=0;
int capacidadTotal=8.5;   //capacidad combinada de ambos lados en mL
float precipitacion = 0.0;
char status;
double tem,presion;
float temperatura,humedad;          //Se declara la variable "temperatura" y "humedad" en tipo flotante para recibir valores con números decimales

DHT dht(2, DHT22);          //Asigna el PIN 2 Digital para la señal del sensor DHT22
SFE_BMP180 sensorPresion;   //sensor de presion

void setup() {
  Serial.begin(9600);
  dht.begin(); //inicio del sensor de temperatura
  sensorPresion.begin(); //inicio del sensor de presion
  pinMode(HALL_SEN_PIN, INPUT);
  attachInterrupt(digitalPinToInterrupt(HALL_SEN_PIN), interrupcionPrecipitacion,RISING );
  tiempoAntesDos = millis();
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
void nubosidad(){
  int   ldrRawData;
  float resistorVoltage, ldrVoltage;
  float ldrResistance;
  float lux;
  const char tipoNubosidad[5]={'C','M','N','P','D'};
  
  // Perform the analog to digital conversion  
  ldrRawData = analogRead(LDR_PIN);
  // RESISTOR VOLTAGE_CONVERSION
  // Convert the raw digital data back to the voltage that was measured on the analog pin
  resistorVoltage = (float)ldrRawData / MAX_ADC_READING * ADC_REF_VOLTAGE;
  // voltage across the LDR is the 5V supply minus the 5k resistor voltage
  ldrVoltage = ADC_REF_VOLTAGE - resistorVoltage;
  // LDR_RESISTANCE_CONVERSION
  // resistance that the LDR would have for that voltage  
  ldrResistance = ldrVoltage/resistorVoltage * REF_RESISTANCE;
  // LDR_LUX
  lux = LUX_CALC_SCALAR * pow(ldrResistance, LUX_CALC_EXPONENT);
  Serial.print("Tipo de nubosidad: ");
  if(lux <0.2){
    Serial.println(tipoNubosidad[0]);
  }
  if(lux > 0.2 && lux <107){
    Serial.println(tipoNubosidad[1]);
  }
  if(lux > 107 && lux <1075){
    Serial.println(tipoNubosidad[2]);
  }
  if(lux > 1075 && lux <10752){
    Serial.println(tipoNubosidad[3]);
  }
  if(lux > 10752){
    Serial.println(tipoNubosidad[4]);
  }
 
  // print out the results
  Serial.print("Iluminancia LDR: "); Serial.print(lux); Serial.println(" lux");
  Serial.println();
}
void interrupcionPrecipitacion() {
  if( millis()>(50+tiempoAntesDos)){
      tiempoDos=(millis()-tiempoAntesDos);
      tiempoAntesDos=millis();
      sumaTiempoDos+=tiempoDos; 
      if(contadorDos<=19){
        contadorDos++;
      }else{
        precipitacion=contadorDos*(((capacidadTotal*10)/(42.84))/(sumaTiempoDos/1000.0));
        Serial.print("* Lluvia detectada ");
        Serial.print(precipitacion);
        Serial.println(" mm/s");
        sumaTiempoDos=0;
        contadorDos=0;
      }
  }
}


void loop(){
  temperature();
  Serial.println();
  pressure();
  Serial.println();
  humidity();
  Serial.println();
  nubosidad();
  Serial.println("-----------------------------");
  delay(3000);
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
