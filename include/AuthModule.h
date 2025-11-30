// ============================================================
// AuthModule.h - Authentication Module
// Hospital Appointment Booking System
// ============================================================

#ifndef AUTH_MODULE_H
#define AUTH_MODULE_H

#include "Models.h"           // Same folder
#include "ConsoleUtils.h"     // Same folder
#include "DatabaseManager.h"  // Same folder

class AuthModule {
private:
    ConsoleUtils& console;
    DatabaseManager& db;
    UserSession& session;
    
public:
    AuthModule(ConsoleUtils& c, DatabaseManager& d, UserSession& s);
    bool login();
};

#endif // AUTH_MODULE_H

