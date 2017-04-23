//int QRE1113_Pin = 0; //connected to analog 0
int penis = 7;

void setup(){
  Serial.begin(9600);
}


void loop(){

  //int QRE_Value = analogRead(QRE1113_Pin);
  int penis1 = digitalRead(penis);
  //Serial.print(QRE_Value);
  Serial.print(" , ");
  Serial.println(penis1);
}
