// ============================================================
// StaffModule.h - Staff Dashboard and Functions
// Hospital Appointment Booking System
// ============================================================

#ifndef STAFF_MODULE_H
#define STAFF_MODULE_H

#include "BaseModule.h"

// Derived class from BaseModule (OOP Inheritance)
class StaffModule : public BaseModule {
private:
    void viewAllAppointments();
    void approveAppointment();
    void searchPatient();
    void viewDoctors();
    
public:
    StaffModule(ConsoleUtils& c, DatabaseManager& d, UserSession& s);
    void showDashboard() override; // Polymorphism - override base class method
};

#endif // STAFF_MODULE_H
