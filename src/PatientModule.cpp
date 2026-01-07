// ============================================================
// PatientModule.cpp - Patient Dashboard Implementation
// Hospital Appointment Booking System
// ============================================================

#include "../include/PatientModule.h"
#include "../include/Utilities.h"
#include <iostream>
#include <iomanip>
#include <vector>
#include <windows.h>
#undef max

// OOP: Constructor calls base class constructor
PatientModule::PatientModule(ConsoleUtils& c, DatabaseManager& d, UserSession& s)
    : BaseModule(c, d, s) {}

void PatientModule::registerPatient() {
    console.clearScreen();
    console.printHeader("PATIENT REGISTRATION");
    
    std::string name, phone, email, address, dob, gender, password, confirmPassword;
    
    console.setColor(CYAN);
    std::cout << "\n  Create your patient account to book appointments.\n" << std::endl;
    console.resetColor();
    
    console.setColor(DARK_GRAY);
    std::cout << "  ================================================" << std::endl;
    std::cout << "  STEP 1 OF 4: PERSONAL INFORMATION" << std::endl;
    std::cout << "  ================================================\n" << std::endl;
    console.resetColor();
    
    console.setColor(YELLOW);
    std::cout << "  >> Enter your full name (or press 0 to return to main menu): ";
    console.resetColor();
    
    if (std::cin.peek() == '\n') std::cin.ignore();
    std::getline(std::cin, name);
    
    // Check for back option
    if (name == "0") {
        console.printInfo("Registration cancelled. Returning to main menu.");
        Sleep(1000);
        return;
    }
    
    // Validate name
    if (name.empty()) {
        console.printError("Name cannot be empty!");
        console.pauseScreen();
        return;
    }
    
    if (name.length() < 2) {
        console.printError("Name must be at least 2 characters long!");
        console.pauseScreen();
        return;
    }
    
    // Phone validation
    do {
        console.setColor(YELLOW);
        std::cout << "  >> Enter your phone number (10-12 digits, or press 0 to return): ";
        console.resetColor();
        std::getline(std::cin, phone);
        
        if (phone == "0") {
            console.printInfo("Registration cancelled. Returning to main menu.");
            Sleep(1000);
            return;
        }
        
        if (phone.empty()) {
            console.printError("Phone number cannot be empty!");
            continue;
        }
        
        if (!console.isValidPhone(phone)) {
            console.printError("Invalid format! Example: 0123456789");
        }
    } while (!console.isValidPhone(phone));
    
    // Email validation
    do {
        console.setColor(YELLOW);
        std::cout << "  >> Enter your email address (or press 0 to return): ";
        console.resetColor();
        std::getline(std::cin, email);
        
        if (email == "0") {
            console.printInfo("Registration cancelled. Returning to main menu.");
            Sleep(1000);
            return;
        }
        
        if (email.empty()) {
            console.printError("Email cannot be empty!");
            continue;
        }
        
        if (!console.isValidEmail(email)) {
            console.printError("Invalid format! Example: name@email.com");
        }
    } while (!console.isValidEmail(email));
    
    // Address
    console.setColor(YELLOW);
    std::cout << "  >> Enter your home address (or press 0 to return): ";
    console.resetColor();
    std::getline(std::cin, address);
    
    if (address == "0") {
        console.printInfo("Registration cancelled. Returning to main menu.");
        Sleep(1000);
        return;
    }
    
    if (address.empty()) {
        console.printError("Address cannot be empty!");
        console.pauseScreen();
        return;
    }
    
    std::cout << std::endl;
    console.setColor(DARK_GRAY);
    std::cout << "  ================================================" << std::endl;
    std::cout << "  STEP 2 OF 4: DATE OF BIRTH & GENDER" << std::endl;
    std::cout << "  ================================================" << std::endl;
    console.resetColor();
    
    // DOB validation
    std::cout << std::endl;
    do {
        console.setColor(YELLOW);
        std::cout << "  >> Enter your date of birth (YYYY-MM-DD, or press 0 to return): ";
        console.resetColor();
        std::getline(std::cin, dob);
        
        if (dob == "0") {
            console.printInfo("Registration cancelled. Returning to main menu.");
            Sleep(1000);
            return;
        }
        
        if (dob.empty()) {
            console.printError("Date of birth cannot be empty!");
            continue;
        }
        
        if (!console.isValidDate(dob)) {
            console.printError("Invalid format! Example: 1990-05-15");
        }
    } while (!console.isValidDate(dob));
    
    // Gender selection
    console.setColor(YELLOW);
    std::cout << "  >> Select your gender (1=Male, 2=Female, 0=Return): ";
    console.resetColor();
    int genderChoice = console.getIntInput("", 0, 2);
    
    if (genderChoice == 0) {
        console.printInfo("Registration cancelled. Returning to main menu.");
        Sleep(1000);
        return;
    }
    
    gender = (genderChoice == 1) ? "Male" : "Female";
    
    
    std::cout << std::endl;
    console.setColor(DARK_GRAY);
    std::cout << "  ================================================" << std::endl;
    std::cout << "  STEP 3 OF 4: CREATE PASSWORD" << std::endl;
    std::cout << "  ================================================\n" << std::endl;
    console.resetColor();
    
    do {
        console.setColor(YELLOW);
        std::cout << "  >> Create a password (min 6 characters, or type '0' to return): ";
        console.resetColor();
        password = console.getPasswordInput();
        
        if (password == "0") {
            console.printInfo("Registration cancelled. Returning to main menu.");
            Sleep(1000);
            return;
        }
        
        if (password.empty()) {
            console.printError("Password cannot be empty!");
            continue;
        }
        
        if (password.length() < 6) {
            console.printError("Too short! Minimum 6 characters.");
            continue;
        }
        
        console.setColor(YELLOW);
        std::cout << "  >> Confirm your password: ";
        console.resetColor();
        confirmPassword = console.getPasswordInput();
        
        if (confirmPassword == "0") {
            console.printInfo("Registration cancelled. Returning to main menu.");
            Sleep(1000);
            return;
        }
        
        if (password != confirmPassword) {
            console.printError("Passwords don't match! Try again.");
        }
    } while (password.length() < 6 || password != confirmPassword);
    
    // Review
    std::cout << std::endl;
    console.setColor(DARK_GRAY);
    std::cout << "  ================================================" << std::endl;
    std::cout << "  STEP 4 OF 4: REVIEW & CONFIRM" << std::endl;
    std::cout << "  ================================================" << std::endl;
    console.resetColor();
    
    std::cout << std::endl;
    console.setColor(WHITE);
    std::cout << "  Please review your information:" << std::endl;
    std::cout << "  +-------------------------------------------+" << std::endl;
    std::cout << "  | Name    : " << std::left << std::setw(31) << name << "|" << std::endl;
    std::cout << "  | Phone   : " << std::left << std::setw(31) << phone << "|" << std::endl;
    std::cout << "  | Email   : " << std::left << std::setw(31) << email << "|" << std::endl;
    std::cout << "  | Address : " << std::left << std::setw(31) << (address.length() > 30 ? address.substr(0,27) + "..." : address) << "|" << std::endl;
    std::cout << "  | DOB     : " << std::left << std::setw(31) << dob << "|" << std::endl;
    std::cout << "  | Gender  : " << std::left << std::setw(31) << gender << "|" << std::endl;
    std::cout << "  +-------------------------------------------+" << std::endl;
    console.resetColor();
    
    
    console.setColor(YELLOW);
    std::cout << "  >> Confirm registration (Y/N, or press 0 to return): ";
    console.resetColor();
    std::string confirm;
    std::getline(std::cin, confirm);
    
    if (confirm == "0") {
        console.printInfo("Registration cancelled. Returning to main menu.");
        Sleep(1000);
        return;
    }
    
    if (confirm != "Y" && confirm != "y") {
        console.printInfo("Registration cancelled. No account created.");
        console.pauseScreen();
        return;
    }
    
    console.showLoading("\n  Creating your account", 3);
    
    if (db.registerPatient(name, phone, email, address, dob, gender, password)) {
        console.setColor(GREEN);
        std::cout << "\n  +-----------------------------------------+" << std::endl;
        std::cout << "  |       REGISTRATION SUCCESSFUL!          |" << std::endl;
        std::cout << "  +-----------------------------------------+" << std::endl;
        console.resetColor();
        
        std::cout << std::endl;
        console.setColor(WHITE);
        std::cout << "  Your account has been created!" << std::endl;
        std::cout << "  You can now login using:" << std::endl;
        std::cout << "    - Email: " << email << std::endl;
        std::cout << "    - Password: (the one you created)" << std::endl;
        console.resetColor();
        
        db.logActivity("Patient", db.getLastInsertId(), "Registration", "New patient registered");
    } else {
        console.printError("Registration failed!");
        console.setColor(WHITE);
        std::cout << "  Possible reasons:" << std::endl;
        std::cout << "    - Email already registered" << std::endl;
        std::cout << "    - Phone number already registered" << std::endl;
        console.resetColor();
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
    
    console.setColor(CYAN);
    std::cout << "\n  Book an appointment with one of our doctors.\n" << std::endl;
    console.resetColor();
    
    console.setColor(DARK_GRAY);
    std::cout << "  ================================================" << std::endl;
    std::cout << "  STEP 1 OF 4: SELECT A DOCTOR" << std::endl;
    std::cout << "  ================================================" << std::endl;
    console.resetColor();
    
    console.setColor(DARK_CYAN);
    std::cout << "  " << std::left 
              << std::setw(5) << "No." 
              << std::setw(25) << "Doctor Name" 
              << std::setw(15) << "Specialty" 
              << std::setw(10) << "Room" << std::endl;
    std::cout << "  " << std::string(55, '-') << std::endl;
    console.resetColor();
    
    std::cout << std::endl;
    for (int i = 0; i < (int)doctors.size(); i++) {
        std::cout << "  " << std::setw(5) << (i + 1)
                  << std::setw(25) << doctors[i].doctorName
                  << std::setw(15) << doctors[i].specialty
                  << std::setw(10) << doctors[i].roomNo << std::endl;
    }
    
    std::cout << std::endl;
    console.setColor(YELLOW);
    std::cout << "  >> Select a doctor from the list above (or enter 0 to return):" << std::endl;
    console.resetColor();
    int doctorChoice = console.getIntInput("     Enter number (0-" + std::to_string(doctors.size()) + "): ", 0, (int)doctors.size());
    
    if (doctorChoice == 0) {
        console.printInfo("Booking cancelled. Returning to main menu.");
        Sleep(1000);
        return;
    }
    
    int selectedDoctorID = doctors[doctorChoice - 1].doctorID;
    
    console.setColor(GREEN);
    std::cout << "\n  [OK] Selected: " << doctors[doctorChoice - 1].doctorName << " (" << doctors[doctorChoice - 1].specialty << ")" << std::endl;
    console.resetColor();
    
    std::cin.ignore(10000, '\n');
    
    std::string date, time, reason;
    
    // Step 2: Select Date
    std::cout << std::endl;
    console.setColor(DARK_GRAY);
    std::cout << "  ================================================" << std::endl;
    std::cout << "  STEP 2 OF 4: SELECT DATE AND TIME" << std::endl;
    std::cout << "  ================================================" << std::endl;
    console.resetColor();
    
    std::cout << std::endl;
    console.setColor(YELLOW);
    std::cout << "  >> Enter the date for your appointment:" << std::endl;
    console.setColor(DARK_GRAY);
    std::cout << "     Format: YYYY-MM-DD (e.g., 2025-12-15)" << std::endl;
    std::cout << "     Note: Cannot book past dates." << std::endl;
    console.resetColor();
    
    do {
        date = console.getStringInput("     Appointment Date (or press 0 to return): ");
        
        if (date == "0") {
            console.printInfo("Booking cancelled. Returning to main menu.");
            Sleep(1000);
            return;
        }
        
        if (date.empty()) {
            console.printError("Date cannot be empty!");
            continue;
        }
        
        if (!console.isValidDate(date)) {
            console.printError("Invalid format! Use YYYY-MM-DD");
            date = "";
        } else if (date < getCurrentDate()) {
            console.printError("Cannot book past dates!");
            date = "";
        }
    } while (date.empty() || !console.isValidDate(date));
    
    std::cout << std::endl;
    console.setColor(YELLOW);
    std::cout << "  >> Enter the time for your appointment:" << std::endl;
    console.setColor(DARK_GRAY);
    std::cout << "     Format: HH:MM in 24-hour (e.g., 09:30 or 14:00)" << std::endl;
    std::cout << "     Clinic hours: 08:00 - 17:00" << std::endl;
    console.resetColor();
    
    do {
        time = console.getStringInput("     Appointment Time (or press 0 to return): ");
        
        if (time == "0") {
            console.printInfo("Booking cancelled. Returning to main menu.");
            Sleep(1000);
            return;
        }
        
        if (time.empty()) {
            console.printError("Time cannot be empty!");
            continue;
        }
        
        if (!console.isValidTime(time)) {
            console.printError("Invalid format! Use HH:MM (e.g., 10:30)");
        } else {
            // Validate clinic hours (08:00 - 17:00)
            int hour = std::stoi(time.substr(0, 2));
            if (hour < 8 || hour >= 17) {
                console.printError("Clinic hours are 08:00 - 17:00. Please select a time within these hours.");
                time = "";
            }
        }
    } while (time.empty() || !console.isValidTime(time));
    
    // Step 3: Duration
    std::cout << std::endl;
    console.setColor(DARK_GRAY);
    std::cout << "  ================================================" << std::endl;
    std::cout << "  STEP 3 OF 4: APPOINTMENT DURATION" << std::endl;
    std::cout << "  ================================================" << std::endl;
    console.resetColor();
    
    std::cout << std::endl;
    console.setColor(YELLOW);
    std::cout << "  >> Select expected appointment duration (in minutes):" << std::endl;
    console.setColor(DARK_GRAY);
    std::cout << "     15 minutes - Quick checkup, Prescription refill" << std::endl;
    std::cout << "     30 minutes - Standard consultation, Follow-up" << std::endl;
    std::cout << "     45 minutes - Detailed examination, Multiple concerns" << std::endl;
    std::cout << "     60 minutes - Complex consultation, Procedure" << std::endl;
    std::cout << "     0 - Return to main menu" << std::endl;
    console.resetColor();
    
    int duration;
    do {
        duration = console.getIntInput("     Duration (15/30/45/60, or 0 to return): ", 0, 60);
        if (duration == 0) {
            console.printInfo("Booking cancelled. Returning to main menu.");
            Sleep(1000);
            return;
        }
        if (duration != 15 && duration != 30 && duration != 45 && duration != 60) {
            console.printError("Invalid duration! Please select 15, 30, 45, or 60 minutes.");
        }
    } while (duration != 15 && duration != 30 && duration != 45 && duration != 60);
    
    // Calculate reason category based on duration
    std::string reasonCategory;
    if (duration <= 15) {
        reasonCategory = "Quick Checkup";
    } else if (duration <= 30) {
        reasonCategory = "Standard Consultation";
    } else if (duration <= 45) {
        reasonCategory = "Detailed Examination";
    } else {
        reasonCategory = "Complex Consultation";
    }
    
    // Step 4: Reason
    std::cout << std::endl;
    console.setColor(DARK_GRAY);
    std::cout << "  ================================================" << std::endl;
    std::cout << "  STEP 4 OF 4: REASON FOR VISIT" << std::endl;
    std::cout << "  ================================================" << std::endl;
    console.resetColor();
    
    std::cout << std::endl;
    console.setColor(CYAN);
    std::cout << "  [CALCULATED] Reason Category: " << reasonCategory << " (based on " << duration << " min duration)" << std::endl;
    console.resetColor();
    
    std::cout << std::endl;
    console.setColor(YELLOW);
    std::cout << "  >> Briefly describe why you need to see the doctor:" << std::endl;
    console.setColor(DARK_GRAY);
    std::cout << "     Example: Fever and headache, Follow-up checkup" << std::endl;
    std::cout << "     Suggested: " << reasonCategory << " - " << std::endl;
    console.resetColor();
    
    reason = console.getStringInput("     Reason: ");
    
    if (reason.empty()) {
        console.printError("Reason cannot be empty!");
        console.pauseScreen();
        return;
    }
    
    // Append duration info to reason
    reason = reason + " [" + reasonCategory + " - " + std::to_string(duration) + " min]";
    
    // Check daily limit
    if (!db.checkPatientDailyLimit(session.userID, selectedDoctorID, date)) {
        console.printError("You already have an appointment with this doctor on " + date);
        console.printInfo("You can only book ONE appointment per doctor per day.");
        console.pauseScreen();
        return;
    }
    
    console.showLoading("\n  Checking availability", 2);
    
    if (!db.checkDoctorAvailability(selectedDoctorID, date, time + ":00")) {
        console.printError("This time slot is already booked!");
        console.printInfo("Please choose a different time.");
        console.pauseScreen();
        return;
    }
    
    // Create appointment
    if (db.createAppointment(session.userID, selectedDoctorID, date, time + ":00", reason, duration)) {
        int appointmentID = db.getLastInsertId();
        double cost = db.calculateCost(duration);
        
        console.setColor(GREEN);
        std::cout << "\n  +-----------------------------------------+" << std::endl;
        std::cout << "  |    APPOINTMENT BOOKED SUCCESSFULLY!     |" << std::endl;
        std::cout << "  +-----------------------------------------+" << std::endl;
        console.resetColor();
        
        std::cout << std::endl;
        console.setColor(WHITE);
        std::cout << "  YOUR APPOINTMENT DETAILS:" << std::endl;
        std::cout << "  " << std::string(40, '-') << std::endl;
        std::cout << "  Appointment ID : APT-" << appointmentID << std::endl;
        std::cout << "  Doctor         : " << doctors[doctorChoice - 1].doctorName << std::endl;
        std::cout << "  Specialty      : " << doctors[doctorChoice - 1].specialty << std::endl;
        std::cout << "  Room           : " << doctors[doctorChoice - 1].roomNo << std::endl;
        std::cout << "  Date           : " << date << std::endl;
        std::cout << "  Time           : " << time << std::endl;
        std::cout << "  Duration       : " << duration << " minutes" << std::endl;
        std::cout << "  Cost           : RM " << std::fixed << std::setprecision(2) << cost << std::endl;
        std::cout << "  Category       : " << reasonCategory << std::endl;
        std::cout << "  Status         : Pending Approval" << std::endl;
        std::cout << "  " << std::string(40, '-') << std::endl;
        console.resetColor();
        
        console.setColor(YELLOW);
        std::cout << "\n  Note: Your appointment is PENDING." << std::endl;
        std::cout << "  Staff will approve it shortly." << std::endl;
        console.resetColor();
        
        db.logActivity("Patient", session.userID, "Book Appointment", 
                      "Booked with Dr. " + doctors[doctorChoice - 1].doctorName);
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
        console.printInfo("You have no appointments yet.");
        console.setColor(WHITE);
        std::cout << "\n  To book an appointment:" << std::endl;
        std::cout << "  Go to Main Menu > Book New Appointment" << std::endl;
        console.resetColor();
        console.pauseScreen();
        return;
    }
    
    console.setColor(WHITE);
    std::cout << "\n  Here are all your appointments.\n" << std::endl;
    console.resetColor();
    
    console.setColor(DARK_CYAN);
    std::cout << "  " << std::left 
              << std::setw(6) << "ID"
              << std::setw(18) << "Doctor"
              << std::setw(12) << "Date"
              << std::setw(8) << "Time"
              << std::setw(8) << "Cost"
              << std::setw(12) << "Status" << std::endl;
    std::cout << "  " << std::string(64, '-') << std::endl;
    console.resetColor();
    
    double totalCost = 0.0;
    for (const auto& apt : appointments) {
        if (apt.status == "Confirmed") console.setColor(GREEN);
        else if (apt.status == "Pending") console.setColor(YELLOW);
        else if (apt.status == "Cancelled") console.setColor(RED);
        else if (apt.status == "Completed") console.setColor(CYAN);
        else console.resetColor();
        
        std::string doctorName = apt.doctorName.length() > 16 ? 
            apt.doctorName.substr(0, 13) + "..." : apt.doctorName;
        
        std::cout << "  " << std::setw(6) << apt.appointmentID
                  << std::setw(18) << doctorName
                  << std::setw(12) << apt.appointmentDate
                  << std::setw(8) << apt.appointmentTime.substr(0, 5)
                  << std::setw(8) << ("RM" + std::to_string((int)apt.cost))
                  << std::setw(12) << apt.status << std::endl;
        
        if (apt.status != "Cancelled") {
            totalCost += apt.cost;
        }
    }
    console.resetColor();
    
    std::cout << "\n  " << std::string(64, '-') << std::endl;
    std::cout << "  Total: " << appointments.size() << " appointment(s)";
    console.setColor(CYAN);
    std::cout << "  |  Total Cost: RM " << std::fixed << std::setprecision(2) << totalCost;
    console.resetColor();
    std::cout << std::endl;
    
    // Legend
    std::cout << "\n  STATUS LEGEND:" << std::endl;
    console.setColor(YELLOW); std::cout << "  - Pending   : Waiting for staff approval" << std::endl;
    console.setColor(GREEN); std::cout << "  - Confirmed : Approved, ready for visit" << std::endl;
    console.setColor(CYAN); std::cout << "  - Completed : Visit done" << std::endl;
    console.setColor(RED); std::cout << "  - Cancelled : Appointment cancelled" << std::endl;
    console.resetColor();
    
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
        console.setColor(WHITE);
        std::cout << "\n  Only Pending or Confirmed appointments can be cancelled." << std::endl;
        console.resetColor();
        console.pauseScreen();
        return;
    }
    
    console.setColor(WHITE);
    std::cout << "\n  Select an appointment to cancel.\n" << std::endl;
    console.resetColor();
    
    console.setColor(DARK_CYAN);
    std::cout << "  " << std::left 
              << std::setw(5) << "No."
              << std::setw(8) << "ID"
              << std::setw(20) << "Doctor"
              << std::setw(12) << "Date"
              << std::setw(10) << "Time" << std::endl;
    std::cout << "  " << std::string(55, '-') << std::endl;
    console.resetColor();
    
    for (int i = 0; i < (int)cancelable.size(); i++) {
        std::cout << "  " << std::setw(5) << (i + 1)
                  << std::setw(8) << cancelable[i].appointmentID
                  << std::setw(20) << cancelable[i].doctorName
                  << std::setw(12) << cancelable[i].appointmentDate
                  << std::setw(10) << cancelable[i].appointmentTime.substr(0, 5) << std::endl;
    }
    
    std::cout << std::endl;
    console.setColor(YELLOW);
    std::cout << "  >> Select an appointment to cancel (or enter 0 to go back):" << std::endl;
    console.resetColor();
    int choice = console.getIntInput("     Enter number (0-" + std::to_string(cancelable.size()) + "): ", 0, (int)cancelable.size());
    
    if (choice == 0) return;
    
    std::cin.ignore(10000, '\n');
    
    console.setColor(RED);
    std::cout << "\n  WARNING: This action CANNOT be undone!" << std::endl;
    console.resetColor();
    
    console.setColor(WHITE);
    std::cout << "\n  APPOINTMENT TO CANCEL:" << std::endl;
    std::cout << "  +-----------------------------------+" << std::endl;
    std::cout << "  | Doctor : " << std::left << std::setw(25) << cancelable[choice - 1].doctorName << "|" << std::endl;
    std::cout << "  | Date   : " << std::left << std::setw(25) << cancelable[choice - 1].appointmentDate << "|" << std::endl;
    std::cout << "  | Time   : " << std::left << std::setw(25) << cancelable[choice - 1].appointmentTime.substr(0, 5) << "|" << std::endl;
    std::cout << "  +-----------------------------------+" << std::endl;
    console.resetColor();
    
    console.setColor(RED);
    std::cout << "\n  WARNING: This action CANNOT be undone!" << std::endl;
    console.resetColor();
    
    std::cout << std::endl;
    console.setColor(YELLOW);
    std::cout << "  >> Confirm cancellation:" << std::endl;
    console.resetColor();
    std::string confirm = console.getStringInput("     Type Y to confirm, N to go back: ");
    
    if (confirm == "Y" || confirm == "y") {
        if (db.cancelAppointment(cancelable[choice - 1].appointmentID)) {
            console.printSuccess("Appointment cancelled successfully.");
            db.logActivity("Patient", session.userID, "Cancel Appointment",
                          "Cancelled ID: " + std::to_string(cancelable[choice - 1].appointmentID));
        } else {
            console.printError("Failed to cancel appointment.");
        }
    } else {
        console.printInfo("Cancellation aborted. Appointment NOT cancelled.");
    }
    
    console.pauseScreen();
}

