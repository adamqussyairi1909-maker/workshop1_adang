// ============================================================
// Models.h - Data Structures and Models
// Hospital Appointment Booking System
// ============================================================

#ifndef MODELS_H
#define MODELS_H

#include <string>

// ============================================================
// User Session Structure
// ============================================================
struct UserSession {
    int userID = -1;
    std::string userType = "";
    std::string userName = "";
    std::string userEmail = "";
    bool isLoggedIn = false;
};

// ============================================================
// Doctor Model
// ============================================================
struct Doctor {
    int doctorID = 0;
    std::string doctorName;
    std::string specialty;
    std::string phoneNumber;
    std::string email;
    std::string roomNo;
    bool isAvailable = true;
};

// ============================================================
// Patient Model
// ============================================================
struct Patient {
    int patientID = 0;
    std::string patientName;
    std::string phoneNumber;
    std::string email;
    std::string address;
    std::string dateOfBirth;
    std::string gender;
};

// ============================================================
// Staff Model
// ============================================================
struct Staff {
    int staffID = 0;
    std::string staffName;
    std::string department;
    std::string phoneNumber;
    std::string email;
};

// ============================================================
// Admin Model
// ============================================================
struct Admin {
    int adminID = 0;
    std::string adminName;
    std::string phoneNumber;
    std::string email;
};

// ============================================================
// Appointment Model
// ============================================================
struct Appointment {
    int appointmentID = 0;
    std::string status;
    std::string appointmentTime;
    std::string appointmentDate;
    std::string reason;
    int duration = 30; // Duration in minutes (default 30 minutes)
    double consultationFee = 30.0; // Consultation fee (RM1 per minute)
    double medicineFee = 0.0; // Medicine fee based on reason
    double totalCost = 30.0; // Total cost (consultation + medicine)
    int patientID = 0;
    int doctorID = 0;
    int staffID = 0;
    std::string patientName;
    std::string doctorName;
};

// ============================================================
// Activity Log Model
// ============================================================
struct ActivityLog {
    int logID = 0;
    std::string userType;
    int userID = 0;
    std::string action;
    std::string details;
    std::string timestamp;
};

#endif // MODELS_H
