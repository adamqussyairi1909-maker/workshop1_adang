// ============================================================
// PatientModule.cpp - Patient Dashboard Implementation
// Hospital Appointment Booking System
// ============================================================

#include "PatientModule.h"
#include "Utilities.h"
#include <iostream>
#include <iomanip>
#include <vector>
#include <windows.h>

PatientModule::PatientModule(ConsoleUtils& c, DatabaseManager& d, UserSession& s)
    : console(c), db(d), session(s) {}

void PatientModule::registerPatient() {
    console.clearScreen();
    console.printHeader("PATIENT REGISTRATION");
    
    std::cin.ignore(10000, '\n');
    
    std::string name, phone, email, address, dob, gender, password, confirmPassword;
    
    console.printInfo("Please fill in your details:");
    std::cout << std::endl;
    
    name = console.getStringInput("Full Name: ");
    
    // Phone validation
    do {
        phone = console.getStringInput("Phone Number (e.g., 0123456789): ");
        if (!console.isValidPhone(phone)) {
            console.printError("Invalid phone format. Please try again.");
        }
    } while (!console.isValidPhone(phone));
    
    // Email validation
    do {
        email = console.getStringInput("Email Address: ");
        if (!console.isValidEmail(email)) {
            console.printError("Invalid email format. Please try again.");
        }
    } while (!console.isValidEmail(email));
    
    address = console.getStringInput("Home Address: ");
    
    // DOB validation
    do {
        dob = console.getStringInput("Date of Birth (YYYY-MM-DD): ");
        if (!console.isValidDate(dob)) {
            console.printError("Invalid date format. Please use YYYY-MM-DD.");
        }
    } while (!console.isValidDate(dob));
    
    // Gender selection
    console.printSubHeader("Select Gender");
    console.printMenuOption(1, "Male");
    console.printMenuOption(2, "Female");
    int genderChoice = console.getIntInput("Enter choice: ", 1, 2);
    gender = (genderChoice == 1) ? "Male" : "Female";
    
    // Password with confirmation
    do {
        console.setColor(CYAN);
        std::cout << "Password: ";
        console.resetColor();
        password = console.getPasswordInput();
        
        if (password.length() < 6) {
            console.printError("Password must be at least 6 characters.");
            continue;
        }
        
        console.setColor(CYAN);
        std::cout << "Confirm Password: ";
        console.resetColor();
        confirmPassword = console.getPasswordInput();
        
        if (password != confirmPassword) {
            console.printError("Passwords do not match. Please try again.");
        }
    } while (password.length() < 6 || password != confirmPassword);
    
    console.showLoading("Registering your account");
    
    if (db.registerPatient(name, phone, email, address, dob, gender, password)) {
        console.printSuccess("Registration successful! You can now login.");
        db.logActivity("Patient", db.getLastInsertId(), "Registration", "New patient registered");
    } else {
        console.printError("Registration failed. Email or phone may already exist.");
    }
    
    console.pauseScreen();
}

