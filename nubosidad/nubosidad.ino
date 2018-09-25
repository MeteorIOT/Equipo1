#define LDR_PIN                   0   // Here Analog pin 0
#define MAX_ADC_READING           1023  // 10bit adc 2^10 == 1023
#define ADC_REF_VOLTAGE           5.0 // 5 volts
#define REF_RESISTANCE            10030 // 10k resistor 
#define LUX_CALC_SCALAR           12518931 // Formula 
#define LUX_CALC_EXPONENT         -1.405  // exponent first calculated with calculator 

void setup(void) 
{
  Serial.begin(9600);
  Serial.println(F("Light Sensor Test")); Serial.println("");
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


void loop(void){  
  nubosidad();
  delay(3000); // every 250 ms 
}
