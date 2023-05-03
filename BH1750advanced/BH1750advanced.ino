const int FAN_PIN = D3;
void setup(){
  Serial.begin(9600);
  pinMode(FAN_PIN, OUTPUT);

}
void loop(){
  digitalWrite(FAN_PIN, HIGH);
}