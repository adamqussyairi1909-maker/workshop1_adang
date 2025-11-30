// ============================================================
// PatientModule.cpp - Patient Dashboard Implementation
// Hospital Appointment Booking System
// ============================================================

#include "../include/PatientModule.h"
#include "../include/Utilities.h"
#include <iostream>
#include <iomanip>
#include <limits>
#include <vector>
#include <windows.h>

PatientModule::PatientModule(ConsoleUtils& c, DatabaseManager& d, UserSession& s)
    : console(c), db(d), session(s) {}

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
    
    // Full Name
    console.setColor(YELLOW);
    std::cout << "  >> Enter your full name as per IC/Passport:" << std::endl;
    console.resetColor();
    
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    name = console.getStringInput("     Full Name: ");
    
    // Phone validation
    std::cout << std::endl;
    console.setColor(YELLOW);
    std::cout << "  >> Enter your phone number (10-12 digits):" << std::endl;
    console.setColor(DARK_GRAY);
    std::cout << "     Example: 0123456789 or 60123456789" << std::endl;
    console.resetColor();
    do {
        phone = console.getStringInput("     Phone Number: ");
        if (!console.isValidPhone(phone)) {
            console.printError("Invalid format! Use 10-12 digits only.");
        }
    } while (!console.isValidPhone(phone));
    
    // Email validation
    std::cout << std::endl;
    console.setColor(YELLOW);
    std::cout << "  >> Enter your email address (for login):" << std::endl;
    console.setColor(DARK_GRAY);
    std::cout << "     Example: yourname@email.com" << std::endl;
    console.resetColor();
    do {
        email = console.getStringInput("     Email Address: ");
        if (!console.isValidEmail(email)) {
            console.printError("Invalid format! Include @ and domain (e.g., .com)");
        }
    } while (!console.isValidEmail(email));
    
    // Address
    std::cout << std::endl;
    console.setColor(YELLOW);
    std::cout << "  >> Enter your home address:" << std::endl;
    console.resetColor();
    address = console.getStringInput("     Home Address: ");
    
    std::cout << std::endl;
    console.setColor(DARK_GRAY);
    std::cout << "  ================================================" << std::endl;
    std::cout << "  STEP 2 OF 4: DATE OF BIRTH & GENDER" << std::endl;
    std::cout << "  ================================================" << std::endl;
    console.resetColor();
    
    // DOB validation
    std::cout << std::endl;
    console.setColor(YELLOW);
    std::cout << "  >> Enter your date of birth:" << std::endl;
    console.setColor(DARK_GRAY);
    std::cout << "     Format: YYYY-MM-DD (e.g., 1990-05-15)" << std::endl;
    console.resetColor();
    do {
        dob = console.getStringInput("     Date of Birth: ");
        if (!console.isValidDate(dob)) {
            console.printError("Invalid format! Use YYYY-MM-DD");
        }
    } while (!console.isValidDate(dob));
    
    // Gender selection
    std::cout << std::endl;
    console.setColor(YELLOW);
    std::cout << "  >> Select your gender:" << std::endl;
    console.resetColor();
    console.printMenuOption(1, "Male");
    console.printMenuOption(2, "Female");
    int genderChoice = console.getIntInput("     Enter 1 or 2: ", 1, 2);
    gender = (genderChoice == 1) ? "Male" : "Female";
    
    std::cout << std::endl;
    console.setColor(DARK_GRAY);
    std::cout << "  ================================================" << std::endl;
    std::cout << "  STEP 3 OF 4: CREATE PASSWORD" << std::endl;
    std::cout << "  ================================================" << std::endl;
    console.resetColor();
    
    std::cout << std::endl;
    console.setColor(YELLOW);
    std::cout << "  >> Create a password (minimum 6 characters):" << std::endl;
    console.setColor(DARK_GRAY);
    std::cout << "     You will use this password to login." << std::endl;
    console.resetColor();
    
    do {
        console.setColor(CYAN);
        std::cout << "     Password: ";
        console.resetColor();
        password = console.getPasswordInput();
        
        if (password.length() < 6) {
            console.printError("Too short! Minimum 6 characters required.");
            continue;
        }
        
        console.setColor(CYAN);
        std::cout << "     Confirm Password: ";
        console.resetColor();
        confirmPassword = console.getPasswordInput();
        
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
    
    std::cout << std::endl;
    console.setColor(YELLOW);
    std::cout << "  >> Is this information correct?" << std::endl;
    console.resetColor();
    std::string confirm = console.getStringInput("     Type Y to confirm, N to cancel: ");
    
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
    std::cout << "  STEP 1 OF 3: SELECT A DOCTOR" << std::endl;
    std::cout << "  ================================================\n" << std::endl;
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
    std::cout << "  >> Select a doctor from the list above:" << std::endl;
    console.resetColor();
    int doctorChoice = console.getIntInput("     Enter number (1-" + std::to_string(doctors.size()) + "): ", 1, (int)doctors.size());
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
    std::cout << "  STEP 2 OF 3: SELECT DATE AND TIME" << std::endl;
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
        date = console.getStringInput("     Appointment Date: ");
        if (!console.isValidDate(date)) {
            console.printError("Invalid format! Use YYYY-MM-DD");
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
        time = console.getStringInput("     Appointment Time: ");
        if (!console.isValidTime(time)) {
            console.printError("Invalid format! Use HH:MM (e.g., 10:30)");
        }
    } while (!console.isValidTime(time));
    
    // Step 3: Reason
    std::cout << std::endl;
    console.setColor(DARK_GRAY);
    std::cout << "  ================================================" << std::endl;
    std::cout << "  STEP 3 OF 3: REASON FOR VISIT" << std::endl;
    std::cout << "  ================================================" << std::endl;
    console.resetColor();
    
    std::cout << std::endl;
    console.setColor(YELLOW);
    std::cout << "  >> Briefly describe why you need to see the doctor:" << std::endl;
    console.setColor(DARK_GRAY);
    std::cout << "     Example: Fever and headache, Follow-up checkup" << std::endl;
    console.resetColor();
    
    reason = console.getStringInput("     Reason: ");
    
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
    if (db.createAppointment(session.userID, selectedDoctorID, date, time + ":00", reason)) {
        int appointmentID = db.getLastInsertId();
        
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
              << std::setw(8) << "ID"
              << std::setw(20) << "Doctor"
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
                  << std::setw(20) << apt.doctorName
                  << std::setw(12) << apt.appointmentDate
                  << std::setw(10) << apt.appointmentTime.substr(0, 5)
                  << std::setw(12) << apt.status << std::endl;
    }
    console.resetColor();
    
    std::cout << "\n  " << std::string(62, '-') << std::endl;
    std::cout << "  Total: " << appointments.size() << " appointment(s)" << std::endl;
    
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
    
    std::cin.ignore(10000, '\n');
    
    std::string name, phone, email, address;
    
    console.setColor(WHITE);
    std::cout << "  New name (or press ENTER to keep: " << p.patientName << ")" << std::endl;
    console.resetColor();
    console.setColor(CYAN);
    std::cout << "  Name: ";
    console.resetColor();
    std::getline(std::cin, name);
    if (name.empty()) name = p.patientName;
    
    std::cout << std::endl;
    console.setColor(WHITE);
    std::cout << "  New phone (or press ENTER to keep: " << p.phoneNumber << ")" << std::endl;
    console.resetColor();
    do {
        console.setColor(CYAN);
        std::cout << "  Phone: ";
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
    
    std::cout << std::endl;
    console.setColor(WHITE);
    std::cout << "  New email (or press ENTER to keep: " << p.email << ")" << std::endl;
    console.resetColor();
    do {
        console.setColor(CYAN);
        std::cout << "  Email: ";
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
    
    std::cout << std::endl;
    console.setColor(WHITE);
    std::cout << "  New address (or press ENTER to keep current)" << std::endl;
    console.resetColor();
    console.setColor(CYAN);
    std::cout << "  Address: ";
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
    std::cout << "  ================================================\n" << std::endl;
    console.resetColor();
    
    console.printMenuOption(1, "Book New Appointment");
    console.printMenuOption(2, "View My Appointments");
    console.printMenuOption(3, "Cancel Appointment");
    console.printMenuOption(4, "Update Personal Details");
    console.printMenuOption(5, "Logout");
    
    std::cout << std::endl;
    console.setColor(YELLOW);
    std::cout << "  >> Enter your choice:" << std::endl;
    console.resetColor();
    int choice = console.getIntInput("     Your choice: ", 1, 5);
        
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
