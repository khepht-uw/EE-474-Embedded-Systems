// Filename: lab2part4.ino
// Author: Ephraim Tran & Benjamin Hoang
// Date: 10/23/2026
// Description: Reads ambient light with a photoresistor voltage divider (ADC).
//              When the light drops below a threshold, a one-time buzzer sequence
//              plays using LEDC PWM and a hardware timer: 500 Hz -> 1000 Hz -> 1500 Hz,
//              1 second each. Above the threshold, the buzzer stays off.

// Version 2.0
// Author: Ephraim Tran & Benjamin Hoang
// Date: 10/26/2026
// Changes: Switched from re-calling ledcAttach to ledcChangeFrequency so the pitch changes.
//          Reduced current time instantiation to a single line

// ================ Includes ================
#include "soc/timer_group_reg.h"
#include "driver/gpio.h"
#include "soc/io_mux_reg.h"
#include "soc/gpio_reg.h"
#include "soc/gpio_periph.h"

// ================ Macros ================
#define BUZZER_PIN 39
#define P_RES 11

#define freq 1000
#define resolution 8
#define read_max 4095
#define threshold 2000
#define SEGMENT_INTERVAL 10000 // 10k ticks/s. One segment = 1 s, so set to 10k

#define TIMER_INCREMENT_MODE (1 << 30)
#define TIMER_ENABLE (1U << 31) //Bit 31 (enable) = 1, use unsigned 1 since it's at the highest bit


void setup() {
  ledcAttach(BUZZER_PIN, freq, resolution); // attach once; change frequency later with ledcChangeFrequency
  ledcWrite(BUZZER_PIN, 0);                 // start silent

  uint32_t timer_config = 0;
  timer_config |= (8000 << 13); // Divider bits from 13:28, which fits 8000 (13 bits) 80MHz / 8000 = 10kHz
  timer_config |= TIMER_INCREMENT_MODE | TIMER_ENABLE; // setup the increment and enable bits
  *((volatile uint32_t *)TIMG_T0CONFIG_REG(0)) = timer_config; // write the finished configuration
  *((volatile uint32_t *)TIMG_T0UPDATE_REG(0)) = 1; // lets us read the timer
  
  Serial.begin(9600);
}

void loop() {
  uint16_t level = analogRead(P_RES);  //from 0 (min light) to 4095 (max). Less light = more R, vice versa

  *((volatile uint32_t *)TIMG_T0UPDATE_REG(0)) = 1; // latch the timer value before every read
  uint32_t current_time = *((volatile uint32_t *)TIMG_T0LO_REG(0)); // get current value from timer

  static uint32_t last_on_time = 0; // time the sequence started
  static bool started = false;      // true while the sequence is playing
  static bool done = false;         // true after one full run, blocks restart until light returns
  static int segment = 0;           // which frequency is active (0 = none), only change once per segment

  if(level < threshold){

    if(!started && !done){              // just got dark, start the timer once
      last_on_time = current_time;
      started = true;
    }

    if(started){
      if ((current_time - last_on_time) < SEGMENT_INTERVAL) { // 1st second
        if(segment != 1) {               // only change when the segment changes
          ledcChangeFrequency(BUZZER_PIN, 500, resolution); // FREQ OF 500HZ
          ledcWrite(BUZZER_PIN, 125);
          segment = 1;
        }

      } else if((current_time - last_on_time) < SEGMENT_INTERVAL * 2) { // 2nd second
        if(segment != 2) {
          ledcChangeFrequency(BUZZER_PIN, 1000, resolution); // FREQ OF 1000HZ
          ledcWrite(BUZZER_PIN, 125);
          segment = 2;
        }

      } else if((current_time - last_on_time) < SEGMENT_INTERVAL * 3){ // 3rd second
        if(segment != 3){
          ledcChangeFrequency(BUZZER_PIN, 1500, resolution);  //FREQ OF 1500HZ
          ledcWrite(BUZZER_PIN, 125);
          segment = 3;
        }

      } else { // SEQUENCE ENDS
        ledcWrite(BUZZER_PIN, 0); // TURN OFF BUZZER
        started = false;          // End instead of restarting
        done = true;              // don't replay until the light comes back
        segment = 0;
      }
    }

  } else {
    ledcWrite(BUZZER_PIN, 0);
    started = false;              // reset so the next dark period triggers a new sequence
    done = false;
    segment = 0;
  }
}