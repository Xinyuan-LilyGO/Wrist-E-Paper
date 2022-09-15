#include <GxEPD.h>
#include <GxDEPG0150BN/GxDEPG0150BN.h>    // 1.54" b/w 200x200
// #include <GxGDEH0154Z90/GxGDEH0154Z90.h>  // 1.54" b/w/r 200x200
#include <GxIO/GxIO_SPI/GxIO_SPI.h>
#include <GxIO/GxIO.h>
#include <WiFi.h>
#include "IMG.h"
#include "buff.h"    // POST request data accumulator
#include "scripts.h" // JavaScript code
#include "css.h"     // Cascading Style Sheets
#include "html.h"    // HTML page of the tool

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

/* Server and IP address ------------------------------------------------------*/
WiFiServer server(80); // Wifi server exemplar using port 80
IPAddress myIP;        // IP address in your local wifi net
/* The 'index' page flag ------------------------------------------------------*/
bool isIndexPage = true; // true : GET  request, client needs 'index' page;

const char *ssid = "xinyuan-2";      //"your ssid";
const char *password = "Xydz202104"; //"your password";

int EPD_dispIndex; // The index of the e-Paper's type
uint8_t EPD_Image[5000] = {0};
uint16_t EPD_Image_count = 0;
GxIO_Class io(SPI, /*CS=*/ 15, /*DC=*/2, /*RST=*/17);
GxEPD_Class display(io, /*RST=*/17, /*BUSY=*/16);

void setup()
{
  Serial.begin(115200);
  delay(10);
  Serial.println("ESP32 Send Image test");
  SPI.begin(SPI_SCK, -1, SPI_DIN, EPD_CS);
  pinMode(PIN_MOTOR, OUTPUT);
  pinMode(PWR_EN,OUTPUT);

  digitalWrite(PWR_EN,HIGH);
  digitalWrite(PIN_MOTOR, HIGH);
  delay(200);
  digitalWrite(PIN_MOTOR, LOW);
  delay(100);
  digitalWrite(PIN_MOTOR, HIGH);
  delay(200);
  digitalWrite(PIN_MOTOR, LOW);

  display.init();
  display.setRotation(0);
  display.setTextColor(GxEPD_BLACK);
  display.fillScreen(GxEPD_WHITE);
  display.drawBitmap(0, 0, lilygo, 200, 200, GxEPD_BLACK);
  display.update();
  delay(3000);

  display.fillScreen(GxEPD_WHITE);
  display.setTextSize(2);
  display.setCursor(0, 0);
  display.print("Waitting Connect");
  display.update();

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    display.print(".");
    delay(100);
  }
  // Start the server
  server.begin();
  Serial.println("Server started");

  // Show obtained IP address in local Wifi net
  Serial.println(myIP = WiFi.localIP());

  display.fillScreen(GxEPD_WHITE);
  display.setCursor(0, 100);
  display.print(myIP);
  display.update();
}

