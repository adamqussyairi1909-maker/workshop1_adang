// ============================================================
// StaffModule.cpp - Staff Dashboard Implementation
// Hospital Appointment Booking System
// ============================================================

#include "StaffModule.h"
#include "Utilities.h"
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
    
    console.setColor(DARK_CYAN);
    std::cout << std::left 
              << std::setw(6) << "ID"
              << std::setw(18) << "Patient"
              << std::setw(18) << "Doctor"
              << std::setw(11) << "Date"
              << std::setw(8) << "Time"
              << std::setw(10) << "Status" << std::endl;
    std::cout << std::string(71, '-') << std::endl;
    console.resetColor();
    
    for (const auto& apt : appointments) {
        if (apt.status == "Confirmed") console.setColor(GREEN);
        else if (apt.status == "Pending") console.setColor(YELLOW);
        else if (apt.status == "Cancelled") console.setColor(RED);
        else if (apt.status == "Completed") console.setColor(CYAN);
        else console.resetColor();
        
        std::cout << std::setw(6) << apt.appointmentID
                  << std::setw(18) << apt.patientName.substr(0, 16)
                  << std::setw(18) << apt.doctorName.substr(0, 16)
                  << std::setw(11) << apt.appointmentDate
                  << std::setw(8) << apt.appointmentTime.substr(0, 5)
                  << std::setw(10) << apt.status << std::endl;
    }
    console.resetColor();
    
    console.pauseScreen();
}

void StaffModule::approveAppointment() {
    console.clearScreen();
    console.printHeader("APPROVE / CANCEL APPOINTMENT");
    
    std::vector<Appointment> pending = db.getAllAppointments("Pending");
    
    if (pending.empty()) {
        console.printInfo("No pending appointments.");
        console.pauseScreen();
        return;
    }
    
    console.setColor(YELLOW);
    std::cout << "\nPending Appointments:\n" << std::endl;
    console.resetColor();
    
    console.setColor(DARK_CYAN);
    std::cout << std::left 
              << std::setw(5) << "No."
              << std::setw(8) << "ID"
              << std::setw(18) << "Patient"
              << std::setw(18) << "Doctor"
              << std::setw(12) << "Date" << std::endl;
    std::cout << std::string(61, '-') << std::endl;
    console.resetColor();
    
    for (int i = 0; i < (int)pending.size(); i++) {
        std::cout << std::setw(5) << (i + 1)
                  << std::setw(8) << pending[i].appointmentID
                  << std::setw(18) << pending[i].patientName.substr(0, 16)
                  << std::setw(18) << pending[i].doctorName.substr(0, 16)
                  << std::setw(12) << pending[i].appointmentDate << std::endl;
    }
    
    std::cout << std::endl;
    int choice = console.getIntInput("Select appointment (0 to go back): ", 0, (int)pending.size());
    
    if (choice == 0) return;
    
    Appointment selected = pending[choice - 1];
    
    console.clearScreen();
    console.printSubHeader("Appointment Details");
    std::cout << "\nPatient: " << selected.patientName << std::endl;
    std::cout << "Doctor: " << selected.doctorName << std::endl;
    std::cout << "Date: " << selected.appointmentDate << " | Time: " << selected.appointmentTime.substr(0, 5) << std::endl;
    std::cout << "Reason: " << selected.reason << std::endl;
    std::cout << std::string(45, '-') << std::endl;
    
    std::cin.ignore(10000, '\n');
    std::string confirm = console.getStringInput("Approve this appointment? (Y/N): ");
    
    if (confirm == "Y" || confirm == "y") {
        if (db.updateAppointmentStatus(selected.appointmentID, "Confirmed", session.userID)) {
            console.printSuccess("Appointment APT" + std::to_string(selected.appointmentID) + " has been APPROVED.");
            db.logActivity("Staff", session.userID, "Approve Appointment",
                          "Approved appointment ID: " + std::to_string(selected.appointmentID));
        } else {
            console.printError("Failed to approve appointment.");
        }
    } else if (confirm == "N" || confirm == "n") {
        std::string cancelConfirm = console.getStringInput("Cancel this appointment? (Y/N): ");
        if (cancelConfirm == "Y" || cancelConfirm == "y") {
            if (db.updateAppointmentStatus(selected.appointmentID, "Cancelled", session.userID)) {
                console.printSuccess("Appointment has been CANCELLED.");
                db.logActivity("Staff", session.userID, "Cancel Appointment",
                              "Cancelled appointment ID: " + std::to_string(selected.appointmentID));
            }
        }
    }
    
    console.pauseScreen();
}

