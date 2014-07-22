#include <CapacitiveSensor.h>

#define SAMPLES 50

#define C_UMBRAL 20

#define C_UP_UMBRAL 20
#define C_DN_UMBRAL 20

//#define ENABLE_INTEGRAL
#define R_TIME 30000
#define SHOW_DI

#define DEBUG 1


/*
 * CapitiveSense Library Demo Sketch
 * Paul Badger 2008
 * Uses a high value resistor e.g. 10M between send pin and receive pin
 * Resistor effects sensitivity, experiment with values, 50K - 50M. Larger resistor values yield larger sensor values.
 * Receive pin is the sensor pin - try different amounts of foil/metal on this pin
 */

#define N_SENSORS 6

CapacitiveSensor   cs_1 = CapacitiveSensor(2,4);        // 10M resistor between pins 4 & 2, pin 2 is sensor pin, add a wire and or foil if desired
CapacitiveSensor   cs_2 = CapacitiveSensor(2,6);        // 10M resistor between pins 4 & 6, pin 6 is sensor pin, add a wire and or foil
CapacitiveSensor   cs_3 = CapacitiveSensor(2,8);        // 10M resistor between pins 4 & 8, pin 8 is sensor pin, add a wire and or foil
CapacitiveSensor   cs_4 = CapacitiveSensor(2,10);
CapacitiveSensor   cs_5 = CapacitiveSensor(2,12);

boolean back[N_SENSORS];
boolean pushed[N_SENSORS];
boolean edge[N_SENSORS];
boolean p_up[N_SENSORS];
boolean p_dn[N_SENSORS];
boolean last_p[N_SENSORS];

int _DI[N_SENSORS];
long last_total[N_SENSORS];
long dt;
long reset_time;
long total_cycles;
long stall_cycles;

int p_length[N_SENSORS];

void setup()                    
{
   // turn off autocalibrate on channel 1 - just as an example
   //cs_4_8.set_CS_AutocaL_Millis(0xFFFFFFFF);   
   // Set timeot millis
   //cs_4_2.set_CS_Timeout_Millis(100);
   
   // Set serial conection
   reset_time = millis();
   Serial.begin(9600);
   
   reset_cap();
   
   dt = 0;
   
   
#ifndef DEBUG
   Keyboard.begin();
#endif
   
}

void reset_cap() {
#ifdef DEBUG
   Serial.print("RESET ");
   Serial.print(millis() - reset_time);
   Serial.println(" ms");
#endif
  
   // Set length to 0
   for (int id = 0; id < N_SENSORS; id++) {
     p_length[id] = 0;
     last_p[id] = false;
     _DI[id] = 0;
   }
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

void capacitive_push_diff(int id, int capacitive_diff) {
    p_up[id] = (capacitive_diff > C_UP_UMBRAL);
    p_dn[id] = (capacitive_diff < -C_DN_UMBRAL);
    
    if (p_up[id] && !last_p[id]) last_p[id] = true;
    else p_up[id] = false;
    if (p_dn[id] && last_p[id])  last_p[id] = false;
    else p_dn[id] = false;
}

void capacitive_DiffInt(int id, int capacitive) {
    // Parte diferencial
    long _D = (capacitive - last_total[id]) / dt; 
    // Parte integral
    // BUG: Cuando la parte diferncias oscila
    //      continuamente pude poco recomendable integrar
#ifdef ENABLE_INTEGRAL
    _DI[id] += _D;
    
    // Contabiliza los ciclos sin actividad y los 
    // cilcos totales desde el ultimo reset
    if (abs(_D) < 5) stall_cycles++;
    total_cycles++;    
#else
    _DI[id] = _D;
#endif
    
    last_total[id] = capacitive;
}


void auto_reset_cap() {
  // Solo autoresetea cuando esta habilitado el calculo integral
#ifdef ENABLE_INTEGRAL
    // Resetea cuando se mantien detenido durante R_TIME ms
    long reset_dt = millis() - reset_time;
    if (reset_dt >= R_TIME) {
      if (stall_cycles >= total_cycles)
        reset_cap(); 
      reset_time = millis();
      total_cycles = 0;
    } 
#endif
}

void show_push(int id) {
#if DEBUG
      if (p_up[id]) {
        Serial.print(id);
        Serial.print(" ");
      }
      if (p_dn[id]) {
        Serial.print("~");
        Serial.print(id);
      }
#endif
}

void loop()                    
{
    long start = millis();
    long total1 =  cs_1.capacitiveSensor(SAMPLES);    
    long total2 =  cs_2.capacitiveSensor(SAMPLES);    
    long total3 =  cs_3.capacitiveSensor(SAMPLES);
    long total4 =  cs_4.capacitiveSensor(SAMPLES);
    long total5 =  cs_5.capacitiveSensor(SAMPLES);

#ifdef SHOW_DI
    capacitive_DiffInt(1, total1);
    capacitive_DiffInt(2, total2);
    capacitive_DiffInt(3, total3);
    capacitive_DiffInt(4, total4);
    capacitive_DiffInt(5, total5);

    capacitive_push_diff(1, _DI[1]);
    capacitive_push_diff(2, _DI[2]);
    capacitive_push_diff(3, _DI[3]);
    capacitive_push_diff(4, _DI[4]);
    capacitive_push_diff(5, _DI[5]);    
#else
    capacitive_push(1, total1);
    capacitive_push(2, total2);
    capacitive_push(3, total3);
    capacitive_push(4, total4);
    capacitive_push(5, total5);
#endif
    
#ifndef DEBUG

    if (p_up[1]) Keyboard.write('c'); 
    if (p_dn[1]) Keyboard.write('d');
    if (p_up[2]) Keyboard.write('a'); 
    if (p_dn[2]) Keyboard.write('b');

#endif
    

#ifdef DEBUG
    Serial.print(dt);        // check on performance in milliseconds
    Serial.print("\t");                    // tab character for debug windown spacing

#ifdef SHOW_DI
    Serial.print(_DI[1]);                  // print sensor output 1
    Serial.print("\t");
    Serial.print(_DI[2]);                  // print sensor output 2
    Serial.print("\t");
    Serial.print(_DI[3]);                // print sensor output 3
    Serial.print("\t");
    Serial.print(_DI[4]);                // print sensor output 3
    Serial.print("\t");
    Serial.print(_DI[5]);                // print sensor output 3
    
    Serial.print("\t");
    show_push(1);
    show_push(2);
    show_push(3);
    show_push(4);
    show_push(5);    
    
#else
    Serial.print(total1);                  // print sensor output 1
    Serial.print("\t");
    Serial.print(total2);                  // print sensor output 2
    Serial.print("\t");
    Serial.print(total3);                // print sensor output 3
    Serial.print("\t");
    Serial.print(total4);                // print sensor output 3
    Serial.print("\t");
    Serial.print(total5);                // print sensor output 3
#endif
    
    //Serial.print("\t");
    //Serial.print(pushed[2]);
    //Serial.print("\t");
    //Serial.print(pushed[3]);
    //Serial.print("\t");
 
    //if (edge[3]) Serial.print("EDGE");
    //else if (pushed[3]) Serial.print(" '' ");
    Serial.println();
    
 #endif
    
    auto_reset_cap();
    
    //delay(30);                             // arbitrary delay to limit data to serial port 
    dt = millis() - start;
}

