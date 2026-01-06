// ============================================================
// PatientModule.h - Patient Dashboard and Functions
// Hospital Appointment Booking System
// ============================================================

#ifndef PATIENT_MODULE_H
#define PATIENT_MODULE_H

#include "BaseModule.h"

// Derived class from BaseModule (OOP Inheritance)
class PatientModule : public BaseModule {
private:
    void bookAppointment();
    void viewAppointments();
    void cancelAppointment();
    void updateDetails();
    
public:
    PatientModule(ConsoleUtils& c, DatabaseManager& d, UserSession& s);
    void showDashboard() override; // Polymorphism - override base class method
    void registerPatient();
};

#endif // PATIENT_MODULE_H

