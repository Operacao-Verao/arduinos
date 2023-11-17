void setup() {
  Serial.begin(115200);
}

void loop() {
  Serial.print("A");
  delay(1000);
  Serial.print("B");
  delay(1000);
}