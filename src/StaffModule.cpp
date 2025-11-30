// ============================================================
// StaffModule.cpp - Staff Dashboard Implementation
// Hospital Appointment Booking System
// ============================================================

#include "../include/StaffModule.h"
#include "../include/Utilities.h"
#include <iostream>
#include <iomanip>
#include <vector>
#include <windows.h>

StaffModule::StaffModule(ConsoleUtils& c, DatabaseManager& d, UserSession& s)
    : console(c), db(d), session(s) {}

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
              << std::setw(6) << "ID"
              << std::setw(18) << "Patient"
              << std::setw(18) << "Doctor"
              << std::setw(11) << "Date"
              << std::setw(8) << "Time"
              << std::setw(10) << "Status" << std::endl;
    std::cout << "  " << std::string(71, '-') << std::endl;
    console.resetColor();
    
    int pending = 0, confirmed = 0, completed = 0, cancelled = 0;
    
    for (const auto& apt : appointments) {
        if (apt.status == "Confirmed") { console.setColor(GREEN); confirmed++; }
        else if (apt.status == "Pending") { console.setColor(YELLOW); pending++; }
        else if (apt.status == "Completed") { console.setColor(CYAN); completed++; }
        else if (apt.status == "Cancelled") { console.setColor(RED); cancelled++; }
        else console.resetColor();
        
        std::string pName = apt.patientName.length() > 16 ? 
            apt.patientName.substr(0, 13) + "..." : apt.patientName;
        std::string dName = apt.doctorName.length() > 16 ? 
            apt.doctorName.substr(0, 13) + "..." : apt.doctorName;
        
        std::cout << "  " << std::setw(6) << apt.appointmentID
                  << std::setw(18) << pName
                  << std::setw(18) << dName
                  << std::setw(11) << apt.appointmentDate
                  << std::setw(8) << apt.appointmentTime.substr(0, 5)
                  << std::setw(10) << apt.status << std::endl;
    }
    console.resetColor();
    
    std::cout << "\n  " << std::string(71, '-') << std::endl;
    std::cout << "  SUMMARY: ";
    console.setColor(YELLOW); std::cout << pending << " Pending  ";
    console.setColor(GREEN); std::cout << confirmed << " Confirmed  ";
    console.setColor(CYAN); std::cout << completed << " Completed  ";
    console.setColor(RED); std::cout << cancelled << " Cancelled";
    console.resetColor();
    std::cout << std::endl;
    
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
    std::cout << "  Patient : " << selected.patientName << std::endl;
    std::cout << "  Doctor  : " << selected.doctorName << std::endl;
    std::cout << "  Date    : " << selected.appointmentDate << std::endl;
    std::cout << "  Time    : " << selected.appointmentTime.substr(0, 5) << std::endl;
    std::cout << "  Reason  : " << selected.reason << std::endl;
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
    
    std::string search = console.getStringInput("  Enter search term: ");
    
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
    
    console.setColor(WHITE);
    std::cout << "\n  PATIENT INFORMATION:" << std::endl;
    std::cout << "  " << std::string(40, '-') << std::endl;
    std::cout << "  ID            : " << p.patientID << std::endl;
    std::cout << "  Name          : " << p.patientName << std::endl;
    std::cout << "  Gender        : " << p.gender << std::endl;
    std::cout << "  Date of Birth : " << p.dateOfBirth << std::endl;
    std::cout << "  Phone         : " << p.phoneNumber << std::endl;
    std::cout << "  Email         : " << p.email << std::endl;
    std::cout << "  Address       : " << p.address << std::endl;
    std::cout << "  " << std::string(40, '-') << std::endl;
    console.resetColor();
    
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

