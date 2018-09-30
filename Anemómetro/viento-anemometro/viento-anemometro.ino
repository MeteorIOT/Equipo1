//palanca 5.5cm
const int pinAnemometro = 3;
unsigned long tiempoAntes;
unsigned long  tiempo=0;
unsigned long sumaTiempo=0;
byte contador=0;
bool bandera=0;

void setup() {
   pinMode(pinAnemometro, INPUT);
   Serial.begin(9600);
   attachInterrupt(digitalPinToInterrupt(pinAnemometro), interrupcionViento,RISING );
   tiempoAntes=millis();
}
 
void loop() {
}

void interrupcionViento() {
  if( millis()>(50+tiempoAntes)){
    bandera=!bandera;
    if(bandera==0){
      tiempo=(millis()-tiempoAntes);
      tiempoAntes=millis();
      sumaTiempo+=tiempo; 
      if(contador<=19){
        contador++;
        Serial.println(contador);
      }else{
        contador=0;
        float velocidad=(2*3.1416*0.05*3.6)/((sumaTiempo/1000.0)/20);
        Serial.print(velocidad);
        Serial.println("  Km/h");
        sumaTiempo=0;
      }
    }
  }
}
