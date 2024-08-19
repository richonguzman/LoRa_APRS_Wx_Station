#include "gps_utils.h"


String beaconPacket;

/*********** TO BE ADDED FROM CONFIGURATION ***********/
extern String callsign;
extern String tocall;
extern String path;
extern String overlay;
extern String symbol;
extern float stationLatitude;
extern float stationLongitude;
/******************************************************/

namespace GPS_Utils {

    char *ax25_base91enc(char *s, uint8_t n, uint32_t v) {
        for(s += n, *s = '\0'; n; n--) {
            *(--s) = v % 91 + 33;
            v /= 91;
        }
        return(s);
    }

    String encodeGPS(float latitude, float longitude, const String& overlay, const String& symbol) {
        String encodedData = overlay;
        uint32_t aprs_lat, aprs_lon;
        aprs_lat = 900000000 - latitude * 10000000;
        aprs_lat = aprs_lat / 26 - aprs_lat / 2710 + aprs_lat / 15384615;
        aprs_lon = 900000000 + longitude * 10000000 / 2;
        aprs_lon = aprs_lon / 26 - aprs_lon / 2710 + aprs_lon / 15384615;

        String Ns, Ew, helper;
        if(latitude < 0) { Ns = "S"; } else { Ns = "N"; }
        if(latitude < 0) { latitude= -latitude; }

        if(longitude < 0) { Ew = "W"; } else { Ew = "E"; }
        if(longitude < 0) { longitude= -longitude; }

        char helper_base91[] = {"0000\0"};
        int i;
        ax25_base91enc(helper_base91, 4, aprs_lat);
        for (i = 0; i < 4; i++) {
            encodedData += helper_base91[i];
        }
        ax25_base91enc(helper_base91, 4, aprs_lon);
        for (i = 0; i < 4; i++) {
            encodedData += helper_base91[i];
        }
        encodedData += symbol + " x" + "\x47";
        return encodedData;
    }

    void generateBeacon() {
        beaconPacket = callsign;
        beaconPacket += ">";
        beaconPacket += tocall;
        if (path != "") {
            beaconPacket += ",";
            beaconPacket += path;
        }
        beaconPacket += ":!";
        beaconPacket += encodeGPS(stationLatitude, stationLongitude, overlay, symbol);
    }

}