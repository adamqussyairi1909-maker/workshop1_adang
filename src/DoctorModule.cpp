// ============================================================
// DoctorModule.cpp - Doctor Dashboard Implementation
// Hospital Appointment Booking System
// ============================================================

#include "../include/DoctorModule.h"
#include "../include/Utilities.h"
#include <iostream>
#include <iomanip>
#include <vector>
#include <windows.h>

// OOP: Constructor calls base class constructor
DoctorModule::DoctorModule(ConsoleUtils& c, DatabaseManager& d, UserSession& s)
    : BaseModule(c, d, s) {}

void DoctorModule::viewTodayAppointments() {
    console.clearScreen();
    console.printHeader("TODAY'S APPOINTMENTS");
    
    std::string today = getCurrentDate();
    std::vector<Appointment> appointments = db.getDoctorAppointments(session.userID, today);
    
    console.setColor(WHITE);
    std::cout << "\n  Date: " << today << std::endl;
    std::cout << "  Showing all your appointments for today.\n" << std::endl;
    console.resetColor();
    
    if (appointments.empty()) {
        console.printInfo("No appointments scheduled for today.");
        console.setColor(WHITE);
        std::cout << "\n  Enjoy your free time!" << std::endl;
        console.resetColor();
        console.pauseScreen();
        return;
    }
    
    console.setColor(DARK_CYAN);
    std::cout << "  " << std::left 
              << std::setw(6) << "ID"
              << std::setw(22) << "Patient Name"
              << std::setw(8) << "Time"
              << std::setw(7) << "Cost"
              << std::setw(11) << "Status"
              << std::setw(18) << "Reason" << std::endl;
    std::cout << "  " << std::string(72, '-') << std::endl;
    console.resetColor();
    
    int confirmed = 0, pending = 0, completed = 0;
    double todayRevenue = 0.0;
    
    for (const auto& apt : appointments) {
        if (apt.status == "Confirmed") { console.setColor(GREEN); confirmed++; }
        else if (apt.status == "Pending") { console.setColor(YELLOW); pending++; }
        else if (apt.status == "Completed") { console.setColor(CYAN); completed++; todayRevenue += apt.totalCost; }
        else console.resetColor();
        
        std::string shortReason = apt.reason.length() > 16 ? 
            apt.reason.substr(0, 13) + "..." : apt.reason;
        std::string patientName = apt.patientName.length() > 20 ? 
            apt.patientName.substr(0, 17) + "..." : apt.patientName;
        
        std::cout << "  " << std::setw(6) << apt.appointmentID
                  << std::setw(22) << patientName
                  << std::setw(8) << apt.appointmentTime.substr(0, 5)
                  << std::setw(7) << ("RM" + std::to_string((int)apt.totalCost))
                  << std::setw(11) << apt.status
                  << std::setw(18) << shortReason << std::endl;
    }
    console.resetColor();
    
    std::cout << "\n  " << std::string(72, '-') << std::endl;
    std::cout << "  SUMMARY: ";
    console.setColor(GREEN); std::cout << confirmed << " Confirmed  ";
    console.setColor(YELLOW); std::cout << pending << " Pending  ";
    console.setColor(CYAN); std::cout << completed << " Completed";
    console.resetColor();
    std::cout << "\n  Today's Revenue: RM " << std::fixed << std::setprecision(2) << todayRevenue << std::endl;
    
    console.pauseScreen();
}

