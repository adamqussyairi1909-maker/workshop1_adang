// ============================================================
// AdminModule.cpp - Admin Dashboard Implementation
// Hospital Appointment Booking System
// ============================================================

#include "../include/AdminModule.h"
#include "../include/Utilities.h"
#include "../include/DatabaseManager.h"
#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <windows.h>
#undef max

// OOP: Constructor calls base class constructor
AdminModule::AdminModule(ConsoleUtils& c, DatabaseManager& d, UserSession& s)
    : BaseModule(c, d, s) {}

void AdminModule::managePatients() {
    while (true) {
        console.clearScreen();
        console.printHeader("MANAGE PATIENTS");
        
        console.setColor(WHITE);
        std::cout << "\n  Add, view, or manage patient records.\n" << std::endl;
        console.resetColor();
        
        console.printMenuOption(1, "View All Patients   - List all patient records");
        console.printMenuOption(2, "Search Patient      - Find specific patient");
        console.printMenuOption(3, "Delete Patient      - Remove patient record");
        console.printMenuOption(4, "Go Back             - Return to dashboard");
        
        std::cout << std::endl;
        console.setColor(WHITE);
        std::cout << "  Enter a number (1-4) to select." << std::endl;
        console.resetColor();
        int choice = console.getIntInput("  Your choice: ", 1, 4);
        
        if (choice == 4) return;
        
        if (choice == 1) {
            viewAllPatients();
        } else if (choice == 2) {
            searchPatient();
        } else if (choice == 3) {
            deletePatient();
        }
    }
}

void AdminModule::viewAllPatients() {
    console.clearScreen();
    console.printHeader("ALL PATIENTS");
    
    std::vector<Patient> patients = db.searchPatients("");
    
    if (patients.empty()) {
        console.printInfo("No patients registered in the system.");
        console.pauseScreen();
        return;
    }
    
    console.setColor(WHITE);
    std::cout << "\n  Total: " << patients.size() << " patient(s) registered.\n" << std::endl;
    console.resetColor();
    
    console.setColor(DARK_CYAN);
    std::cout << "  " << std::left 
              << std::setw(5) << "ID"
              << std::setw(25) << "Name"
              << std::setw(15) << "Phone"
              << std::setw(25) << "Email" << std::endl;
    std::cout << "  " << std::string(70, '-') << std::endl;
    console.resetColor();
    
    for (const auto& p : patients) {
        std::cout << "  " << std::setw(5) << p.patientID
                  << std::setw(25) << p.patientName
                  << std::setw(15) << p.phoneNumber
                  << std::setw(25) << p.email << std::endl;
    }
    
    console.pauseScreen();
}

void AdminModule::searchPatient() {
    console.clearScreen();
    console.printHeader("SEARCH PATIENT");
    
    std::cin.ignore(10000, '\n');
    
    console.setColor(WHITE);
    std::cout << "\n  Search for patients by name, email, or phone." << std::endl;
    std::cout << "  Enter partial or full text to search.\n" << std::endl;
    console.resetColor();
    
    std::string search = console.getStringInput("  Search term: ");
    
    console.showLoading("\n  Searching", 2);
    
    std::vector<Patient> results = db.searchPatients(search);
    
    if (results.empty()) {
        console.printInfo("No patients found matching: " + search);
        console.pauseScreen();
        return;
    }
    
    console.setColor(GREEN);
    std::cout << "\n  Found " << results.size() << " patient(s):\n" << std::endl;
    console.resetColor();
    
    console.setColor(DARK_CYAN);
    std::cout << "  " << std::left 
              << std::setw(5) << "ID"
              << std::setw(25) << "Name"
              << std::setw(15) << "Phone"
              << std::setw(25) << "Email" << std::endl;
    std::cout << "  " << std::string(70, '-') << std::endl;
    console.resetColor();
    
    for (const auto& p : results) {
        std::cout << "  " << std::setw(5) << p.patientID
                  << std::setw(25) << p.patientName
                  << std::setw(15) << p.phoneNumber
                  << std::setw(25) << p.email << std::endl;
    }
    
    console.pauseScreen();
}

