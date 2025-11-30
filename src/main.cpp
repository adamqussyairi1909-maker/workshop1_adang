// ============================================================
// Hospital Appointment Booking System
// Main Application Entry Point
// 
// Project Structure:
// - Models.h          : Data structures
// - ConsoleUtils.h    : HCI console utilities
// - DatabaseManager   : MySQL database operations
// - Utilities         : Common helper functions
// - AuthModule        : Authentication
// - PatientModule     : Patient dashboard
// - DoctorModule      : Doctor dashboard
// - StaffModule       : Staff dashboard
// - AdminModule       : Admin dashboard
// 
// Requirements:
// - Visual Studio 2022
// - XAMPP (MySQL)
// - MySQL Connector C++
// ============================================================

#include <iostream>
#include <windows.h>

// Include all modules
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

// Module instances (will be initialized in main)
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
        std::cout << std::endl;
        console.printMenuOption(1, "Login");
        console.printMenuOption(2, "Patient Registration");
        console.printMenuOption(3, "Exit");
        
        int choice = console.getIntInput("\nEnter your choice: ", 1, 3);
        
        switch (choice) {
            case 1: {
                if (authModule->login()) {
                    // Navigate to appropriate dashboard based on user type
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
                console.setColor(CYAN);
                std::cout << "\n\n";
                console.printCentered("Thank you for using");
                console.printCentered("Hospital Appointment Booking System");
                std::cout << "\n";
                console.printCentered("Goodbye!");
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
    
    // Initialize database connection
    console.showLoading("Connecting to database");
    
    if (!db.connect()) {
        console.printError("Failed to connect to database!");
        console.printInfo("Please ensure:");
        std::cout << "  1. XAMPP is running with MySQL service active" << std::endl;
        std::cout << "  2. Database 'hospital_appointment_db' exists" << std::endl;
        std::cout << "  3. Run 'database_setup.sql' in phpMyAdmin first" << std::endl;
        console.pauseScreen();
        return 1;
    }
    
    console.printSuccess("Database connected successfully!");
    Sleep(1000);
    
    // Initialize modules
    authModule = new AuthModule(console, db, currentSession);
    patientModule = new PatientModule(console, db, currentSession);
    doctorModule = new DoctorModule(console, db, currentSession);
    staffModule = new StaffModule(console, db, currentSession);
    adminModule = new AdminModule(console, db, currentSession);
    
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

