#include <GxEPD.h>
#include <GxDEPG0150BN/GxDEPG0150BN.h>    // 1.54" b/w 200x200
// #include <GxGDEH0154Z90/GxGDEH0154Z90.h>  // 1.54" b/w/r 200x200
#include <GxIO/GxIO_SPI/GxIO_SPI.h>
#include <GxIO/GxIO.h>
#include "IMG.h"
#include <WiFi.h>
#include "time.h"
#include "HTTPClient.h"
#include <ArduinoJson.h>
#include <AceButton.h>
#include <Adafruit_I2CDevice.h>
#include <ESP32Time.h>
#include <soc/soc.h>
#include <soc/rtc_cntl_reg.h>


#define PIN_MOTOR 4
#define PIN_KEY 35
#define PWR_EN 5
#define Backlight 33
#define Bat_ADC 34

#define SPI_SCK 14
#define SPI_DIN 13
#define EPD_CS 15
#define EPD_DC 2
#define SRAM_CS -1
#define EPD_RESET 17
#define EPD_BUSY 16

using namespace ace_button;

GxIO_Class io(SPI, /*CS*/ EPD_CS, /*DC=*/EPD_DC, /*RST=*/EPD_RESET);
GxEPD_Class display(io, /*RST=*/EPD_RESET, /*BUSY=*/EPD_BUSY);
struct tm timeinfo;

const char *ntpServer1 = "pool.ntp.org";
const char *ntpServer2 = "ntp1.aliyun.com";
const long gmtOffset_sec = 60 * 60 * 8;
const int daylightOffset_sec = 0;

const char *ssid = "xinyuan-2";      //"your ssid";
const char *password = "Xydz202104"; //"your password";

const char *host = "https://api.seniverse.com"; //心知天气APIhost
const char *apiKey = "SOLy-aC_1gUQ-v6js";       //API key 私钥
const char *city = "shenzhen";                  //查询的城市
//示例: https://api.seniverse.com/v3/weather/now.json?key=your_private_key&location=beijing&language=zh-Hans&unit=c

HTTPClient http_client;
String req;
String rsp;
StaticJsonDocument<500> doc;
AceButton button(PIN_KEY);
ESP32Time rtc;

bool SleepFlag = false;
bool WIFIUpdataFlag = false;
bool GPSUpdataFlag = false;
bool DisplayFullUpdata = true;
uint8_t RealTime = 0, LastTime = 0;
float Lat = 0, Long = 0, Speed = 0;
uint8_t Sat = 0;

void Task1code(void *pvParameters);
void Task2code(void *pvParameters);
// The event handler for the button.
void handleEvent(AceButton * /* button */, uint8_t eventType,
                 uint8_t buttonState)
{

  // Print out a message for all events.
  Serial.print(F("handleEvent(): eventType: "));
  Serial.print(eventType);
  Serial.print(F("; buttonState: "));
  Serial.println(buttonState);

  // Control the LED only for the Pressed and Released events.
  // Notice that if the MCU is rebooted while the button is pressed down, no
  // event is triggered and the LED remains off.
  switch (eventType)
  {
  case AceButton::kEventReleased:
    break;
  case AceButton::kEventPressed:
    break;
  case AceButton::kEventClicked:
    GPSUpdataFlag = !GPSUpdataFlag;
    break;
  case AceButton::kEventDoubleClicked:
    WIFIUpdataFlag = 1;
    break;
  case AceButton::kEventLongPressed:
    SleepFlag = 1;
    break;
  default:
    break;
  }
}

void printLocalTime(uint16_t x, uint16_t y)
{
  //display.fillRect(x, y, 200, 70, GxEPD_WHITE); //clean
  display.setTextColor(GxEPD_BLACK);
  display.setTextSize(2);
  display.setTextWrap(false);
  display.setCursor(x, y);
  display.print(&timeinfo, "%Y  %b  %a");
  display.setCursor(x, y + 20);
  display.setTextSize(7);
#ifdef _GxGDEH0154Z90_H_
  display.setTextColor(GxEPD_RED);
#endif
  display.printf("%02d", rtc.getHour());
  display.setCursor(x + 80, y + 20);
  display.print(":");
  display.setCursor(x + 120, y + 20);
  display.printf("%02d", rtc.getMinute());
}

void printPower(uint16_t x, uint16_t y)
{
  pinMode(Bat_ADC, ANALOG);
  adcAttachPin(Bat_ADC);
  analogReadResolution(12);
  analogSetWidth(50);
  //display.fillRect(x - 32, y, 28, 12, GxEPD_WHITE); //clean
  display.drawRect(x, y, 28, 12, GxEPD_BLACK);
  display.drawRect(x + 28, y + 3, 3, 6, GxEPD_BLACK);
  int bat = 0;
  for (uint8_t i = 0; i < 20; i++)
  {
    bat += analogRead(Bat_ADC);
  }
  bat /= 20;
  float volt = (bat * 3.3 / 4096) * 2;
  volt = bat > 2600 ? 1 : volt / 4.2;
  display.fillRect(x + 2, y + 2, 24 * volt, 8, GxEPD_BLACK);
  display.setTextColor(GxEPD_BLACK);
  display.setTextSize(1);
  display.setCursor(x - 26, y + 4);
  display.print((int)(volt * 100));
  display.print("%");
}