void AdminModule::deletePatient() {
    console.clearScreen();
    console.printHeader("DELETE PATIENT");
    
    console.setColor(RED);
    std::cout << "\n  WARNING: Deleting a patient will also delete all their appointments!" << std::endl;
    console.resetColor();
    
    console.setColor(WHITE);
    std::cout << "\n  Enter the Patient ID to delete.\n" << std::endl;
    console.resetColor();
    
    console.setColor(YELLOW);
    std::cout << "  >> Enter Patient ID (or 0 to cancel):" << std::endl;
    console.resetColor();
    int patientID = console.getIntInput("     Patient ID: ", 0, 99999);
    
    if (patientID == 0) return;
    
    Patient p = db.getPatientById(patientID);
    
    if (p.patientID == 0) {
        console.printError("Patient ID " + std::to_string(patientID) + " not found!");
        console.pauseScreen();
        return;
    }
    
    console.setColor(WHITE);
    std::cout << "\n  PATIENT TO DELETE:" << std::endl;
    std::cout << "  " << std::string(40, '-') << std::endl;
    std::cout << "  ID    : " << p.patientID << std::endl;
    std::cout << "  Name  : " << p.patientName << std::endl;
    std::cout << "  Email : " << p.email << std::endl;
    std::cout << "  Phone : " << p.phoneNumber << std::endl;
    std::cout << "  " << std::string(40, '-') << std::endl;
    console.resetColor();
    
    console.setColor(RED);
    std::cout << "\n  This action CANNOT be undone!" << std::endl;
    console.resetColor();
    
    std::cin.ignore(10000, '\n');
    std::string confirm = console.getStringInput("  Type DELETE to confirm: ");
    
    if (confirm == "DELETE") {
        if (db.deletePatient(patientID)) {
            console.printSuccess("Patient deleted successfully!");
            db.logActivity("Admin", session.userID, "Delete Patient", 
                          "Deleted ID: " + std::to_string(patientID));
        } else {
            console.printError("Failed to delete patient.");
        }
    } else {
        console.printInfo("Deletion cancelled.");
    }
    
    console.pauseScreen();
}

void AdminModule::manageDoctors() {
    while (true) {
        console.clearScreen();
        console.printHeader("MANAGE DOCTORS");
        
        console.setColor(WHITE);
        std::cout << "\n  Add, view, or manage doctor accounts.\n" << std::endl;
        console.resetColor();
        
        console.printMenuOption(1, "View All Doctors    - List all doctors");
        console.printMenuOption(2, "Add New Doctor      - Create doctor account");
        console.printMenuOption(3, "Update Doctor       - Edit doctor details");
        console.printMenuOption(4, "Delete Doctor       - Remove doctor account");
        console.printMenuOption(5, "Go Back             - Return to dashboard");
        
        std::cout << std::endl;
        console.setColor(WHITE);
        std::cout << "  Enter a number (1-5) to select." << std::endl;
        console.resetColor();
        int choice = console.getIntInput("  Your choice: ", 1, 5);
        
        if (choice == 5) return;
        
        if (choice == 1) {
            viewAllDoctors();
        } else if (choice == 2) {
            addDoctor();
        } else if (choice == 3) {
            updateDoctor();
        } else if (choice == 4) {
            deleteDoctor();
        }
    }
}

void AdminModule::viewAllDoctors() {
    console.clearScreen();
    console.printHeader("ALL DOCTORS");
    
    std::vector<Doctor> doctors = db.getAllDoctors(false);
    
    if (doctors.empty()) {
        console.printInfo("No doctors in the system.");
        console.pauseScreen();
        return;
    }
    
    console.setColor(WHITE);
    std::cout << "\n  Total: " << doctors.size() << " doctor(s) registered.\n" << std::endl;
    console.resetColor();
    
    console.setColor(DARK_CYAN);
    std::cout << "  " << std::left 
              << std::setw(5) << "ID"
              << std::setw(25) << "Name"
              << std::setw(20) << "Specialty"
              << std::setw(10) << "Room"
              << std::setw(12) << "Status" << std::endl;
    std::cout << "  " << std::string(72, '-') << std::endl;
    console.resetColor();
    
    for (const auto& doc : doctors) {
        std::cout << "  " << std::setw(5) << doc.doctorID
                  << std::setw(25) << doc.doctorName
                  << std::setw(20) << doc.specialty
                  << std::setw(10) << doc.roomNo;
        if (doc.isAvailable) {
            console.setColor(GREEN);
            std::cout << std::setw(12) << "Available" << std::endl;
        } else {
            console.setColor(RED);
            std::cout << std::setw(12) << "Unavailable" << std::endl;
        }
        console.resetColor();
    }
    
    console.pauseScreen();
}

