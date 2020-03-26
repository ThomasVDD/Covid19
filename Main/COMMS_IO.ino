#ifndef COMMS_IO_H
#define COMMS_IO_H

//---------------------------------------------------------------
// BREATHING VARIABLES
//---------------------------------------------------------------

// define bytes to send
unsigned int RR = 20; 
// Number of breaths per minute setting
unsigned int VT = 0; // TODO
// Tidal volume= target to deliver
unsigned int PK = 50;
//Peak pressure
unsigned int TS = 0; // TODO
// Breath Trigger Sensitivity = amount the machine should look for
float IE = 0.3;
// Inspiration-expiration rate
unsigned int PP = 0; // NOT USED
//PEEP Pressure = Max pressure to deliver
bool Mode = false; //false = Pressure // TODO
//Mode
bool ACTIVE = true;
// active: start or stop

unsigned int PKtresh = 10;
unsigned int VTtresh = 10;
unsigned int PPtresh = 5;

unsigned long comms_getInhaleTime(){
  float target_inhale_duration = 1000.0 * 60.0 * IE / RR  ;   
  unsigned long target_inhale_duration_int = (unsigned long) target_inhale_duration;
  return target_inhale_duration_int;
}

unsigned long comms_getExhaleTime(){
  float target_exhale_duration = 1000.0 * 60.0 * (1-IE) / RR  ;   
  unsigned long target_exhale_duration_int = (unsigned long) target_exhale_duration;
  return target_exhale_duration_int;
}

unsigned int comms_getRR(){
  return RR;
}

unsigned int comms_getVT(){
  return VT;
}

unsigned int comms_getPK(){
  return PK;
}

unsigned int comms_getTS(){
  return TS;
}

float comms_getIE(){
  return IE; 
}

unsigned int comms_getPP(){
  return PP;
}

bool comms_getMode(){
  return Mode;
}

bool comms_getActive(){
  return ACTIVE;
}

//---------------------------------------------------------------
// PYTHON VARIABLES
//---------------------------------------------------------------

unsigned int ALARM = 0;

// send value * 10!!!
unsigned int BPM = 10;
// Breaths per minute
unsigned int VOL = 20;
// volume
unsigned int TRIG = 30;
// trigger
unsigned int PRES = 40;
// pressure

void comms_setBPM(unsigned long bpm_time){
  BPM = 1/(float)bpm_time;
}
void comms_setVOL(unsigned int vol){
  VOL = vol;
}
void comms_setTRIG(unsigned int trig){
  TRIG = trig;
}
void comms_setPRES(unsigned int pres){
  PRES = pres;
}

//---------------------------------------------------------------
// SERIAL MONITOR VARIABLES
//---------------------------------------------------------------

String value1;
const byte numChars = 32;
char receivedChars1[numChars];
boolean newData1 = false;
String value0;
char receivedChars0[numChars];
boolean newData0 = false;

//---------------------------------------------------------------
// FUNCTIONS PYTHON
//---------------------------------------------------------------

void processPython(String input){
  value0 = getvalue(input, '=', 1); 
  if (input.startsWith("ALARM")){
    setAlarmState(value0.toInt());
  }
}

void sendDataToPython(){
  Serial.print("BPM=");
  Serial.println(BPM*10);
  Serial.print("VOL=");
  Serial.println(VOL*10);
  Serial.print("TRIG=");
  Serial.println(TRIG*10);
  Serial.print("PRES=");
  Serial.println(PRES*10);
}

void recvWithEndMarkerSer0() {
   static byte ndx = 0;
   char endMarker = '\n';
   char rc;
   
   while (Serial.available() > 0 && newData0 == false) {
     rc = Serial.read();
    
     if (rc != endMarker) {
       receivedChars0[ndx] = rc;
       ndx++;
       if (ndx >= numChars) {
         ndx = numChars - 1;
       }
     }
     else {
       receivedChars0[ndx] = '\0'; // terminate the string
       ndx = 0;
       newData0 = true;
       Serial.println(receivedChars0);
     }
   }

   if (newData0 == true){
      lastWatchdogTime = millis();
      processPython(receivedChars0);
      newData0 = false;
  }
}

//---------------------------------------------------------------
// FUNCTIONS IO
//---------------------------------------------------------------

void processSerialPort(String input){
  value1 = getvalue(input, '=', 1);
  //Serial.println(value1);
  
  if (input.startsWith("PKtresh")){
    PKtresh = value1.toInt(); // update value1
  }
  else if (input.startsWith("VTtresh")){
    VTtresh = value1.toInt(); // update value1
  }
  else if (input.startsWith("PPtresh")){
    PPtresh = value1.toInt(); // update value1
  }
  else if (input.startsWith("Mode")){
    Mode = value1.toInt(); // update value1
  }
  else if (input.startsWith("PP")){
    PP = value1.toInt(); // update value1
  }
  else if (input.startsWith("IE")){
    IE = value1.toFloat(); // update value
  }
  else if (input.startsWith("TS")){
    TS = value1.toInt(); // update value1
  }
  else if (input.startsWith("PK")){
    PK = value1.toInt(); // update value1
  }
  else if (input.startsWith("VT")){
    VT = value1.toInt(); // update value1
  }  
  else if (input.startsWith("RR")){
    RR = value1.toInt(); // update value1
  }
}

void recvWithEndMarkerSer1() {
   static byte ndx = 0;
   char endMarker = '\n';
   char rc;
   
   while (Serial1.available() > 0 && newData1 == false) {
     rc = Serial1.read();
    
     if (rc != endMarker) {
       receivedChars1[ndx] = rc;
       ndx++;
       if (ndx >= numChars) {
         ndx = numChars - 1;
       }
     }
     else {
       receivedChars1[ndx] = '\0'; // terminate the string
       ndx = 0;
       newData1 = true;
       Serial.println(receivedChars1);
     }
   }

   if (newData1 == true){
      processSerialPort(receivedChars1);
      newData1 = false;
   }
}

//---------------------------------------------------------------
// FUNCTIONS ALARM
//---------------------------------------------------------------

void setAlarmState(int alarm){
  if (alarm >= ALARM){ // don't overwrite alarm with 0 if alarm state exists!
    if (alarm >= ALARM){
      ALARM = alarm;
    }
  }
}

int getAlarmState(void){
  return ALARM;
}

//---------------------------------------------------------------
// FUNCTIONS EXTRA
//---------------------------------------------------------------

String getvalue(String data, char separator, int index)
{
    int found = 0;
    int strIndex[] = { 0, -1 };
    int maxIndex = data.length() - 1;

    for (int i = 0; i <= maxIndex && found <= index; i++) {
        if (data.charAt(i) == separator || i == maxIndex) {
            found++;
            strIndex[0] = strIndex[1] + 1;
            strIndex[1] = (i == maxIndex) ? i+1 : i;
        }
    }
    return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}

#endif
