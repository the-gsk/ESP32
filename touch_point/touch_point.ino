
void setup() {
  pinMode(7, INPUT);
  pinMode(D3, OUTPUT);
  Serial.begin(115200);
}

int v;
void loop() {
  v = touchRead(7);
  Serial.println(v);
  if(v > 10000){
    digitalWrite(D3, HIGH);
    Serial.println("TOUCH");
  }else{
    digitalWrite(D3, LOW);
    Serial.println("NO");
  }
}
