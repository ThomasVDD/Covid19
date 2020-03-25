#include "TimerThree.h"

//---------------------------------------------------------------
// BREATHING VARIABLES
//---------------------------------------------------------------

// define bytes to send
unsigned int RR = 0;
// Number of breaths per minute setting
unsigned int VT = 0;
// Tidal volume= target to deliver
unsigned int PK = 50;
//Peak pressure
unsigned int TS = 0;
// Breath Trigger Sensitivity = amount the machine should look for
float IE = 0;
// Inspiration-expiration rate
unsigned int PP = 0;
//PEEP Pressure = Max pressure to deliver
bool Mode = false; //false = Pressure
//Mode

unsigned int PKtresh = 10;
unsigned int VTtresh = 10;
unsigned int PPtresh = 5;

//---------------------------------------------------------------

String value;
const byte numChars = 32;
char receivedChars[numChars];
boolean newData = false;


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
 
void loop()
{
  // Handle uart receive from display module
  recvWithEndMarker();
  if (newData == true){
    processSerialPort(receivedChars);
    newData = false;
  }
  
  // Handle uart send to PC


  // Control motors
  delay(200);
}

// ---------------------------------------------------------------------------------------------------------
 
void controller()
{
  // Handle main control loop
  digitalWrite(13, digitalRead(13) ^ 1);
}

// ---------------------------------------------------------------------------------------------------------

void processSerialPort(String input){
  value = getValue(input, '=', 1);
  //Serial.println(value);
  
  if (input.startsWith("PKtresh")){
    PKtresh = value.toInt(); // update value
  }
  else if (input.startsWith("VTtresh")){
    VTtresh = value.toInt(); // update value
  }
  else if (input.startsWith("PPtresh")){
    PPtresh = value.toInt(); // update value
  }
  else if (input.startsWith("Mode")){
    Mode = value.toInt(); // update value
  }
  else if (input.startsWith("PP")){
    PP = value.toInt(); // update value
  }
  else if (input.startsWith("IE")){
    IE = value.toFloat(); // update value
//    Serial.print("new IE: ");
//    Serial.println(IE);
  }
  else if (input.startsWith("TS")){
    TS = value.toInt(); // update value
  }
  else if (input.startsWith("PK")){
    PK = value.toInt(); // update value
  }
  else if (input.startsWith("VT")){
    VT = value.toInt(); // update value
  }  
  else if (input.startsWith("RR")){
    RR = value.toInt(); // update value
  }
}

String getValue(String data, char separator, int index)
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

void recvWithEndMarker() {
   static byte ndx = 0;
   char endMarker = '\n';
   char rc;
   
   while (Serial1.available() > 0 && newData == false) {
     rc = Serial1.read();
    
     if (rc != endMarker) {
       receivedChars[ndx] = rc;
       ndx++;
       if (ndx >= numChars) {
         ndx = numChars - 1;
       }
     }
     else {
       receivedChars[ndx] = '\0'; // terminate the string
       ndx = 0;
       newData = true;
       Serial.println(receivedChars);
     }
   }
}
