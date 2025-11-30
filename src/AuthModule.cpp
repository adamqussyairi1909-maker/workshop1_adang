// ============================================================
// AuthModule.cpp - Authentication Module Implementation
// Hospital Appointment Booking System
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
    int roleChoice;
    
    std::cin.ignore(10000, '\n');
    
    email = console.getStringInput("Enter Email: ");
    
    console.setColor(CYAN);
    std::cout << "Enter Password: ";
    console.resetColor();
    password = console.getPasswordInput();
    
    console.printSubHeader("Select Role");
    console.printMenuOption(1, "Patient");
    console.printMenuOption(2, "Doctor");
    console.printMenuOption(3, "Staff");
    console.printMenuOption(4, "Admin");
    
    roleChoice = console.getIntInput("Enter choice: ", 1, 4);
    
    console.showLoading("Authenticating");
    
    int userID = -1;
    std::string userName = "";
    
    switch (roleChoice) {
        case 1: {
            userID = db.authenticatePatient(email, password);
            if (userID > 0) {
                Patient p = db.getPatientById(userID);
                userName = p.patientName;
                session.userType = "Patient";
            }
            break;
        }
        case 2: {
            userID = db.authenticateDoctor(email, password);
            if (userID > 0) {
                Doctor d = db.getDoctorById(userID);
                userName = d.doctorName;
                session.userType = "Doctor";
            }
            break;
        }
        case 3: {
            userID = db.authenticateStaff(email, password);
            if (userID > 0) {
                Staff s = db.getStaffById(userID);
                userName = s.staffName;
                session.userType = "Staff";
            }
            break;
        }
        case 4: {
            userID = db.authenticateAdmin(email, password);
            if (userID > 0) {
                Admin a = db.getAdminById(userID);
                userName = a.adminName;
                session.userType = "Admin";
            }
            break;
        }
    }
    
    if (userID > 0) {
        session.userID = userID;
        session.userName = userName;
        session.isLoggedIn = true;
        
        db.logActivity(session.userType, userID, "Login", "User logged in successfully");
        
        console.printSuccess("Welcome, " + userName + "!");
        Sleep(1500);
        
        return true;
    } else {
        console.printError("Invalid credentials or account not approved!");
        console.pauseScreen();
        return false;
    }
}

