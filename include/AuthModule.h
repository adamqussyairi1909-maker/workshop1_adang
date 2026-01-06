// ============================================================
// AuthModule.h - Authentication Module
// Hospital Appointment Booking System
// ============================================================

#ifndef AUTH_MODULE_H
#define AUTH_MODULE_H

#include "Models.h"
#include "ConsoleUtils.h"
#include "DatabaseManager.h"

// OOP: Encapsulation - Private members, public interface
class AuthModule {
private:
    ConsoleUtils& console;
    DatabaseManager& db;
    UserSession& session;
    
    // Private helper methods (Encapsulation)
    bool authenticateUser(const std::string& email, const std::string& password);
    
public:
    AuthModule(ConsoleUtils& c, DatabaseManager& d, UserSession& s);
    bool login();
    bool loginAsPatient();
    bool loginAsDoctor();
    bool loginAsStaff();
    bool loginAsAdmin();
    void logout();
    bool isLoggedIn() const;
    std::string getUserType() const;
};

#endif // AUTH_MODULE_H
