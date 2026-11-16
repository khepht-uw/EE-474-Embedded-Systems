// Filename: lab3part1.ino
// Author: Ephraim Tran & Benjamin Hoang
// Date: 11/9/2026
// Description: 
//              

// Version 1.0
// Author: Ephraim Tran & Benjamin Hoang
// Date: 11/9/2026
// Changes: 

//INCLUDES
#include <Wire.h>
// Ensure that the LiquidCrystal I2C library by Frank de Brabander is installed in your Arduino IDE Library Manager
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2); // Initialize the LCD

//DEFINES
#define SDA_PIN 14
#define SCL_PIN 13

//MACROS
const uint8_t LCD_ADDRESS = 0x27; // I2C address of the LCD
const uint8_t LCD_COLUMNS = 16;   // number of columns of the LCD
const uint8_t LCD_ROWS = 2;       // number of rows of the LCD
const uint8_t BIT_RS = 0x01;  // 0000 0001
const uint8_t BIT_RW = 0x02;  // 0000 0010
const uint8_t BIT_EN = 0x04;  // 0000 0100
const uint8_t BIT_BL = 0x08;  // 0000 1000

const uint8_t MODE_CMD  = 0;
const uint8_t MODE_DATA = BIT_RS;
const uint8_t CMD_CLEAR = 0x01;  // clear the screen
const uint8_t CMD_LINE1 = 0x80;  // set cursor to start of line 1

void setup() {
 Serial.begin(115200);
 Wire.begin(SDA_PIN, SCL_PIN);
 lcd.init();
 delay(2);
}

void writeExpander(uint8_t value) {
  Wire.beginTransmission(LCD_ADDRESS);  // Start I2C transmission to the LCD
  Wire.write(value | BIT_BL);           // Send the value to the LCD with backlight on 
  Wire.endTransmission();               // End I2C transmission
}

// Write the value to the expander with the enable bit set
// then clear it after a short delay so it latches the value into the LCD
void pulseEnable(uint8_t value) {
  writeExpander(value | BIT_EN);  // Enable on
  delay(1);
  writeExpander(value & ~BIT_EN); // Enable off
  delay(5)
}

void writeNibble(uint8_t nibble, bool isData) {
  uint8_t value = nibble | (isData ? MODE_DATA : MODE_CMD); // if it's a data command
                                                            // use RS = 1, else use 0
  pulseEnable(value);
}

void sendLcdByte(uint8_t value, bool isData) {
  writeNibble(value & 0xF0, isData); // turn last 4 bits to 0's, so just send char's first half
  writeNibble((value << 4) & 0xF0, isData); // shift 4 bits to the left, then turn last 4 bits
                                            // to 0 to send second half of char
}

void sendLcdCommand(uint8_t command) {
  sendLcdByte(command, false); // isData set to false because we are sending a command
  if (command == CMD_CLEAR) { // give a little delay for display to clear its screen
    delay(2);
  }
}

void displaySerialLine(String line) {
  sendLcdCommand(CMD_CLEAR);
  sendLcdCommand(CMD_LINE1);

  for (uint8_t index = 0;
       index < line.length() && index < LCD_COLUMNS * LCD_ROWS;
       index++) { // increment per character from line
    if (index == LCD_COLUMNS) { // if the index reaches the last column, go to next row
      sendLcdCommand(CMD_LINE2);
    }
    sendLcdByte(line[index], true);
  }
}

void loop() {
 // ====================> TODO:
 // Write code that takes Serial input and displays it to the LCD. Do NOT use any 
 // functions from the LiquidCrystal I2C library here.
    if (Serial.available() > 0) { // if there is text in the serial monitor
    String line = Serial.readStringUntil('\n');
    if (line.endsWith("\r")) { // remove the extra character when user presses return on keyboard
      line.remove(line.length() - 1);
    }
    displaySerialLine(line);
  }
}