// Filename: lab3part1.ino
// Author: Ephraim Tran & Benjamin Hoang
// Date: 11/9/2026
// Description: Takes the input typed in from the serial monitor and displays it on
// the LCD using the I2C protocol to communicate.           

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
const uint8_t CMD_LINE2 = 0xC0;

void setup() {
 Serial.begin(115200);
 Wire.begin(SDA_PIN, SCL_PIN);
 lcd.init();
 delay(2);
}

// Name: writeExpander
// Description: Writes the value to the expander with the backlight bit set
// Parameters: value - the value to add the backlight bit to
// Returns: nothing
void writeExpander(uint8_t value) {
  Wire.beginTransmission(LCD_ADDRESS);
  Wire.write(value | BIT_BL);
  Wire.endTransmission();
}

// Name: pulseEnable
// Description: Writes the value to the expander with the enable bit set
// then clears it after a short delay so it latches the value into the LCD
// Parameters: value - the value to write to the expander
// Returns: nothing
void pulseEnable(uint8_t value) {
  writeExpander(value | BIT_EN);
  delay(1);
  writeExpander(value & ~BIT_EN);
  delay(5);
}

// Name: writeNibble
// Description: Adds the RS bit to a 4-bit nibble if it's classified as data
// (or leaves it alone if it's a command) and pulses EN so the LCD latches it.
// Parameters: nibble - data in the upper 4 bits;
//             isData - true for a character, false for a command
// Returns: nothing
void writeNibble(uint8_t nibble, bool isData) {
  uint8_t value = nibble | (isData ? MODE_DATA : MODE_CMD);
  pulseEnable(value);
}

// Name: sendLcdByte
// Description: Sends a byte to the LCD by sending the upper nibble first, then the lower nibble
// Parameters: value - the byte to send;
//             isData - true for a character, false for a command
// Returns: nothing
void sendLcdByte(uint8_t value, bool isData) {
  writeNibble(value & 0xF0, isData); // turn last 4 bits to 0's, so just send char's first half
  writeNibble((value << 4) & 0xF0, isData); // shift 4 bits to the left, then turn last 4 bits
//                                             to 0 to send second half of char
}
// Name: sendLcdCommand
// Description: Sends a command to the LCD and gives a delay if the command is to clear the screen
// Parameters: command - the command to send
// Returns: nothing
void sendLcdCommand(uint8_t command) {
  sendLcdByte(command, false); // isData set to false because we are sending a command
  if (command == CMD_CLEAR) { // give a little delay for display to clear its screen
    delay(2);
  }
}

//Name: displaySerialLine
//Description: Displays the string passed in to the LCD, clearing the screen first
//Parameters: line - the string to display
//Returns: nothing
void displaySerialLine(String line) {
  sendLcdCommand(CMD_CLEAR);
  sendLcdCommand(CMD_LINE1);
  for (uint8_t index = 0;
      index < line.length() && index < LCD_COLUMNS * LCD_ROWS;
      index++) {
    if (index == LCD_COLUMNS) {   // If the index reaches the last column,
      sendLcdCommand(CMD_LINE2);  // go to next row
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
    displaySerialLine(line);
  }
}