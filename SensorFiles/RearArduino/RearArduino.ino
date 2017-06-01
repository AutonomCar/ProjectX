int count = 0;
boolean wait = true;
//*************************INITIATE CAN****************************
#include <mcp_can.h>
#include <SPI.h>

long unsigned int rxId;
unsigned char len = 0;
unsigned char rxBuf[8];
const int sizeMsg = 3;        // Sets the ammount of bytes sent on the CAN

//Defining adresses for CAN bus that RPi will listen for
const byte backSideAd = 106;
const byte frontSideAd = 107;

#define CAN0_INT 2                              // Set INT to pin 2
MCP_CAN CAN0(10);                               // Set CS to pin 10
//****************************************************************
//*************************INITIATE RANGE SENSOR******************
const int backSideTrigPin = 5;
const int backSideEchoPin = 6;
const int frontSideTrigPin = 8;
const int frontSideEchoPin = 9;

int backSide;
int frontSide;
//****************************************************************
void setup() {

  Serial.begin(115200);
  //***************************SETUP CAN****************************
  // Initialize MCP2515 running at 16MHz with a baudrate of 500kb/s and the masks and filters disabled.
  if (CAN0.begin(MCP_ANY, CAN_500KBPS, MCP_16MHZ) == CAN_OK)
    Serial.println("MCP2515 Initialized Successfully!");
  else
    Serial.println("Error Initializing MCP2515...");

  CAN0.setMode(MCP_NORMAL);                     // Set operation mode to normal so the MCP2515 sends acks to received data.

  pinMode(CAN0_INT, INPUT);                            // Configuring pin for /INT input

  Serial.println("MCP2515 Library Receive Example...");

  //***************************************************************
  //**************************SETUP RANGE SENSOR*******************
  pinMode(backSideTrigPin, OUTPUT);
  pinMode(backSideEchoPin, INPUT);
  pinMode(frontSideTrigPin, OUTPUT);
  pinMode(frontSideEchoPin, INPUT);
  //***************************************************************
  // Wait until RPI sends message on CAN-bus
  while (wait == true) {
    if (!digitalRead(CAN0_INT)) {
      wait == false;
      break;
    }
  }
}
//***************************MAIN PROGRAM*************************
void loop() {
  delay(60);
  backSide = measure(backSideTrigPin, backSideEchoPin);
  frontSide = measure(frontSideTrigPin, frontSideEchoPin);
  Serial.print("BACK SIDE SENSOR : ");
  Serial.print(backSide);
  Serial.print(" ; ADDRESS : ");
  Serial.println(backSideAd);
  Serial.print("FRONT SIDE SENSOR : ");
  Serial.print(frontSide);
  Serial.print(" ; ADDRESS : ");
  Serial.println(frontSideAd);
  sendData();
}
//*****************CAN FUNCTIONS*********************************
void sendData() {
  sendCan(backSide, backSideAd);
  sendCan(frontSide, frontSideAd);
}
void sendCan(int value, byte adress) {
  byte data[sizeMsg];

  //Simplifying having to use signed logic
  if (value < 0) {
    value = value * -1;
    data[sizeMsg - 1] = -1;
  }
  //Masks first bits of the integer and saves it in the byte array
  //then rightshifts value and saves it again
  //This is in order to make sure every package is only one byte
  for (int i = 0; i < sizeMsg - 1; i++) {
    data[i] = (byte) (value & 0xFF);
    value = value >> 8;
  }
  // send data:  ID = 0x100, Standard CAN Frame, Data length = 8 bytes, 'data' = array of data bytes to send
  byte sndStat = CAN0.sendMsgBuf(adress, 0, sizeMsg, data);

  //  if(sndStat == CAN_OK){
  //    Serial.println("Message Sent Successfully!");
  //  } else {
  //    Serial.println("Error Sending Message...");
  //  }
}
//***************************************************************
//***************************ULTRASONIC FUNCTIONS****************
int measure(int trigPin, int echoPin) {

  int duration;

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

  //Convert the time into a distance
  return microsecondsToCentimeters(duration);
}
int microsecondsToCentimeters(int microseconds) {
  // The speed of sound is 340 m/s or 29 microseconds per centimeter.
  // The ping travels out and back, so to find the distance of the
  // object we take half of the distance travelled.
  return (microseconds / 29 / 2);
}
//***************************************************************
