#include "mytime.h"
#include <time.h>

void MyTime::init() {
    // configTime(gmtOffsetSec, daylightOffsetSec, ntpServer);
    configTzTime("EST5EDT,M3.2.0,M11.1.0", "pool.ntp.org");
}

String MyTime::getTimeString() {
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo)) {
        return "No Time";
    }
    char buffer[6];  // HH:MM
    strftime(buffer, sizeof(buffer), "%H:%M", &timeinfo);
    return String(buffer);
}
