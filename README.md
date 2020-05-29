# LsCompanionFirmware: Firmware for an esp32 to get the height of sit/stand tables

Authors: Simon Bogutzky

License: [MIT](https://opensource.org/licenses/MIT)

Version: 1.0.0

Document version: 1.0.0 

Date: 29/05/2020

## LsCompanionFirmware?

## Using the LsCompanionFirmware

### Hardware Installation Instructions
I use for the wireless connection a NodeMCU-32S ESP32 development board (similar to [Waveshare NodeMCU-32S ESP32 Development Board](https://www.amazon.com/dp/B07TXNK6P1/ref=cm_sw_em_r_mt_dp_U_DbcMEbPJSWKTA)), based on the popular [ESP32](http://espressif.com/) chipset.

Putting the NodeMCU-32S ESP32 development board:  

1. press and hold the BOOT button
2. press the Upload button in the Arduino IDE
3. after the message "Connecting..." appears in the Arduino IDE, release the BOOT button

### Adding SPI OLED Display

### Adding DHT

### Programming ESP32 Development Board in the Arduino IDE

Download the [Arduino IDE](https://www.arduino.cc/en/Main/Software). 

*I use macOS Catalina and the Version 1.8.12. The IDE uses Python and you have to create the right links to it. Type in the terminal:*

```
sudo ln -s /usr/bin/python3 /usr/local/bin/python3
sudo mkdir /usr/local/bin/
```
Arduino IDE offers support for esp32. All you have to do is to go to File / Preferences, and on Additional Boards Manager URL add “https://dl.espressif.com/dl/package_esp32_index.json”. In Arduino IDE, Tools, setup the communication to board. I select: DOIT ESP32 DEVKIT V1. 

I use the SDD1306 library by [Adafruit](https://github.com/adafruit/Adafruit_SSD1306) for the programming.

For the DHT I use the library by [Adafruit](https://github.com/adafruit/DHT-sensor-library). 

### Example Usage

## Author and Contribution
As by the License this is free software released by the Simon Bogutzky. The author (Simon Bogutzky) welcomes external contributors to freely use and extend this software. If you need some help, please write an [issue](https://github.com/sbogutzky/LsCompanionFrimware/issues).
