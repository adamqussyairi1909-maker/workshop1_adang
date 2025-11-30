// ============================================================
// AdminModule.cpp - Admin Dashboard Implementation
// Hospital Appointment Booking System
// ============================================================

#include "AdminModule.h"
#include "Utilities.h"
#include <iostream>
#include <iomanip>
#include <vector>
#include <windows.h>

AdminModule::AdminModule(ConsoleUtils& c, DatabaseManager& d, UserSession& s)
    : console(c), db(d), session(s) {}

void AdminModule::manageUsers() {
    console.clearScreen();
    console.printHeader("MANAGE USER ACCOUNTS");
    
    console.printMenuOption(1, "View All Patients");
    console.printMenuOption(2, "View All Doctors");
    console.printMenuOption(3, "View All Staff");
    console.printMenuOption(4, "Back");
    
    int choice = console.getIntInput("\nEnter choice: ", 1, 4);
    
    console.clearScreen();
    
    switch (choice) {
        case 1: {
            console.printHeader("ALL PATIENTS");
            std::vector<Patient> patients = db.getAllPatients();
            
            console.setColor(DARK_CYAN);
            std::cout << std::left 
                      << std::setw(5) << "ID"
                      << std::setw(25) << "Name"
                      << std::setw(15) << "Phone"
                      << std::setw(25) << "Email" << std::endl;
            std::cout << std::string(70, '-') << std::endl;
            console.resetColor();
            
            for (const auto& p : patients) {
                std::cout << std::setw(5) << p.patientID
                          << std::setw(25) << p.patientName.substr(0, 23)
                          << std::setw(15) << p.phoneNumber
                          << std::setw(25) << p.email.substr(0, 23) << std::endl;
            }
            break;
        }
        case 2: {
            console.printHeader("ALL DOCTORS");
            std::vector<Doctor> doctors = db.getAllDoctors(false);
            
            console.setColor(DARK_CYAN);
            std::cout << std::left 
                      << std::setw(5) << "ID"
                      << std::setw(22) << "Name"
                      << std::setw(15) << "Specialty"
                      << std::setw(10) << "Room"
                      << std::setw(10) << "Status" << std::endl;
            std::cout << std::string(62, '-') << std::endl;
            console.resetColor();
            
            for (const auto& d : doctors) {
                if (d.isApproved) console.setColor(GREEN);
                else console.setColor(YELLOW);
                
                std::cout << std::setw(5) << d.doctorID
                          << std::setw(22) << d.doctorName.substr(0, 20)
                          << std::setw(15) << d.specialty
                          << std::setw(10) << d.roomNo
                          << std::setw(10) << (d.isApproved ? "Approved" : "Pending") << std::endl;
            }
            console.resetColor();
            break;
        }
        case 3: {
            console.printHeader("ALL STAFF");
            std::vector<Staff> staffList = db.getAllStaff(false);
            
            console.setColor(DARK_CYAN);
            std::cout << std::left 
                      << std::setw(5) << "ID"
                      << std::setw(22) << "Name"
                      << std::setw(12) << "Role"
                      << std::setw(15) << "Phone"
                      << std::setw(10) << "Status" << std::endl;
            std::cout << std::string(64, '-') << std::endl;
            console.resetColor();
            
            for (const auto& s : staffList) {
                if (s.isApproved) console.setColor(GREEN);
                else console.setColor(YELLOW);
                
                std::cout << std::setw(5) << s.staffID
                          << std::setw(22) << s.staffName.substr(0, 20)
                          << std::setw(12) << s.role
                          << std::setw(15) << s.phoneNumber
                          << std::setw(10) << (s.isApproved ? "Approved" : "Pending") << std::endl;
            }
            console.resetColor();
            break;
        }
        case 4: return;
    }
    
    console.pauseScreen();
}