void AdminModule::addDoctor() {
    console.clearScreen();
    console.printHeader("ADD NEW DOCTOR");
    
    std::cin.ignore(10000, '\n');
    
    console.setColor(WHITE);
    std::cout << "\n  Create a new doctor account." << std::endl;
    std::cout << "  All fields are required.\n" << std::endl;
    console.resetColor();
    
    std::string name, specialty, room, phone, email, password;
    
    console.setColor(DARK_GRAY);
    std::cout << "  ------------------------------------------------" << std::endl;
    std::cout << "  DOCTOR INFORMATION" << std::endl;
    std::cout << "  ------------------------------------------------\n" << std::endl;
    console.resetColor();
    
    console.setColor(WHITE);
    std::cout << "  Enter doctor's full name (with title if applicable)." << std::endl;
    std::cout << "  Example: Dr. Ahmad bin Abdullah" << std::endl;
    console.resetColor();
    name = console.getStringInput("  Full Name: ");
    
    std::cout << std::endl;
    console.setColor(WHITE);
    std::cout << "  Enter medical specialty." << std::endl;
    std::cout << "  Examples: General, Cardiology, Pediatrics, Orthopedics" << std::endl;
    console.resetColor();
    specialty = console.getStringInput("  Specialty: ");
    
    std::cout << std::endl;
    console.setColor(WHITE);
    std::cout << "  Enter room number/code." << std::endl;
    std::cout << "  Example: A101, B205, C-312" << std::endl;
    console.resetColor();
    room = console.getStringInput("  Room No: ");
    
    std::cout << std::endl;
    console.setColor(WHITE);
    std::cout << "  Enter phone number (10-12 digits)." << std::endl;
    console.resetColor();
    do {
        phone = console.getStringInput("  Phone: ");
        if (!console.isValidPhone(phone)) {
            console.printError("Invalid format! Use 10-12 digits.");
        }
    } while (!console.isValidPhone(phone));
    
    std::cout << std::endl;
    console.setColor(WHITE);
    std::cout << "  Enter email address (for login)." << std::endl;
    console.resetColor();
    do {
        email = console.getStringInput("  Email: ");
        if (!console.isValidEmail(email)) {
            console.printError("Invalid format! Include @ and domain.");
        }
    } while (!console.isValidEmail(email));
    
    std::cout << std::endl;
    console.setColor(DARK_GRAY);
    std::cout << "  ------------------------------------------------" << std::endl;
    std::cout << "  CREATE LOGIN PASSWORD" << std::endl;
    std::cout << "  ------------------------------------------------\n" << std::endl;
    console.resetColor();
    
    console.setColor(WHITE);
    std::cout << "  Create a password for the doctor (min 6 characters)." << std::endl;
    console.resetColor();
    do {
        console.setColor(CYAN);
        std::cout << "  Password: ";
        console.resetColor();
        password = console.getPasswordInput();
        if (password.length() < 6) {
            console.printError("Too short! Minimum 6 characters.");
        }
    } while (password.length() < 6);
    
    std::cout << std::endl;
    console.setColor(WHITE);
    std::cout << "  REVIEW NEW DOCTOR:" << std::endl;
    std::cout << "  " << std::string(40, '-') << std::endl;
    std::cout << "  Name      : " << name << std::endl;
    std::cout << "  Specialty : " << specialty << std::endl;
    std::cout << "  Room      : " << room << std::endl;
    std::cout << "  Phone     : " << phone << std::endl;
    std::cout << "  Email     : " << email << std::endl;
    std::cout << "  " << std::string(40, '-') << std::endl;
    console.resetColor();
    
    std::string confirm = console.getStringInput("\n  Create this doctor account? (Y/N): ");
    
    if (confirm == "Y" || confirm == "y") {
        if (db.addDoctor(name, specialty, room, phone, email, password)) {
            console.printSuccess("Doctor account created successfully!");
            db.logActivity("Admin", session.userID, "Add Doctor", "Added: " + name);
        } else {
            console.printError("Failed to create. Email/phone may already exist.");
        }
    } else {
        console.printInfo("Operation cancelled.");
    }
    
    console.pauseScreen();
}

