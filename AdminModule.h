// ============================================================
// AdminModule.h - Admin Dashboard and Functions
// Hospital Appointment Booking System
// ============================================================

#ifndef ADMIN_MODULE_H
#define ADMIN_MODULE_H

#include "Models.h"
#include "ConsoleUtils.h"
#include "DatabaseManager.h"

class AdminModule {
private:
    ConsoleUtils& console;
    DatabaseManager& db;
    UserSession& session;
    
    void manageUsers();
    void approveAccounts();
    void monitorActivity();
    void generateReports();
    
public:
    AdminModule(ConsoleUtils& c, DatabaseManager& d, UserSession& s);
    void showDashboard();
};

#endif // ADMIN_MODULE_H

