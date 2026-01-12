// ============================================================
// StaffModule.cpp - Staff Dashboard Implementation
// Hospital Appointment Booking System
// ============================================================

#include "../include/StaffModule.h"
#include "../include/Utilities.h"
#include "../include/DatabaseManager.h"
#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <windows.h>
#undef max

// OOP: Constructor calls base class constructor
StaffModule::StaffModule(ConsoleUtils& c, DatabaseManager& d, UserSession& s)
    : BaseModule(c, d, s) {}

void StaffModule::viewAllAppointments() {
    console.clearScreen();
    console.printHeader("ALL APPOINTMENTS");
    
    std::vector<Appointment> appointments = db.getAllAppointments();
    
    if (appointments.empty()) {
        console.printInfo("No appointments in the system.");
        console.pauseScreen();
        return;
    }
    
    console.setColor(WHITE);
    std::cout << "\n  Showing all appointments in the system.\n" << std::endl;
    console.resetColor();
    
    console.setColor(DARK_CYAN);
    std::cout << "  " << std::left 
              << std::setw(5) << "ID"
              << std::setw(16) << "Patient"
              << std::setw(16) << "Doctor"
              << std::setw(11) << "Date"
              << std::setw(7) << "Time"
              << std::setw(8) << "Cost"
              << std::setw(11) << "Status" << std::endl;
    std::cout << "  " << std::string(74, '-') << std::endl;
    console.resetColor();
    
    int pending = 0, confirmed = 0, completed = 0, cancelled = 0;
    double totalRevenue = 0.0;
    
    for (const auto& apt : appointments) {
        if (apt.status == "Confirmed") { console.setColor(GREEN); confirmed++; }
        else if (apt.status == "Pending") { console.setColor(YELLOW); pending++; }
        else if (apt.status == "Completed") { console.setColor(CYAN); completed++; totalRevenue += apt.totalCost; }
        else if (apt.status == "Cancelled") { console.setColor(RED); cancelled++; }
        else console.resetColor();
        
        std::string pName = apt.patientName.length() > 14 ? 
            apt.patientName.substr(0, 11) + "..." : apt.patientName;
        std::string dName = apt.doctorName.length() > 14 ? 
            apt.doctorName.substr(0, 11) + "..." : apt.doctorName;
        
        std::cout << "  " << std::setw(5) << apt.appointmentID
                  << std::setw(16) << pName
                  << std::setw(16) << dName
                  << std::setw(11) << apt.appointmentDate
                  << std::setw(7) << apt.appointmentTime.substr(0, 5)
                  << std::setw(8) << ("RM" + std::to_string((int)apt.totalCost))
                  << std::setw(11) << apt.status << std::endl;
    }
    console.resetColor();
    
    std::cout << "\n  " << std::string(74, '-') << std::endl;
    std::cout << "  SUMMARY: ";
    console.setColor(YELLOW); std::cout << pending << " Pending  ";
    console.setColor(GREEN); std::cout << confirmed << " Confirmed  ";
    console.setColor(CYAN); std::cout << completed << " Completed  ";
    console.setColor(RED); std::cout << cancelled << " Cancelled";
    console.resetColor();
    std::cout << "\n  Total Revenue (Completed): RM " << std::fixed << std::setprecision(2) << totalRevenue << std::endl;
    
    console.pauseScreen();
}

