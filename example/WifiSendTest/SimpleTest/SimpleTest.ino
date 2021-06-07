#include "Adafruit_EPD.h"//https://github.com/adafruit/Adafruit_EPD
#include "IMG.h"

#define PIN_MOTOR 4
#define PIN_KEY GPIO_NUM_35
#define PWR_EN 5

#define SPI_SCK 14
#define SPI_DIN 13

#define EPD_CS 15
#define EPD_DC 2
#define SRAM_CS -1
#define EPD_RESET 17
#define EPD_BUSY 16

/// Uncomment the following line if you are using 1.54" EPD with SSD1608
Adafruit_SSD1681 display(200, 200, EPD_DC, EPD_RESET, EPD_CS, SRAM_CS, EPD_BUSY);

void DeepSleep(void)
{
  display.powerDown();
  pinMode(PIN_MOTOR, INPUT_PULLDOWN);
  pinMode(PWR_EN, INPUT);
  pinMode(PIN_KEY, INPUT);
  pinMode(SPI_SCK, INPUT);
  pinMode(SPI_DIN, INPUT);
  pinMode(EPD_CS, INPUT);
  pinMode(EPD_DC, INPUT);
  pinMode(EPD_RESET, INPUT);
  pinMode(EPD_BUSY, INPUT);
  esp_sleep_enable_ext0_wakeup(PIN_KEY, 0);
  esp_deep_sleep_start();
}

void setup()
{
  Serial.begin(115200);
  delay(10);
  Serial.println("ESP32 EPD Simple Test");
  SPI.begin(SPI_SCK, -1, SPI_DIN, EPD_CS);
  pinMode(PIN_MOTOR,OUTPUT);

  digitalWrite(PIN_MOTOR,HIGH);
  delay(200);
  digitalWrite(PIN_MOTOR,LOW);
  delay(100);
  digitalWrite(PIN_MOTOR,HIGH);
  delay(200);
  digitalWrite(PIN_MOTOR,LOW);
  
  display.begin();
  display.setRotation(1);
  display.invertDisplay(true);
  display.clearBuffer();
  display.drawBitmap(0, 0, lilygo, 200, 200, EPD_BLACK);
  display.display();
  delay(3000);
  
  DeepSleep();
}

void loop()
{
}