void StaffModule::generateReport() {
    console.clearScreen();
    console.printHeader("GENERATE REPORT");
    
    console.setColor(WHITE);
    std::cout << "\n  Select the type of report to generate.\n" << std::endl;
    console.resetColor();
    
    console.printMenuOption(1, "Today's Report      - Appointments for today");
    console.printMenuOption(2, "Weekly Summary      - Last 7 days summary");
    console.printMenuOption(3, "Monthly Summary     - Last 30 days summary");
    console.printMenuOption(4, "Go Back             - Return to dashboard");
    
    std::cout << std::endl;
    console.setColor(WHITE);
    std::cout << "  Enter a number (1-4) to select." << std::endl;
    console.resetColor();
    int choice = console.getIntInput("  Your choice: ", 1, 4);
    
    if (choice == 4) return;
    
    console.showLoading("\n  Generating report", 2);
    
    std::string today = getCurrentDate();
    
    console.clearScreen();
    
    if (choice == 1) {
        console.printHeader("TODAY'S REPORT");
        console.setColor(WHITE);
        std::cout << "\n  Date: " << today << std::endl;
        console.resetColor();
    } else if (choice == 2) {
        console.printHeader("WEEKLY SUMMARY REPORT");
        console.setColor(WHITE);
        std::cout << "\n  Period: Last 7 days" << std::endl;
        console.resetColor();
    } else {
        console.printHeader("MONTHLY SUMMARY REPORT");
        console.setColor(WHITE);
        std::cout << "\n  Period: Last 30 days" << std::endl;
        console.resetColor();
    }
    
    std::vector<Appointment> appointments = db.getAllAppointments();
    
    int total = (int)appointments.size();
    int pending = 0, confirmed = 0, completed = 0, cancelled = 0;
    
    for (const auto& apt : appointments) {
        if (apt.status == "Pending") pending++;
        else if (apt.status == "Confirmed") confirmed++;
        else if (apt.status == "Completed") completed++;
        else if (apt.status == "Cancelled") cancelled++;
    }
    
    std::cout << std::endl;
    console.setColor(DARK_GRAY);
    std::cout << "  ------------------------------------------------" << std::endl;
    std::cout << "  APPOINTMENT STATISTICS" << std::endl;
    std::cout << "  ------------------------------------------------\n" << std::endl;
    console.resetColor();
    
    console.setColor(WHITE);
    std::cout << "  Total Appointments    : " << total << std::endl;
    console.setColor(YELLOW);
    std::cout << "  Pending               : " << pending << std::endl;
    console.setColor(GREEN);
    std::cout << "  Confirmed             : " << confirmed << std::endl;
    console.setColor(CYAN);
    std::cout << "  Completed             : " << completed << std::endl;
    console.setColor(RED);
    std::cout << "  Cancelled             : " << cancelled << std::endl;
    console.resetColor();
    
    std::vector<Patient> patients = db.searchPatients("");
    std::vector<Doctor> doctors = db.getAllDoctors(false);
    
    std::cout << std::endl;
    console.setColor(DARK_GRAY);
    std::cout << "  ------------------------------------------------" << std::endl;
    std::cout << "  SYSTEM OVERVIEW" << std::endl;
    std::cout << "  ------------------------------------------------\n" << std::endl;
    console.resetColor();
    
    console.setColor(WHITE);
    std::cout << "  Total Patients        : " << patients.size() << std::endl;
    std::cout << "  Total Doctors         : " << doctors.size() << std::endl;
    
    int availableDocs = 0;
    for (const auto& doc : doctors) {
        if (doc.isAvailable) availableDocs++;
    }
    std::cout << "  Available Doctors     : " << availableDocs << std::endl;
    console.resetColor();
    
    db.logActivity("Staff", session.userID, "Generate Report", 
                  "Report Type: " + std::to_string(choice));
    
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
        
        console.printMenuOption(1, "View All Appointments");
        console.printMenuOption(2, "Approve Appointments");
        console.printMenuOption(3, "Search Patient");
        console.printMenuOption(4, "View Doctors");
        console.printMenuOption(5, "Generate Report");
        console.printMenuOption(6, "Logout");
        
        std::cout << std::endl;
        console.setColor(YELLOW);
        std::cout << "  >> Enter your choice:" << std::endl;
        console.resetColor();
        int choice = console.getIntInput("     Your choice: ", 1, 6);
        
        switch (choice) {
            case 1: viewAllAppointments(); break;
            case 2: approveAppointment(); break;
            case 3: searchPatient(); break;
            case 4: viewDoctors(); break;
            case 5: generateReport(); break;
            case 6:
                db.logActivity("Staff", session.userID, "Logout", "User logged out");
                session = UserSession();
                console.printSuccess("You have been logged out successfully!");
                Sleep(1500);
                return;
        }
    }
}
