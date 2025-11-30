// ============================================================
// Hospital Appointment Booking System
// Main Application Entry Point
// ============================================================

#include <iostream>
#include <windows.h>

#include "../include/Models.h"
#include "../include/ConsoleUtils.h"
#include "../include/DatabaseManager.h"
#include "../include/Utilities.h"
#include "../include/AuthModule.h"
#include "../include/PatientModule.h"
#include "../include/DoctorModule.h"
#include "../include/StaffModule.h"
#include "../include/AdminModule.h"

// Global instances
ConsoleUtils console;
DatabaseManager db;
UserSession currentSession;

// Module instances
AuthModule* authModule = nullptr;
PatientModule* patientModule = nullptr;
DoctorModule* doctorModule = nullptr;
StaffModule* staffModule = nullptr;
AdminModule* adminModule = nullptr;

// ============================================================
// Main Menu
// ============================================================
void displayMainMenu() {
    while (true) {
        console.displayWelcomeBanner();
        
        console.setColor(WHITE);
        std::cout << "  Please select an option:\n" << std::endl;
        console.printMenuOption(1, "Login");
        console.printMenuOption(2, "Register as New Patient");
        console.printMenuOption(3, "Exit System");
        
        std::cout << std::endl;
        int choice = console.getIntInput("  Enter your choice: ", 1, 3);
        
        switch (choice) {
            case 1: {
                if (authModule->login()) {
                    // Auto-redirect based on detected role
                    if (currentSession.userType == "Patient") {
                        patientModule->showDashboard();
                    } else if (currentSession.userType == "Doctor") {
                        doctorModule->showDashboard();
                    } else if (currentSession.userType == "Staff") {
                        staffModule->showDashboard();
                    } else if (currentSession.userType == "Admin") {
                        adminModule->showDashboard();
                    }
                }
                break;
            }
            case 2: 
                patientModule->registerPatient(); 
                break;
            case 3:
                console.clearScreen();
                std::cout << "\n\n";
                console.setColor(CYAN);
                std::cout << "      ========================================================" << std::endl;
                console.setColor(WHITE);
                std::cout << "                      Thank you for using                     " << std::endl;
                std::cout << "               Hospital Appointment Booking System            " << std::endl;
                console.setColor(CYAN);
                std::cout << "      ========================================================" << std::endl;
                std::cout << "\n";
                console.setColor(DARK_GRAY);
                std::cout << "                          Goodbye!                            " << std::endl;
                console.resetColor();
                std::cout << "\n\n";
                Sleep(2000);
                return;
        }
    }
}

// ============================================================
// Main Entry Point
// ============================================================
int main() {
    // Set console title
    SetConsoleTitleA("Hospital Appointment Booking System");
    
    // Clear screen and show loading
    console.clearScreen();
    std::cout << "\n\n";
    console.setColor(CYAN);
    std::cout << "      ========================================================" << std::endl;
    console.setColor(WHITE);
    std::cout << "                  HOSPITAL APPOINTMENT SYSTEM                 " << std::endl;
    console.setColor(CYAN);
    std::cout << "      ========================================================" << std::endl;
    console.resetColor();
    
    console.showLoading("\n      Initializing system", 3);
    console.showLoading("      Connecting to database", 3);
    
    if (!db.connect()) {
        console.printError("Failed to connect to database!");
        std::cout << std::endl;
        console.setColor(WHITE);
        std::cout << "  Please ensure:" << std::endl;
        std::cout << "    1. XAMPP is running with MySQL service active" << std::endl;
        std::cout << "    2. Database 'hospital_appointment_db' exists" << std::endl;
        std::cout << "    3. Run 'database_setup.sql' in phpMyAdmin first" << std::endl;
        console.resetColor();
        console.pauseScreen();
        return 1;
    }
    
    console.printSuccess("Database connected successfully!");
    console.showLoading("      Loading modules", 2);
    
    // Initialize modules
    authModule = new AuthModule(console, db, currentSession);
    patientModule = new PatientModule(console, db, currentSession);
    doctorModule = new DoctorModule(console, db, currentSession);
    staffModule = new StaffModule(console, db, currentSession);
    adminModule = new AdminModule(console, db, currentSession);
    
    console.printSuccess("System ready!");
    Sleep(1000);
    
    // Start main application
    displayMainMenu();
    
    // Cleanup
    delete authModule;
    delete patientModule;
    delete doctorModule;
    delete staffModule;
    delete adminModule;
    
    db.disconnect();
    
    return 0;
}
