// Filename: lab2part4.ino
// Author: Ephraim Tran & Benjamin Hoang
// Date: 10/23/2026
// Description:  ...

//Version 1.0
// Author: Ephraim Tran & Benjamin Hoang
// Date: 10/26/2026
// Changes: 

// ================ Includes ================

// ================ Macros ================
#define BUZZER_PIN 39
#define P_RES 11
#define freq 1000
#define read_max 4095


void setup() {
  // put your setup code here, to run once:
  ledcAttach(BUZZER_PIN, freq, 8);  //Configures the PWM signal on the specified GPIO pin.
                                    //pin: GPIO pin number to output the PWM signal.
                                    //freq: Frequency of the PWM signal in Hertz.
                                    //res: Resolution of the PWM signal (1-14 bits).

  //ledcWrite(LEDY_PIN, 50);  //Sets the duty cycle for the PWM signal on the specified pin
                            //pin: GPIO pin number to write the duty cycle to.
                            //duty: Duty cycle to set (from 0 to 2res-1).
  Serial.begin(9600);


}

void loop() {
  // put your main code here, to run repeatedly:
  // uint16_t level = analogRead(P_RES);  //reads V on analog input pin, returns digital value corresponding to that V
  //               //This ranges from 0 to 4095 depending on the microcontroller.
  //               //Represents the analog signal's intensity.
  // //Less light = more res, more light = less res

  // if(level < 2000){



  // }else{
  //   ledcWrite(BUZZER_PIN, 0);

  // }
    // ledcWrite(BUZZER_PIN, 0);
     //delay(1000);
    //  ledcWrite(BUZZER_PIN, 255);
    //  delay(1000);
    // ledcWrite(BUZZER_PIN, 0);
    //  delay(1000);
    ledcWrite(BUZZER_PIN, 100);
     delay(1000);


}
