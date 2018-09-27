#define HALL_SEN_PIN 3
#define HALL_SEN_ANALOG_PIN A0
unsigned long tiempoAntesDos;
unsigned long  tiempoDos=0;
unsigned long sumaTiempoDos=0;
byte contadorDos=0;
int capacidadTotal=8.5;   //capacidad combinada de ambos lados en mL
float precipitacion = 0.0;


void setup() {
  Serial.begin(9600);
  delay(200);

  pinMode(HALL_SEN_PIN, INPUT);
  attachInterrupt(digitalPinToInterrupt(HALL_SEN_PIN), interrupcionPrecipitacion,RISING );
  tiempoAntesDos = millis();

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
}
