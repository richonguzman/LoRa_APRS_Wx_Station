# LoRa_APRS_Wx_Station

### First:

connect RS485/TLL board to TTGO_T_LoRa32 as per pins_config.h description

connect RS485 Wind Direction Sensor to RS485/TTL board.

pin13 connected to GND.

power TTGO_T_LoRa32 with USB

power RS485 Wind Direction Sensor with 10-30V as per pins_config.h description

---> Serial Output shows if WindDirection Sensor Address. (it should be 0x01)

Turn it off.


### Second:

with the validation of Wind Direcction Sensor Adress (it should be 0x01 and we will change it to 0x02)

pin13 disconnected from GND.

pin14 connected to GND.

---> Oled screen will show the procedure and will show when sensor address is changed

Turn it off.


### Third:

pin13 and pin14 disconnected from GND.

Connect all the other sensors as its shown on pins_config.h

Edit relevant data in LoRa_APRS_WX_Station.cpp (callsign and more)

Power the TTGO_T_LoRa32 and enjoy!
