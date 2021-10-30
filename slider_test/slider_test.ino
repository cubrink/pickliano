const int slider1 = A0;
const int slider2 = A1;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  
}

void loop() {
  // put your main code here, to run repeatedly:
  int slider1_value = (analogRead(slider1) / 1023.0) * 127.0;
  int slider2_value = (analogRead(slider2) / 1023.0) * 127.0;
  
  Serial.println(slider1_value);
  delay(1000);
}
