// ============================================================
// AuthModule.cpp - Authentication Module Implementation
// Hospital Appointment Booking System
// ============================================================

#include "../include/AuthModule.h"
#include <iostream>
#include <windows.h>
#undef max

AuthModule::AuthModule(ConsoleUtils& c, DatabaseManager& d, UserSession& s)
    : console(c), db(d), session(s) {}

bool AuthModule::login() {
    console.clearScreen();
    console.printHeader("USER LOGIN");
    
    console.setColor(CYAN);
    std::cout << "\n  Login to access your account.\n" << std::endl;
    console.resetColor();
    
    console.setColor(DARK_GRAY);
    std::cout << "  ================================================" << std::endl;
    std::cout << "  ENTER YOUR CREDENTIALS" << std::endl;
    std::cout << "  ================================================\n" << std::endl;
    console.resetColor();
    
    console.setColor(YELLOW);
    std::cout << "  >> Enter your email address: ";
    console.resetColor();
    
    if (std::cin.peek() == '\n') std::cin.ignore();
    std::string email;
    std::getline(std::cin, email);
    
    
    console.setColor(YELLOW);
    std::cout << "  >> Enter your password: ";
    console.resetColor();
    std::string password = console.getPasswordInput();
    
    console.showLoading("\n  Verifying credentials", 2);
    
    // Try login as Patient
    int patientID = db.loginPatient(email, password);
    if (patientID > 0) {
        Patient patient = db.getPatientById(patientID);
        session.userID = patientID;
        session.userName = patient.patientName;
        session.userEmail = patient.email;
        session.userType = "Patient";
        session.isLoggedIn = true;
        
        console.setColor(GREEN);
        std::cout << "\n  +-----------------------------------------+" << std::endl;
        std::cout << "  |          LOGIN SUCCESSFUL!              |" << std::endl;
        std::cout << "  +-----------------------------------------+" << std::endl;
        console.resetColor();
        
        console.setColor(WHITE);
        std::cout << "\n  Welcome, " << session.userName << "!" << std::endl;
        std::cout << "  You are logged in as: PATIENT" << std::endl;
        console.resetColor();
        
        db.logActivity("Patient", patientID, "Login", "Successful login");
        Sleep(1500);
        return true;
    }
    
    // Try login as Staff
    int staffID = db.loginStaff(email, password);
    if (staffID > 0) {
        Staff staff = db.getStaffById(staffID);
        session.userID = staffID;
        session.userName = staff.staffName;
        session.userEmail = staff.email;
        session.userType = "Staff";
        session.isLoggedIn = true;
        
        console.setColor(GREEN);
        std::cout << "\n  +-----------------------------------------+" << std::endl;
        std::cout << "  |          LOGIN SUCCESSFUL!              |" << std::endl;
        std::cout << "  +-----------------------------------------+" << std::endl;
        console.resetColor();
        
        console.setColor(WHITE);
        std::cout << "\n  Welcome, " << session.userName << "!" << std::endl;
        std::cout << "  You are logged in as: STAFF" << std::endl;
        console.resetColor();
        
        db.logActivity("Staff", staffID, "Login", "Successful login");
        Sleep(1500);
        return true;
    }
    
    // Try login as Doctor
    int doctorID = db.loginDoctor(email, password);
    if (doctorID > 0) {
        Doctor doctor = db.getDoctorById(doctorID);
        session.userID = doctorID;
        session.userName = doctor.doctorName;
        session.userEmail = doctor.email;
        session.userType = "Doctor";
        session.isLoggedIn = true;
        
        console.setColor(GREEN);
        std::cout << "\n  +-----------------------------------------+" << std::endl;
        std::cout << "  |          LOGIN SUCCESSFUL!              |" << std::endl;
        std::cout << "  +-----------------------------------------+" << std::endl;
        console.resetColor();
        
        console.setColor(WHITE);
        std::cout << "\n  Welcome, Dr. " << session.userName << "!" << std::endl;
        std::cout << "  You are logged in as: DOCTOR" << std::endl;
        console.resetColor();
        
        db.logActivity("Doctor", doctorID, "Login", "Successful login");
        Sleep(1500);
        return true;
    }
    
    // Try login as Admin
    int adminID = db.loginAdmin(email, password);
    if (adminID > 0) {
        Admin admin = db.getAdminById(adminID);
        session.userID = adminID;
        session.userName = admin.adminName;
        session.userEmail = admin.email;
        session.userType = "Admin";
        session.isLoggedIn = true;
        
        console.setColor(GREEN);
        std::cout << "\n  +-----------------------------------------+" << std::endl;
        std::cout << "  |          LOGIN SUCCESSFUL!              |" << std::endl;
        std::cout << "  +-----------------------------------------+" << std::endl;
        console.resetColor();
        
        console.setColor(WHITE);
        std::cout << "\n  Welcome, " << session.userName << "!" << std::endl;
        std::cout << "  You are logged in as: ADMIN" << std::endl;
        console.resetColor();
        
        db.logActivity("Admin", adminID, "Login", "Successful login");
        Sleep(1500);
        return true;
    }
    
    // Login failed
    console.setColor(RED);
    std::cout << "\n  +-----------------------------------------+" << std::endl;
    std::cout << "  |            LOGIN FAILED!                |" << std::endl;
    std::cout << "  +-----------------------------------------+" << std::endl;
    console.resetColor();
    
    std::cout << std::endl;
    console.setColor(WHITE);
    std::cout << "  Invalid email or password." << std::endl;
    std::cout << "\n  Possible reasons:" << std::endl;
    std::cout << "    - Email is not registered" << std::endl;
    std::cout << "    - Password is incorrect" << std::endl;
    std::cout << "    - Caps Lock may be on" << std::endl;
    std::cout << "\n  If you don't have an account, please register first." << std::endl;
    console.resetColor();
    
    console.pauseScreen();
    return false;
}

