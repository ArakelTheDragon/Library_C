# C.Library.CCSPICCProjects

This repository contains multiple projects about programming with CCS PIC C compiler for baseline PIC MCUs from Microchip. Programming for Arudino/ESP8266/ESP32/ESPx boards.

List of projects:

35. **ESP_GameHub_v2 - Loads html games on an ESP and sets it up as a game hub. You can play offline or online.**
- This project makes a game hub with more than 10 games on an ESP8266. Games can be accessed through the web. You can play offline or online.
- Flash the sketch ESP_GameHub_v2.ino.
- Install [LittleFS](https://github.com/earlephilhower/arduino-esp8266littlefs-plugin/releases) plugin by extracting it into C:\Users\<You>\Documents\Arduino\tools\ESP8266LittleFS\tool\esp8266littlefs.jar, if the Arduino\tools folder does not exists make it. Reboot Arduino IDE and go to Tools - ESP8266 LittleFS Data Upload. This will upload all html files from ESP_GameHub_v2\data onto the ESP. Games are single page html files in this case.
- Download the files from ESP_GameHub_v2

The license for this repo is GPL3.
