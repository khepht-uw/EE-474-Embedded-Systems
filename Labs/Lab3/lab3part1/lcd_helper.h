#pragma once
#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Pin definitions
#define SDA_PIN 14
#define SCL_PIN 13

// LCD constants
const uint8_t LCD_ADDR = 0x27;
const uint8_t LCD_COLS = 16;
const uint8_t LCD_ROWS = 2;

// Bit definitions
const uint8_t BIT_RS = 0x01;
const uint8_t BIT_RW = 0x02;
const uint8_t BIT_EN = 0x04;
const uint8_t BIT_BL = 0x08;

// LCD modes
const uint8_t MODE_CMD  = 0;
const uint8_t MODE_DATA = BIT_RS;

// LCD commands
const uint8_t CMD_CLEAR = 0x01;
const uint8_t CMD_LINE1 = 0x80;
const uint8_t CMD_LINE2 = 0xC0;

// Function declarations
void writeExpander(uint8_t value);
void pulseEnable(uint8_t value);
void writeNibble(uint8_t nibble, bool isData);
void sendLcdByte(uint8_t value, bool isData);
void sendLcdCommand(uint8_t command);
void displaySerialLine(String line);