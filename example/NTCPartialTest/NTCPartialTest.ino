#include <GxEPD.h>
#include <GxDEPG0150BN/GxDEPG0150BN.h>    // 1.54" b/w 200x200
// #include <GxGDEH0154Z90/GxGDEH0154Z90.h>  // 1.54" b/w/r 200x200
#include <GxIO/GxIO_SPI/GxIO_SPI.h>
#include <GxIO/GxIO.h>
#include "IMG.h"
#include "NTPClient.h"
#include <WiFi.h>

#define PIN_MOTOR 4
#define PIN_KEY 35
#define PWR_EN 5
#define Backlight 33

#define SPI_SCK 14
#define SPI_DIN 13
#define EPD_CS 15
#define EPD_DC 2
#define SRAM_CS -1
#define EPD_RESET 17
#define EPD_BUSY 16

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "ntp1.aliyun.com", 60 * 60 * 8, 30 * 60 * 1000);
GxIO_Class io(SPI, /*CS=5*/ 15, /*DC=*/2, /*RST=*/17);
GxEPD_Class display(io, /*RST=*/17, /*BUSY=*/16);

const char *ssid = "";      //"your ssid";
const char *password = ""; //"your password";

void setup()
{
  Serial.begin(115200);
  delay(10);
  SPI.begin(SPI_SCK, -1, SPI_DIN, EPD_CS);
  pinMode(PIN_MOTOR, OUTPUT);
  pinMode(PWR_EN, OUTPUT);
  digitalWrite(PWR_EN, HIGH);

  digitalWrite(PIN_MOTOR, HIGH);
  delay(200);
  digitalWrite(PIN_MOTOR, LOW);
  delay(100);
  digitalWrite(PIN_MOTOR, HIGH);
  delay(200);
  digitalWrite(PIN_MOTOR, LOW);
  Serial.println("ESP32 WIFI NTC test");
  display.init();
  display.setRotation(0);
  display.fillScreen(GxEPD_WHITE);
  display.drawBitmap(0, 0, lilygo, 200, 200, GxEPD_BLACK);
  display.update();
  display.fillScreen(GxEPD_WHITE);
  display.setTextColor(GxEPD_BLACK);
  display.setTextWrap(true);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
  }
  timeClient.begin();
  display.eraseDisplay();
  display.setTextSize(2);
  display.fillScreen(GxEPD_WHITE);
  display.setCursor(0, 73);
  display.println("Beijing time :");
  display.setTextSize(3);
}

void loop()
{
  uint8_t RealTime, LastTime = 0;
  timeClient.update();
  RealTime = timeClient.getSeconds();
  if (LastTime != RealTime)
  {
    LastTime = RealTime;
    display.setCursor(28, 100);
    display.fillRect(0, 100, 200, 24, GxEPD_WHITE);
    display.print(timeClient.getFormattedTime());
    display.updateWindow(0, 0, 200, 200);
  }
}
