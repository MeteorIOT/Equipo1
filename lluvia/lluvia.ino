const byte pinPluviometro = 2;  //pin digital

float precipitacion = 0.0;

//variables manejo de proceso precipitacion
unsigned long tiempoAntesDos;
unsigned long  tiempoDos=0;
unsigned long sumaTiempoDos=0;
byte contadorDos=0;
int capacidadTotal=8.5;   //capacidad combinada de ambos lados en mL

void setup () {
  Serial.begin(57600);        //iniciamos comunicación serial               
  delay(200);
  
   //Iniciamos pluviometro
   pinMode(pinPluviometro, INPUT);
   attachInterrupt(digitalPinToInterrupt(pinPluviometro), interrupcionPrecipitacion,RISING );
   tiempoAntesDos=millis();

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
        Serial.print(precipitacion);
        Serial.println(" mm/s");
        sumaTiempoDos=0;
        contadorDos=0;
      }
  }
}

void loop () {
}