void DoctorModule::viewAllAppointments() {
    console.clearScreen();
    console.printHeader("ALL MY APPOINTMENTS");
    
    std::vector<Appointment> appointments = db.getDoctorAllAppointments(session.userID);
    
    if (appointments.empty()) {
        console.printInfo("You have no appointments.");
        console.pauseScreen();
        return;
    }
    
    console.setColor(WHITE);
    std::cout << "\n  Showing all your appointments (past and future).\n" << std::endl;
    console.resetColor();
    
    console.setColor(DARK_CYAN);
    std::cout << "  " << std::left 
              << std::setw(6) << "ID"
              << std::setw(18) << "Patient"
              << std::setw(12) << "Date"
              << std::setw(8) << "Time"
              << std::setw(8) << "Cost"
              << std::setw(11) << "Status" << std::endl;
    std::cout << "  " << std::string(63, '-') << std::endl;
    console.resetColor();
    
    double totalEarnings = 0.0;
    for (const auto& apt : appointments) {
        if (apt.status == "Confirmed") console.setColor(GREEN);
        else if (apt.status == "Pending") console.setColor(YELLOW);
        else if (apt.status == "Completed") { console.setColor(CYAN); totalEarnings += apt.totalCost; }
        else if (apt.status == "Cancelled") console.setColor(RED);
        else console.resetColor();
        
        std::string patientName = apt.patientName.length() > 16 ? 
            apt.patientName.substr(0, 13) + "..." : apt.patientName;
        
        std::cout << "  " << std::setw(6) << apt.appointmentID
                  << std::setw(18) << patientName
                  << std::setw(12) << apt.appointmentDate
                  << std::setw(8) << apt.appointmentTime.substr(0, 5)
                  << std::setw(8) << ("RM" + std::to_string((int)apt.totalCost))
                  << std::setw(11) << apt.status << std::endl;
    }
    console.resetColor();
    
    std::cout << "\n  Total: " << appointments.size() << " appointment(s)";
    console.setColor(CYAN);
    std::cout << "  |  Total Earnings: RM " << std::fixed << std::setprecision(2) << totalEarnings;
    console.resetColor();
    std::cout << std::endl;
    
    console.pauseScreen();
}

void DoctorModule::completeAppointment() {
    console.clearScreen();
    console.printHeader("COMPLETE APPOINTMENT");
    
    std::string today = getCurrentDate();
    std::vector<Appointment> appointments = db.getDoctorAppointments(session.userID, today);
    
    std::vector<Appointment> confirmed;
    for (const auto& apt : appointments) {
        if (apt.status == "Confirmed") {
            confirmed.push_back(apt);
        }
    }
    
    if (confirmed.empty()) {
        console.printInfo("No confirmed appointments to complete today.");
        console.setColor(WHITE);
        std::cout << "\n  Only CONFIRMED appointments can be marked as completed." << std::endl;
        console.resetColor();
        console.pauseScreen();
        return;
    }
    
    console.setColor(WHITE);
    std::cout << "\n  Mark an appointment as COMPLETED after the patient visit." << std::endl;
    std::cout << "  Select from the list below.\n" << std::endl;
    console.resetColor();
    
    console.setColor(DARK_CYAN);
    std::cout << "  " << std::left 
              << std::setw(5) << "No."
              << std::setw(8) << "ID"
              << std::setw(25) << "Patient Name"
              << std::setw(10) << "Time" << std::endl;
    std::cout << "  " << std::string(48, '-') << std::endl;
    console.resetColor();
    
    for (int i = 0; i < (int)confirmed.size(); i++) {
        std::cout << "  " << std::setw(5) << (i + 1)
                  << std::setw(8) << confirmed[i].appointmentID
                  << std::setw(25) << confirmed[i].patientName
                  << std::setw(10) << confirmed[i].appointmentTime.substr(0, 5) << std::endl;
    }
    
    std::cout << std::endl;
    console.setColor(YELLOW);
    std::cout << "  >> Select an appointment to complete (or enter 0 to go back):" << std::endl;
    console.resetColor();
    int choice = console.getIntInput("     Enter number (0-" + std::to_string(confirmed.size()) + "): ", 0, (int)confirmed.size());
    
    if (choice == 0) return;
    
    console.setColor(WHITE);
    std::cout << "\n  APPOINTMENT TO COMPLETE:" << std::endl;
    std::cout << "  +-----------------------------------+" << std::endl;
    std::cout << "  | Patient : " << std::left << std::setw(24) << confirmed[choice - 1].patientName << "|" << std::endl;
    std::cout << "  | Time    : " << std::left << std::setw(24) << confirmed[choice - 1].appointmentTime.substr(0, 5) << "|" << std::endl;
    std::cout << "  +-----------------------------------+" << std::endl;
    console.resetColor();
    
    std::cin.ignore(10000, '\n');
    std::cout << std::endl;
    console.setColor(YELLOW);
    std::cout << "  >> Confirm completion:" << std::endl;
    console.resetColor();
    std::string confirm = console.getStringInput("     Type Y to confirm, N to go back: ");
    
    if (confirm == "Y" || confirm == "y") {
        if (db.updateAppointmentStatus(confirmed[choice - 1].appointmentID, "Completed")) {
            console.printSuccess("Appointment marked as COMPLETED!");
            db.logActivity("Doctor", session.userID, "Complete Appointment",
                          "ID: " + std::to_string(confirmed[choice - 1].appointmentID));
        } else {
            console.printError("Failed to update appointment status.");
        }
    } else {
        console.printInfo("Operation cancelled.");
    }
    
    console.pauseScreen();
}

