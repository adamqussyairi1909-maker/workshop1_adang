// ============================================================
// DoctorModule.h - Doctor Dashboard and Functions
// Hospital Appointment Booking System
// ============================================================

#ifndef DOCTOR_MODULE_H
#define DOCTOR_MODULE_H

#include "Models.h"           // Same folder
#include "ConsoleUtils.h"     // Same folder
#include "DatabaseManager.h"  // Same folder

class DoctorModule {
private:
    ConsoleUtils& console;
    DatabaseManager& db;
    UserSession& session;
    
    void viewSchedule();
    void viewAllAppointments();
    void viewPatientList();
    
public:
    DoctorModule(ConsoleUtils& c, DatabaseManager& d, UserSession& s);
    void showDashboard();
};

#endif // DOCTOR_MODULE_H

