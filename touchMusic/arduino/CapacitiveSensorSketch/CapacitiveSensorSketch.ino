#include <CapacitiveSensor.h>

#define C_UMBRAL 2000

//#define DEBUG 1

/*
 * CapitiveSense Library Demo Sketch
 * Paul Badger 2008
 * Uses a high value resistor e.g. 10M between send pin and receive pin
 * Resistor effects sensitivity, experiment with values, 50K - 50M. Larger resistor values yield larger sensor values.
 * Receive pin is the sensor pin - try different amounts of foil/metal on this pin
 */


CapacitiveSensor   cs_4_2 = CapacitiveSensor(2,4);        // 10M resistor between pins 4 & 2, pin 2 is sensor pin, add a wire and or foil if desired
CapacitiveSensor   cs_4_6 = CapacitiveSensor(2,6);        // 10M resistor between pins 4 & 6, pin 6 is sensor pin, add a wire and or foil
CapacitiveSensor   cs_4_8 = CapacitiveSensor(2,8);        // 10M resistor between pins 4 & 8, pin 8 is sensor pin, add a wire and or foil

boolean back_3;
boolean pushed_3;
boolean edge_3;
boolean p_up_3;
boolean p_dn_3;
boolean last_p_3;

int p_length_3;


void setup()                    
{
   // turn off autocalibrate on channel 1 - just as an example
   //cs_4_8.set_CS_AutocaL_Millis(0xFFFFFFFF);
   
   // Set timeot millis
   //cs_4_2.set_CS_Timeout_Millis(100);
   
   // Set serial conection
   Serial.begin(9600);
   
   // Set length to 0
   p_length_3 = 0;
   last_p_3 = false;
   
#ifndef DEBUG
   Keyboard.begin();
#endif
   
}

void capacitive_push(int capacitive) {

    pushed_3 = (capacitive > C_UMBRAL);
    edge_3 = (back_3 != pushed_3);
   
    //if (edge_3 && pushed_3) p_length_3 = 0;
    //else if (pushed_3) p_length_3 += 1;
    
    p_up_3 = (edge_3 && pushed_3);
    p_dn_3 = (edge_3 && !pushed_3);
    back_3 = pushed_3;
    
    if (p_up_3 && !last_p_3) last_p_3 = true;
    else p_up_3 = false;
    if (p_dn_3 && last_p_3)  last_p_3 = false;
    else p_dn_3 = false;
    
}


void loop()                    
{
    long start = millis();
    //long total1 =  cs_4_2.capacitiveSensor(3);
    //long total2 =  cs_4_6.capacitiveSensor(3);
    long total3 =  cs_4_8.capacitiveSensor(30);

    capacitive_push(total3);
    
#ifndef DEBUG

    if (p_up_3) Keyboard.write('a'); 
    if (p_dn_3) Keyboard.write('b');

#endif
    

#ifdef DEBUG
    Serial.print(millis() - start);        // check on performance in milliseconds
    Serial.print("\t");                    // tab character for debug windown spacing

    //Serial.print(total1);                  // print sensor output 1
    Serial.print("\t");
    //Serial.print(total2);                  // print sensor output 2
    Serial.print("\t");
    Serial.print(total3);                // print sensor output 3
    
    Serial.print("\t");
    Serial.print(pushed_3);

    Serial.print("\t");
 
    if (edge_3) Serial.print("EDGE");
    else if (pushed_3) Serial.print(" '' ");
    Serial.println();
    
 #endif
    
    delay(10);                             // arbitrary delay to limit data to serial port 
}
