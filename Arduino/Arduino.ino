const int BAUD = 115200; 

void setup() {
  Serial.begin(9600);
  Serial1.begin(BAUD);
}

void loop() {
  Serial.write('.');
  Serial1.write("SALUT\n");
 
  if (Serial1.available()) {
    
  }
}