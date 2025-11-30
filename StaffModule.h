// ============================================================
// StaffModule.h - Staff Dashboard and Functions
// Hospital Appointment Booking System
// ============================================================

#ifndef STAFF_MODULE_H
#define STAFF_MODULE_H

#include "Models.h"
#include "ConsoleUtils.h"
#include "DatabaseManager.h"

class StaffModule {
private:
    ConsoleUtils& console;
    DatabaseManager& db;
    UserSession& session;
    
    void viewAppointments();
    void approveAppointment();
    void managePatients();
    void generateReport();
    
public:
    StaffModule(ConsoleUtils& c, DatabaseManager& d, UserSession& s);
    void showDashboard();
};

#endif // STAFF_MODULE_H

