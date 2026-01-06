// ============================================================
// DoctorModule.h - Doctor Dashboard and Functions
// Hospital Appointment Booking System
// ============================================================

#ifndef DOCTOR_MODULE_H
#define DOCTOR_MODULE_H

#include "BaseModule.h"

// Derived class from BaseModule (OOP Inheritance)
class DoctorModule : public BaseModule {
private:
    void viewTodayAppointments();
    void viewAllAppointments();
    void completeAppointment();
    void viewPatientInfo();
    void updateAvailability();
    
public:
    DoctorModule(ConsoleUtils& c, DatabaseManager& d, UserSession& s);
    void showDashboard() override; // Polymorphism - override base class method
};

#endif // DOCTOR_MODULE_H