void DoctorModule::viewPatientInfo() {
    console.clearScreen();
    console.printHeader("VIEW PATIENT INFORMATION");
    
    std::vector<Appointment> appointments = db.getDoctorAllAppointments(session.userID);
    
    std::vector<int> patientIDs;
    std::vector<std::string> patientNames;
    
    for (const auto& apt : appointments) {
        bool found = false;
        for (int id : patientIDs) {
            if (id == apt.patientID) { found = true; break; }
        }
        if (!found) {
            patientIDs.push_back(apt.patientID);
            patientNames.push_back(apt.patientName);
        }
    }
    
    if (patientIDs.empty()) {
        console.printInfo("No patients found.");
        console.setColor(WHITE);
        std::cout << "\n  You have not seen any patients yet." << std::endl;
        console.resetColor();
        console.pauseScreen();
        return;
    }
    
    console.setColor(WHITE);
    std::cout << "\n  View detailed information about your patients." << std::endl;
    std::cout << "  Select a patient from the list.\n" << std::endl;
    console.resetColor();
    
    console.setColor(DARK_CYAN);
    std::cout << "  " << std::left << std::setw(5) << "No." << "Patient Name" << std::endl;
    std::cout << "  " << std::string(30, '-') << std::endl;
    console.resetColor();
    
    for (int i = 0; i < (int)patientNames.size(); i++) {
        std::cout << "  " << std::setw(5) << (i + 1) << patientNames[i] << std::endl;
    }
    
    std::cout << std::endl;
    console.setColor(YELLOW);
    std::cout << "  >> Select a patient to view details (or enter 0 to go back):" << std::endl;
    console.resetColor();
    int choice = console.getIntInput("     Enter number (0-" + std::to_string(patientNames.size()) + "): ", 0, (int)patientNames.size());
    
    if (choice == 0) return;
    
    Patient p = db.getPatientById(patientIDs[choice - 1]);
    
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
    
    // Get appointments for this patient with this doctor
    std::vector<Appointment> patientAppointments;
    for (const auto& apt : appointments) {
        if (apt.patientID == p.patientID) {
            patientAppointments.push_back(apt);
        }
    }
    
    // Display appointment history
    if (!patientAppointments.empty()) {
        std::cout << std::endl;
        console.setColor(DARK_GRAY);
        std::cout << "  ------------------------------------------------" << std::endl;
        std::cout << "  APPOINTMENT HISTORY WITH YOU" << std::endl;
        std::cout << "  ------------------------------------------------\n" << std::endl;
        console.resetColor();
        
        for (const auto& apt : patientAppointments) {
            console.setColor(CYAN);
            std::cout << "  Appointment ID: " << apt.appointmentID << std::endl;
            console.resetColor();
            console.setColor(WHITE);
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
            std::cout << "  Total Cost    : RM" << std::fixed << std::setprecision(2) << apt.totalCost << std::endl;
            console.resetColor();
            
            console.setColor(DARK_GRAY);
            std::cout << "  " << std::string(48, '-') << std::endl;
            console.resetColor();
        }
    }
    
    db.logActivity("Doctor", session.userID, "View Patient", "Patient ID: " + std::to_string(p.patientID));
    
    console.pauseScreen();
}

