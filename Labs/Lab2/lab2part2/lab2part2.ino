// Filename: lab2part2.ino
// Author: Ephraim Tran & Benjamin Hoang
// Date: 10/23/2026
// Description: This file Toggles an LED every second by polling the ESP32 hardware timer by using direct register access.

//Version 1.0
// Author: Ephraim Tran & Benjamin Hoang
// Date: 10/23/2026
// Changes: n/a

// ================ Includes ================
#include "soc/timer_group_reg.h"
#include "driver/gpio.h"
#include "soc/io_mux_reg.h"
#include "soc/gpio_reg.h"
#include "soc/gpio_periph.h"

// ================ Macros ================
#define LEDB_PIN 21
#define LED_TOGGLE_INTERVAL 10000 // 10k ticks/s. We want to toggle every s, so set to 10k
#define TIMER_INCREMENT_MODE (1 << 30) // Bit 30: timer increment
#define TIMER_ENABLE (1U << 31) //Bit 31 (enable) = 1, use unsigned 1 since it's at the highest bit

void setup() {
  PIN_FUNC_SELECT(GPIO_PIN_MUX_REG[LEDB_PIN], PIN_FUNC_GPIO); // Sets the GPIO register function to GPIO
  *((volatile uint32_t *)GPIO_ENABLE_REG) |= (1 << LEDB_PIN); // Turn GPIO 21 into an output pin, by changing to 1 

  uint32_t timer_config = 0;
  timer_config |= (8000 << 13); // Divider bits from 13:28, which fits 8000 (13 bits) 8Mhz / 8000 = 10kHz
  timer_config |= TIMER_INCREMENT_MODE | TIMER_ENABLE; // setup the increment and enable bits
  *((volatile uint32_t *)TIMG_T0CONFIG_REG(0)) = timer_config; // write the finished configuration
  *((volatile uint32_t *)TIMG_T0UPDATE_REG(0)) = 1; // lets us read the timer
}

void loop() {
  static uint32_t last_toggle_time = 0;
  uint32_t current_time = 0;
  current_time = *((volatile uint32_t *)TIMG_T0LO_REG(0)); // extract the current value from timer

  if ((current_time - last_toggle_time) >= LED_TOGGLE_INTERVAL) { // check if 10,000 ticks have passsed yet
    uint32_t gpio_out = 0;
    gpio_out = *((volatile uint32_t *)GPIO_OUT_REG); // read the current outputs of the GPIO pins
    *((volatile uint32_t *)GPIO_OUT_REG) = gpio_out ^ (1 << LEDB_PIN); // toggle the LED 
    last_toggle_time = current_time; //Update so the if statement math is correct
  }
  *((volatile uint32_t *)TIMG_T0UPDATE_REG(0)) = 1; // setup to capture for next time
}
