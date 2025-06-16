int led = 8;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("ESP32-C3 Test");
  pinMode(led, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(led, HIGH);
  delay(1000);
  digitalWrite(led, LOW);
  delay(1000);
  Serial.println("ESP32-C3 TestTTT");
}
