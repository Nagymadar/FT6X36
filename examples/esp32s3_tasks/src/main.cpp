#include <Arduino.h>
#include <Wire.h>
#include <FT6X36.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"

// https://github.com/adafruit/Adafruit_FT6206_Library/tree/master
// https://github.com/strange-v/FT6X36/tree/master


// Touch panel pins
#define TOUCH_WIRE_SPEED 400000U
#define TOUCH_INT        GPIO_NUM_21
#define TOUCH_RESET      GPIO_NUM_45
#define TOUCH_SDA        GPIO_NUM_48
#define TOUCH_SCL        GPIO_NUM_47
#define LED_PIN 0

#define TouchEvent            ( 1 << 0) //1
#define ButtonEvent           ( 1 << 1) //10

uint8_t led_state = 0;
EventGroupHandle_t eg;
FT6X36 ts(&Wire, TOUCH_INT);

void LedSwitch()
{
  if (led_state == 0) {digitalWrite(LED_PIN, HIGH); led_state = 1;}   // turn the LED on (HIGH is the voltage level)
  else {digitalWrite(LED_PIN, LOW);  led_state = 0;}   // turnHIGH the LED off by making the voltage LOW
}

void TaskProcessTouch(void *pvParameters)
{
  for (;;)
  {
    xEventGroupWaitBits(eg, TouchEvent, pdTRUE, pdTRUE, portMAX_DELAY);
    ts.processTouch();
  }
}

void TaskProcessButton(void *pvParameters)
{
  for (;;)
  {
    xEventGroupWaitBits(eg, ButtonEvent, pdTRUE, pdTRUE, portMAX_DELAY);
    Serial.println(F("Button has been pressed"));
  }
}

void IRAM_ATTR isr()
{
  BaseType_t xHigherPriorityTaskWoken;
  xEventGroupSetBitsFromISR(eg, TouchEvent, &xHigherPriorityTaskWoken);
}

void touch(TPoint p, TEvent e)
{
  if (e != TEvent::Tap)
    return;

  LedSwitch();

  if (p.x < 50 && p.y < 50) //Left top area of the screen
  {
    xEventGroupSetBits(eg, ButtonEvent);
  }

  Serial.printf("x: %d, y: %d\n", p.x, p.y);
}

void setup()
{
  Serial.begin(115200);
  Serial.println(F("Starting..."));

  // initialize digital pin LED as an output.
  pinMode(LED_PIN, OUTPUT);
    
  // Touchpanel reset
  Serial.println("Touchpanel reset");
  pinMode(TOUCH_RESET, OUTPUT);
  digitalWrite(TOUCH_RESET, LOW);
  vTaskDelay(20);
  digitalWrite(TOUCH_RESET, HIGH);
  vTaskDelay(150); // must be more than 100ms (300ms per datashet), otherwise init fail

  // initialize I2C
  Serial.println("I2C init");
  Wire.end();
  Wire.begin(TOUCH_SDA, TOUCH_SCL, TOUCH_WIRE_SPEED);

  eg = xEventGroupCreate();
  
  ts.begin();
  ts.registerIsrHandler(isr);
  ts.registerTouchHandler(touch);

  xTaskCreatePinnedToCore(TaskProcessTouch, "fTaskProcessTouch", 10000, NULL, 3, NULL, 1);
  xTaskCreatePinnedToCore(TaskProcessButton, "fTaskProcessButton", 10000, NULL, 3, NULL, 1);
  Serial.println("Setup finished");
}




void loop()
{  
  uint8_t cnt_state = 0;
  
  vTaskDelay(2000);
  Serial.println("nothing happened...");
  LedSwitch();

  /*
  cnt_state++;
  if (cnt_state >= 10) {
    Serial.println("Debug info:");
    ts.debugInfo();
    cnt_state = 0;
  }*/
}