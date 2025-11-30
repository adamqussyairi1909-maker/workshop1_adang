// ============================================================
// AuthModule.cpp - Authentication Module Implementation
// Hospital Appointment Booking System
// Auto-detects user role from credentials
// ============================================================

#include "../include/AuthModule.h"
#include <iostream>
#include <windows.h>

AuthModule::AuthModule(ConsoleUtils& c, DatabaseManager& d, UserSession& s)
    : console(c), db(d), session(s) {}

bool AuthModule::login() {
    console.clearScreen();
    console.printHeader("LOGIN PAGE");
    
    std::string email, password;
    
    std::cin.ignore(10000, '\n');
    
    // Get email with validation
    console.setColor(WHITE);
    std::cout << "\n  Please enter your credentials:\n" << std::endl;
    
    do {
        email = console.getStringInput("  Email    : ");
        if (!console.isValidEmail(email)) {
            console.printError("Invalid email format. Please try again.");
        }
    } while (!console.isValidEmail(email));
    
    // Get password (masked)
    console.setColor(CYAN);
    std::cout << "  Password : ";
    console.resetColor();
    password = console.getPasswordInput();
    
    console.showLoading("\n  Authenticating", 3);
    
    // Auto-detect role by trying each user type
    int userID = -1;
    std::string userName = "";
    std::string userType = "";
    
    // Try Patient
    userID = db.authenticatePatient(email, password);
    if (userID > 0) {
        Patient p = db.getPatientById(userID);
        userName = p.patientName;
        userType = "Patient";
    }
    
    // Try Doctor
    if (userID <= 0) {
        userID = db.authenticateDoctor(email, password);
        if (userID > 0) {
            Doctor d = db.getDoctorById(userID);
            userName = d.doctorName;
            userType = "Doctor";
        }
    }
    
    // Try Staff
    if (userID <= 0) {
        userID = db.authenticateStaff(email, password);
        if (userID > 0) {
            Staff s = db.getStaffById(userID);
            userName = s.staffName;
            userType = "Staff";
        }
    }
    
    // Try Admin
    if (userID <= 0) {
        userID = db.authenticateAdmin(email, password);
        if (userID > 0) {
            Admin a = db.getAdminById(userID);
            userName = a.adminName;
            userType = "Admin";
        }
    }
    
    // Check if login successful
    if (userID > 0) {
        session.userID = userID;
        session.userName = userName;
        session.userType = userType;
        session.isLoggedIn = true;
        
        db.logActivity(userType, userID, "Login", "User logged in successfully");
        
        // Display success with role info
        console.setColor(GREEN);
        std::cout << "\n  +-----------------------------------------+" << std::endl;
        std::cout << "  |            LOGIN SUCCESSFUL             |" << std::endl;
        std::cout << "  +-----------------------------------------+" << std::endl;
        console.resetColor();
        
        console.setColor(WHITE);
        std::cout << "\n  Welcome, " << userName << "!" << std::endl;
        std::cout << "  Role: " << userType << std::endl;
        console.resetColor();
        
        console.setColor(DARK_GRAY);
        std::cout << "\n  Redirecting to " << userType << " Dashboard..." << std::endl;
        console.resetColor();
        
        Sleep(2000);
        return true;
    } else {
        console.setColor(RED);
        std::cout << "\n  +-----------------------------------------+" << std::endl;
        std::cout << "  |             LOGIN FAILED                |" << std::endl;
        std::cout << "  +-----------------------------------------+" << std::endl;
        console.resetColor();
        
        console.printError("Invalid email or password!");
        console.printInfo("If you are a new Doctor/Staff, your account may be pending approval.");
        
        console.pauseScreen();
        return false;
    }
}
