#include "TimerThree.h"

//---------------------------------------------------------------
// SERIAL MONITOR VARIABLES
//---------------------------------------------------------------

unsigned long lastWatchdogTime = millis();
unsigned long Watchdog = 3000; // 3 seconds watchdog

volatile float Flow2Patient = 0;

//---------------------------------------------------------------
// SETUP
//---------------------------------------------------------------
 
void setup()
{
  Serial.begin(115200);
  Serial1.begin(115200);
  //--- set up flow sensors here, if init fails, we can continue
  Serial.print("Setting up flow sensor: ");
  if (FLOW_SENSOR_INIT()){
    Serial.println(" OK");
  }
  else {
    Serial.println(" Failed");
  }

  // interrupt
  pinMode(13, OUTPUT);
  Timer3.initialize(150000);         // initialize timer3 in us, set 100 ms timing
  Timer3.attachInterrupt(controller);  // attaches callback() as a timer overflow interrupt


  Serial.println("Setup done");
}

//---------------------------------------------------------------
// LOOP
//---------------------------------------------------------------

void loop()
{
  // Handle uart send to PC
//  sendDataToPython();
  // Handle uart receive from PC
  recvWithEndMarkerSer0();
  // Check alarm and watchdog
//  doWatchdog();
  if (getAlarmState != 1){
    // SOUND BUZZER
    // COMMUNICATE TO SCREEN
  }
  
  // Handle uart receive from display module
  recvWithEndMarkerSer1();
  
  // Control motors
  delay(20);

  Serial.println(Flow2Patient);
}

// ---------------------------------------------------------------------------------------------------------
// PID
// ---------------------------------------------------------------------------------------------------------
 
void controller()
{
  interrupts();
  // Handle main control loop
  digitalWrite(13, digitalRead(13) ^ 1);
  bool isFlow2PatientRead = FLOW_SENSOR_Measure(&Flow2Patient);
}

// ---------------------------------------------------------------------------------------------------------

void doWatchdog(void){
  if (millis() - lastWatchdogTime > Watchdog){
    setAlarmState(5);
  }
  Serial.print("ALARM=");
  Serial.println(getAlarmState());
}
