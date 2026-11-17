// Filename: lab3part2.ino
// Author: Ephraim Tran & Benjamin Hoang
// Date: 11/9/2026
// Description: This program is designed to simultaneously control an LED blinker, a counter on
// an I2C LCD screen, a buzzer with a note display, and an alphabet printer in the serial monitor.
// This is accomplished through Task Control Blocks that manage the execution of tasks based
// on their priority and completion status.

//Version 1.0
// Author: Ephraim Tran & Benjamin Hoang
// Date: 11/9/2026
// Changes: 

//INCLUDES
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

//DEFINES
#define SDA_PIN 14
#define SCL_PIN 13
#define LED_PIN 15
#define LED_2_PIN 45
#define BUZZER_PIN 40
#define freq 1000
#define resolution 8

//MACROS
const uint8_t LCD_ADDR = 0x27; // I2C address of the LCD
const uint8_t LCD_COLS = 16;
const uint8_t LCD_ROWS = 2;
const uint8_t BIT_RS = 0x01;  // 0000 0001
const uint8_t BIT_RW = 0x02;  // 0000 0010
const uint8_t BIT_EN = 0x04;  // 0000 0100
const uint8_t BIT_BL = 0x08;  // 0000 1000

const uint8_t MODE_CMD  = 0;
const uint8_t MODE_DATA = BIT_RS;
const uint8_t CMD_CLEAR = 0x01; // clear the screen
const uint8_t CMD_LINE1 = 0x80; // set cursor to start of line 1
const uint8_t CMD_LINE2 = 0xC0; // set cursor to start of line 2

const int NUM_TASKS = 4;

struct TCB {
  void (*func)();   // which function to run
  int priority;     // how important it is right now
  bool done;        // has it run this cycle?
  const char* name; // task name for printing
};

LiquidCrystal_I2C lcd(LCD_ADDR, LCD_COLS, LCD_ROWS); // Initialize the LCD
// 

void ledBlinker() {
  for (int i = 0; i < 8; i++) {
    digitalWrite(LED_PIN, HIGH);
    delay(1000);
    digitalWrite(LED_PIN, LOW);
    delay(1000);
  }
}

void counterTask() {
  for (int i = 1; i <= 10; i++) {
    lcd.clear();
    lcd.print(i);      // print() handles ints directly
    delay(1000);
  }
  lcd.clear();
}

void printAlphabet() {
  for (char c = 'A'; c <= 'Z'; c++) {
    if (c == 'A' + 16){
        lcd.setCursor(0, 1);
    }
    lcd.print(c);
    Serial.print(c);
    Serial.print(' ');
    delay(350);
  }
  lcd.clear();
  Serial.println();
}

void buzzerTask() {
  int notes[] = {260, 588, 392, 880, 1320, 1976, 1480, 1108, 784, 524};
  //             C4   D5   G4   A5   E6    B6    F#6   C#6   G5   C5
  ledcWrite(BUZZER_PIN, 128); // set duty cycle to 50% to make sound audible
  for (int i = 0; i < 10; i++) {
    ledcChangeFrequency(BUZZER_PIN, notes[i], resolution);
    uint8_t out = 255 * ((float)notes[i] / 494);
    ledcWrite(LED_2_PIN, out);
    delay(500);
  }
  ledcWrite(BUZZER_PIN, 0); // turn off buzzer
  ledcWrite(LED_2_PIN, 0); // turn off LED
}

TCB tasks[4] = {
  {ledBlinker, 4, false, "LED Blinker"},
  {counterTask, 3, false, "Counter"},
  {buzzerTask, 2, false, "Music Player"},
  {printAlphabet, 1, false, "Alphabet Printer"}
};


void setup() {
  Serial.begin(115200);
  Wire.begin(SDA_PIN, SCL_PIN);
  lcd.init();
  lcd.backlight();
  // delay(2);
  pinMode(LED_PIN, OUTPUT);
  ledcAttach(BUZZER_PIN, freq, resolution); // attach once; change frequency later with ledcChangeFrequency
  ledcWrite(BUZZER_PIN, 0);    
  ledcAttach(LED_2_PIN, freq, resolution);
}


// struct TCB {
//   void (*func)();   // which function to run
//   int priority;     // how important it is right now
//   bool done;        // has it run this cycle?
//   const char* name; // task name for printing
// };

void loop() {
  // Find the not-done task with the highest priority
  int highestTask = -1;
  int highestPriority = 0;
  for (int i = 0; i < NUM_TASKS; i++) {

    if (!tasks[i].done && tasks[i].priority > highestPriority) {
      highestPriority = tasks[i].priority;
      highestTask = i;
    }
  }

  if (highestTask != -1) {
    // Run it to completion (non-preemptive)
    tasks[highestTask].func();
    tasks[highestTask].done = true;

    // Print "Name: priority"
    Serial.print(tasks[highestTask].name);
    Serial.print(": ");
    Serial.println(tasks[highestTask].priority);

  } else {
    // Every task is done: reset flags and rotate priorities
    for (int i = 0; i < NUM_TASKS; i++) {
      tasks[i].done = false;
      tasks[i].priority = tasks[i].priority % 4 + 1;
    }
  }
}