void printWeather(uint16_t x, uint16_t y)
{
  const char *name = doc["results"][0]["location"]["name"];
  const char *weather = doc["results"][0]["now"]["text"];
  const char *temperature = doc["results"][0]["now"]["temperature"];
  //display.fillRect(x, y, 200, 44, GxEPD_WHITE); //clean
  display.setTextColor(GxEPD_BLACK);
  display.setTextSize(2);
  display.setCursor(x, y);
  display.println(name);

  display.setCursor(x + 160, y + 10);
  display.print(temperature);
  display.println("C");

  display.setTextSize(3);
  display.setCursor(x, y + 20);
  display.println(weather);
}

void setUpHttpClient()
{
  req = (String)host + "/v3/weather/now.json?key=";
  req += apiKey;
  req += "&location=";
  req += city;
  req += "&language=en&unit=c";
  Serial.println(req);
  if (http_client.begin(req))
  {
    Serial.println("HTTPclient setUp done!");
  }
}

void WIFIUpdataTime(bool EN)
{
  if (EN == true)
  {
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED)
    {
      Serial.print(".");
      delay(500);
    }

    //Synchronize RTC time
    while (!getLocalTime(&timeinfo))
    {
      //init and get the time
      configTime(gmtOffset_sec, daylightOffset_sec, ntpServer2, ntpServer1);
      Serial.println("Failed to obtain time , try again");
    }
    Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
    rtc.setTime(timeinfo.tm_sec, timeinfo.tm_min, timeinfo.tm_hour, timeinfo.tm_mday, timeinfo.tm_mon, timeinfo.tm_year + 1900);

    setUpHttpClient();

    int http_code = http_client.GET();
    if (http_code == HTTP_CODE_OK)
    {
      rsp = http_client.getString();
      //Serial.println(rsp);
      DeserializationError error = deserializeJson(doc, rsp);
      // Test if parsing succeeds.
      if (error)
      {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.f_str());
        return;
      }
    }
    //disconnect WiFi as it's no longer needed
    WiFi.disconnect(true);
    WiFi.mode(WIFI_OFF);
  }
}

void LogoDispaly(uint8_t x, uint8_t y)
{
  display.drawBitmap(x, y, logo, 200, 65, GxEPD_BLACK);
}

void setup()
{
  Serial.begin(115200);
  delay(10);
  SPI.begin(SPI_SCK, -1, SPI_DIN, EPD_CS);
  pinMode(PIN_MOTOR, OUTPUT);
  pinMode(PWR_EN, OUTPUT);
  digitalWrite(PWR_EN, HIGH);
  pinMode(PIN_KEY, INPUT_PULLUP);

  digitalWrite(PIN_MOTOR, HIGH);
  delay(100);
  digitalWrite(PIN_MOTOR, LOW);
  delay(50);
  digitalWrite(PIN_MOTOR, HIGH);
  delay(100);
  digitalWrite(PIN_MOTOR, LOW);

  Serial.println("ESP32 Watch Demo");
  display.init();
  display.setRotation(0);
  display.fillScreen(GxEPD_WHITE);
  display.drawBitmap(0, 0, lilygo, 200, 200, GxEPD_BLACK);
  display.update();
  display.fillScreen(GxEPD_WHITE);
  WIFIUpdataTime(true);
  //display.eraseDisplay();

  // Configure the ButtonConfig with the event handler, and enable all higher
  // level events.
  ButtonConfig *buttonConfig = button.getButtonConfig();
  buttonConfig->setEventHandler(handleEvent);
  buttonConfig->setFeature(ButtonConfig::kFeatureClick);
  buttonConfig->setFeature(ButtonConfig::kFeatureDoubleClick);
  buttonConfig->setFeature(ButtonConfig::kFeatureLongPress);

  // Check if the button was pressed while booting
  if (button.isPressedRaw())
  {
    Serial.println(F("setup(): button was pressed while booting"));
  }
  Serial.println(F("setup(): ready"));

  xTaskCreate(Task1code, "Task1", 10000, NULL, 1, NULL);
  xTaskCreate(Task2code, "Task2", 10000, NULL, 2, NULL);
}

void loop()
{
  //Sleep
  if (SleepFlag)
  {
    SleepFlag = false;
    display.setTextSize(2);
    display.setCursor(130, 184);
    display.print("Sleep");
    display.updateWindow(0, 0, GxEPD_WIDTH, GxEPD_HEIGHT, false);
    display.powerDown();
    while (digitalRead(PIN_KEY) == LOW)
      ;
    esp_sleep_enable_ext0_wakeup(GPIO_NUM_35, LOW); //1 = High, 0 = Low
    esp_deep_sleep_start();
  }
}

void Task1code(void *parameter)
{
  while (1)
  {
    button.check();
    vTaskDelay(10);
  }
  Serial.println("Ending task 1");
  vTaskDelete(NULL);
}

void Task2code(void *parameter)
{
  while (1)
  {
    getLocalTime(&timeinfo, 10);
    RealTime = timeinfo.tm_min;
    //if (LastTime != RealTime)
    {
      LastTime = RealTime;
      display.fillScreen(GxEPD_WHITE);
      WIFIUpdataTime(WIFIUpdataFlag);
      printWeather(0, 15);
      printPower(165, 5);
      printLocalTime(0, 65);
      //GPSUpdataTime(0, 135, GPSUpdataFlag);
      LogoDispaly(0, 135);
      if (DisplayFullUpdata == false)
      {
        display.updateWindow(0, 0, GxEPD_WIDTH, GxEPD_HEIGHT, false);
      }
      else
      {
        display.update();
        DisplayFullUpdata = false;
      }
      //display.powerDown();
    }
    vTaskDelay(10);
  }
  Serial.println("Ending task 2");
  vTaskDelete(NULL);
}