void AdminModule::updateDoctor() {
    console.clearScreen();
    console.printHeader("UPDATE DOCTOR");
    
    console.setColor(WHITE);
    std::cout << "\n  Enter the Doctor ID to update.\n" << std::endl;
    console.resetColor();
    
    console.setColor(YELLOW);
    std::cout << "  >> Enter Doctor ID (or 0 to cancel):" << std::endl;
    console.resetColor();
    int doctorID = console.getIntInput("     Doctor ID: ", 0, 99999);
    
    if (doctorID == 0) return;
    
    Doctor doc = db.getDoctorById(doctorID);
    
    if (doc.doctorID == 0) {
        console.printError("Doctor ID " + std::to_string(doctorID) + " not found!");
        console.pauseScreen();
        return;
    }
    
    console.setColor(WHITE);
    std::cout << "\n  CURRENT INFORMATION:" << std::endl;
    std::cout << "  " << std::string(40, '-') << std::endl;
    std::cout << "  Name      : " << doc.doctorName << std::endl;
    std::cout << "  Specialty : " << doc.specialty << std::endl;
    std::cout << "  Room      : " << doc.roomNo << std::endl;
    std::cout << "  Phone     : " << doc.phoneNumber << std::endl;
    std::cout << "  Email     : " << doc.email << std::endl;
    std::cout << "  " << std::string(40, '-') << std::endl;
    console.resetColor();
    
    std::cout << std::endl;
    console.setColor(DARK_GRAY);
    std::cout << "  ------------------------------------------------" << std::endl;
    std::cout << "  ENTER NEW VALUES (press ENTER to keep current)" << std::endl;
    std::cout << "  ------------------------------------------------\n" << std::endl;
    console.resetColor();
    
    std::cin.ignore(10000, '\n');
    
    std::string name, specialty, room, phone;
    
    console.setColor(WHITE);
    std::cout << "  New name (ENTER to keep: " << doc.doctorName << ")" << std::endl;
    console.resetColor();
    console.setColor(CYAN);
    std::cout << "  Name: ";
    console.resetColor();
    std::getline(std::cin, name);
    if (name.empty()) name = doc.doctorName;
    
    std::cout << std::endl;
    console.setColor(WHITE);
    std::cout << "  New specialty (ENTER to keep: " << doc.specialty << ")" << std::endl;
    console.resetColor();
    console.setColor(CYAN);
    std::cout << "  Specialty: ";
    console.resetColor();
    std::getline(std::cin, specialty);
    if (specialty.empty()) specialty = doc.specialty;
    
    std::cout << std::endl;
    console.setColor(WHITE);
    std::cout << "  New room (ENTER to keep: " << doc.roomNo << ")" << std::endl;
    console.resetColor();
    console.setColor(CYAN);
    std::cout << "  Room: ";
    console.resetColor();
    std::getline(std::cin, room);
    if (room.empty()) room = doc.roomNo;
    
    std::cout << std::endl;
    console.setColor(WHITE);
    std::cout << "  New phone (ENTER to keep: " << doc.phoneNumber << ")" << std::endl;
    console.resetColor();
    do {
        console.setColor(CYAN);
        std::cout << "  Phone: ";
        console.resetColor();
        std::getline(std::cin, phone);
        if (phone.empty()) {
            phone = doc.phoneNumber;
            break;
        }
        if (!console.isValidPhone(phone)) {
            console.printError("Invalid format! Use 10-12 digits.");
        }
    } while (!phone.empty() && !console.isValidPhone(phone));
    
    std::string confirm = console.getStringInput("\n  Save changes? (Y/N): ");
    
    if (confirm == "Y" || confirm == "y") {
        if (db.updateDoctor(doctorID, name, specialty, room, phone)) {
            console.printSuccess("Doctor updated successfully!");
            db.logActivity("Admin", session.userID, "Update Doctor", 
                          "Updated ID: " + std::to_string(doctorID));
        } else {
            console.printError("Failed to update doctor.");
        }
    } else {
        console.printInfo("Changes discarded.");
    }
    
    console.pauseScreen();
}