void PatientModule::updateDetails() {
    console.clearScreen();
    console.printHeader("UPDATE PERSONAL DETAILS");
    
    Patient p = db.getPatientById(session.userID);
    
    console.setColor(WHITE);
    std::cout << "\n  Update your personal information." << std::endl;
    std::cout << "  Press ENTER to keep the current value.\n" << std::endl;
    console.resetColor();
    
    console.setColor(DARK_GRAY);
    std::cout << "  ------------------------------------------------" << std::endl;
    std::cout << "  CURRENT INFORMATION" << std::endl;
    std::cout << "  ------------------------------------------------\n" << std::endl;
    console.resetColor();
    
    console.setColor(WHITE);
    std::cout << "  Name    : " << p.patientName << std::endl;
    std::cout << "  Phone   : " << p.phoneNumber << std::endl;
    std::cout << "  Email   : " << p.email << std::endl;
    std::cout << "  Address : " << p.address << std::endl;
    console.resetColor();
    
    std::cout << std::endl;
    console.setColor(DARK_GRAY);
    std::cout << "  ------------------------------------------------" << std::endl;
    std::cout << "  ENTER NEW VALUES" << std::endl;
    std::cout << "  ------------------------------------------------\n" << std::endl;
    console.resetColor();
    
    // Clear input buffer properly
    if (std::cin.peek() == '\n') std::cin.ignore();
    std::cin.ignore(10000, '\n');
    
    std::string name, phone, email, address;
    
    // Name input - direct prompt
    console.setColor(YELLOW);
    std::cout << "  >> Name: ";
    console.resetColor();
    std::getline(std::cin, name);
    if (name.empty()) name = p.patientName;
    
    // Phone input - direct prompt
    std::cout << std::endl;
    do {
        console.setColor(YELLOW);
        std::cout << "  >> Phone: ";
        console.resetColor();
        std::getline(std::cin, phone);
        if (phone.empty()) {
            phone = p.phoneNumber;
            break;
        }
        if (!console.isValidPhone(phone)) {
            console.printError("Invalid format! Use 10-12 digits.");
        }
    } while (!phone.empty() && !console.isValidPhone(phone));
    
    // Email input - direct prompt
    std::cout << std::endl;
    do {
        console.setColor(YELLOW);
        std::cout << "  >> Email: ";
        console.resetColor();
        std::getline(std::cin, email);
        if (email.empty()) {
            email = p.email;
            break;
        }
        if (!console.isValidEmail(email)) {
            console.printError("Invalid format! Include @ and domain.");
        }
    } while (!email.empty() && !console.isValidEmail(email));
    
    // Address input - direct prompt
    std::cout << std::endl;
    console.setColor(YELLOW);
    std::cout << "  >> Address: ";
    console.resetColor();
    std::getline(std::cin, address);
    if (address.empty()) address = p.address;
    
    std::cout << std::endl;
    std::string confirm = console.getStringInput("  Save these changes? (Y/N): ");
    
    if (confirm == "Y" || confirm == "y") {
        if (db.updatePatient(session.userID, name, phone, email, address)) {
            session.userName = name;
            console.printSuccess("Your details have been updated!");
            db.logActivity("Patient", session.userID, "Update Profile", "Updated personal details");
        } else {
            console.printError("Failed to update. Email/phone may already exist.");
        }
    } else {
        console.printInfo("Changes discarded. Nothing was updated.");
    }
    
    console.pauseScreen();
}