bool AuthModule::loginAsPatient() {
    console.clearScreen();
    console.printHeader("PATIENT LOGIN");
    
    console.setColor(CYAN);
    std::cout << "\n  Login as Patient to access your account.\n" << std::endl;
    console.resetColor();
    
    console.setColor(DARK_GRAY);
    std::cout << "  ================================================" << std::endl;
    std::cout << "  ENTER YOUR CREDENTIALS" << std::endl;
    std::cout << "  ================================================\n" << std::endl;
    console.resetColor();
    
    console.setColor(YELLOW);
    std::cout << "  >> Enter your email address: ";
    console.resetColor();
    
    if (std::cin.peek() == '\n') std::cin.ignore();
    std::string email;
    std::getline(std::cin, email);
    
    if (email.empty() || !console.isValidEmail(email)) {
        console.printError("Invalid email format!");
        console.pauseScreen();
        return false;
    }
    
    console.setColor(YELLOW);
    std::cout << "  >> Enter your password: ";
    console.resetColor();
    std::string password = console.getPasswordInput();
    
    if (password.empty()) {
        console.printError("Password cannot be empty!");
        console.pauseScreen();
        return false;
    }
    
    console.showLoading("\n  Verifying credentials", 2);
    
    int patientID = db.loginPatient(email, password);
    if (patientID > 0) {
        Patient patient = db.getPatientById(patientID);
        session.userID = patientID;
        session.userName = patient.patientName;
        session.userEmail = patient.email;
        session.userType = "Patient";
        session.isLoggedIn = true;
        
        console.setColor(GREEN);
        std::cout << "\n  +-----------------------------------------+" << std::endl;
        std::cout << "  |          LOGIN SUCCESSFUL!              |" << std::endl;
        std::cout << "  +-----------------------------------------+" << std::endl;
        console.resetColor();
        
        console.setColor(WHITE);
        std::cout << "\n  Welcome, " << session.userName << "!" << std::endl;
        std::cout << "  You are logged in as: PATIENT" << std::endl;
        console.resetColor();
        
        db.logActivity("Patient", patientID, "Login", "Successful login");
        Sleep(1500);
        return true;
    }
    
    console.setColor(RED);
    std::cout << "\n  +-----------------------------------------+" << std::endl;
    std::cout << "  |            LOGIN FAILED!                |" << std::endl;
    std::cout << "  +-----------------------------------------+" << std::endl;
    console.resetColor();
    
    std::cout << std::endl;
    console.setColor(WHITE);
    std::cout << "  Invalid email or password for Patient account." << std::endl;
    std::cout << "\n  Possible reasons:" << std::endl;
    std::cout << "    - Email is not registered as Patient" << std::endl;
    std::cout << "    - Password is incorrect" << std::endl;
    std::cout << "    - Caps Lock may be on" << std::endl;
    console.resetColor();
    
    console.pauseScreen();
    return false;
}

