boolean wait = true;
int count = 0;
//*************************INITIATE CAN******************************
int speedV[] = {0, 0};
float timeKeeper = millis();
#include <mcp_can.h>
#include <SPI.h>

long unsigned int rxId;
unsigned char len = 0;
unsigned char rxBuf[8];
const int sizeMsg = 3;                      // Sets the ammount of bytes sent on the CAN

//Defining adresses for CAN bus that RPi will listen for
const byte frontUltAd = 101;
const byte frontRUltAd = 102;
const byte leftIRAd = 103;
const byte rightIRAd = 104;
const byte crossLineAd = 105;
const byte motoAdd = 140;

#define CAN0_INT 2                              // Set INT to pin 2
MCP_CAN CAN0(10);                               // Set CS to pin 10
//*******************************************************************
/*
  //************************INITIATE MPU9250***************************
  #include <MPU9250.h>
  #include <quaternionFilters.h>

  //#define SerialDebug true  // Set to true to get Serial output for debugging

  const int intPinI2C = 12; //Can be changed to 2 and 3 as external interrupts

  MPU9250 myIMU;
  //*******************************************************************
*/
//*************************INITIATE RANGE SENSOR*********************
const int frontTrigPin = 8;
const int frontEchoPin = 9;
const int frontRightTrigPin = 5;
const int frontRightEchoPin = 6;
const int aSize = 5;

int front;
int frontRight;
//*******************************************************************
//************************INITIATE IR SENSOR*************************
const int leftIRPin = 1;
const int rightIRPin = 0;
int leftIR;
int rightIR;
int crossLine = 0;