void AdminModule::approveAccounts() {
    console.clearScreen();
    console.printHeader("APPROVE DOCTOR/STAFF ACCOUNTS");
    
    int pendingDoctors = db.getPendingDoctorsCount();
    int pendingStaff = db.getPendingStaffCount();
    
    if (pendingDoctors == 0 && pendingStaff == 0) {
        console.printInfo("No pending accounts to approve.");
        console.pauseScreen();
        return;
    }
    
    console.setColor(YELLOW);
    std::cout << "\nPending Doctors: " << pendingDoctors << std::endl;
    std::cout << "Pending Staff: " << pendingStaff << std::endl;
    console.resetColor();
    
    console.printMenuOption(1, "Approve Doctors");
    console.printMenuOption(2, "Approve Staff");
    console.printMenuOption(3, "Back");
    
    int choice = console.getIntInput("\nEnter choice: ", 1, 3);
    
    if (choice == 1 && pendingDoctors > 0) {
        std::vector<Doctor> doctors = db.getAllDoctors(false);
        
        console.clearScreen();
        console.printSubHeader("Pending Doctor Accounts");
        
        int idx = 1;
        for (const auto& d : doctors) {
            if (!d.isApproved) {
                std::cout << idx++ << ". " << d.doctorName << " (" << d.specialty << ") - " << d.email << std::endl;
            }
        }
        
        int docChoice = console.getIntInput("\nSelect doctor to approve (0 to cancel): ", 0, pendingDoctors);
        if (docChoice > 0) {
            int count = 0;
            for (const auto& d : doctors) {
                if (!d.isApproved) {
                    count++;
                    if (count == docChoice) {
                        std::cin.ignore(10000, '\n');
                        console.printSubHeader("Doctor Details");
                        std::cout << "User: " << d.doctorName << std::endl;
                        std::cout << "Status: Pending Approval" << std::endl;
                        
                        std::string confirm = console.getStringInput("\nApprove this account? (Y/N): ");
                        if (confirm == "Y" || confirm == "y") {
                            if (db.approveDoctor(d.doctorID)) {
                                console.printSuccess("Account for " + d.doctorName + " has been APPROVED successfully.");
                                db.logActivity("Admin", session.userID, "Approve Doctor",
                                              "Approved doctor: " + d.doctorName);
                            }
                        }
                        break;
                    }
                }
            }
        }
    } else if (choice == 2 && pendingStaff > 0) {
        std::vector<Staff> staffList = db.getAllStaff(false);
        
        console.clearScreen();
        console.printSubHeader("Pending Staff Accounts");
        
        int idx = 1;
        for (const auto& s : staffList) {
            if (!s.isApproved) {
                std::cout << idx++ << ". " << s.staffName << " (" << s.role << ") - " << s.email << std::endl;
            }
        }
        
        int staffChoice = console.getIntInput("\nSelect staff to approve (0 to cancel): ", 0, pendingStaff);
        if (staffChoice > 0) {
            int count = 0;
            for (const auto& s : staffList) {
                if (!s.isApproved) {
                    count++;
                    if (count == staffChoice) {
                        std::cin.ignore(10000, '\n');
                        std::string confirm = console.getStringInput("Approve this account? (Y/N): ");
                        if (confirm == "Y" || confirm == "y") {
                            if (db.approveStaff(s.staffID)) {
                                console.printSuccess("Account for " + s.staffName + " has been APPROVED successfully.");
                                db.logActivity("Admin", session.userID, "Approve Staff",
                                              "Approved staff: " + s.staffName);
                            }
                        }
                        break;
                    }
                }
            }
        }
    }
    
    console.pauseScreen();
}

void AdminModule::monitorActivity() {
    console.clearScreen();
    console.printHeader("SYSTEM ACTIVITY LOG");
    
    std::vector<ActivityLogEntry> logs = db.getActivityLogs(20);
    
    if (logs.empty()) {
        console.printInfo("No activity logs found.");
        console.pauseScreen();
        return;
    }
    
    console.setColor(DARK_CYAN);
    std::cout << std::left 
              << std::setw(20) << "Time"
              << std::setw(10) << "User"
              << std::setw(5) << "ID"
              << std::setw(20) << "Action" << std::endl;
    std::cout << std::string(55, '-') << std::endl;
    console.resetColor();
    
    for (const auto& log : logs) {
        if (log.action.find("Login") != std::string::npos) console.setColor(GREEN);
        else if (log.action.find("Logout") != std::string::npos) console.setColor(CYAN);
        else if (log.action.find("Cancel") != std::string::npos) console.setColor(RED);
        else console.setColor(WHITE);
        
        std::cout << std::setw(20) << log.logTime.substr(0, 19)
                  << std::setw(10) << log.userType
                  << std::setw(5) << log.userID
                  << std::setw(20) << log.action.substr(0, 18) << std::endl;
    }
    console.resetColor();
    
    console.pauseScreen();
}

