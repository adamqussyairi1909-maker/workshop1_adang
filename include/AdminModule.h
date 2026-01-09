// ============================================================
// AdminModule.h - Admin Dashboard and Functions
// Hospital Appointment Booking System
// ============================================================

#ifndef ADMIN_MODULE_H
#define ADMIN_MODULE_H

#include "BaseModule.h"

// Derived class from BaseModule (OOP Inheritance)
class AdminModule : public BaseModule {
private:
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
    void generateReport();
    
public:
    AdminModule(ConsoleUtils& c, DatabaseManager& d, UserSession& s);
    void showDashboard() override; // Polymorphism - override base class method
};

#endif // ADMIN_MODULE_H
