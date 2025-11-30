// ============================================================
// DoctorModule.h - Doctor Dashboard and Functions
// Hospital Appointment Booking System
// ============================================================

#ifndef DOCTOR_MODULE_H
#define DOCTOR_MODULE_H

#include "Models.h"
#include "ConsoleUtils.h"
#include "DatabaseManager.h"

class DoctorModule {
private:
    ConsoleUtils& console;
    DatabaseManager& db;
    UserSession& session;
    
    void viewTodayAppointments();
    void viewAllAppointments();
    void completeAppointment();
    void viewPatientInfo();
    void updateAvailability();
    
public:
    DoctorModule(ConsoleUtils& c, DatabaseManager& d, UserSession& s);
    void showDashboard();
};

#endif // DOCTOR_MODULE_H
