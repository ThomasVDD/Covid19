#include "TimerThree.h"

//---------------------------------------------------------------
// SERIAL MONITOR VARIABLES
//---------------------------------------------------------------

unsigned long lastWatchdogTime = millis();
unsigned long Watchdog = 3000; // 3 seconds watchdog

volatile unsigned long exhaleStartTime = millis();

volatile float CurrentPressurePatient = 0;
volatile float Flow2Patient = 0;

typedef enum {ini = 0x00, wait = 0x01, inhale = 0x02, exhale = 0x03} controller_state_t;
controller_state_t controller_state = 0x00;

//---------------------------------------------------------------
// SETUP
//---------------------------------------------------------------

void setup()
{
  Serial.begin(115200);
  Serial1.begin(115200);
  //--- set up flow sensors here, if init fails, we can continue
  Serial.print("Setting up flow sensor: ");
  if (FLOW_SENSOR_INIT()) {
    Serial.println("FLOW SENSOR OK");
  }
  else {
    Serial.println("FLOW SENSOR Failed");
  }
  //-- set up BME
  while (!BME280_Setup()) // must start, if not, do not continue
  {
    delay(100);
  }
  Serial.println("BME OK");

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
  if (getAlarmState != 1) {
    // SOUND BUZZER
    // COMMUNICATE TO SCREEN
  }

  // Handle uart receive from display module
  recvWithEndMarkerSer1();

  // Control motors
  delay(20);

  Serial.println(Flow2Patient);
  Serial.println(CurrentPressurePatient);
}

// ---------------------------------------------------------------------------------------------------------
// PID
// ---------------------------------------------------------------------------------------------------------

void controller()
{
  // readout sensors
  interrupts();
  bool isFlow2PatientRead = FLOW_SENSOR_Measure(&Flow2Patient);
  bool isPatientPressureCorrect = BME280_readPressurePatient(&CurrentPressurePatient);
  noInterrupts();

  switch (controller_state) {
    case ini:
      // Check user input to start controller
      if (comms_getActive() == true) {
        controller_state = wait; // start controller
      }
      break;
    case inhale: //code van de jongens rechts van ons
      // 1 keer tot einde gaan (geen exhale)
      // load 'new' setting values for controller
//      BREATHE_CONTROL_setTIDALVolume(comms_getVT());
//      BREATHE_CONTROL_setPEAKPressure(comms_getPK());
//      BREATHE_CONTROL_setBreathingSensitivity(comms_getTS());
//      BREATHE_CONTROL_setRespirationRatio(comms_getRR());
      controller_state = exhale;
      break;
    case exhale: // Stuur motor naar startpositie
      //
      // Check alarm
      //
      //if(alarm){
      //  sound alarm;
      //}
      exhaleStartTime = millis();
      controller_state = wait;
      break;
    case wait: 
      // Restart when 1) inhalation detected OR 2) timer passed
      if ((millis() - exhaleStartTime > comms_getTS() || true){ // TODO: replace true by underpressure
        controller_state = inhale;
      }
      // Check user input to stop controller
      if (comms_getActive() == false) { 
        controller_state = ini; // stop controller
      }
    default: controller_state = wait;
  }



}

// ---------------------------------------------------------------------------------------------------------

void doWatchdog(void) {
  if (millis() - lastWatchdogTime > Watchdog) {
    setAlarmState(5);
  }
  Serial.print("ALARM=");
  Serial.println(getAlarmState());
}