void AdminModule::generateReports() {
    console.clearScreen();
    console.printHeader("GENERATE REPORTS");
    
    console.printMenuOption(1, "Appointment Summary");
    console.printMenuOption(2, "User Statistics");
    console.printMenuOption(3, "Back");
    
    int choice = console.getIntInput("\nEnter choice: ", 1, 3);
    
    console.clearScreen();
    
    switch (choice) {
        case 1: {
            console.printHeader("APPOINTMENT SUMMARY REPORT");
            
            AppointmentSummary summary = db.getDailyAppointmentSummary("");
            
            std::cout << "\n+----------------------------+" << std::endl;
            std::cout << "|  APPOINTMENT STATISTICS    |" << std::endl;
            std::cout << "+----------------------------+" << std::endl;
            std::cout << "| Total Appointments: " << std::setw(6) << summary.total << " |" << std::endl;
            
            console.setColor(GREEN);
            std::cout << "| Confirmed:          " << std::setw(6) << summary.confirmed << " |" << std::endl;
            console.setColor(YELLOW);
            std::cout << "| Pending:            " << std::setw(6) << summary.pending << " |" << std::endl;
            console.setColor(CYAN);
            std::cout << "| Completed:          " << std::setw(6) << summary.completed << " |" << std::endl;
            console.setColor(RED);
            std::cout << "| Cancelled:          " << std::setw(6) << summary.cancelled << " |" << std::endl;
            console.resetColor();
            std::cout << "+----------------------------+" << std::endl;
            
            std::cout << "\nMost Active Doctor: " << db.getMostActiveDoctor() << std::endl;
            std::cout << "Most Frequent Patient: " << db.getMostFrequentPatient() << std::endl;
            break;
        }
        case 2: {
            console.printHeader("USER STATISTICS REPORT");
            
            std::vector<Patient> patients = db.getAllPatients();
            std::vector<Doctor> doctors = db.getAllDoctors(false);
            std::vector<Staff> staffList = db.getAllStaff(false);
            
            int approvedDoctors = 0, pendingDoctors = 0;
            for (const auto& d : doctors) {
                if (d.isApproved) approvedDoctors++;
                else pendingDoctors++;
            }
            
            int approvedStaff = 0, pendingStaff = 0;
            for (const auto& s : staffList) {
                if (s.isApproved) approvedStaff++;
                else pendingStaff++;
            }
            
            std::cout << "\n+----------------------------+" << std::endl;
            std::cout << "|    USER STATISTICS         |" << std::endl;
            std::cout << "+----------------------------+" << std::endl;
            std::cout << "| Total Patients:     " << std::setw(6) << patients.size() << " |" << std::endl;
            std::cout << "+----------------------------+" << std::endl;
            std::cout << "| Total Doctors:      " << std::setw(6) << doctors.size() << " |" << std::endl;
            console.setColor(GREEN);
            std::cout << "|   - Approved:       " << std::setw(6) << approvedDoctors << " |" << std::endl;
            console.setColor(YELLOW);
            std::cout << "|   - Pending:        " << std::setw(6) << pendingDoctors << " |" << std::endl;
            console.resetColor();
            std::cout << "+----------------------------+" << std::endl;
            std::cout << "| Total Staff:        " << std::setw(6) << staffList.size() << " |" << std::endl;
            console.setColor(GREEN);
            std::cout << "|   - Approved:       " << std::setw(6) << approvedStaff << " |" << std::endl;
            console.setColor(YELLOW);
            std::cout << "|   - Pending:        " << std::setw(6) << pendingStaff << " |" << std::endl;
            console.resetColor();
            std::cout << "+----------------------------+" << std::endl;
            break;
        }
        case 3: return;
    }
    
    console.setColor(GREEN);
    std::cout << "\nReport generated on " << getCurrentDateTime() << std::endl;
    console.resetColor();
    
    db.logActivity("Admin", session.userID, "Generate Report", "Generated system report");
    
    console.pauseScreen();
}

void AdminModule::showDashboard() {
    while (session.isLoggedIn) {
        console.clearScreen();
        console.printHeader("ADMIN DASHBOARD");
        
        Admin admin = db.getAdminById(session.userID);
        
        console.setColor(WHITE);
        std::cout << "\nWelcome, " << admin.adminName << std::endl;
        console.resetColor();
        
        // Show pending notifications
        int pendingDoctors = db.getPendingDoctorsCount();
        int pendingStaff = db.getPendingStaffCount();
        
        if (pendingDoctors > 0 || pendingStaff > 0) {
            console.setColor(YELLOW);
            std::cout << "\n[!] Pending approvals: " << pendingDoctors << " doctor(s), " 
                      << pendingStaff << " staff member(s)" << std::endl;
            console.resetColor();
        }
        
        std::cout << std::endl;
        console.printMenuOption(1, "Manage User Accounts");
        console.printMenuOption(2, "Approve Doctor/Staff Accounts");
        console.printMenuOption(3, "Monitor System Activity");
        console.printMenuOption(4, "Generate Reports");
        console.printMenuOption(5, "Logout");
        
        int choice = console.getIntInput("\nEnter your choice: ", 1, 5);
        
        switch (choice) {
            case 1: manageUsers(); break;
            case 2: approveAccounts(); break;
            case 3: monitorActivity(); break;
            case 4: generateReports(); break;
            case 5:
                db.logActivity("Admin", session.userID, "Logout", "Admin logged out");
                session = UserSession();
                console.printSuccess("Logged out successfully!");
                Sleep(1000);
                return;
        }
    }
}