void StaffModule::approveAppointment() {
    console.clearScreen();
    console.printHeader("APPROVE APPOINTMENTS");
    
    std::vector<Appointment> pending = db.getPendingAppointments();
    
    if (pending.empty()) {
        console.printInfo("No pending appointments to approve.");
        console.setColor(WHITE);
        std::cout << "\n  All appointments have been processed!" << std::endl;
        console.resetColor();
        console.pauseScreen();
        return;
    }
    
    console.setColor(WHITE);
    std::cout << "\n  Review and approve patient appointment requests." << std::endl;
    std::cout << "  Select an appointment to approve.\n" << std::endl;
    console.resetColor();
    
    console.setColor(DARK_CYAN);
    std::cout << "  " << std::left 
              << std::setw(5) << "No."
              << std::setw(6) << "ID"
              << std::setw(18) << "Patient"
              << std::setw(18) << "Doctor"
              << std::setw(11) << "Date"
              << std::setw(8) << "Time" << std::endl;
    std::cout << "  " << std::string(66, '-') << std::endl;
    console.resetColor();
    
    for (int i = 0; i < (int)pending.size(); i++) {
        std::cout << "  " << std::setw(5) << (i + 1)
                  << std::setw(6) << pending[i].appointmentID
                  << std::setw(18) << pending[i].patientName
                  << std::setw(18) << pending[i].doctorName
                  << std::setw(11) << pending[i].appointmentDate
                  << std::setw(8) << pending[i].appointmentTime.substr(0, 5) << std::endl;
    }
    
    std::cout << std::endl;
    console.setColor(YELLOW);
    std::cout << "  >> Select an appointment to review (or enter 0 to go back):" << std::endl;
    console.resetColor();
    int choice = console.getIntInput("     Enter number (0-" + std::to_string(pending.size()) + "): ", 0, (int)pending.size());
    
    if (choice == 0) return;
    
    Appointment selected = pending[choice - 1];
    
    console.setColor(WHITE);
    std::cout << "\n  APPOINTMENT DETAILS:" << std::endl;
    std::cout << "  " << std::string(40, '-') << std::endl;
    std::cout << "  Patient          : " << selected.patientName << std::endl;
    std::cout << "  Doctor           : " << selected.doctorName << std::endl;
    std::cout << "  Date             : " << selected.appointmentDate << std::endl;
    std::cout << "  Time             : " << selected.appointmentTime.substr(0, 5) << std::endl;
    std::cout << "  Duration         : " << selected.duration << " minutes" << std::endl;
    std::cout << "  Reason           : " << selected.reason << std::endl;
    std::cout << "  " << std::string(40, '-') << std::endl;
    std::cout << "  Consultation Fee : RM " << std::fixed << std::setprecision(2) << selected.consultationFee << std::endl;
    std::cout << "  Medicine Fee     : RM " << std::fixed << std::setprecision(2) << selected.medicineFee << std::endl;
    console.setColor(GREEN);
    std::cout << "  TOTAL COST       : RM " << std::fixed << std::setprecision(2) << selected.totalCost << std::endl;
    console.resetColor();
    std::cout << "  " << std::string(40, '-') << std::endl;
    console.resetColor();
    
    std::cout << std::endl;
    console.setColor(DARK_GRAY);
    std::cout << "  ------------------------------------------------" << std::endl;
    std::cout << "  SELECT ACTION" << std::endl;
    std::cout << "  ------------------------------------------------\n" << std::endl;
    console.resetColor();
    
    console.printMenuOption(1, "Approve - Confirm this appointment");
    console.printMenuOption(2, "Reject  - Cancel this appointment");
    console.printMenuOption(3, "Skip    - Go back without action");
    
    std::cout << std::endl;
    console.setColor(WHITE);
    std::cout << "  Enter a number (1-3) to select." << std::endl;
    console.resetColor();
    int action = console.getIntInput("  Your action: ", 1, 3);
    
    if (action == 1) {
        if (db.updateAppointmentStatus(selected.appointmentID, "Confirmed")) {
            console.printSuccess("Appointment APPROVED!");
            console.setColor(WHITE);
            std::cout << "\n  Patient will be notified of confirmation." << std::endl;
            console.resetColor();
            db.logActivity("Staff", session.userID, "Approve Appointment", 
                          "ID: " + std::to_string(selected.appointmentID));
        } else {
            console.printError("Failed to approve appointment.");
        }
    } else if (action == 2) {
        if (db.updateAppointmentStatus(selected.appointmentID, "Cancelled")) {
            console.printWarning("Appointment REJECTED.");
            db.logActivity("Staff", session.userID, "Reject Appointment", 
                          "ID: " + std::to_string(selected.appointmentID));
        } else {
            console.printError("Failed to reject appointment.");
        }
    } else {
        console.printInfo("No action taken.");
    }
    
    console.pauseScreen();
}

