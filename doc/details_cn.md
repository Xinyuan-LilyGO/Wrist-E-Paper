

<h1 align = "center">🌟LilyGO E-Paper-watch🌟</h1>

### [English](../README.md) | 中文

--------------------------------------


<h2 align = "left">快速开始⚡:</h2>

1. 安装下面两个依赖
    - [Adafruit-GFX-Library](https://github.com/adafruit/Adafruit-GFX-Library),请使用这个分支[lewisxhe/fork/GxEPD](https://github.com/lewisxhe/GxEPD),请注意，引用错误的分支会导致程序无法编译
2. 把lib的库添加到`"C:\User\<YourName>\Documents\Arduino\libraries"`
3. 在草图的最上方,定义要使用的板子和屏幕的型号
4. 在开发板中选择`ESP32 Dev Module`，其他选项保持默认
5. 如果你在开发板中没找到ESP32系列，那么你该看下面安装方法[如何在ArduinoIDE中安装ESP32](https://github.com/espressif/arduino-esp32/blob/master/docs/arduino-ide/boards_manager.md)
6. 关于你购买的板子的版本,请查看下面的产品连接
7. 当你认为你的板子有问题时,你可以在`firmware`目录拉取与你对应屏幕型号的二进制文件，按照目录内的`SETTING.png`设置后,将它下载到板子，然后观察执行情况.



<h2 align = "left">产品链接📷:</h2>

|     Name     |                            Product  Link                             |
| :----------: | :------------------------------------------------------------------: |
| [E-Paper-watch]() | [Product link]() |



<h2 align = "left">示例描述:</h2>

```
examples
├── SimpleTest                   # 墨水屏简单LOGO演示休眠示例
├── NTCPartialTest               # 局刷NTC时间示例
├── WifiSendTest                 # WIFI传送图片示例 
├── WatchDemo                    # 手表Demo
└── WatchDemoNoGPS               # 手表Demo (No gps)
```