void PatientModule::bookAppointment() {
    console.clearScreen();
    console.printHeader("BOOK NEW APPOINTMENT");
    
    std::vector<Doctor> doctors = db.getAllDoctors(true);
    
    if (doctors.empty()) {
        console.printError("No doctors available at this time.");
        console.pauseScreen();
        return;
    }
    
    console.printSubHeader("Available Doctors");
    console.setColor(DARK_CYAN);
    std::cout << std::left 
              << std::setw(5) << "No." 
              << std::setw(25) << "Doctor Name" 
              << std::setw(15) << "Specialty" 
              << std::setw(10) << "Room" << std::endl;
    std::cout << std::string(55, '-') << std::endl;
    console.resetColor();
    
    for (size_t i = 0; i < doctors.size(); i++) {
        std::cout << std::setw(5) << (i + 1)
                  << std::setw(25) << doctors[i].doctorName
                  << std::setw(15) << doctors[i].specialty
                  << std::setw(10) << doctors[i].roomNo << std::endl;
    }
    
    std::cout << std::endl;
    int doctorChoice = console.getIntInput("Select Doctor (number): ", 1, (int)doctors.size());
    int selectedDoctorID = doctors[doctorChoice - 1].doctorID;
    
    std::cin.ignore(10000, '\n');
    
    std::string date, time, reason;
    
    // Date validation
    do {
        date = console.getStringInput("Appointment Date (YYYY-MM-DD): ");
        if (!console.isValidDate(date)) {
            console.printError("Invalid date format. Please use YYYY-MM-DD.");
        } else if (date < getCurrentDate()) {
            console.printError("Cannot book appointments in the past.");
            date = "";
        }
    } while (date.empty() || !console.isValidDate(date));
    
    // Time validation
    do {
        time = console.getStringInput("Time Slot (HH:MM, e.g., 10:30): ");
        if (!console.isValidTime(time)) {
            console.printError("Invalid time format. Please use HH:MM.");
        }
    } while (!console.isValidTime(time));
    
    reason = console.getStringInput("Reason for Visit: ");
    
    // Check if patient already has appointment with same doctor on same day
    if (!db.checkPatientDailyLimit(session.userID, selectedDoctorID, date)) {
        console.printError("You already have an appointment with this doctor on " + date);
        console.pauseScreen();
        return;
    }
    
    console.showLoading("Checking doctor availability");
    
    // Check doctor availability
    if (!db.checkDoctorAvailability(selectedDoctorID, date, time + ":00")) {
        console.printError("This time slot is not available. Please choose another.");
        console.pauseScreen();
        return;
    }
    
    // Create appointment
    if (db.createAppointment(session.userID, selectedDoctorID, date, time + ":00", reason)) {
        int appointmentID = db.getLastInsertId();
        
        console.printSuccess("Appointment booked successfully!");
        
        console.setColor(WHITE);
        std::cout << "\n" << std::string(45, '-') << std::endl;
        std::cout << "Appointment ID: APT" << getCurrentDate().substr(0, 4) << "-" << appointmentID << std::endl;
        std::cout << "Doctor: " << doctors[doctorChoice - 1].doctorName << std::endl;
        std::cout << "Date: " << date << std::endl;
        std::cout << "Time: " << time << std::endl;
        std::cout << std::string(45, '-') << std::endl;
        console.resetColor();
        
        db.logActivity("Patient", session.userID, "Book Appointment", 
                      "Booked appointment with Dr. " + doctors[doctorChoice - 1].doctorName);
    } else {
        console.printError("Failed to book appointment. Please try again.");
    }
    
    console.pauseScreen();
}

void PatientModule::viewAppointments() {
    console.clearScreen();
    console.printHeader("MY APPOINTMENTS");
    
    std::vector<Appointment> appointments = db.getPatientAppointments(session.userID);
    
    if (appointments.empty()) {
        console.printInfo("You have no appointments.");
        console.pauseScreen();
        return;
    }
    
    console.setColor(DARK_CYAN);
    std::cout << std::left 
              << std::setw(8) << "ID"
              << std::setw(20) << "Doctor"
              << std::setw(12) << "Date"
              << std::setw(10) << "Time"
              << std::setw(12) << "Status" << std::endl;
    std::cout << std::string(62, '-') << std::endl;
    console.resetColor();
    
    for (const auto& apt : appointments) {
        if (apt.status == "Confirmed") console.setColor(GREEN);
        else if (apt.status == "Pending") console.setColor(YELLOW);
        else if (apt.status == "Cancelled") console.setColor(RED);
        else if (apt.status == "Completed") console.setColor(CYAN);
        else console.resetColor();
        
        std::cout << std::setw(8) << apt.appointmentID
                  << std::setw(20) << apt.doctorName
                  << std::setw(12) << apt.appointmentDate
                  << std::setw(10) << apt.appointmentTime.substr(0, 5)
                  << std::setw(12) << apt.status << std::endl;
    }
    console.resetColor();
    
    std::cout << "\n" << std::string(62, '-') << std::endl;
    std::cout << "Total Appointments: " << appointments.size() << std::endl;
    
    console.pauseScreen();
}

