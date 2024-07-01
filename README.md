# LoRa_APRS_Wx_Station...

### First:

a) Connect RS485/TLL board to TTGO_T_LoRa32 as per boards_pinout.h description.

b) Connect RS485 Wind Direction Sensor to RS485/TTL board.

c) Connect Pin13 to 3.3V pin.

d) Power TTGO_T_LoRa32 with USB

e) Power RS485 Wind Direction Sensor with 10-30V(DC) as per boards_pinout.h description

---> Serial Output will show the board looking for 0x00, 0x01, 0x02 Adresses for WindDirection Sensor. (It should be 0x01).

f) Turn/Power it off.


### Second:

With the validation of Wind Direcction Sensor Adress (it should be 0x01 and we will change it to 0x02):

a) Disconnect Pin13 from 3.3V pin.

b) Connect Pin14 to 3.3V pin.

---> Oled screen will show the procedure and will show when sensor address is changed.

c) Turn/Power it off.


### Third:

a) Disconnect Pin13 and Pin14 from 3.3V pin.

b) Connect all the other sensors as its shown on boards_pinout.h

c) Edit relevant data in LoRa_APRS_WX_Station.cpp (callsign and more)

d) Power the TTGO_T_LoRa32 and enjoy!