void AdminModule::deleteDoctor() {
    console.clearScreen();
    console.printHeader("DELETE DOCTOR");
    
    console.setColor(RED);
    std::cout << "\n  WARNING: Deleting a doctor will cancel all their appointments!" << std::endl;
    console.resetColor();
    
    console.setColor(WHITE);
    std::cout << "\n  Enter the Doctor ID to delete.\n" << std::endl;
    console.resetColor();
    
    console.setColor(YELLOW);
    std::cout << "  >> Enter Doctor ID (or 0 to cancel):" << std::endl;
    console.resetColor();
    int doctorID = console.getIntInput("     Doctor ID: ", 0, 99999);
    
    if (doctorID == 0) return;
    
    Doctor doc = db.getDoctorById(doctorID);
    
    if (doc.doctorID == 0) {
        console.printError("Doctor ID " + std::to_string(doctorID) + " not found!");
        console.pauseScreen();
        return;
    }
    
    console.setColor(WHITE);
    std::cout << "\n  DOCTOR TO DELETE:" << std::endl;
    std::cout << "  " << std::string(40, '-') << std::endl;
    std::cout << "  ID        : " << doc.doctorID << std::endl;
    std::cout << "  Name      : " << doc.doctorName << std::endl;
    std::cout << "  Specialty : " << doc.specialty << std::endl;
    std::cout << "  Room      : " << doc.roomNo << std::endl;
    std::cout << "  " << std::string(40, '-') << std::endl;
    console.resetColor();
    
    console.setColor(RED);
    std::cout << "\n  This action CANNOT be undone!" << std::endl;
    console.resetColor();
    
    std::cin.ignore(10000, '\n');
    std::string confirm = console.getStringInput("  Type DELETE to confirm: ");
    
    if (confirm == "DELETE") {
        if (db.deleteDoctor(doctorID)) {
            console.printSuccess("Doctor deleted successfully!");
            db.logActivity("Admin", session.userID, "Delete Doctor", 
                          "Deleted ID: " + std::to_string(doctorID));
        } else {
            console.printError("Failed to delete doctor.");
        }
    } else {
        console.printInfo("Deletion cancelled.");
    }
    
    console.pauseScreen();
}

void AdminModule::manageStaff() {
    while (true) {
        console.clearScreen();
        console.printHeader("MANAGE STAFF");
        
        console.setColor(WHITE);
        std::cout << "\n  Add, view, or manage staff accounts.\n" << std::endl;
        console.resetColor();
        
        console.printMenuOption(1, "View All Staff      - List all staff");
        console.printMenuOption(2, "Add New Staff       - Create staff account");
        console.printMenuOption(3, "Delete Staff        - Remove staff account");
        console.printMenuOption(4, "Go Back             - Return to dashboard");
        
        std::cout << std::endl;
        console.setColor(WHITE);
        std::cout << "  Enter a number (1-4) to select." << std::endl;
        console.resetColor();
        int choice = console.getIntInput("  Your choice: ", 1, 4);
        
        if (choice == 4) return;
        
        if (choice == 1) {
            viewAllStaff();
        } else if (choice == 2) {
            addStaff();
        } else if (choice == 3) {
            deleteStaff();
        }
    }
}

void AdminModule::viewAllStaff() {
    console.clearScreen();
    console.printHeader("ALL STAFF");
    
    std::vector<Staff> staffList = db.getAllStaff();
    
    if (staffList.empty()) {
        console.printInfo("No staff in the system.");
        console.pauseScreen();
        return;
    }
    
    console.setColor(WHITE);
    std::cout << "\n  Total: " << staffList.size() << " staff member(s).\n" << std::endl;
    console.resetColor();
    
    console.setColor(DARK_CYAN);
    std::cout << "  " << std::left 
              << std::setw(5) << "ID"
              << std::setw(25) << "Name"
              << std::setw(20) << "Department"
              << std::setw(25) << "Email" << std::endl;
    std::cout << "  " << std::string(75, '-') << std::endl;
    console.resetColor();
    
    for (const auto& s : staffList) {
        std::cout << "  " << std::setw(5) << s.staffID
                  << std::setw(25) << s.staffName
                  << std::setw(20) << s.department
                  << std::setw(25) << s.email << std::endl;
    }
    
    console.pauseScreen();
}

