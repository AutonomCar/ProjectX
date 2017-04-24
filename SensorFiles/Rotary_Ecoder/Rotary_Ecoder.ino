const int rotaryPin = 3;
int count = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(rotaryPin, INPUT);
  
}

void loop() {

if(analogRead(rotaryPin)==0){
  count++;
}
  Serial.println(count);
}
