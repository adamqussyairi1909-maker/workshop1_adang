// ============================================================
// Utilities.cpp - Common Utility Functions Implementation
// Hospital Appointment Booking System
// ============================================================

#include "../include/Utilities.h"
#include <ctime>
#include <sstream>

std::string getCurrentDate() {
    time_t now = time(0);
    struct tm tstruct;
    char buf[11];
    localtime_s(&tstruct, &now);
    strftime(buf, sizeof(buf), "%Y-%m-%d", &tstruct);
    return std::string(buf);
}

std::string getCurrentTime() {
    time_t now = time(0);
    struct tm tstruct;
    char buf[9];
    localtime_s(&tstruct, &now);
    strftime(buf, sizeof(buf), "%H:%M:%S", &tstruct);
    return std::string(buf);
}

std::string getCurrentDateTime() {
    time_t now = time(0);
    struct tm tstruct;
    char buf[80];
    localtime_s(&tstruct, &now);
    strftime(buf, sizeof(buf), "%d/%m/%Y - %I:%M %p", &tstruct);
    return std::string(buf);
}

