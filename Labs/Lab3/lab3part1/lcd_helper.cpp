
#include "lcd_helper.h"

// Name: writeExpander
// Description: Writes the value to the expander with the backlight bit set
// Parameters: value - the value to add the backlight bit to
// Returns: nothing
void writeExpander(uint8_t value) {
    Wire.beginTransmission(LCD_ADDR);
    Wire.write(value | BIT_BL);
    Wire.endTransmission();
}

// Name: pulseEnable
// Description: Sets and clears the enable bit to latch the value into the LCD
// Parameters: value - the value to write to the expander
// Returns: nothing
void pulseEnable(uint8_t value) {
    writeExpander(value | BIT_EN);
    delay(1);
    writeExpander(value & ~BIT_EN);
    delay(5);
}

// Name: writeNibble
// Description: Adds the RS bit if sending data, then pulses EN
// Parameters: nibble - data in the upper 4 bits
//             isData - true for a character, false for a command
// Returns: nothing
void writeNibble(uint8_t nibble, bool isData) {
    uint8_t value = nibble | (isData ? MODE_DATA : MODE_CMD);
    pulseEnable(value);
}

// Name: sendLcdByte
// Description: Sends a byte to the LCD, upper nibble first
// Parameters: value - the byte to send
//             isData - true for a character, false for a command
// Returns: nothing
void sendLcdByte(uint8_t value, bool isData) {
    writeNibble(value & 0xF0, isData);
    writeNibble((value << 4) & 0xF0, isData);
}

// Name: sendLcdCommand
// Description: Sends a command to the LCD
// Parameters: command - the command to send
// Returns: nothing
void sendLcdCommand(uint8_t command) {
    sendLcdByte(command, false);

    if (command == CMD_CLEAR) {
        delay(2);
    }
}

// Name: displaySerialLine
// Description: Displays the string passed in to the LCD, clearing the screen first
// Parameters: line - the string to display
// Returns: nothing
void displaySerialLine(String line) {
    sendLcdCommand(CMD_CLEAR);
    sendLcdCommand(CMD_LINE1);

    for (uint8_t index = 0;
         index < line.length() && index < LCD_COLS * LCD_ROWS;
         index++) {
        if (index == LCD_COLS) {
            sendLcdCommand(CMD_LINE2);
        }
        sendLcdByte(line[index], true);
    }
}