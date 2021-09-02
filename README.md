
<h1 align = "center">🌟LilyGO E-Paper-watch🌟</h1>

### English | [中文](docs/details_cn.md) 

--------------------------------------

<h2 align = "left">Quick start ⚡:</h2>


1. Install the following two dependencies
     - [Adafruit-GFX-Library](https://github.com/adafruit/Adafruit-GFX-Library)
     - please use this branch [lewisxhe/fork/GxEPD](https://github.com/lewisxhe/GxEPD),Please note that referencing the wrong branch will cause the program to fail to compile
2. Copy all the folders in the lib directory to `"C:\User\<YourName>\Documents\Arduino\libraries"`
3. At the top of the sketch, define the model of the board and screen to be used 
5. Select `ESP32 Dev Module` in the development board, and keep the other options as default
6. If you do not find the ESP32 series in the development board, then you should see the following installation method [How to install ESP32 in ArduinoIDE](https://github.com/espressif/arduino-esp32/blob/master/docs/arduino-ide/boards_manager.md)
7. For the version of the board you purchased, please check the product link below 
8. When you think there is a problem with your board, you can pull the binary file corresponding to your screen model in the firmware directory, follow the `SETTING.png` settings in the directory, download it to the board, and then observe the execution. 

![image1](https://github.com/Xinyuan-LilyGO/E-Paper-watch/blob/main/doc/image/image1.jpg)

![image2](https://github.com/Xinyuan-LilyGO/E-Paper-watch/blob/main/doc/image/image2.jpg)

<h2 align = "left">Product link📷:</h2>

|     Name     |                            Product  Link                             |
| :----------: | :------------------------------------------------------------------: |
| [E-Paper-watch]() | [Product link]() |

<h2 align = "left">Example description:</h2>

```
examples
├── SimpleTest                   # EPD  Hello world example 
├── NTCPartialTest               # EPD Partial refresh example 
├── WifiSendTest                 # EPD Send pictures by WIFI  
├── WatchDemo                    # EPD Watch example 
└── WatchDemoNoGPS               # EPD Watch example (No gps)
```


