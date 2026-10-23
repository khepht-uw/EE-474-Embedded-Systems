// Filename: lab2part1.1.ino
// Author: Ephraim Tran & Benjamin Hoang
// Date: 10/22/2026
// Description: This file controls an external LED, making turn on and off in one second intervals
// Version 1.0

// ============== Includes ================
#include "driver/gpio.h"
#include "soc/io_mux_reg.h"
#include "soc/gpio_reg.h"
#include "soc/gpio_periph.h"

// ============== Macros ================
#define GPIO_PIN 5 // GPIO5

void setup() {
 // This predefined macro sets the functionality of a specified pin (to UART, SPI, 
 // GPIO, etc). Here we are setting pin 5 to be a general purpose input/output 
 // pin.
  PIN_FUNC_SELECT(GPIO_PIN_MUX_REG[GPIO_PIN], PIN_FUNC_GPIO);
 
// ===============> TODO:
 // Write code here (1-3 lines) to mark pin 5 as output
 // using the GPIO_ENABLE_REG macro
  *((volatile uint32_t *)GPIO_ENABLE_REG) |= (1 << GPIO_PIN); //Enable set to 0...010000
}

void loop() {
 // ===============> TODO:
 // Turn LED on (set corresponding bit in GPIO output register)
 // Write code here (1-3 lines) to mark pin 5 as HIGH output
 // using the GPIO_OUT_REG macro
 // Wait for 1 second
  *((volatile uint32_t *)GPIO_OUT_REG) |= (1 << GPIO_PIN); //Output set to 0...010000, same as enable, so HIGH
  delay(1000);

 // ===============> TODO:
 // Turn LED off (clear corresponding bit in GPIO output register)
 // Write code here (1-3 lines) to mark pin 5 as LOW output
 // using the GPIO_OUT_REG macro
 // Wait for 1 second
 *((volatile uint32_t *)GPIO_OUT_REG) &= (0); //0...10000 AND 0 = 0, output is LOW
 delay(1000);
}