void AdminModule::addStaff() {
    console.clearScreen();
    console.printHeader("ADD NEW STAFF");
    
    std::cin.ignore(10000, '\n');
    
    console.setColor(WHITE);
    std::cout << "\n  Create a new staff account." << std::endl;
    std::cout << "  All fields are required.\n" << std::endl;
    console.resetColor();
    
    std::string name, department, phone, email, password;
    
    console.setColor(DARK_GRAY);
    std::cout << "  ------------------------------------------------" << std::endl;
    std::cout << "  STAFF INFORMATION" << std::endl;
    std::cout << "  ------------------------------------------------\n" << std::endl;
    console.resetColor();
    
    console.setColor(WHITE);
    std::cout << "  Enter staff's full name." << std::endl;
    console.resetColor();
    name = console.getStringInput("  Full Name: ");
    
    std::cout << std::endl;
    console.setColor(WHITE);
    std::cout << "  Enter department." << std::endl;
    std::cout << "  Examples: Reception, Administration, Records" << std::endl;
    console.resetColor();
    department = console.getStringInput("  Department: ");
    
    std::cout << std::endl;
    console.setColor(WHITE);
    std::cout << "  Enter phone number (10-12 digits)." << std::endl;
    console.resetColor();
    do {
        phone = console.getStringInput("  Phone: ");
        if (!console.isValidPhone(phone)) {
            console.printError("Invalid format! Use 10-12 digits.");
        }
    } while (!console.isValidPhone(phone));
    
    std::cout << std::endl;
    console.setColor(WHITE);
    std::cout << "  Enter email address (for login)." << std::endl;
    console.resetColor();
    do {
        email = console.getStringInput("  Email: ");
        if (!console.isValidEmail(email)) {
            console.printError("Invalid format! Include @ and domain.");
        }
    } while (!console.isValidEmail(email));
    
    std::cout << std::endl;
    console.setColor(DARK_GRAY);
    std::cout << "  ------------------------------------------------" << std::endl;
    std::cout << "  CREATE LOGIN PASSWORD" << std::endl;
    std::cout << "  ------------------------------------------------\n" << std::endl;
    console.resetColor();
    
    console.setColor(WHITE);
    std::cout << "  Create a password (min 6 characters)." << std::endl;
    console.resetColor();
    do {
        console.setColor(CYAN);
        std::cout << "  Password: ";
        console.resetColor();
        password = console.getPasswordInput();
        if (password.length() < 6) {
            console.printError("Too short! Minimum 6 characters.");
        }
    } while (password.length() < 6);
    
    std::cout << std::endl;
    console.setColor(WHITE);
    std::cout << "  REVIEW NEW STAFF:" << std::endl;
    std::cout << "  " << std::string(40, '-') << std::endl;
    std::cout << "  Name       : " << name << std::endl;
    std::cout << "  Department : " << department << std::endl;
    std::cout << "  Phone      : " << phone << std::endl;
    std::cout << "  Email      : " << email << std::endl;
    std::cout << "  " << std::string(40, '-') << std::endl;
    console.resetColor();
    
    std::string confirm = console.getStringInput("\n  Create this staff account? (Y/N): ");
    
    if (confirm == "Y" || confirm == "y") {
        if (db.addStaff(name, department, phone, email, password)) {
            console.printSuccess("Staff account created successfully!");
            db.logActivity("Admin", session.userID, "Add Staff", "Added: " + name);
        } else {
            console.printError("Failed to create. Email/phone may already exist.");
        }
    } else {
        console.printInfo("Operation cancelled.");
    }
    
    console.pauseScreen();
}

void AdminModule::deleteStaff() {
    console.clearScreen();
    console.printHeader("DELETE STAFF");
    
    console.setColor(WHITE);
    std::cout << "\n  Enter the Staff ID to delete.\n" << std::endl;
    console.resetColor();
    
    console.setColor(YELLOW);
    std::cout << "  >> Enter Staff ID (or 0 to cancel):" << std::endl;
    console.resetColor();
    int staffID = console.getIntInput("     Staff ID: ", 0, 99999);
    
    if (staffID == 0) return;
    
    Staff s = db.getStaffById(staffID);
    
    if (s.staffID == 0) {
        console.printError("Staff ID " + std::to_string(staffID) + " not found!");
        console.pauseScreen();
        return;
    }
    
    console.setColor(WHITE);
    std::cout << "\n  STAFF TO DELETE:" << std::endl;
    std::cout << "  " << std::string(40, '-') << std::endl;
    std::cout << "  ID         : " << s.staffID << std::endl;
    std::cout << "  Name       : " << s.staffName << std::endl;
    std::cout << "  Department : " << s.department << std::endl;
    std::cout << "  Email      : " << s.email << std::endl;
    std::cout << "  " << std::string(40, '-') << std::endl;
    console.resetColor();
    
    console.setColor(RED);
    std::cout << "\n  This action CANNOT be undone!" << std::endl;
    console.resetColor();
    
    std::cin.ignore(10000, '\n');
    std::string confirm = console.getStringInput("  Type DELETE to confirm: ");
    
    if (confirm == "DELETE") {
        if (db.deleteStaff(staffID)) {
            console.printSuccess("Staff deleted successfully!");
            db.logActivity("Admin", session.userID, "Delete Staff", 
                          "Deleted ID: " + std::to_string(staffID));
        } else {
            console.printError("Failed to delete staff.");
        }
    } else {
        console.printInfo("Deletion cancelled.");
    }
    
    console.pauseScreen();
}

