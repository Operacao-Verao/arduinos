void setup() {
  Serial.begin(115200);
}

void loop() {
  if (Serial.available()) {
    char c = Serial.read();
    if (c == 'A') Serial.println("Received A");
    if (c == 'B') Serial.println("Received B");
  }
}