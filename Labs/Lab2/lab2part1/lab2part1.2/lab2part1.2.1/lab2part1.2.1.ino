// Filename: lab2part1.2.1.ino
// Author: Ephraim Tran & Benjamin Hoang
// Date: 10/22/2026
// Description: This file controls an external LED, making it turn on and off
//              1000 times using library functions and measuring how long it takes

//Version 3.0
// Author: Ephraim Tran & Benjamin Hoang
// Date: 10/22/2026
// Changes: Added startTime variable for more accurate measuring

// ================ Macros ================
#define LEDR_PIN 7

void setup() {
 // ================> TODO:
 // Set a pin as output
 pinMode(LEDR_PIN, OUTPUT);
 digitalWrite(LEDR_PIN, LOW);
 Serial.begin(9600); //V2, needed to see serial output. Make sure Tools -> USB CDC on Boot is set to "Enable" to see serial monitor output
}

void loop() {
 // ================> TODO:
 // For 1000 repetitions:
 //		Measure time to:
 //        		- Turn pin's output to HIGH
 //        		- Turn pin's output to LOW
 // Print out total time to the serial monitor
 // 1 second delay
 unsigned long startTime = micros(); //V3, measures time from right before we run the LED toggling loop
 for (int i = 0; i < 1000; i++) {
  digitalWrite(LEDR_PIN, HIGH);
  digitalWrite(LEDR_PIN, LOW);
 }
 Serial.print("Time: ");
 Serial.println(micros() - startTime); //This was consistently 1084 after initial start values
 delay(1000);
}
