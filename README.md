# Arduino-ESP8266-RGB-Web-Server-FastLED
A Wi-Fi web server for the ESP8266 platform driving a WS2812B addressable RGB Led strip - Arduino sketch

Description:
An Arduino sketch for running a Wi-Fi web server on the ESP8266-12F. Designed to drive the WS2812B addressable RGB Led strips using the FastLED Arduino library.

Updates:
I have now implemented brightness and color controls that is usable via Adafruit IO. For my setup at home, I use a toggle button for the on/off, a slider for brightness, and a textbox for the color commands. Casing not matter for the color and on/off commands. Valid brightness numbers are 0-100 and are mapped to the 0-255 the program uses. 
I've also included some photos in the photos folder of my Adafruit IO dashboard and how I've setup some of my IFTTT applets. 

Construction Notes:
This repository contains the Arduino program, HTML pages, and associated Eagle schematic and board design files. This project is forked from Giznoise's GitHub repo which I've modified extensively. I wanted to create a lightbox for my girlfriend and was able to control LED's with my Arduino but had no idea how to get them to be controlled via a webpage. Giznoise's project had this implemented wonderfully and I made adjustments along the way. I've reworked a fair bit of their program and added various features, libraries, and more. 

Changes include:
1) Changing the LED control library from Adafruit's Neopixel to the FastLED library.
2) Improving the look of the web control pages and making them better able to adjust to different screen sizes.
3) Swapping the HTML5 color picker to the JSColor javascript library. iOS devices did not support the HTML5 color picker. 
4) Adding additional web control settings to support changing blending modes, choosing different patterns, etc.
5) Added Google Assistant control support throught IFTT and Adafruit IO.
6) Created a schematic and board layout in Eagle that supports dropping in a NodeMCU v2 for controls.
7) Using a SN74AHCT125D level shifter in the schematic to enable the NodeMCU to better control the WS2812b strips. The strips need 5v signal logic and the NodeMCU provides 3.3v; this IC addresses this issue.
8) Adds Android OTA support which enables you to easily upload new programs to the NodeMCU/ESP8266 board without having to use a USB cable. 

Useful Information:
You will need to utilize the ESP8266 uploader tools to upload the data folder. You don't have to do anything special, just follow the directions for installing the tool, and just click tools, ESP8266 Sketch Data Upload and it will upload the needed files. This allows the JScolor.js file to be hosted locally on the ESP8266 vs having to host it online. A link to this tool is provided at the bottom of this page. 

Possible issues and features missing from the original project:

1) Candle mode was not converted to the FastLED library. 
2) It can be accessed via IP or via mDNS by devices on the same wifi network by typing lightbox.local in the address bar - However this feature does not function in Chrome on Android or iOS. It works in Safari on iOS and Chrome for Windows. In the future it may be possible to announce the IP address via the MQTT client but this is not implemented. The original author had similar issues and recommended finding the IP via your router admin panel. 
3) Present board design hosted on GitHub is a revision of the one I had originally gotten fabricated and has not been fabricated and tested again. It should work without issue as I only made some small tweaks to improve the board's layout.
4) The board and schematic allow for 4 LED strips to be connected but the program only supports one. I have not tested function of terminals 2-4 as of yet. The extra 3 were implemented as a future expandability option.
5) The holes for the headers for the NodeMCU to drop into are spaced according to the NodeMCU Eagle library but most NodeMCU devboard pins sort of bow out a bit. I had to bend the pins on my devboards to make them align better. 

Items to be improved:

1) A method to announce the board's IP address, likely through the MQTT client.
2) Improving the board design to directly implement an actual ESP8266 or ESP32 chip vs dropping in a NodeMCU v2 devboard. Dropping in the devboard was far easier for me to get the board design tested and up and running. 
3) Updating board design to allow for both 2.54mm and 3.5mm 3 position screw terminals, similar to how the Big Easy stepper driver allows for both sizes. 
4) Improving board design to utilize more SMD components where possible. 
5) An ESP32 version of this design for the future may be nice. 
6) Potentially splitting up the html_pages.h file into separate html pages now that I know how to store and read files from the SPIFFS.

Implements the following:

FastLED library:
https://github.com/FastLED/FastLED

Pubsubclient (MQTT) for the Google Assistant control:
https://github.com/knolleary/pubsubclient

ESP8266 Web Server:
https://github.com/esp8266/Arduino/blob/master/libraries/ESP8266WebServer/examples/AdvancedWebServer/AdvancedWebServer.ino

Using the onboard SPIFFS to store the JScolor library:
https://tttapa.github.io/ESP8266/Chap11%20-%20SPIFFS.html

ESP8266fs uploader tool:
https://github.com/esp8266/arduino-esp8266fs-plugin

JSColor javascript color picker:
http://jscolor.com/

Special thanks to:
* The original author's hard work.
* This Reddit post and guide for showing how to fix the flickering issues by using the level shifting IC:
https://www.reddit.com/r/esp8266/comments/4v24c1/d1_mini_level_converter_2812b_led_power_supply/
https://happyinmotion.com/?p=1247
* This guide on how to use Google Assistant in conjunction with IFTTT and Adafruit IO:
http://www.instructables.com/id/Control-Your-Projects-With-Google-Assistant-and-Ad/
* Countless other users on StackExchange, the Arduino Forums, and other websites whose questions and answers helped me along the way. 
* This guide on using multiple MQTT topics:
https://www.baldengineer.com/multiple-mqtt-topics-pubsubclient.html

A bill of materials for Digikey is also provided within this repository. 


Demo video viewable here: https://youtu.be/otg-TUXe8Jg