void AdminModule::viewSystemLogs() {
    console.clearScreen();
    console.printHeader("SYSTEM ACTIVITY LOGS");
    
    console.setColor(WHITE);
    std::cout << "\n  Recent system activities (last 50 entries).\n" << std::endl;
    console.resetColor();
    
    std::vector<ActivityLog> logs = db.getActivityLogs(50);
    
    if (logs.empty()) {
        console.printInfo("No activity logs found.");
        console.pauseScreen();
        return;
    }
    
    console.setColor(DARK_CYAN);
    std::cout << "  " << std::left 
              << std::setw(12) << "User Type"
              << std::setw(6) << "ID"
              << std::setw(20) << "Action"
              << std::setw(30) << "Details" << std::endl;
    std::cout << "  " << std::string(68, '-') << std::endl;
    console.resetColor();
    
    for (const auto& log : logs) {
        std::string shortDetails = log.details.length() > 28 ?
            log.details.substr(0, 25) + "..." : log.details;
        
        std::cout << "  " << std::setw(12) << log.userType
                  << std::setw(6) << log.userID
                  << std::setw(20) << log.action
                  << std::setw(30) << shortDetails << std::endl;
    }
    
    std::cout << "\n  Showing " << logs.size() << " most recent activities." << std::endl;
    
    console.pauseScreen();
}

void AdminModule::systemStatistics() {
    console.clearScreen();
    console.printHeader("SYSTEM STATISTICS");
    
    console.showLoading("\n  Generating statistics", 2);
    
    std::vector<Patient> patients = db.searchPatients("");
    std::vector<Doctor> doctors = db.getAllDoctors(false);
    std::vector<Staff> staff = db.getAllStaff();
    std::vector<Appointment> appointments = db.getAllAppointments();
    
    int pending = 0, confirmed = 0, completed = 0, cancelled = 0;
    for (const auto& apt : appointments) {
        if (apt.status == "Pending") pending++;
        else if (apt.status == "Confirmed") confirmed++;
        else if (apt.status == "Completed") completed++;
        else if (apt.status == "Cancelled") cancelled++;
    }
    
    int availableDoctors = 0;
    for (const auto& doc : doctors) {
        if (doc.isAvailable) availableDoctors++;
    }
    
    std::cout << std::endl;
    console.setColor(DARK_GRAY);
    std::cout << "  ------------------------------------------------" << std::endl;
    std::cout << "  USER STATISTICS" << std::endl;
    std::cout << "  ------------------------------------------------\n" << std::endl;
    console.resetColor();
    
    console.setColor(WHITE);
    std::cout << "  Total Patients        : " << patients.size() << std::endl;
    std::cout << "  Total Doctors         : " << doctors.size() << std::endl;
    std::cout << "  Available Doctors     : " << availableDoctors << std::endl;
    std::cout << "  Total Staff           : " << staff.size() << std::endl;
    console.resetColor();
    
    std::cout << std::endl;
    console.setColor(DARK_GRAY);
    std::cout << "  ------------------------------------------------" << std::endl;
    std::cout << "  APPOINTMENT STATISTICS" << std::endl;
    std::cout << "  ------------------------------------------------\n" << std::endl;
    console.resetColor();
    
    console.setColor(WHITE);
    std::cout << "  Total Appointments    : " << appointments.size() << std::endl;
    console.setColor(YELLOW);
    std::cout << "  Pending               : " << pending << std::endl;
    console.setColor(GREEN);
    std::cout << "  Confirmed             : " << confirmed << std::endl;
    console.setColor(CYAN);
    std::cout << "  Completed             : " << completed << std::endl;
    console.setColor(RED);
    std::cout << "  Cancelled             : " << cancelled << std::endl;
    console.resetColor();
    
    if (appointments.size() > 0) {
        double completionRate = (completed * 100.0) / appointments.size();
        double cancellationRate = (cancelled * 100.0) / appointments.size();
        
        std::cout << std::endl;
        console.setColor(DARK_GRAY);
        std::cout << "  ------------------------------------------------" << std::endl;
        std::cout << "  PERFORMANCE METRICS" << std::endl;
        std::cout << "  ------------------------------------------------\n" << std::endl;
        console.resetColor();
        
        console.setColor(WHITE);
        std::cout << std::fixed << std::setprecision(1);
        std::cout << "  Completion Rate       : " << completionRate << "%" << std::endl;
        std::cout << "  Cancellation Rate     : " << cancellationRate << "%" << std::endl;
        console.resetColor();
    }
    
    // Grade A: Text-Based Bar Chart for Statistics
    std::cout << std::endl;
    console.setColor(DARK_GRAY);
    std::cout << "  ------------------------------------------------" << std::endl;
    std::cout << "  APPOINTMENT STATUS CHART" << std::endl;
    std::cout << "  ------------------------------------------------\n" << std::endl;
    console.resetColor();
    
    int maxCount = pending;
    if (confirmed > maxCount) maxCount = confirmed;
    if (completed > maxCount) maxCount = completed;
    if (cancelled > maxCount) maxCount = cancelled;
    if (maxCount > 0) {
        int scale = maxCount > 20 ? maxCount / 20 : 1;
        if (scale == 0) scale = 1;
        console.setColor(YELLOW);
        std::cout << "  Pending   : " << std::string(pending / scale, '*') << " (" << pending << ")" << std::endl;
        console.setColor(GREEN);
        std::cout << "  Confirmed : " << std::string(confirmed / scale, '*') << " (" << confirmed << ")" << std::endl;
        console.setColor(CYAN);
        std::cout << "  Completed : " << std::string(completed / scale, '*') << " (" << completed << ")" << std::endl;
        console.setColor(RED);
        std::cout << "  Cancelled : " << std::string(cancelled / scale, '*') << " (" << cancelled << ")" << std::endl;
        console.resetColor();
    }
    
    // Grade A: Doctor Statistics with SQL Aggregation
    std::vector<DatabaseManager::DoctorStats> doctorStats = db.getDoctorStatistics();
    if (!doctorStats.empty()) {
        std::cout << std::endl;
        console.setColor(DARK_GRAY);
        std::cout << "  ------------------------------------------------" << std::endl;
        std::cout << "  DOCTOR PERFORMANCE (SQL GROUP BY)" << std::endl;
        std::cout << "  ------------------------------------------------\n" << std::endl;
        console.resetColor();
        
        console.setColor(DARK_CYAN);
        std::cout << "  " << std::left 
                  << std::setw(25) << "Doctor Name"
                  << std::setw(15) << "Total"
                  << std::setw(15) << "Confirmed"
                  << std::setw(15) << "Rate %" << std::endl;
        std::cout << "  " << std::string(70, '-') << std::endl;
        console.resetColor();
        
        for (const auto& stat : doctorStats) {
            console.setColor(WHITE);
            std::cout << "  " << std::setw(25) << stat.doctorName
                      << std::setw(15) << stat.totalAppointments
                      << std::setw(15) << stat.confirmedCount
                      << std::setw(15) << std::fixed << std::setprecision(1) << stat.completionRate << std::endl;
        }
        console.resetColor();
    }
    
    db.logActivity("Admin", session.userID, "View Statistics", "System stats viewed");
    
    console.pauseScreen();
}