void StaffModule::managePatients() {
    console.clearScreen();
    console.printHeader("MANAGE PATIENT RECORDS");
    
    std::vector<Patient> patients = db.getAllPatients();
    
    if (patients.empty()) {
        console.printInfo("No patients registered.");
        console.pauseScreen();
        return;
    }
    
    console.setColor(DARK_CYAN);
    std::cout << std::left 
              << std::setw(5) << "ID"
              << std::setw(22) << "Name"
              << std::setw(15) << "Phone"
              << std::setw(8) << "Gender"
              << std::setw(12) << "DOB" << std::endl;
    std::cout << std::string(62, '-') << std::endl;
    console.resetColor();
    
    for (const auto& p : patients) {
        std::cout << std::setw(5) << p.patientID
                  << std::setw(22) << p.patientName.substr(0, 20)
                  << std::setw(15) << p.phoneNumber
                  << std::setw(8) << p.gender
                  << std::setw(12) << p.dob << std::endl;
    }
    
    std::cout << "\nTotal Patients: " << patients.size() << std::endl;
    
    console.pauseScreen();
}

void StaffModule::generateReport() {
    console.clearScreen();
    console.printHeader("DAILY APPOINTMENT SUMMARY");
    
    AppointmentSummary summary = db.getDailyAppointmentSummary("");
    
    console.setColor(WHITE);
    std::cout << "\nTotal Appointments: " << summary.total << std::endl;
    console.setColor(GREEN);
    std::cout << "Confirmed: " << summary.confirmed << std::endl;
    console.setColor(YELLOW);
    std::cout << "Pending: " << summary.pending << std::endl;
    console.setColor(CYAN);
    std::cout << "Completed: " << summary.completed << std::endl;
    console.setColor(RED);
    std::cout << "Cancelled: " << summary.cancelled << std::endl;
    console.resetColor();
    
    std::cout << "\n" << std::string(45, '-') << std::endl;
    std::cout << "\nMost Active Doctor: " << db.getMostActiveDoctor() << std::endl;
    std::cout << "Most Frequent Patient: " << db.getMostFrequentPatient() << std::endl;
    
    console.setColor(GREEN);
    std::cout << "\nReport generated successfully on " << getCurrentDateTime() << std::endl;
    console.resetColor();
    
    db.logActivity("Staff", session.userID, "Generate Report", "Generated daily summary report");
    
    console.pauseScreen();
}

void StaffModule::showDashboard() {
    while (session.isLoggedIn) {
        console.clearScreen();
        console.printHeader("STAFF DASHBOARD");
        
        Staff staff = db.getStaffById(session.userID);
        
        console.setColor(WHITE);
        std::cout << "\nWelcome, " << staff.staffName << std::endl;
        std::cout << "Role: " << staff.role << std::endl;
        console.resetColor();
        
        // Show pending count
        std::vector<Appointment> pending = db.getAllAppointments("Pending");
        if (!pending.empty()) {
            console.setColor(YELLOW);
            std::cout << "\n[!] " << pending.size() << " appointment(s) pending approval" << std::endl;
            console.resetColor();
        }
        
        std::cout << std::endl;
        console.printMenuOption(1, "View Appointments");
        console.printMenuOption(2, "Approve / Cancel Appointment");
        console.printMenuOption(3, "Manage Patient Records");
        console.printMenuOption(4, "Generate Daily Report");
        console.printMenuOption(5, "Logout");
        
        int choice = console.getIntInput("\nEnter your choice: ", 1, 5);
        
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

