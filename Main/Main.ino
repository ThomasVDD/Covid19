#include "TimerThree.h"

//---------------------------------------------------------------
// SERIAL MONITOR VARIABLES
//---------------------------------------------------------------

unsigned long lastWatchdogTime = millis();
unsigned long Watchdog = 3000; // 3 seconds watchdog

//---------------------------------------------------------------
// SETUP
//---------------------------------------------------------------
 
void setup()
{
  pinMode(13, OUTPUT);
  Timer3.initialize(100000);         // initialize timer3 in us, set 100 ms timing
  Timer3.attachInterrupt(controller);  // attaches callback() as a timer overflow interrupt

  Serial.begin(115200);
  Serial1.begin(115200);
  Serial.println("Setup done");
}

//---------------------------------------------------------------
// LOOP
//---------------------------------------------------------------

void loop()
{
  // Handle uart send to PC
  sendDataToPython();
  // Handle uart receive from PC
  recvWithEndMarkerSer0();
  // Check alarm and watchdog
  doWatchdog();
  if (getAlarmState != 1){
    // SOUND BUZZER
    // COMMUNICATE TO SCREEN
  }
  
  // Handle uart receive from display module
  recvWithEndMarkerSer1();
  
  // Control motors
  delay(20);
}

// ---------------------------------------------------------------------------------------------------------
// PID
// ---------------------------------------------------------------------------------------------------------
 
void controller()
{
  // Handle main control loop
  digitalWrite(13, digitalRead(13) ^ 1);
}

// ---------------------------------------------------------------------------------------------------------

void doWatchdog(void){
  if (millis() - lastWatchdogTime > Watchdog){
    setAlarmState(5);
  }
  Serial.print("ALARM=");
  Serial.println(getAlarmState());
}
