#define LEDR_PIN 7

void setup() {
 // ================> TODO:
 // Set a pin as output
 pinMode(LEDR_PIN, OUTPUT);
 digitalWrite(LEDR_PIN, LOW);
 Serial.begin(9600);
}

void loop() {
 // ================> TODO:
 // For 1000 repetitions:
 //		Measure time to:
 //        		- Turn pin's output to HIGH
 //        		- Turn pin's output to LOW
 // Print out total time to the serial monitor
 // 1 second delay
 unsigned long startTime = micros();
 for (int i = 0; i < 1000; i++) {
  digitalWrite(LEDR_PIN, HIGH);
  digitalWrite(LEDR_PIN, LOW);
 }
 Serial.print("Time: ");
 Serial.println(micros() - startTime);
 delay(1000);
}