void loop()
{
  Srvr__loop();
}
/* The server state observation loop -------------------------------------------*/
bool Srvr__loop()
{
  // Looking for a client trying to connect to the server
  WiFiClient client = server.available();

  // Exit if there is no any clients
  if (!client)
    return false;
  // Print log message: the start of request processing
  Serial.print("<<<");

  // Waiting the client is ready to send data
  while (!client.available())
    delay(1);

  // Set buffer's index to zero
  // It means the buffer is empty initially
  Buff__bufInd = 0;

  // While the stream of 'client' has some data do...
  while (client.available())
  {
    // Read a character from 'client'
    int q = client.read();

    // Save it in the buffer and increment its index
    Buff__bufArr[Buff__bufInd++] = (byte)q;

    // If the carachter means the end of line, then...
    if ((q == 10) || (q == 13))
    {
      // Clean the buffer
      Buff__bufInd = 0;
      continue;
    }

    // Requests of files
    if (Buff__bufInd >= 11)
    {
      if (Buff__signature(Buff__bufInd - 11, "/styles.css"))
        return Srvr__file(client, 0, "styles.css");

      if (Buff__signature(Buff__bufInd - 11, "/scriptA.js"))
        return Srvr__file(client, 1, "scriptA.js");

      if (Buff__signature(Buff__bufInd - 11, "/scriptB.js"))
        return Srvr__file(client, 2, "scriptB.js");

      if (Buff__signature(Buff__bufInd - 11, "/scriptC.js"))
        return Srvr__file(client, 3, "scriptC.js");

      if (Buff__signature(Buff__bufInd - 11, "/scriptD.js"))
        return Srvr__file(client, 4, "scriptD.js");
    }

    // If the buffer's length is larger, than 4 (length of command's name), then...
    if (Buff__bufInd > 4)
    {
      // It is probably POST request, no need to send the 'index' page
      isIndexPage = false;

      // e-Paper driver initialization
      if (Buff__signature(Buff__bufInd - 4, "EPD"))
      {
        Serial.print("\r\nEPD\r\n");
        // Getting of e-Paper's type

        EPD_dispIndex = (int)Buff__bufArr[Buff__bufInd - 1] - (int)'a';
        Serial.println(EPD_dispIndex);
        if (EPD_dispIndex < 0)
        {
          EPD_dispIndex = (int)Buff__bufArr[Buff__bufInd - 1] - (int)'A' + 26;
          Serial.println(EPD_dispIndex);
        }
        // Print log message: initialization of e-Paper (e-Paper's type)
        //Serial.printf("EPD %s", EPD_dispMass[EPD_dispIndex].title);

        // Initialization
        //client.print("HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n");
        break;
      }

      // Image loading
      if (Buff__signature(Buff__bufInd - 4, "LOAD"))
      {
        // Print log message: image loading
        Serial.print("LOAD");

        // Load data into the e-Paper
        // if there is loading function for current channel (black or red)
        //client.print("HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n");

        // Come back to the image data end
        Buff__bufInd -= 8;

        // Get the index of the image data begin
        int pos = Buff__bufInd - Buff__getWord(Buff__bufInd);

        // Enumerate all of image data bytes
        while (pos < Buff__bufInd)
        {
          // Get current byte
          int value = Buff__getByte(pos);
          //		Serial.printf("address:%d, data:%x ",pos, value);
          //		if(pos % 8 == 0)
          //			Serial.printf("\r\n");
          // Invert byte's bits in case of '2.7' e-Paper
          value = ~value;
          EPD_Image[EPD_Image_count++] = (byte)value;

          // Increment the current byte index on 2 characters
          pos += 2;
        }
        break;
      }

      // If the loading is complete, then...
      if (Buff__signature(Buff__bufInd - 4, "SHOW"))
      {
        // Show results 
        EPD_Image_count = 0;
        display.fillScreen(GxEPD_WHITE);
        display.drawBitmap(0, 0, EPD_Image, 200, 200, GxEPD_BLACK);
        display.update();
        
        //Print log message: show
        Serial.print("\r\nSHOW");
        //client.print("HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n");
        break;
      }

      // If the routine reaches this code,
      // it means the there is no known commands,
      // the server has to send the 'index' page
      isIndexPage = true;
    }
  }

  // Clear data stream of the 'client'
  client.flush();

  // Sent to the 'client' the header describing the type of data.
  // In this case 'Content-Type' is 'text/html'
  client.print("HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n");

  // Send the 'index' page if it's needed
  if (isIndexPage)
    sendHtml(client, myIP);
  else
    client.print("Ok!");

  client.print("\r\n");
  delay(1);

  // Print log message: the end of request processing
  Serial.println(">>>");
  return true;
}

/* Sending a script to the client's browser ------------------------------------*/
bool Srvr__file(WiFiClient client, int fileIndex, char *fileName)
{
  // Print log message: sending of script file
  Serial.print(fileName);

  // Sent to the 'client' the header describing the type of data.
  client.print(fileIndex == 0
                   ? "HTTP/1.1 200 OK\r\nContent-Type: text/css\r\n\r\n"
                   : "HTTP/1.1 200 OK\r\nContent-Type: text/javascript\r\n\r\n");

  // Choose the index of script
  // (ESP8266 can't to send all of code by one file
  // and needs split it on a few parts)
  switch (fileIndex)
  {
  case 0:
    sendCSS(client);
    break;
  case 1:
    sendJS_A(client);
    break;
  case 2:
    sendJS_B(client);
    break;
  case 3:
    sendJS_C(client);
    break;
  case 4:
    sendJS_D(client);
    break;
  }

  client.print("\r\n");
  delay(1);

  // Print log message: the end of request processing
  Serial.println(">>>");

  return true;
}