bool AuthModule::loginAsDoctor() {
    console.clearScreen();
    console.printHeader("DOCTOR LOGIN");
    
    console.setColor(CYAN);
    std::cout << "\n  Login as Doctor to access your account.\n" << std::endl;
    console.resetColor();
    
    console.setColor(DARK_GRAY);
    std::cout << "  ================================================" << std::endl;
    std::cout << "  ENTER YOUR CREDENTIALS" << std::endl;
    std::cout << "  ================================================\n" << std::endl;
    console.resetColor();
    
    console.setColor(YELLOW);
    std::cout << "  >> Enter your email address: ";
    console.resetColor();
    
    if (std::cin.peek() == '\n') std::cin.ignore();
    std::string email;
    std::getline(std::cin, email);
    
    if (email.empty() || !console.isValidEmail(email)) {
        console.printError("Invalid email format!");
        console.pauseScreen();
        return false;
    }
    
    console.setColor(YELLOW);
    std::cout << "  >> Enter your password: ";
    console.resetColor();
    std::string password = console.getPasswordInput();
    
    if (password.empty()) {
        console.printError("Password cannot be empty!");
        console.pauseScreen();
        return false;
    }
    
    console.showLoading("\n  Verifying credentials", 2);
    
    int doctorID = db.loginDoctor(email, password);
    if (doctorID > 0) {
        Doctor doctor = db.getDoctorById(doctorID);
        session.userID = doctorID;
        session.userName = doctor.doctorName;
        session.userEmail = doctor.email;
        session.userType = "Doctor";
        session.isLoggedIn = true;
        
        console.setColor(GREEN);
        std::cout << "\n  +-----------------------------------------+" << std::endl;
        std::cout << "  |          LOGIN SUCCESSFUL!              |" << std::endl;
        std::cout << "  +-----------------------------------------+" << std::endl;
        console.resetColor();
        
        console.setColor(WHITE);
        std::cout << "\n  Welcome, Dr. " << session.userName << "!" << std::endl;
        std::cout << "  You are logged in as: DOCTOR" << std::endl;
        console.resetColor();
        
        db.logActivity("Doctor", doctorID, "Login", "Successful login");
        Sleep(1500);
        return true;
    }
    
    console.setColor(RED);
    std::cout << "\n  +-----------------------------------------+" << std::endl;
    std::cout << "  |            LOGIN FAILED!                |" << std::endl;
    std::cout << "  +-----------------------------------------+" << std::endl;
    console.resetColor();
    
    std::cout << std::endl;
    console.setColor(WHITE);
    std::cout << "  Invalid email or password for Doctor account." << std::endl;
    std::cout << "\n  Possible reasons:" << std::endl;
    std::cout << "    - Email is not registered as Doctor" << std::endl;
    std::cout << "    - Password is incorrect" << std::endl;
    std::cout << "    - Caps Lock may be on" << std::endl;
    console.resetColor();
    
    console.pauseScreen();
    return false;
}

bool AuthModule::loginAsStaff() {
    console.clearScreen();
    console.printHeader("STAFF LOGIN");
    
    console.setColor(CYAN);
    std::cout << "\n  Login as Staff to access your account.\n" << std::endl;
    console.resetColor();
    
    console.setColor(DARK_GRAY);
    std::cout << "  ================================================" << std::endl;
    std::cout << "  ENTER YOUR CREDENTIALS" << std::endl;
    std::cout << "  ================================================\n" << std::endl;
    console.resetColor();
    
    console.setColor(YELLOW);
    std::cout << "  >> Enter your email address: ";
    console.resetColor();
    
    if (std::cin.peek() == '\n') std::cin.ignore();
    std::string email;
    std::getline(std::cin, email);
    
    if (email.empty() || !console.isValidEmail(email)) {
        console.printError("Invalid email format!");
        console.pauseScreen();
        return false;
    }
    
    console.setColor(YELLOW);
    std::cout << "  >> Enter your password: ";
    console.resetColor();
    std::string password = console.getPasswordInput();
    
    if (password.empty()) {
        console.printError("Password cannot be empty!");
        console.pauseScreen();
        return false;
    }
    
    console.showLoading("\n  Verifying credentials", 2);
    
    int staffID = db.loginStaff(email, password);
    if (staffID > 0) {
        Staff staff = db.getStaffById(staffID);
        session.userID = staffID;
        session.userName = staff.staffName;
        session.userEmail = staff.email;
        session.userType = "Staff";
        session.isLoggedIn = true;
        
        console.setColor(GREEN);
        std::cout << "\n  +-----------------------------------------+" << std::endl;
        std::cout << "  |          LOGIN SUCCESSFUL!              |" << std::endl;
        std::cout << "  +-----------------------------------------+" << std::endl;
        console.resetColor();
        
        console.setColor(WHITE);
        std::cout << "\n  Welcome, " << session.userName << "!" << std::endl;
        std::cout << "  You are logged in as: STAFF" << std::endl;
        console.resetColor();
        
        db.logActivity("Staff", staffID, "Login", "Successful login");
        Sleep(1500);
        return true;
    }
    
    console.setColor(RED);
    std::cout << "\n  +-----------------------------------------+" << std::endl;
    std::cout << "  |            LOGIN FAILED!                |" << std::endl;
    std::cout << "  +-----------------------------------------+" << std::endl;
    console.resetColor();
    
    std::cout << std::endl;
    console.setColor(WHITE);
    std::cout << "  Invalid email or password for Staff account." << std::endl;
    std::cout << "\n  Possible reasons:" << std::endl;
    std::cout << "    - Email is not registered as Staff" << std::endl;
    std::cout << "    - Password is incorrect" << std::endl;
    std::cout << "    - Caps Lock may be on" << std::endl;
    console.resetColor();
    
    console.pauseScreen();
    return false;
}

