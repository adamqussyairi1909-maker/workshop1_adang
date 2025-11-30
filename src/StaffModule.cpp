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

void StaffModule::viewAppointments() {
    console.clearScreen();
    console.printHeader("ALL APPOINTMENTS");
    
    std::vector<Appointment> appointments = db.getAllAppointments();
    
    if (appointments.empty()) {
        console.printInfo("No appointments found.");
        console.pauseScreen();
        return;
    }
    
    console.setColor(WHITE);
    std::cout << "\n  All system appointments:\n" << std::endl;
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
    
    for (const auto& apt : appointments) {
        if (apt.status == "Confirmed") console.setColor(GREEN);
        else if (apt.status == "Pending") console.setColor(YELLOW);
        else if (apt.status == "Cancelled") console.setColor(RED);
        else if (apt.status == "Completed") console.setColor(CYAN);
        else console.resetColor();
        
        std::cout << "  " << std::setw(6) << apt.appointmentID
                  << std::setw(18) << apt.patientName.substr(0, 16)
                  << std::setw(18) << apt.doctorName.substr(0, 16)
                  << std::setw(11) << apt.appointmentDate
                  << std::setw(8) << apt.appointmentTime.substr(0, 5)
                  << std::setw(10) << apt.status << std::endl;
    }
    console.resetColor();
    
    std::cout << "\n  Total: " << appointments.size() << " appointment(s)" << std::endl;
    
    console.pauseScreen();
}

void StaffModule::approveAppointment() {
    console.clearScreen();
    console.printHeader("APPROVE / CANCEL APPOINTMENT");
    
    std::vector<Appointment> pending = db.getAllAppointments("Pending");
    
    if (pending.empty()) {
        console.printInfo("No pending appointments to process.");
        console.pauseScreen();
        return;
    }
    
    console.setColor(YELLOW);
    std::cout << "\n  Pending Appointments (" << pending.size() << "):\n" << std::endl;
    console.resetColor();
    
    console.setColor(DARK_CYAN);
    std::cout << "  " << std::left 
              << std::setw(5) << "No."
              << std::setw(8) << "ID"
              << std::setw(18) << "Patient"
              << std::setw(18) << "Doctor"
              << std::setw(12) << "Date" << std::endl;
    std::cout << "  " << std::string(61, '-') << std::endl;
    console.resetColor();
    
    for (int i = 0; i < (int)pending.size(); i++) {
        std::cout << "  " << std::setw(5) << (i + 1)
                  << std::setw(8) << pending[i].appointmentID
                  << std::setw(18) << pending[i].patientName.substr(0, 16)
                  << std::setw(18) << pending[i].doctorName.substr(0, 16)
                  << std::setw(12) << pending[i].appointmentDate << std::endl;
    }
    
    std::cout << std::endl;
    int choice = console.getIntInput("  Select appointment (0 to go back): ", 0, (int)pending.size());
    
    if (choice == 0) return;
    
    Appointment selected = pending[choice - 1];
    
    console.clearScreen();
    console.printSubHeader("Appointment Details");
    
    std::cout << "\n  Patient  : " << selected.patientName << std::endl;
    std::cout << "  Doctor   : " << selected.doctorName << std::endl;
    std::cout << "  Date     : " << selected.appointmentDate << std::endl;
    std::cout << "  Time     : " << selected.appointmentTime.substr(0, 5) << std::endl;
    std::cout << "  Reason   : " << selected.reason << std::endl;
    std::cout << "  " << std::string(45, '-') << std::endl;
    
    std::cout << "\n  Actions:" << std::endl;
    console.printMenuOption(1, "Approve");
    console.printMenuOption(2, "Reject/Cancel");
    console.printMenuOption(3, "Go Back");
    
    int action = console.getIntInput("\n  Enter choice: ", 1, 3);
    
    if (action == 1) {
        if (db.updateAppointmentStatus(selected.appointmentID, "Confirmed", session.userID)) {
            console.printSuccess("Appointment APPROVED successfully!");
            db.logActivity("Staff", session.userID, "Approve Appointment",
                          "Approved appointment ID: " + std::to_string(selected.appointmentID));
        } else {
            console.printError("Failed to approve appointment.");
        }
    } else if (action == 2) {
        if (db.updateAppointmentStatus(selected.appointmentID, "Cancelled", session.userID)) {
            console.printSuccess("Appointment CANCELLED.");
            db.logActivity("Staff", session.userID, "Cancel Appointment",
                          "Cancelled appointment ID: " + std::to_string(selected.appointmentID));
        } else {
            console.printError("Failed to cancel appointment.");
        }
    }
    
    console.pauseScreen();
}

