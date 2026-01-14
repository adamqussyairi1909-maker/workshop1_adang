// ============================================================
// Hospital Appointment Booking System
// Main Entry Point
// ============================================================

#include "../include/ConsoleUtils.h"
#include "../include/DatabaseManager.h"
#include "../include/Models.h"
#include "../include/AuthModule.h"
#include "../include/PatientModule.h"
#include "../include/DoctorModule.h"
#include "../include/StaffModule.h"
#include "../include/AdminModule.h"
#include <iostream>
#include <windows.h>

// Global instances
ConsoleUtils console;
DatabaseManager db;
UserSession currentSession;

void displayMainMenu() {
    console.displayWelcomeBanner();
    
    console.setColor(CYAN);
    std::cout << "  Welcome to the Hospital Appointment System!\n" << std::endl;
    console.resetColor();
    
    console.setColor(DARK_GRAY);
    std::cout << "  ================================================" << std::endl;
    std::cout << "  MAIN MENU" << std::endl;
    std::cout << "  ================================================\n" << std::endl;
    console.resetColor();
    
    // Option 1 - Blue
    console.setColor(BLUE);
    std::cout << "  [1] Login as Patient" << std::endl;
    
    // Option 2 - Red
    console.setColor(RED);
    std::cout << "  [2] Login as Doctor" << std::endl;
    
    // Option 3 - Purple (Magenta)
    console.setColor(MAGENTA);
    std::cout << "  [3] Login as Staff" << std::endl;
    
    // Option 4 - Orange (Dark Yellow)
    console.setColor(DARK_YELLOW);
    std::cout << "  [4] Login as Admin" << std::endl;
    
    // Option 5 - Brown (Dark Red)
    console.setColor(DARK_RED);
    std::cout << "  [5] Register as Patient" << std::endl;
    
    // Option 6 - Yellow
    console.setColor(YELLOW);
    std::cout << "  [6] Exit" << std::endl;
    
    console.resetColor();
    
    
    console.setColor(YELLOW);
    std::cout << "  >> Enter your choice (1-6): ";
    console.resetColor();
}

int main() {
    // Set console to UTF-8
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    
    // Initialize database connection
    console.showLoading("Connecting to database", 2);
    
    if (!db.connect()) {
        console.setColor(RED);
        std::cout << "\n  +------------------------------------------+" << std::endl;
        std::cout << "  |      DATABASE CONNECTION FAILED!         |" << std::endl;
        std::cout << "  +------------------------------------------+" << std::endl;
        console.resetColor();
        
        std::cout << std::endl;
        console.setColor(WHITE);
        std::cout << "  Please check:" << std::endl;
        std::cout << "    1. XAMPP MySQL is running" << std::endl;
        std::cout << "    2. Database 'hospital_appointment_db' exists" << std::endl;
        std::cout << "    3. Database credentials are correct" << std::endl;
        console.resetColor();
        
        console.pauseScreen();
        return 1;
    }
    
    console.printSuccess("Database connected successfully!");
    Sleep(1000);
    
    // Initialize modules
    AuthModule auth(console, db, currentSession);
    PatientModule patientModule(console, db, currentSession);
    DoctorModule doctorModule(console, db, currentSession);
    StaffModule staffModule(console, db, currentSession);
    AdminModule adminModule(console, db, currentSession);
    
    // Main application loop
    while (true) {
        displayMainMenu();
        int choice = console.getIntInput("", 1, 6);
        
        switch (choice) {
            case 1: // Login as Patient
                if (auth.loginAsPatient()) {
                    patientModule.showDashboard();
                }
                break;
                
            case 2: // Login as Doctor
                if (auth.loginAsDoctor()) {
                    doctorModule.showDashboard();
                }
                break;
                
            case 3: // Login as Staff
                if (auth.loginAsStaff()) {
                    staffModule.showDashboard();
                }
                break;
                
            case 4: // Login as Admin
                if (auth.loginAsAdmin()) {
                    adminModule.showDashboard();
                }
                break;
                
            case 5: // Register as Patient
                patientModule.registerPatient();
                break;
                
            case 6: // Exit
                console.clearScreen();
                console.setColor(CYAN);
                std::cout << "\n\n";
                std::cout << "  ========================================================" << std::endl;
                console.setColor(WHITE);
                std::cout << "              THANK YOU FOR USING OUR SYSTEM!             " << std::endl;
                console.setColor(CYAN);
                std::cout << "  ========================================================" << std::endl;
                console.resetColor();
                
                std::cout << std::endl;
                console.setColor(WHITE);
                std::cout << "           Hospital Appointment Booking System            " << std::endl;
                std::cout << "                    Have a great day!                     " << std::endl;
                console.resetColor();
                
                std::cout << "\n\n";
                Sleep(2000);
                db.disconnect();
                return 0;
        }
    }
    
    return 0;
}
