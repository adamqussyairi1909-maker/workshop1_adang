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

DoctorModule::DoctorModule(ConsoleUtils& c, DatabaseManager& d, UserSession& s)
    : console(c), db(d), session(s) {}

void DoctorModule::viewSchedule() {
    console.clearScreen();
    console.printHeader("TODAY'S SCHEDULE");
    
    Doctor doc = db.getDoctorById(session.userID);
    
    console.setColor(WHITE);
    std::cout << "\n  Doctor  : " << doc.doctorName << std::endl;
    std::cout << "  Date    : " << getCurrentDate() << std::endl;
    console.resetColor();
    
    std::vector<Appointment> appointments = db.getDoctorAppointments(session.userID, getCurrentDate());
    
    if (appointments.empty()) {
        console.printInfo("No appointments scheduled for today.");
    } else {
        std::cout << "\n  Today's Appointments:\n" << std::endl;
        
        console.setColor(DARK_CYAN);
        std::cout << "  " << std::left
                  << std::setw(10) << "Time"
                  << std::setw(25) << "Patient"
                  << std::setw(15) << "Status" << std::endl;
        std::cout << "  " << std::string(50, '-') << std::endl;
        console.resetColor();
        
        for (const auto& apt : appointments) {
            if (apt.status == "Confirmed") console.setColor(GREEN);
            else if (apt.status == "Pending") console.setColor(YELLOW);
            else console.setColor(WHITE);
            
            std::cout << "  " << std::setw(10) << apt.appointmentTime.substr(0, 5)
                      << std::setw(25) << apt.patientName
                      << std::setw(15) << apt.status << std::endl;
        }
        console.resetColor();
        
        std::cout << "\n  " << std::string(50, '-') << std::endl;
        std::cout << "  Total: " << appointments.size() << " appointment(s)" << std::endl;
    }
    
    console.pauseScreen();
}

void DoctorModule::viewAllAppointments() {
    console.clearScreen();
    console.printHeader("ALL MY APPOINTMENTS");
    
    std::vector<Appointment> appointments = db.getDoctorAppointments(session.userID);
    
    if (appointments.empty()) {
        console.printInfo("No appointments found.");
        console.pauseScreen();
        return;
    }
    
    console.setColor(WHITE);
    std::cout << "\n  All appointments:\n" << std::endl;
    console.resetColor();
    
    console.setColor(DARK_CYAN);
    std::cout << "  " << std::left 
              << std::setw(8) << "ID"
              << std::setw(20) << "Patient"
              << std::setw(12) << "Date"
              << std::setw(10) << "Time"
              << std::setw(12) << "Status" << std::endl;
    std::cout << "  " << std::string(62, '-') << std::endl;
    console.resetColor();
    
    for (const auto& apt : appointments) {
        if (apt.status == "Confirmed") console.setColor(GREEN);
        else if (apt.status == "Pending") console.setColor(YELLOW);
        else if (apt.status == "Cancelled") console.setColor(RED);
        else if (apt.status == "Completed") console.setColor(CYAN);
        else console.resetColor();
        
        std::cout << "  " << std::setw(8) << apt.appointmentID
                  << std::setw(20) << apt.patientName
                  << std::setw(12) << apt.appointmentDate
                  << std::setw(10) << apt.appointmentTime.substr(0, 5)
                  << std::setw(12) << apt.status << std::endl;
    }
    console.resetColor();
    
    std::cout << "\n  Total: " << appointments.size() << " appointment(s)" << std::endl;
    
    console.pauseScreen();
}

void DoctorModule::viewPatientList() {
    console.clearScreen();
    console.printHeader("MY PATIENTS");
    
    std::vector<Appointment> appointments = db.getDoctorAppointments(session.userID);
    
    std::vector<int> patientIDs;
    for (const auto& apt : appointments) {
        bool found = false;
        for (int id : patientIDs) {
            if (id == apt.patientID) {
                found = true;
                break;
            }
        }
        if (!found) patientIDs.push_back(apt.patientID);
    }
    
    if (patientIDs.empty()) {
        console.printInfo("No patients assigned yet.");
        console.pauseScreen();
        return;
    }
    
    console.setColor(WHITE);
    std::cout << "\n  Patients under your care:\n" << std::endl;
    console.resetColor();
    
    console.setColor(DARK_CYAN);
    std::cout << "  " << std::left 
              << std::setw(5) << "No."
              << std::setw(25) << "Patient Name"
              << std::setw(15) << "Phone"
              << std::setw(10) << "Gender" << std::endl;
    std::cout << "  " << std::string(55, '-') << std::endl;
    console.resetColor();
    
    int count = 1;
    for (int pid : patientIDs) {
        Patient p = db.getPatientById(pid);
        std::cout << "  " << std::setw(5) << count++
                  << std::setw(25) << p.patientName
                  << std::setw(15) << p.phoneNumber
                  << std::setw(10) << p.gender << std::endl;
    }
    
    std::cout << "\n  Total: " << patientIDs.size() << " patient(s)" << std::endl;
    
    console.pauseScreen();
}

void DoctorModule::showDashboard() {
    while (session.isLoggedIn) {
        console.clearScreen();
        console.printHeader("DOCTOR DASHBOARD");
        
        Doctor doc = db.getDoctorById(session.userID);
        
        console.setColor(WHITE);
        std::cout << "\n  Welcome, " << doc.doctorName << std::endl;
        console.setColor(DARK_GRAY);
        std::cout << "  Specialty: " << doc.specialty << " | Room: " << doc.roomNo << std::endl;
        console.resetColor();
        
        // Show today's appointment count
        std::vector<Appointment> todayApts = db.getDoctorAppointments(session.userID, getCurrentDate());
        if (!todayApts.empty()) {
            console.setColor(YELLOW);
            std::cout << "\n  [!] You have " << todayApts.size() << " appointment(s) today" << std::endl;
            console.resetColor();
        }
        
        std::cout << "\n  What would you like to do?\n" << std::endl;
        console.printMenuOption(1, "View Today's Schedule");
        console.printMenuOption(2, "View All Appointments");
        console.printMenuOption(3, "View Patient List");
        console.printMenuOption(4, "Logout");
        
        std::cout << std::endl;
        int choice = console.getIntInput("  Enter your choice: ", 1, 4);
        
        switch (choice) {
            case 1: viewSchedule(); break;
            case 2: viewAllAppointments(); break;
            case 3: viewPatientList(); break;
            case 4:
                db.logActivity("Doctor", session.userID, "Logout", "Doctor logged out");
                session = UserSession();
                console.printSuccess("Logged out successfully!");
                Sleep(1000);
                return;
        }
    }
}
