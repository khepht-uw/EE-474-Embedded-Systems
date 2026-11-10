// Filename: lab3part3.ino
// Author: Ephraim Tran & Benjamin Hoang
// Date: 11/9/2026
// Description: 
//              

// Version 1.0
// Author: Ephraim Tran & Benjamin Hoang
// Date: 11/9/2026
// Changes: 

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2); // Initialize the LCD
void setup() {
 Serial.begin(115200);
 Wire.begin();
 lcd.init();
 delay(2);
}


void loop() {

}
