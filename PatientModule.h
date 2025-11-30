// ============================================================
// PatientModule.h - Patient Dashboard and Functions
// Hospital Appointment Booking System
// ============================================================

#ifndef PATIENT_MODULE_H
#define PATIENT_MODULE_H

#include "Models.h"
#include "ConsoleUtils.h"
#include "DatabaseManager.h"

class PatientModule {
private:
    ConsoleUtils& console;
    DatabaseManager& db;
    UserSession& session;
    
    void bookAppointment();
    void viewAppointments();
    void cancelAppointment();
    void updateDetails();
    
public:
    PatientModule(ConsoleUtils& c, DatabaseManager& d, UserSession& s);
    void showDashboard();
    void registerPatient();
};

#endif // PATIENT_MODULE_H