void PatientModule::showDashboard() {
    while (session.isLoggedIn) {
        console.clearScreen();
        console.printHeader("PATIENT DASHBOARD");
        
        console.setColor(CYAN);
        std::cout << "\n  Welcome back, " << session.userName << "!\n" << std::endl;
        console.resetColor();
        
        console.setColor(DARK_GRAY);
        std::cout << "  ================================================" << std::endl;
        std::cout << "  MENU" << std::endl;
        std::cout << "  ================================================" << std::endl;
        console.resetColor();
        
        std::cout << std::endl;
        console.printMenuOption(1, "Book New Appointment");
        console.printMenuOption(2, "View My Appointments");
        console.printMenuOption(3, "Cancel Appointment");
        console.printMenuOption(4, "Update Personal Details");
        console.printMenuOption(5, "Logout");
        
        std::cout << std::endl;
        console.setColor(YELLOW);
        std::cout << "  >> Enter your choice (1-5): ";
        console.resetColor();
        int choice = console.getIntInput("", 1, 5);
        
        switch (choice) {
            case 1: bookAppointment(); break;
            case 2: viewAppointments(); break;
            case 3: cancelAppointment(); break;
            case 4: updateDetails(); break;
            case 5:
                db.logActivity("Patient", session.userID, "Logout", "User logged out");
                session = UserSession();
                console.printSuccess("You have been logged out successfully!");
                Sleep(1500);
                return;
        }
    }
}
