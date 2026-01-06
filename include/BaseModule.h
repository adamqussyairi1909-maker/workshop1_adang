// ============================================================
// BaseModule.h - Base Class for All Modules (OOP Design)
// Hospital Appointment Booking System
// ============================================================

#ifndef BASE_MODULE_H
#define BASE_MODULE_H

#include "Models.h"
#include "ConsoleUtils.h"
#include "DatabaseManager.h"

// Base class for all modules (OOP Inheritance)
class BaseModule {
protected:
    ConsoleUtils& console;
    DatabaseManager& db;
    UserSession& session;
    
public:
    BaseModule(ConsoleUtils& c, DatabaseManager& d, UserSession& s)
        : console(c), db(d), session(s) {}
    
    virtual ~BaseModule() {}
    
    // Pure virtual function - must be implemented by derived classes
    virtual void showDashboard() = 0;
    
    // Common utility methods
    bool isLoggedIn() const {
        return session.isLoggedIn;
    }
    
    std::string getUserType() const {
        return session.userType;
    }
    
    void logActivity(const std::string& action, const std::string& details = "") {
        db.logActivity(session.userType, session.userID, action, details);
    }
};

#endif // BASE_MODULE_H












