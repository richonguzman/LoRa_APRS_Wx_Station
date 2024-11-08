#include "configuration.h"
#include "station_utils.h"
#include "query_utils.h"
#include "lora_utils.h"


extern Configuration                    Config;
extern String                           versionDate;


namespace QUERY_Utils {

    String process(const String& query, const String& station, bool thirdParty) {
        String answer;
        String queryQuestion = query;
        queryQuestion.toUpperCase();
        if (queryQuestion == "?APRS?" || queryQuestion == "H" || queryQuestion == "HELP" || queryQuestion=="?") {
            answer.concat("?APRSV ?APRSP");
        } else if (queryQuestion == "?APRSV") {
            answer.concat("CA2RXU_LoRa_Wx 1.0 v");
            answer.concat(versionDate);
        } else if (queryQuestion == "?APRSP") {
            answer.concat("iGate QTH: ");
            answer.concat(String(Config.beacon.latitude,3));
            answer.concat(" ");
            answer.concat(String(Config.beacon.longitude,3));
        } 

        String queryAnswer = Config.callsign;
        queryAnswer += ">APLRW1";
        if (!thirdParty) queryAnswer += ",RFONLY";
        if (Config.beacon.path != "") {
            queryAnswer += ",";
            queryAnswer += Config.beacon.path;
        }
        queryAnswer += "::";

        String processedStation = station;
        for (int i = station.length(); i < 9; i++) {
            processedStation += ' ';
        }
        queryAnswer += processedStation;
        queryAnswer += ":";
        queryAnswer += answer;
        return queryAnswer;
    }

}