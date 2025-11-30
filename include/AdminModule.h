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
    
    // Patient management
    void managePatients();
    void viewAllPatients();
    void searchPatient();
    void deletePatient();
    
    // Doctor management
    void manageDoctors();
    void viewAllDoctors();
    void addDoctor();
    void updateDoctor();
    void deleteDoctor();
    
    // Staff management
    void manageStaff();
    void viewAllStaff();
    void addStaff();
    void deleteStaff();
    
    // Reports
    void viewSystemLogs();
    void systemStatistics();
    
public:
    AdminModule(ConsoleUtils& c, DatabaseManager& d, UserSession& s);
    void showDashboard();
};

#endif // ADMIN_MODULE_H
