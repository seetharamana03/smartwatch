#include "mytime.h"
#include <time.h>

MyTime::MyTime(){
    memset(&timeinfo, 0, sizeof(timeinfo));
}

void MyTime::init() {
    configTzTime("EST5EDT,M3.2.0,M11.1.0", "pool.ntp.org");
}

String MyTime::getTimeString() {
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo)) {
        return "";
    }
    char buffer[6];  // HH:MM
    strftime(buffer, sizeof(buffer), "%H:%M", &timeinfo);
    return String(buffer);
}

String MyTime::getDateString() {
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo)) {
        return "";
    }
    char buffer[11];  // YYYY-MM-DD
    strftime(buffer, sizeof(buffer), "%Y-%m-%d", &timeinfo);
    return String(buffer);
}
