# ESPVents

# Connect over TTL
picocom -b 115200 --omap crcrlf /dev/ttyUSB0


# AT Commands
 Then, you may want to list the Access Points that are visible from your ESP module:

    Set Wifi mode to both Access Point and STAtion:
    AT+CWMODE=3
    List Access Points:
    AT+CWLAP

And connect to one of the listed APs:

    Join an Access Point:
    AT+CWJAP=“SSID”,“password”


# start over
 python esptool.py -p /dev/ttyUSB0 -b 115000 write_flash 0x000000 ai-thinker-v1.1.1-115200.bin




