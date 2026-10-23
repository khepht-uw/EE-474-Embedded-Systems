#include "driver/gpio.h"
#include "soc/io_mux_reg.h"
#include "soc/gpio_reg.h"
#include "soc/gpio_periph.h"
#define LEDR_PIN 15

void setup() {
 // ================> TODO:
 // Set a pin as output
  PIN_FUNC_SELECT(GPIO_PIN_MUX_REG[LEDR_PIN], PIN_FUNC_GPIO);
  *((volatile uint32_t *)GPIO_ENABLE_REG) |= (1 << LEDR_PIN);
  Serial.begin(9600);
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
  *((volatile uint32_t *)GPIO_OUT_REG) |= (1 << LEDR_PIN);
  *((volatile uint32_t *)GPIO_OUT_REG) &= (0);
 }
 Serial.print("Time: ");
 Serial.println(micros() - startTime);
 delay(1000);
}