#include <CapacitiveSensor.h>

#define C_UMBRAL 3000

#define SAMPLES 80

//#define DEBUG 1

/*
 * CapitiveSense Library Demo Sketch
 * Paul Badger 2008
 * Uses a high value resistor e.g. 10M between send pin and receive pin
 * Resistor effects sensitivity, experiment with values, 50K - 50M. Larger resistor values yield larger sensor values.
 * Receive pin is the sensor pin - try different amounts of foil/metal on this pin
 */

#define N_SENSORS 8

CapacitiveSensor   cs_1 = CapacitiveSensor(2,4);        // 10M resistor between pins 4 & 2, pin 2 is sensor pin, add a wire and or foil if desired
CapacitiveSensor   cs_2 = CapacitiveSensor(2,7);        // 10M resistor between pins 4 & 6, pin 6 is sensor pin, add a wire and or foil
CapacitiveSensor   cs_3 = CapacitiveSensor(2,9);        // 10M resistor between pins 4 & 8, pin 8 is sensor pin, add a wire and or foil

boolean back[N_SENSORS];
boolean pushed[N_SENSORS];
boolean edge[N_SENSORS];
boolean p_up[N_SENSORS];
boolean p_dn[N_SENSORS];
boolean last_p[N_SENSORS];

int p_length[N_SENSORS];


void setup()                    
{
   // turn off autocalibrate on channel 1 - just as an example
   //cs_4_8.set_CS_AutocaL_Millis(0xFFFFFFFF);
   
   // Set timeot millis
   //cs_4_2.set_CS_Timeout_Millis(100);
   
   // Set serial conection
   Serial.begin(9600);
   
   // Set length to 0
   for (int id = 0; id < N_SENSORS; id++) {
     p_length[id] = 0;
     last_p[id] = false;
   }
   
#ifndef DEBUG
   Keyboard.begin();
#endif
   
}

void capacitive_push(int id, int capacitive) {

    pushed[id] = (capacitive > C_UMBRAL);
    edge[id] = (back[id] != pushed[id]);
   
    //if (edge[id] && pushed[id]) p_length[id] = 0;
    //else if (pushed[id]) p_length[id] += 1;
    
    p_up[id] = (edge[id] && pushed[id]);
    p_dn[id] = (edge[id] && !pushed[id]);
    back[id] = pushed[id];
    
    if (p_up[id] && !last_p[id]) last_p[id] = true;
    else p_up[id] = false;
    if (p_dn[id] && last_p[id])  last_p[id] = false;
    else p_dn[id] = false;
    
}


void loop()                    
{
    long start = millis();
    //long total1 =  cs_1.capacitiveSensor(SAMPLES);
    
    long total2 =  cs_2.capacitiveSensor(SAMPLES);
    //digitalWrite(7, LOW);
    //pinMode(7, OUTPUT);
    //delay(1);
    //pinMode(7, INPUT);
    
    long total3 =  cs_3.capacitiveSensor(SAMPLES);
    //digitalWrite(9, LOW);
    //pinMode(9, OUTPUT);
    //delay(1);
    //pinMode(9, INPUT);

    capacitive_push(2, total2);
    capacitive_push(3, total3);
    
#ifndef DEBUG

    if (p_up[2]) Keyboard.write('c'); 
    if (p_dn[2]) Keyboard.write('d');
    if (p_up[3]) Keyboard.write('a'); 
    if (p_dn[3]) Keyboard.write('b');

#endif
    

#ifdef DEBUG
    Serial.print(millis() - start);        // check on performance in milliseconds
    Serial.print("\t");                    // tab character for debug windown spacing

    //Serial.print(total1);                  // print sensor output 1
    Serial.print("\t");
    Serial.print(total2);                  // print sensor output 2
    Serial.print("\t");
    Serial.print(total3);                // print sensor output 3
    
    Serial.print("\t");
    Serial.print(pushed[2]);
    Serial.print("\t");
    Serial.print(pushed[3]);

    Serial.print("\t");
 
    if (edge[3]) Serial.print("EDGE");
    else if (pushed[3]) Serial.print(" '' ");
    Serial.println();
    
 #endif
    
    delay(30);                             // arbitrary delay to limit data to serial port 
}
