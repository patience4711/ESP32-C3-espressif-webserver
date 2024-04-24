# ESP32-C3-espressif-webserver
A webserver running on the ESP32C3<br>

With the esp32 c3 i had a lot of troubles with webservers that i could not get to work. So i decided to start with the webserver supplied by espressif. This involved a lot of work because the examples provided by espressif are very cryptic, to me anyway. Somehow i ended up with a working example that i want to share here, hoping that it can help others to get a little more insight in this matter.

This piece of software is in fact a dimmer for an LED, with the help of a fet module you can dim and switch ledstrips of a higher voltage. It contains a webserver complete with a wifi config portal for easy connection to wifi. One of the interesting things is that the website is kept uptodate with events. Has something changed in the server, the webpage is informed to retrieve new data. So if a domotica system switches the led on, this can be seen on the webpage.
<br>
![front_page](https://github.com/patience4711/ESP32-C3-espressif-webserver/assets/12282915/44faba29-4831-4115-838a-0ba281ac25d7)

Is has some interesting features such as:
- it can be switched by a button or by setting the slider
- a default dim value can be saved.
- the on-swich uses always the last used dim value
- a serial console with some system commands
- 4 timers that can switch p.e. relative to sunrise
- can send mosquitto messages and react on mqtt
- the frontpage is kep up-todate with events

There is binary in the files that you can use to flash the software. If you want to compile yourself, these are the settings i used.
![settings_s3](https://github.com/patience4711/ESP32-C3-espressif-webserver/assets/12282915/ce416448-dc20-4030-bfad-8f470de99bfb)

### get started ###
After powerup the first time, the blue led is on, indicating that an ap is opened with the name like ESP32_C3-12345. Connect to this, browse to 192.168.4.1/WIFIPORTAL and follow the obvious steps.<br>
If you connect a white led on pin5 and gnd, you can test.

Connect via serial console: type WIFI;ssid;password;<br> If connected type device-reboot.
  ## totdo ##
- ota software update, couldn't get that to work so far
- authentication for multiple users, on with access only to the frontpage, another also access to the menu

