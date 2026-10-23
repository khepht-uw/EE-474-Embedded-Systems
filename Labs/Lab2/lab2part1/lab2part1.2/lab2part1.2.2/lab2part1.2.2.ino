// Filename: lab2part1.2.2.ino
// Author: Ephraim Tran & Benjamin Hoang
// Date: 10/22/2026
// Description: This file controls an external LED, making it turn on and off
//              1000 times using register manipulation and measuring how long it takes

//Version 2.0
// Author: Ephraim Tran & Benjamin Hoang
// Date: 10/22/2026
// Changes: Added includes so the register access code worked

// ================ Includes ================
#include "driver/gpio.h"
#include "soc/io_mux_reg.h"
#include "soc/gpio_reg.h"
#include "soc/gpio_periph.h"

// ================ Macros ================
#define LEDW_PIN 15

void setup() {
 // ================> TODO:
 // Set a pin as output
  PIN_FUNC_SELECT(GPIO_PIN_MUX_REG[LEDW_PIN], PIN_FUNC_GPIO);
  *((volatile uint32_t *)GPIO_ENABLE_REG) |= (1 << LEDW_PIN);
  Serial.begin(9600); //Make sure Tools -> USB CDC on Boot is set to "Enable" to see serial monitor output
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
  *((volatile uint32_t *)GPIO_OUT_REG) |= (1 << LEDW_PIN);
  *((volatile uint32_t *)GPIO_OUT_REG) &= (0);
 }
 Serial.print("Time: ");
 Serial.println(micros() - startTime);
 delay(1000);
}