void AdminModule::showDashboard() {
    while (session.isLoggedIn) {
        console.clearScreen();
        console.printHeader("ADMIN DASHBOARD");
        
        console.setColor(CYAN);
        std::cout << "\n  Welcome back, " << session.userName << "!\n" << std::endl;
        console.resetColor();
        
        console.setColor(DARK_GRAY);
        std::cout << "  ================================================" << std::endl;
        std::cout << "  MENU" << std::endl;
        std::cout << "  ================================================" << std::endl;
        console.resetColor();
        
        console.printMenuOption(1, "Manage Patients");
        console.printMenuOption(2, "Manage Doctors");
        console.printMenuOption(3, "Manage Staff");
        console.printMenuOption(4, "System Logs");
        console.printMenuOption(5, "Statistics");
        console.printMenuOption(6, "Logout");
        
        std::cout << std::endl;
        console.setColor(YELLOW);
        std::cout << "  >> Enter your choice:" << std::endl;
        console.resetColor();
        int choice = console.getIntInput("     Your choice: ", 1, 6);
        
        switch (choice) {
            case 1: managePatients(); break;
            case 2: manageDoctors(); break;
            case 3: manageStaff(); break;
            case 4: viewSystemLogs(); break;
            case 5: systemStatistics(); break;
            case 6:
                db.logActivity("Admin", session.userID, "Logout", "User logged out");
                session = UserSession();
                console.printSuccess("You have been logged out successfully!");
                Sleep(1500);
                return;
        }
    }
}
