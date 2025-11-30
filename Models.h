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
    bool isApproved = false;
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
    std::string dob;
    std::string gender;
};

// ============================================================
// Staff Model
// ============================================================
struct Staff {
    int staffID = 0;
    std::string staffName;
    std::string phoneNumber;
    std::string email;
    std::string role;
    bool isApproved = false;
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
    int patientID = 0;
    int doctorID = 0;
    int staffID = 0;
    std::string patientName;
    std::string doctorName;
};

// ============================================================
// Payment Model
// ============================================================
struct Payment {
    int paymentID = 0;
    double amount = 0.0;
    std::string paymentDate;
    std::string paymentMethod;
    int appointmentID = 0;
};

// ============================================================
// Doctor Schedule Model
// ============================================================
struct DoctorSchedule {
    int scheduleID = 0;
    int doctorID = 0;
    std::string dayOfWeek;
    std::string startTime;
    std::string endTime;
    bool isAvailable = true;
};

// ============================================================
// Activity Log Entry Model
// ============================================================
struct ActivityLogEntry {
    int logID = 0;
    std::string userType;
    int userID = 0;
    std::string action;
    std::string details;
    std::string logTime;
};

// ============================================================
// Appointment Summary for Reports
// ============================================================
struct AppointmentSummary {
    int total = 0;
    int confirmed = 0;
    int pending = 0;
    int completed = 0;
    int cancelled = 0;
};

#endif // MODELS_H

