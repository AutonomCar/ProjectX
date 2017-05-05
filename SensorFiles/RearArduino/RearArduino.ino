//*************************INITIATE CAN****************************
#include <mcp_can.h>
#include <SPI.h>

long unsigned int rxId;
unsigned char len = 0;
unsigned char rxBuf[8];
char msgString[8];                        // Array to store serial string
boolean intFlag = false;

#define CAN0_INT 2                              // Set INT to pin 2
MCP_CAN CAN0(10);                               // Set CS to pin 10
//****************************************************************
//*************************INITIATE RANGE SENSOR******************
const int backSideTrigPin = 5;
const int backSideEchoPin = 6;
const int frontSideTrigPin = 8;
const int frontSideEchoPin = 9;
const int aSize = 5;

int backSide;
int frontSide;
//****************************************************************
void setup() { 
//***************************SETUP CAN****************************
  // Initialize MCP2515 running at 16MHz with a baudrate of 500kb/s and the masks and filters disabled.
  if(CAN0.begin(MCP_ANY, CAN_500KBPS, MCP_16MHZ) == CAN_OK)
    Serial.println("MCP2515 Initialized Successfully!");
  else
    Serial.println("Error Initializing MCP2515...");
  
  CAN0.setMode(MCP_NORMAL);                     // Set operation mode to normal so the MCP2515 sends acks to received data.

  pinMode(CAN0_INT, INPUT);                            // Configuring pin for /INT input
  
  Serial.println("MCP2515 Library Receive Example...");
  //***********TO DO, FIX INTTERRUPT*******************
  //attachInterrupt(0, setFlag, CHANGE);
//***************************************************************
//**************************SETUP RANGE SENSOR*******************
  pinMode(backSideTrigPin, OUTPUT);
  pinMode(backSideEchoPin, INPUT);
  pinMode(frontSideTrigPin, OUTPUT);
  pinMode(frontSideEchoPin, INPUT);

//***************************************************************
}
//***************************MAIN PROGRAM*************************
void loop() {
  
  long duration, cm;
  int backSideArray[aSize];
  int frontSideArray[aSize];

  if(flagCan==true){
    readCan();
    /*
     * if(canMessage==ultrasonicBackSide){
     *  sendCan(measure(backSideTrigPin,backSideEchoPin));
     *  }
     *  else if(canMessage==ultrasonicFrontSide){
     *    sendCan(measure(frontSideTrigPin,frontSideEchoPin));
     *  }
     */
     intFlag=false;
  }
//
//  for(int i=0; i<aSize; i++){
//    
//    backSideArray[i]=measure(backSideTrigPin,backSideEchoPin);
//    frontSideArray[i]=measure(frontSideTrigPin,frontSideEchoPin);
//  }
//  backSide = sortArray(backSideArray);
//  frontSide = sortArray(frontSideArray);
  
}
//*****************CAN FUNCTIONS*********************************
//*****************TO DO, SET WHAT DATATYPE TO SEND***************
void sendCan(){
  
  byte data[8] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07};

  // send data:  ID = 0x100, Standard CAN Frame, Data length = 8 bytes, 'data' = array of data bytes to send
  byte sndStat = CAN0.sendMsgBuf(0x100, 0, 8, data);
  if(sndStat == CAN_OK){
    Serial.println("Message Sent Successfully!");
  } else {
    Serial.println("Error Sending Message...");
  }
  
}
//***************** TO DO, MAYBE MOVE FUNCTION AND CALL sendCan*****************
void readCan (){
  
  if(!digitalRead(CAN0_INT)) {                 // If CAN0_INT pin is low, read receive buffer
  
    CAN0.readMsgBuf(&rxId, &len, rxBuf);      // Read data: len = data length, buf = data byte(s)
    
    if((rxId & 0x80000000) == 0x80000000)     // Determine if ID is standard (11 bits) or extended (29 bits)
      sprintf(msgString, "Extended ID: 0x%.8lX  DLC: %1d  Data:", (rxId & 0x1FFFFFFF), len);
    else
      sprintf(msgString, "Standard ID: 0x%.3lX       DLC: %1d  Data:", rxId, len);
  
    Serial.print(msgString);
  
    if((rxId & 0x40000000) == 0x40000000){    // Determine if message is a remote request frame.
      sprintf(msgString, " REMOTE REQUEST FRAME");
      Serial.print(msgString);
    } else {
      for(byte i = 0; i<len; i++){
        sprintf(msgString, " 0x%.2X", rxBuf[i]);
        Serial.print(msgString);
      }
    }
        
    Serial.println();
  }
}
void flagCan(){
  intFlag = true;
}
//***************************************************************
//***************************ULTRASONIC FUNCTIONS****************
int measure(int trigPin, int echoPin){
  
  int duration, cm;

  // The sensor is triggered by a HIGH pulse of 10 or more microseconds.
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
  pinMode(trigPin, OUTPUT);
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Read the signal from the sensor: a HIGH pulse whose
  // duration is the time (in microseconds) from the sending
  // of the ping to the reception of its echo off of an object.
  pinMode(echoPin, INPUT);
  duration = pulseIn(echoPin, HIGH);

  // convert the time into a distance
  cm = microsecondsToCentimeters(duration);
  if(cm<0){
    cm = measure(trigPin, echoPin);
  }
  return cm;

}

int microsecondsToCentimeters(int microseconds){  
  // The speed of sound is 340 m/s or 29 microseconds per centimeter.
  // The ping travels out and back, so to find the distance of the
  // object we take half of the distance travelled.
  return (microseconds / 29 / 2);
}
/*******************TO DO*********************
void intTobyte(int value){

  for(int i=0; i<8; i++){
    data[i] = value%16 , HEX;
    value = value/16;
  Serial.println(data[i]);
  }
}*/

//Sorts and returns the median value of the array acting as an median filter.
//May not be used due to slowing down the system
int sortArray (int a[]){
  
  int b[aSize];
  int temp;
  
  for(int i=0; i<aSize-1; i++){
    for(int j=aSize; j<aSize; j++){
      if(a[i]<b[j])
        temp = a[i]; a[i]=b[j]; b[j]=temp;
    }
  }
  return a[2];
}
//***************************************************************