bool AuthModule::loginAsAdmin() {
    console.clearScreen();
    console.printHeader("ADMIN LOGIN");
    
    console.setColor(CYAN);
    std::cout << "\n  Login as Admin to access your account.\n" << std::endl;
    console.resetColor();
    
    console.setColor(DARK_GRAY);
    std::cout << "  ================================================" << std::endl;
    std::cout << "  ENTER YOUR CREDENTIALS" << std::endl;
    std::cout << "  ================================================\n" << std::endl;
    console.resetColor();
    
    console.setColor(YELLOW);
    std::cout << "  >> Enter your email address: ";
    console.resetColor();
    
    if (std::cin.peek() == '\n') std::cin.ignore();
    std::string email;
    std::getline(std::cin, email);
    
    if (email.empty() || !console.isValidEmail(email)) {
        console.printError("Invalid email format!");
        console.pauseScreen();
        return false;
    }
    
    console.setColor(YELLOW);
    std::cout << "  >> Enter your password: ";
    console.resetColor();
    std::string password = console.getPasswordInput();
    
    if (password.empty()) {
        console.printError("Password cannot be empty!");
        console.pauseScreen();
        return false;
    }
    
    console.showLoading("\n  Verifying credentials", 2);
    
    int adminID = db.loginAdmin(email, password);
    if (adminID > 0) {
        Admin admin = db.getAdminById(adminID);
        session.userID = adminID;
        session.userName = admin.adminName;
        session.userEmail = admin.email;
        session.userType = "Admin";
        session.isLoggedIn = true;
        
        console.setColor(GREEN);
        std::cout << "\n  +-----------------------------------------+" << std::endl;
        std::cout << "  |          LOGIN SUCCESSFUL!              |" << std::endl;
        std::cout << "  +-----------------------------------------+" << std::endl;
        console.resetColor();
        
        console.setColor(WHITE);
        std::cout << "\n  Welcome, " << session.userName << "!" << std::endl;
        std::cout << "  You are logged in as: ADMIN" << std::endl;
        console.resetColor();
        
        db.logActivity("Admin", adminID, "Login", "Successful login");
        Sleep(1500);
        return true;
    }
    
    console.setColor(RED);
    std::cout << "\n  +-----------------------------------------+" << std::endl;
    std::cout << "  |            LOGIN FAILED!                |" << std::endl;
    std::cout << "  +-----------------------------------------+" << std::endl;
    console.resetColor();
    
    std::cout << std::endl;
    console.setColor(WHITE);
    std::cout << "  Invalid email or password for Admin account." << std::endl;
    std::cout << "\n  Possible reasons:" << std::endl;
    std::cout << "    - Email is not registered as Admin" << std::endl;
    std::cout << "    - Password is incorrect" << std::endl;
    std::cout << "    - Caps Lock may be on" << std::endl;
    console.resetColor();
    
    console.pauseScreen();
    return false;
}

void AuthModule::logout() {
    if (session.isLoggedIn) {
        db.logActivity(session.userType, session.userID, "Logout", "User logged out");
    }
    session = UserSession();
}

bool AuthModule::isLoggedIn() const {
    return session.isLoggedIn;
}

std::string AuthModule::getUserType() const {
    return session.userType;
}
