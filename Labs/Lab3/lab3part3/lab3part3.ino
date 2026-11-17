// Filename: lab3part3.ino
// Author: Ephraim Tran & Benjamin Hoang
// Date: 11/17/2026
// Description: Displays a 1-second counter on an LCD, interrupted by
//              button presses and BLE writes.

// Version 1.0
// Author: Ephraim Tran & Benjamin Hoang
// Date: 11/17/2026
// Changes: 

//INCLUDES
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "esp_timer.h"

//DEFINES
#define BUTTON_PIN 35
#define SDA_PIN 14
#define SCL_PIN 13
#define SERVICE_UUID        "da77f5d8-1d74-40f7-8af5-5f7e065d4391"
#define CHARACTERISTIC_UUID "ff13712b-d369-425f-a712-37dde2e6ab93"

//MACROS
volatile unsigned long lastInterruptTime = 0;
const unsigned long debounceDelay = 200;
volatile int counter = 0;
const uint8_t LCD_ADDR = 0x27;
const uint8_t LCD_COLS = 16;
const uint8_t LCD_ROWS = 2;
volatile bool printMessage = false;
volatile bool buttonPressed = false;

int previousCounter = -1;

LiquidCrystal_I2C lcd(LCD_ADDR, LCD_COLS, LCD_ROWS);

esp_timer_handle_t periodicTimer;


// Name: MyCallbacks::onWrite
// Description: Sets a flag when a BLE write is received.
// Parameters: pCharacteristic - written characteristic (unused)
// Returns: nothing
class MyCallbacks: public BLECharacteristicCallbacks {
   void onWrite(BLECharacteristic *pCharacteristic) {
     printMessage = true;
   }
};

// Name: onTimer
// Description: Increments the counter every second.
// Parameters: arg - unused
// Returns: nothing
void IRAM_ATTR onTimer(void* arg) {
  counter++;
}

// Name: handleButtonInterrupt
// Description: Sets a flag on a debounced button press.
// Parameters: none
// Returns: nothing
void IRAM_ATTR handleButtonInterrupt() {
  uint32_t current_time = millis();
  if(current_time - lastInterruptTime > debounceDelay){
    buttonPressed = true;
    lastInterruptTime = current_time;
  }
}

// Name: displayMessage
// Description: Shows a message on the LCD for 2 seconds, then forces a counter redraw.
// Parameters: message - text to display
// Returns: nothing
void displayMessage(String message) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(message);                  // show the message
  delay(2000);                         // hold it for 2 seconds
  previousCounter = -1;                // force loop() to redraw the counter
  printMessage = false;
  buttonPressed = false;
}

void setup() {
  Serial.begin(115200);
  // Set up BLE: create the server, service, and read/write characteristic, attach the write callback, then start advertising
  BLEDevice::init("MyESP32");
  BLEServer *pServer = BLEDevice::createServer();
  BLEService *pService = pServer->createService(SERVICE_UUID);
  BLECharacteristic *pCharacteristic = pService->createCharacteristic(
                                        CHARACTERISTIC_UUID,
                                        BLECharacteristic::PROPERTY_READ |
                                        BLECharacteristic::PROPERTY_WRITE
                                      );

  pCharacteristic->setCallbacks(new MyCallbacks());
  pService->start();
  BLEAdvertising *pAdvertising = pServer->getAdvertising();
  pAdvertising->start();

  // Initialize LCD display
  Wire.begin(SDA_PIN, SCL_PIN);
  lcd.init();
  lcd.backlight();
  
  // create a timer, attach an interrupt, set an alarm which will update the counter every second.
  const esp_timer_create_args_t timerArgs = {///
    .callback = &onTimer,
    .arg = nullptr,
    .dispatch_method = ESP_TIMER_TASK,
    .name = "periodicTimer",
    .skip_unhandled_events = true
  };
  esp_timer_create(&timerArgs, &periodicTimer); 
  esp_timer_start_periodic(periodicTimer, 1000000); // 1 second in microseconds

  // Set button pin as input and attach an interrupt
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), handleButtonInterrupt, FALLING);
}

void loop() {
  // BLE write received: show message, then discard any flags set during the delay
  if (printMessage) {
    int saved = counter;
    displayMessage("New Message!");
    counter = saved; // restore the counter value after displaying the message
  }
  // Button pressed: show message, then discard any flags set during the delay 
  else if (buttonPressed) {
    int saved = counter;
    displayMessage("Button Pressed!");
    counter = saved;
  } 
  // Counter changed (timer ticked): redraw the counter on the LCD
  else if (counter != previousCounter) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Counter: ");
    lcd.print(counter);
    previousCounter = counter; // remember what is on screen
  }
}
