//Key as hex: 6db6d7dbe90d03331d12b51a4af714ea2b772eecba34adc818

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.write("6db6d7dbe90d03331d12b51a4af714ea2b772eecba34adc818");
  delay(600000);
}