void PatientModule::cancelAppointment() {
    console.clearScreen();
    console.printHeader("CANCEL APPOINTMENT");
    
    std::vector<Appointment> appointments = db.getPatientAppointments(session.userID);
    
    std::vector<Appointment> cancelable;
    for (const auto& apt : appointments) {
        if (apt.status == "Pending" || apt.status == "Confirmed") {
            cancelable.push_back(apt);
        }
    }
    
    if (cancelable.empty()) {
        console.printInfo("No appointments available for cancellation.");
        console.pauseScreen();
        return;
    }
    
    console.setColor(DARK_CYAN);
    std::cout << std::left 
              << std::setw(5) << "No."
              << std::setw(8) << "ID"
              << std::setw(20) << "Doctor"
              << std::setw(12) << "Date"
              << std::setw(10) << "Time" << std::endl;
    std::cout << std::string(55, '-') << std::endl;
    console.resetColor();
    
    for (size_t i = 0; i < cancelable.size(); i++) {
        std::cout << std::setw(5) << (i + 1)
                  << std::setw(8) << cancelable[i].appointmentID
                  << std::setw(20) << cancelable[i].doctorName
                  << std::setw(12) << cancelable[i].appointmentDate
                  << std::setw(10) << cancelable[i].appointmentTime.substr(0, 5) << std::endl;
    }
    
    std::cout << std::endl;
    int choice = console.getIntInput("Select appointment to cancel (0 to go back): ", 0, (int)cancelable.size());
    
    if (choice == 0) return;
    
    std::cin.ignore(10000, '\n');
    std::string confirm = console.getStringInput("Are you sure you want to cancel? (Y/N): ");
    
    if (confirm == "Y" || confirm == "y") {
        if (db.cancelAppointment(cancelable[choice - 1].appointmentID)) {
            console.printSuccess("Appointment cancelled successfully.");
            db.logActivity("Patient", session.userID, "Cancel Appointment",
                          "Cancelled appointment ID: " + std::to_string(cancelable[choice - 1].appointmentID));
        } else {
            console.printError("Failed to cancel appointment.");
        }
    } else {
        console.printInfo("Cancellation aborted.");
    }
    
    console.pauseScreen();
}

void PatientModule::updateDetails() {
    console.clearScreen();
    console.printHeader("UPDATE PERSONAL DETAILS");
    
    Patient p = db.getPatientById(session.userID);
    
    console.printInfo("Current Information:");
    std::cout << "\n";
    std::cout << "Name: " << p.patientName << std::endl;
    std::cout << "Phone: " << p.phoneNumber << std::endl;
    std::cout << "Email: " << p.email << std::endl;
    std::cout << "Address: " << p.address << std::endl;
    std::cout << std::endl;
    
    console.printSubHeader("Enter New Details (press Enter to keep current)");
    
    std::cin.ignore(10000, '\n');
    
    std::string name, phone, email, address;
    
    console.setColor(CYAN);
    std::cout << "New Name [" << p.patientName << "]: ";
    console.resetColor();
    std::getline(std::cin, name);
    if (name.empty()) name = p.patientName;
    
    do {
        console.setColor(CYAN);
        std::cout << "New Phone [" << p.phoneNumber << "]: ";
        console.resetColor();
        std::getline(std::cin, phone);
        if (phone.empty()) {
            phone = p.phoneNumber;
            break;
        }
        if (!console.isValidPhone(phone)) {
            console.printError("Invalid phone format.");
        }
    } while (!console.isValidPhone(phone));
    
    do {
        console.setColor(CYAN);
        std::cout << "New Email [" << p.email << "]: ";
        console.resetColor();
        std::getline(std::cin, email);
        if (email.empty()) {
            email = p.email;
            break;
        }
        if (!console.isValidEmail(email)) {
            console.printError("Invalid email format.");
        }
    } while (!console.isValidEmail(email));
    
    console.setColor(CYAN);
    std::cout << "New Address [" << p.address << "]: ";
    console.resetColor();
    std::getline(std::cin, address);
    if (address.empty()) address = p.address;
    
    if (db.updatePatient(session.userID, name, phone, email, address)) {
        session.userName = name;
        console.printSuccess("Details updated successfully!");
        db.logActivity("Patient", session.userID, "Update Profile", "Updated personal details");
    } else {
        console.printError("Failed to update details.");
    }
    
    console.pauseScreen();
}

void PatientModule::showDashboard() {
    while (session.isLoggedIn) {
        console.clearScreen();
        console.printHeader("PATIENT DASHBOARD");
        
        console.setColor(WHITE);
        std::cout << "\nWelcome, " << session.userName << "!" << std::endl;
        console.resetColor();
        
        std::cout << std::endl;
        console.printMenuOption(1, "Book Appointment");
        console.printMenuOption(2, "View Appointments");
        console.printMenuOption(3, "Cancel Appointment");
        console.printMenuOption(4, "Update Personal Details");
        console.printMenuOption(5, "Logout");
        
        int choice = console.getIntInput("\nEnter your choice: ", 1, 5);
        
        switch (choice) {
            case 1: bookAppointment(); break;
            case 2: viewAppointments(); break;
            case 3: cancelAppointment(); break;
            case 4: updateDetails(); break;
            case 5:
                db.logActivity("Patient", session.userID, "Logout", "User logged out");
                session = UserSession();
                console.printSuccess("Logged out successfully!");
                Sleep(1000);
                return;
        }
    }
}