void DoctorModule::updateAvailability() {
    console.clearScreen();
    console.printHeader("UPDATE AVAILABILITY STATUS");
    
    Doctor doc = db.getDoctorById(session.userID);
    
    console.setColor(WHITE);
    std::cout << "\n  Update your availability status." << std::endl;
    std::cout << "  This affects whether patients can book appointments with you.\n" << std::endl;
    console.resetColor();
    
    console.setColor(DARK_GRAY);
    std::cout << "  ------------------------------------------------" << std::endl;
    std::cout << "  CURRENT STATUS" << std::endl;
    std::cout << "  ------------------------------------------------\n" << std::endl;
    console.resetColor();
    
    console.setColor(WHITE);
    std::cout << "  Doctor Name : " << doc.doctorName << std::endl;
    std::cout << "  Specialty   : " << doc.specialty << std::endl;
    std::cout << "  Room No     : " << doc.roomNo << std::endl;
    std::cout << "  Status      : ";
    if (doc.isAvailable) {
        console.setColor(GREEN);
        std::cout << "AVAILABLE" << std::endl;
    } else {
        console.setColor(RED);
        std::cout << "NOT AVAILABLE" << std::endl;
    }
    console.resetColor();
    
    std::cout << std::endl;
    console.setColor(DARK_GRAY);
    std::cout << "  ------------------------------------------------" << std::endl;
    std::cout << "  SELECT NEW STATUS" << std::endl;
    std::cout << "  ------------------------------------------------\n" << std::endl;
    console.resetColor();
    
    console.printMenuOption(1, "Available     - Patients can book appointments");
    console.printMenuOption(2, "Not Available - Patients cannot book new appointments");
    console.printMenuOption(3, "Go Back       - Keep current status");
    
    std::cout << std::endl;
    console.setColor(WHITE);
    std::cout << "  Enter a number (1-3) to select." << std::endl;
    console.resetColor();
    int choice = console.getIntInput("  Your choice: ", 1, 3);
    
    if (choice == 3) return;
    
    bool newStatus = (choice == 1);
    
    if (db.updateDoctorAvailability(session.userID, newStatus)) {
        if (newStatus) {
            console.printSuccess("Status updated to: AVAILABLE");
        } else {
            console.printSuccess("Status updated to: NOT AVAILABLE");
        }
        db.logActivity("Doctor", session.userID, "Update Availability",
                      newStatus ? "Set Available" : "Set Not Available");
    } else {
        console.printError("Failed to update status.");
    }
    
    console.pauseScreen();
}

void DoctorModule::showDashboard() {
    while (session.isLoggedIn) {
        console.clearScreen();
        console.printHeader("DOCTOR DASHBOARD");
        
        console.setColor(CYAN);
        std::cout << "\n  Welcome back, Dr. " << session.userName << "!\n" << std::endl;
        console.resetColor();
        
        console.setColor(DARK_GRAY);
        std::cout << "  ================================================" << std::endl;
        std::cout << "  MENU" << std::endl;
        std::cout << "  ================================================" << std::endl;
        console.resetColor();
        
        std::cout << std::endl;
        console.printMenuOption(1, "Today's Appointments");
        console.printMenuOption(2, "All Appointments");
        console.printMenuOption(3, "Complete Appointment");
        console.printMenuOption(4, "View Patient Info");
        console.printMenuOption(5, "Update Availability");
        console.printMenuOption(6, "Logout");
        
        std::cout << std::endl;
        console.setColor(YELLOW);
        std::cout << "  >> Enter your choice (1-6): ";
        console.resetColor();
        int choice = console.getIntInput("", 1, 6);
        
        switch (choice) {
            case 1: viewTodayAppointments(); break;
            case 2: viewAllAppointments(); break;
            case 3: completeAppointment(); break;
            case 4: viewPatientInfo(); break;
            case 5: updateAvailability(); break;
            case 6:
                db.logActivity("Doctor", session.userID, "Logout", "User logged out");
                session = UserSession();
                console.printSuccess("You have been logged out successfully!");
                Sleep(1500);
                return;
        }
    }
}