void StaffModule::searchPatient() {
    console.clearScreen();
    console.printHeader("SEARCH PATIENT");
    
    std::cin.ignore(10000, '\n');
    
    console.setColor(WHITE);
    std::cout << "\n  Search for patients by name, email, or phone." << std::endl;
    std::cout << "  Enter partial or full text to search.\n" << std::endl;
    console.resetColor();
    
    console.setColor(WHITE);
    std::cout << "  Examples:" << std::endl;
    std::cout << "    - Name: John, Ahmad, Sarah" << std::endl;
    std::cout << "    - Email: john@, @gmail.com" << std::endl;
    std::cout << "    - Phone: 012, 0123456789\n" << std::endl;
    console.resetColor();
    
    console.setColor(DARK_GRAY);
    std::cout << "  (Enter 0 to return to menu)\n" << std::endl;
    console.resetColor();
    
    std::string search = console.getStringInput("  Enter search term: ");
    
    // Check if user wants to return
    if (search == "0") {
        return;
    }
    
    console.showLoading("\n  Searching", 2);
    
    std::vector<Patient> results = db.searchPatients(search);
    
    if (results.empty()) {
        console.printInfo("No patients found matching: " + search);
        console.setColor(WHITE);
        std::cout << "\n  Try a different search term." << std::endl;
        console.resetColor();
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
    
    std::cout << std::endl;
    console.setColor(YELLOW);
    std::cout << "  >> Enter a Patient ID to view details (or 0 to go back):" << std::endl;
    console.resetColor();
    int patientID = console.getIntInput("     Patient ID: ", 0, 99999);
    
    if (patientID == 0) return;
    
    bool found = false;
    for (const auto& p : results) {
        if (p.patientID == patientID) {
            found = true;
            break;
        }
    }
    
    if (!found) {
        console.printError("Patient ID not in search results.");
        console.pauseScreen();
        return;
    }
    
    Patient p = db.getPatientById(patientID);
    
    console.clearScreen();
    console.printHeader("PATIENT DETAILS");
    
    console.setColor(DARK_GRAY);
    std::cout << "\n  ------------------------------------------------" << std::endl;
    std::cout << "  PATIENT INFORMATION" << std::endl;
    std::cout << "  ------------------------------------------------\n" << std::endl;
    console.resetColor();
    
    console.setColor(WHITE);
    std::cout << "  Patient ID    : " << p.patientID << std::endl;
    std::cout << "  Full Name     : " << p.patientName << std::endl;
    std::cout << "  Gender        : " << p.gender << std::endl;
    std::cout << "  Date of Birth : " << p.dateOfBirth << std::endl;
    std::cout << "  Phone         : " << p.phoneNumber << std::endl;
    std::cout << "  Email         : " << p.email << std::endl;
    std::cout << "  Address       : " << p.address << std::endl;
    console.resetColor();
    
    // Get all appointments for this patient
    std::vector<Appointment> patientAppointments = db.getPatientAppointments(p.patientID);
    
    if (!patientAppointments.empty()) {
        std::cout << std::endl;
        console.setColor(DARK_GRAY);
        std::cout << "  ------------------------------------------------" << std::endl;
        std::cout << "  APPOINTMENT HISTORY" << std::endl;
        std::cout << "  ------------------------------------------------\n" << std::endl;
        console.resetColor();
        
        for (const auto& apt : patientAppointments) {
            console.setColor(CYAN);
            std::cout << "  Appointment ID: " << apt.appointmentID << std::endl;
            console.resetColor();
            console.setColor(WHITE);
            std::cout << "  Doctor        : " << apt.doctorName << std::endl;
            std::cout << "  Date          : " << apt.appointmentDate << std::endl;
            std::cout << "  Time          : " << apt.appointmentTime << std::endl;
            std::cout << "  Reason        : " << apt.reason << std::endl;
            std::cout << "  Duration      : " << apt.duration << " minutes" << std::endl;
            std::cout << "  Status        : ";
            console.resetColor();
            
            if (apt.status == "Completed") console.setColor(GREEN);
            else if (apt.status == "Confirmed") console.setColor(CYAN);
            else if (apt.status == "Pending") console.setColor(YELLOW);
            else if (apt.status == "Cancelled") console.setColor(RED);
            std::cout << apt.status << std::endl;
            console.resetColor();
            
            console.setColor(WHITE);
            std::cout << "  Consultation  : RM" << std::fixed << std::setprecision(2) << apt.consultationFee << std::endl;
            std::cout << "  Medicine      : RM" << std::fixed << std::setprecision(2) << apt.medicineFee << std::endl;
            std::cout << "  Total Cost    : RM" << std::fixed << std::setprecision(2) << apt.totalCost << std::endl;
            console.resetColor();
            
            console.setColor(DARK_GRAY);
            std::cout << "  " << std::string(48, '-') << std::endl;
            console.resetColor();
        }
        
        // Summary
        int totalAppointments = patientAppointments.size();
        int completedCount = 0;
        double totalRevenue = 0.0;
        for (const auto& apt : patientAppointments) {
            if (apt.status == "Completed") {
                completedCount++;
                totalRevenue += apt.totalCost;
            }
        }
        
        std::cout << std::endl;
        console.setColor(CYAN);
        std::cout << "  SUMMARY:" << std::endl;
        console.resetColor();
        console.setColor(WHITE);
        std::cout << "  Total Appointments : " << totalAppointments << std::endl;
        std::cout << "  Completed          : " << completedCount << std::endl;
        std::cout << "  Total Revenue      : RM" << std::fixed << std::setprecision(2) << totalRevenue << std::endl;
        console.resetColor();
    } else {
        std::cout << std::endl;
        console.setColor(DARK_GRAY);
        std::cout << "  No appointment history found." << std::endl;
        console.resetColor();
    }
    
    db.logActivity("Staff", session.userID, "Search Patient", 
                  "Viewed ID: " + std::to_string(patientID));
    
    console.pauseScreen();
}

void StaffModule::viewDoctors() {
    console.clearScreen();
    console.printHeader("VIEW ALL DOCTORS");
    
    std::vector<Doctor> doctors = db.getAllDoctors(false);
    
    if (doctors.empty()) {
        console.printInfo("No doctors in the system.");
        console.pauseScreen();
        return;
    }
    
    console.setColor(WHITE);
    std::cout << "\n  List of all doctors in the hospital.\n" << std::endl;
    console.resetColor();
    
    console.setColor(DARK_CYAN);
    std::cout << "  " << std::left 
              << std::setw(5) << "ID"
              << std::setw(25) << "Doctor Name"
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
    
    std::cout << "\n  Total: " << doctors.size() << " doctor(s)" << std::endl;
    
    console.pauseScreen();
}

void StaffModule::showDashboard() {
    while (session.isLoggedIn) {
        console.clearScreen();
        console.printHeader("STAFF DASHBOARD");
        
        console.setColor(CYAN);
        std::cout << "\n  Welcome back, " << session.userName << "!\n" << std::endl;
        console.resetColor();
        
        console.setColor(DARK_GRAY);
        std::cout << "  ================================================" << std::endl;
        std::cout << "  MENU" << std::endl;
        std::cout << "  ================================================" << std::endl;
        console.resetColor();
        
        std::cout << std::endl;
        console.printMenuOption(1, "View All Appointments");
        console.printMenuOption(2, "Approve Appointments");
        console.printMenuOption(3, "Search Patient");
        console.printMenuOption(4, "View Doctors");
        console.printMenuOption(5, "Logout");
        
        std::cout << std::endl;
        console.setColor(YELLOW);
        std::cout << "  >> Enter your choice (1-5): ";
        console.resetColor();
        int choice = console.getIntInput("", 1, 5);
        
        switch (choice) {
            case 1: viewAllAppointments(); break;
            case 2: approveAppointment(); break;
            case 3: searchPatient(); break;
            case 4: viewDoctors(); break;
            case 5:
                db.logActivity("Staff", session.userID, "Logout", "User logged out");
                session = UserSession();
                console.printSuccess("You have been logged out successfully!");
                Sleep(1500);
                return;
        }
    }
}