// Threshold values needs to be set according to sensor values on the course
const int threshValL = 600;
const int threshValR = 420;
//*******************************************************************
//***************************SETUP***********************************
void setup() {
  Serial.begin(115200);
  /*
    //************************SETUP MPU9250******************************
    Wire.begin();

    //Set up the interrupt pin, its set as active high, push-pull
    pinMode(intPinI2C, INPUT);
    digitalWrite(intPinI2C, LOW);


    // Read the WHO_AM_I register, this is a good test of communication
    byte c = myIMU.readByte(MPU9250_ADDRESS, WHO_AM_I_MPU9250);
    Serial.print("MPU9250 "); Serial.print("I AM "); Serial.print(c, HEX);
    Serial.print(" I should be "); Serial.println(0x71, HEX);

    if (c == 0x71){ // WHO_AM_I should always be 0x68
      Serial.println("MPU9250 is online...");


      // Start by performing self test and reporting values
      myIMU.MPU9250SelfTest(myIMU.selfTest);

      Serial.print("x-axis self test: acceleration trim within : ");
      Serial.print(myIMU.selfTest[0], 1); Serial.println("% of factory value");
      Serial.print("y-axis self test: acceleration trim within : ");
      Serial.print(myIMU.selfTest[1], 1); Serial.println("% of factory value");
      Serial.print("z-axis self test: acceleration trim within : ");
      Serial.print(myIMU.selfTest[2], 1); Serial.println("% of factory value");
      Serial.print("x-axis self test: gyration trim within : ");
      Serial.print(myIMU.selfTest[3], 1); Serial.println("% of factory value");
      Serial.print("y-axis self test: gyration trim within : ");
      Serial.print(myIMU.selfTest[4], 1); Serial.println("% of factory value");
      Serial.print("z-axis self test: gyration trim within : ");
      Serial.print(myIMU.selfTest[5], 1); Serial.println("% of factory value");

      // Calibrate gyro and accelerometers, load biases in bias registers
      myIMU.calibrateMPU9250(myIMU.gyroBias, myIMU.accelBias);

      // Initialize device for active mode read of acclerometer, gyroscope, and
      // temperature
      myIMU.initMPU9250();
      Serial.println("MPU9250 initialized for active data mode....");

      // Read the WHO_AM_I register of the magnetometer, this is a good test of
      // communication
      byte d = myIMU.readByte(AK8963_ADDRESS, WHO_AM_I_AK8963);
      Serial.print("AK8963 "); Serial.print("I AM "); Serial.print(d, HEX);
      Serial.print(" I should be "); Serial.println(0x48, HEX);

      // Get magnetometer calibration from AK8963 ROM
        myIMU.initAK8963(myIMU.factoryMagCalibration);
      // Initialize device for active mode read of magnetometer
        Serial.println("AK8963 initialized for active data mode....");
        Serial.println("Calibration values: ");
        Serial.print("X-Axis sensitivity adjustment value ");
        Serial.println(myIMU.factoryMagCalibration[0], 2);
        Serial.print("Y-Axis sensitivity adjustment value ");
        Serial.println(myIMU.factoryMagCalibration[1], 2);
        Serial.print("Z-Axis sensitivity adjustment value ");
        Serial.println(myIMU.factoryMagCalibration[2], 2);

    }
  */
  //*******************************************************************
  //***************************SETUP CAN*******************************
  // Initialize MCP2515 running at 16MHz with a baudrate of 500kb/s and the masks and filters disabled.
  if (CAN0.begin(MCP_ANY, CAN_500KBPS, MCP_16MHZ) == CAN_OK)
    Serial.println("MCP2515 Initialized Successfully!");
  else
    Serial.println("Error Initializing MCP2515...");

  CAN0.setMode(MCP_NORMAL);                     // Set operation mode to normal so the MCP2515 sends acks to received data.

  pinMode(CAN0_INT, INPUT);                     // Configuring pin for /INT input

  Serial.println("MCP2515 Library Receive Example...");
  //*******************************************************************
  //**************************SETUP RANGE SENSOR***********************
  pinMode(frontTrigPin, OUTPUT);
  pinMode(frontEchoPin, INPUT);
  pinMode(frontRightTrigPin, OUTPUT);
  pinMode(frontRightEchoPin, INPUT);
  //*******************************************************************
  //************************SETUP IR SENSOR****************************
  pinMode(leftIRPin, INPUT);
  pinMode(rightIRPin, INPUT);
  //*******************************************************************
  // Wait until RPI sends message on CAN-bus
  // This is to ensure that the RPi is ready to receive data
  while (wait == true) {
    if (!digitalRead(CAN0_INT)) {
      wait == false;
      break;
    }
  }
}
//***************************MAIN PROGRAM****************************
void loop() {
  /*
    //**********************READ MPU 9250 DATA*********************
    // If intPin goes high, all data registers have new data
    // On interrupt, check if data ready interrupt
    if (myIMU.readByte(MPU9250_ADDRESS, INT_STATUS) & 0x01) {
      updateData();
    }
  */
  delay(60);
  //Testing sending data between every sensor read to speed up updating to RPi
  front = measure(frontTrigPin, frontEchoPin);
  frontRight = measure(frontRightTrigPin, frontRightEchoPin);

  if (front <= 30) {
    //Serial.println("STOP");
    sendCan(2, motoAdd);
  }

//  Serial.print("LEFT : ");
//  Serial.print(analogRead(leftIRPin));
//  Serial.print(" || RIGHT : ");
//  Serial.println(analogRead(rightIRPin));
//  Serial.print("FRONT U : ");
//  Serial.print(front);
//  Serial.print(" || FRONT RIGHT U : ");
//  Serial.println(frontRight);

  if (analogRead(leftIRPin) < threshValL) {
    leftIR = 1;
    Serial.println("LEFT FOUND");
  } else {
    leftIR = 0;
  }

  if (analogRead(rightIRPin) < threshValR) {
    rightIR = 1;
    Serial.println("RIGHT FOUND");
  } else {
    rightIR = 0;
  }
  if (leftIR == 1 && rightIR == 1) {
    crossLine = 1;
  }
  sendData();
  //  if (count == 25) {
  //    sendData();
  //    count = 0;
  //  }
  //  count++;

  /* ******* MAYBE SOLVABLE ******************
    timeKeeper = (millis() - timeKeeper);
    speedV[0]=(myIMU.ax*9.81);
    speedV[1]=(myIMU.ay*9.81);
    Serial.print("Speed X-Axis : ");
    Serial.println(speedV[0]*timeKeeper);
    Serial.print("Speed Y-Axis : ");
    Serial.println(speedV[1]*timeKeeper);
    Serial.println((int)myIMU.gz*(timeKeeper));
    delay(100);

  */
}
//*******************************************************************
//*********************CAN FUNCTIONS*********************************
void sendData() {
  Serial.println("SENT");
  sendCan(front, frontUltAd);
  //Serial.println(front);
  sendCan(frontRight, frontRUltAd);
  //Serial.println(frontRight);
  sendCan(leftIR, leftIRAd);
//  Serial.println(leftIR);
  sendCan(rightIR, rightIRAd);
//  Serial.println(rightIR);
  sendCan(crossLine, crossLineAd);
  crossLine = 0;
}
void sendCan(int value, byte adress) {
  byte data[sizeMsg] = {0, 0, 0};
//  Serial.print(" CAN adress : ");
//  Serial.print(adress);
//  Serial.print(" ; Value : ");
//  Serial.println(value);
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
  /*
    //Debug data sent on CAN-bus
    Serial.println(adress);
    Serial.println(data[0]);
    Serial.println(data[1]);
    Serial.println(data[2]);
  */
  // Send data format:  ID = adress, Standard CAN Frame, Data length = 8 bytes, 'data' = array of data bytes to send
  byte sndStat = CAN0.sendMsgBuf(adress, 0, sizeMsg, data);

  //  if(sndStat == CAN_OK){
  //    Serial.println("Message Sent Successfully!");
  //  } else {
  //    Serial.println("Error Sending Message...");
  //  }
}
//***************** TO DO, MAYBE MOVE FUNCTION AND CALL sendCan()*****************
void readCan () {

  CAN0.readMsgBuf(&rxId, &len, rxBuf);      // Read data: len = data length, buf = data byte(s)
  //*******************SERIAL DEBUGGER*********************************
  //    if((rxId & 0x80000000) == 0x80000000)     // Determine if ID is standard (11 bits) or extended (29 bits)
  //      sprintf(msgString, "Extended ID: 0x%.8lX  DLC: %1d  Data:", (rxId & 0x1FFFFFFF), len);
  //    else
  //      sprintf(msgString, "Standard ID: 0x%.3lX       DLC: %1d  Data:", rxId, len);
  //
  //    Serial.print(msgString);
  //
  //    if((rxId & 0x40000000) == 0x40000000){    // Determine if message is a remote request frame.
  //      sprintf(msgString, " REMOTE REQUEST FRAME");
  //      Serial.print(msgString);
  //    } else {
  //      for(byte i = 0; i<len; i++){
  //        sprintf(msgString, " 0x%.2X", rxBuf[i]);
  //        //Serial.print(msgString);
  //      }
  //    }
  //Serial.println();
}
//*******************************************************************
/*
  //****************************MPU 9250 FUNCTIONS*********************
  void updateData (){

    myIMU.readAccelData(myIMU.accelCount);  // Read the x/y/z adc values
    myIMU.getAres();

    // Now we'll calculate the accleration value into actual g's
    // This depends on scale being set
    myIMU.ax = (float)myIMU.accelCount[0] * myIMU.aRes; // - accelBias[0];
    myIMU.ay = (float)myIMU.accelCount[1] * myIMU.aRes; // - accelBias[1];
    //myIMU.az = (float)myIMU.accelCount[2] * myIMU.aRes; // - accelBias[2];

    myIMU.readGyroData(myIMU.gyroCount);  // Read the x/y/z adc values
    myIMU.getGres();

    // Calculate the gyro value into actual degrees per second
    // This depends on scale being set
    //myIMU.gx = (float)myIMU.gyroCount[0] * myIMU.gRes;
    //myIMU.gy = (float)myIMU.gyroCount[1] * myIMU.gRes;
    myIMU.gz = (float)myIMU.gyroCount[2] * myIMU.gRes;


      myIMU.tempCount = myIMU.readTempData();  // Read the adc values
      myIMU.temperature = ((float) myIMU.tempCount) / 333.87 + 21.0;  // Temperature in degrees Centigrade

    //**************MAY HAVE TO MOVE****************
    // Must be called before updating quaternions!
    myIMU.updateTime();

    // Sensors x (y)-axis of the accelerometer is aligned with the y (x)-axis of
    // the magnetometer; the magnetometer z-axis (+ down) is opposite to z-axis
    // (+ up) of accelerometer and gyro! We have to make some allowance for this
    // orientationmismatch in feeding the output to the quaternion filter. For the
    // MPU-9250, we have chosen a magnetic rotation that keeps the sensor forward
    // along the x-axis just like in the LSM9DS0 sensor. This rotation can be
    // modified to allow any convenient orientation convention. This is ok by
    // aircraft orientation standards! Pass gyro rate as rad/s
    //  MadgwickQuaternionUpdate(ax, ay, az, gx*PI/180.0f, gy*PI/180.0f, gz*PI/180.0f,  my,  mx, mz);

    MahonyQuaternionUpdate(myIMU.ax, myIMU.ay, myIMU.az, myIMU.gx * DEG_TO_RAD,
                           myIMU.gy * DEG_TO_RAD, myIMU.gz * DEG_TO_RAD, myIMU.my,
                           myIMU.mx, myIMU.mz, myIMU.deltat);
  }
  //*******************************************************************
*/
//***************************ULTRASONIC FUNCTIONS********************
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
//*******************************************************************

