

const int trigPin1 = 5;
const int echoPin1 = 6;
const int trigPin2 = 8;
const int echoPin2 = 9;


void setup() 
{
  Serial.begin(9600);
  pinMode(trigPin1, OUTPUT);
  pinMode(echoPin1, INPUT);
  pinMode(trigPin2, OUTPUT);
  pinMode(echoPin2, INPUT);

}

void loop() {

  int cm1 = measure(trigPin1, echoPin1);
  int cm2 = measure(trigPin2, echoPin2);

  if(cm1 > 0 && cm2 > 0){
  Serial.print(cm1);
  Serial.print(" ; ");
  Serial.println(cm2);
  delay(100);
  }
}

int measure(int trigPin, int echoPin)
{
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

int microsecondsToCentimeters(int microseconds)
{
  // The speed of sound is 340 m/s or 29 microseconds per centimeter.
  // The ping travels out and back, so to find the distance of the
  // object we take half of the distance travelled.
  return microseconds / 29 / 2;
}
