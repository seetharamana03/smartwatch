#include "mytime.h"
#include <time.h>

void MyTime::init() {
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

String MyTime::getDateString() {
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo)) {
        return "No Date";
    }
    char buffer[11];  // YYYY-MM-DD
    strftime(buffer, sizeof(buffer), "%Y-%m-%d", &timeinfo);
    return String(buffer);
}
