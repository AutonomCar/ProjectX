

const int leftTrigPin = 5;
const int leftEchoPin = 6;
const int rightTrigPin = 8;
const int rightEchoPin = 9;
const int aSize = 5;

int frontLeft;
int frontRight;

void setup() {
  
  Serial.begin(9600);
  pinMode(leftTrigPin, OUTPUT);
  pinMode(leftEchoPin, INPUT);
  pinMode(rightTrigPin, OUTPUT);
  pinMode(rightEchoPin, INPUT);

}

void loop() {

  int frontLeftArray[aSize];
  int frontRightArray[aSize];

  for(int i=0; i<aSize; i++){
    
    frontLeftArray[i]=measure(leftTrigPin,leftEchoPin);
    frontRightArray[i]=measure(rightTrigPin,rightEchoPin);
  }
  frontLeft = sortArray(frontLeftArray);
  frontRight = sortArray(frontRightArray);

  Serial.print(frontLeft); 
  Serial.print(" ; ");
  Serial.println(frontRight);
}

int measure(int trigPin, int echoPin){
  
  int duration, cm;

  // The sensor is triggered by a HIGH pulse of 10 or more microseconds.
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
  
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Read the signal from the sensor: a HIGH pulse whose
  // duration is the time (in microseconds) from the sending
  // of the ping to the reception of its echo off of an object.
  
  duration = pulseIn(echoPin, HIGH);

  cm = microsecondsToCentimeters(duration);
  
  return cm;
}

int microsecondsToCentimeters(int microseconds){
  
  // The speed of sound is 340 m/s or 29 microseconds per centimeter.
  // The ping travels out and back, so to find the distance of the
  // object we take half of the distance travelled.
  return microseconds / 29 / 2;
}

//Sorts and returns the median value of the array acting as an median filter.
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

