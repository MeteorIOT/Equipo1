int sumaVeleta=0;      
const byte pinDireccion = A2;       //pin Analógico 
int direccion = 0;
int tiempoEnvio=30;

int leerDireccion(int suma){
  suma=suma/tiempoEnvio;
  if(suma>=415 && suma< 440) return 0;
  if(suma>=440 && suma< 490) return 45;
  if(suma>=490 && suma< 510) return 90;
  if(suma>=540 && suma< 550) return 135;
  if(suma>=510 && suma< 525) return 18;
  if(suma>=525 && suma< 540) return 225;
  if(suma>=590 && suma< 615) return 270;
  if(suma>=615 && suma< 620) return 315;
}

void setup () {
  Serial.begin(9600);        //iniciamos comunicación serial               
  delay(200);
}

void loop () {
  for(int i=0;i<=tiempoEnvio;i++){
    sumaVeleta+=analogRead(pinDireccion);
    delay(1000);
  }
  direccion=leerDireccion(sumaVeleta);
  sumaVeleta=0;
  Serial.print("dirección: ");
  Serial.println(direccion);
}



