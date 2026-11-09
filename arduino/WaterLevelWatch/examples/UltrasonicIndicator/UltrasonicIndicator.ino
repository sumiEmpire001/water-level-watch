#include <Arduino.h>
#include <WaterLevelWatch.h>
// HC-SR04-style sensor. 5V ECHO needs level shifting on 3.3V ESP32.
// GPIO drives LED indicator ONLY, never a water pump.
#if defined(ARDUINO_ARCH_ESP32)
const int TRIGGER_PIN=26,ECHO_PIN=27,PUMP_LED=18;
#else
const int TRIGGER_PIN=2,ECHO_PIN=3,PUMP_LED=8;
#endif
const double TANK_HEIGHT_CM=100.0;
waterlevel::Monitor monitor(waterlevel::Settings(TANK_HEIGHT_CM));
void setup(){
 Serial.begin(115200);pinMode(TRIGGER_PIN,OUTPUT);pinMode(ECHO_PIN,INPUT);
 pinMode(PUMP_LED,OUTPUT);digitalWrite(PUMP_LED,LOW);
}
void loop(){
 digitalWrite(TRIGGER_PIN,LOW);delayMicroseconds(2);
 digitalWrite(TRIGGER_PIN,HIGH);delayMicroseconds(10);digitalWrite(TRIGGER_PIN,LOW);
 unsigned long echo_us=pulseIn(ECHO_PIN,HIGH,30000UL);
 double distance=echo_us ? echo_us*0.0343/2.0 : -1.0;
 uint32_t seconds=millis()/1000UL;
 waterlevel::Report r=monitor.record(seconds,distance,seconds);
 digitalWrite(PUMP_LED,r.valid&&r.pump_on?HIGH:LOW);
 Serial.print("valid=");Serial.print(r.valid);
 Serial.print(" level_pct=");Serial.print(r.level_percent);
 Serial.print(" pump_LED=");Serial.println(r.pump_on);
 delay(1000);
}
