#include <CapSense.h>

#define C_UMBRAL 5

#define DEBUG


void setup() {
  
#ifdef DEBUG
  Serial.begin(9600);
#endif

#ifndef DEBUG
   Keyboard.begin();
#endif


}

void loop() {

  long start = millis();
  
  uint8_t c_7 = readCapacitivePin(7);
  uint8_t c_9 = readCapacitivePin(9);
  
#ifndef DEBUG
   if (c_7 >= C_UMBRAL) Keyboard.write("a");
   else Keyboard.write("b");
   if (c_9 >= C_UMBRAL) Keyboard.write("c");
   else Keyboard.write("d");
#endif
  
#ifdef DEBUG
  Serial.print(millis() - start);
  Serial.print("\t");
  Serial.print(c_7, DEC);
  Serial.print("\t");
  Serial.print(c_9, DEC);
  Serial.println("");
#endif

}