void StaffModule::managePatients() {
    console.clearScreen();
    console.printHeader("PATIENT RECORDS");
    
    std::vector<Patient> patients = db.getAllPatients();
    
    if (patients.empty()) {
        console.printInfo("No patients registered.");
        console.pauseScreen();
        return;
    }
    
    console.setColor(WHITE);
    std::cout << "\n  Registered Patients:\n" << std::endl;
    console.resetColor();
    
    console.setColor(DARK_CYAN);
    std::cout << "  " << std::left 
              << std::setw(5) << "ID"
              << std::setw(22) << "Name"
              << std::setw(15) << "Phone"
              << std::setw(8) << "Gender"
              << std::setw(12) << "DOB" << std::endl;
    std::cout << "  " << std::string(62, '-') << std::endl;
    console.resetColor();
    
    for (const auto& p : patients) {
        std::cout << "  " << std::setw(5) << p.patientID
                  << std::setw(22) << p.patientName.substr(0, 20)
                  << std::setw(15) << p.phoneNumber
                  << std::setw(8) << p.gender
                  << std::setw(12) << p.dob << std::endl;
    }
    
    std::cout << "\n  Total: " << patients.size() << " patient(s)" << std::endl;
    
    console.pauseScreen();
}

void StaffModule::generateReport() {
    console.clearScreen();
    console.printHeader("APPOINTMENT SUMMARY REPORT");
    
    AppointmentSummary summary = db.getDailyAppointmentSummary("");
    
    std::cout << "\n  +----------------------------------+" << std::endl;
    std::cout << "  |     APPOINTMENT STATISTICS       |" << std::endl;
    std::cout << "  +----------------------------------+" << std::endl;
    
    console.setColor(WHITE);
    std::cout << "  | Total Appointments : " << std::setw(10) << summary.total << " |" << std::endl;
    console.setColor(GREEN);
    std::cout << "  | Confirmed          : " << std::setw(10) << summary.confirmed << " |" << std::endl;
    console.setColor(YELLOW);
    std::cout << "  | Pending            : " << std::setw(10) << summary.pending << " |" << std::endl;
    console.setColor(CYAN);
    std::cout << "  | Completed          : " << std::setw(10) << summary.completed << " |" << std::endl;
    console.setColor(RED);
    std::cout << "  | Cancelled          : " << std::setw(10) << summary.cancelled << " |" << std::endl;
    console.resetColor();
    
    std::cout << "  +----------------------------------+" << std::endl;
    
    std::cout << "\n  Most Active Doctor   : " << db.getMostActiveDoctor() << std::endl;
    std::cout << "  Most Frequent Patient: " << db.getMostFrequentPatient() << std::endl;
    
    console.setColor(GREEN);
    std::cout << "\n  Report generated on " << getCurrentDateTime() << std::endl;
    console.resetColor();
    
    db.logActivity("Staff", session.userID, "Generate Report", "Generated summary report");
    
    console.pauseScreen();
}

void StaffModule::showDashboard() {
    while (session.isLoggedIn) {
        console.clearScreen();
        console.printHeader("STAFF DASHBOARD");
        
        Staff staff = db.getStaffById(session.userID);
        
        console.setColor(WHITE);
        std::cout << "\n  Welcome, " << staff.staffName << std::endl;
        console.setColor(DARK_GRAY);
        std::cout << "  Role: " << staff.role << std::endl;
        console.resetColor();
        
        // Show pending count
        std::vector<Appointment> pending = db.getAllAppointments("Pending");
        if (!pending.empty()) {
            console.setColor(YELLOW);
            std::cout << "\n  [!] " << pending.size() << " appointment(s) pending approval" << std::endl;
            console.resetColor();
        }
        
        std::cout << "\n  What would you like to do?\n" << std::endl;
        console.printMenuOption(1, "View All Appointments");
        console.printMenuOption(2, "Approve / Cancel Appointments");
        console.printMenuOption(3, "View Patient Records");
        console.printMenuOption(4, "Generate Report");
        console.printMenuOption(5, "Logout");
        
        std::cout << std::endl;
        int choice = console.getIntInput("  Enter your choice: ", 1, 5);
        
        switch (choice) {
            case 1: viewAppointments(); break;
            case 2: approveAppointment(); break;
            case 3: managePatients(); break;
            case 4: generateReport(); break;
            case 5:
                db.logActivity("Staff", session.userID, "Logout", "Staff logged out");
                session = UserSession();
                console.printSuccess("Logged out successfully!");
                Sleep(1000);
                return;
        }
    }
}
