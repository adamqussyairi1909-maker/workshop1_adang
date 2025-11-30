// ============================================================
// AuthModule.h - Authentication Module
// Hospital Appointment Booking System
// ============================================================

#ifndef AUTH_MODULE_H
#define AUTH_MODULE_H

#include "Models.h"
#include "ConsoleUtils.h"
#include "DatabaseManager.h"

class AuthModule {
private:
    ConsoleUtils& console;
    DatabaseManager& db;
    UserSession& session;
    
public:
    AuthModule(ConsoleUtils& c, DatabaseManager& d, UserSession& s);
    bool login();
    void logout();
    bool isLoggedIn();
    std::string getUserType();
};

#endif // AUTH_MODULE_H
