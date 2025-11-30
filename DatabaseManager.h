// ============================================================
// DatabaseManager.h - MySQL Database Connection Manager
// Hospital Appointment Booking System
// ============================================================

#ifndef DATABASE_MANAGER_H
#define DATABASE_MANAGER_H

#include <mysql.h>
#include <string>
#include <vector>
#include <iostream>
#include "Models.h"

class DatabaseManager {
private:
    MYSQL* connection;
    bool isConnected;
    
    // Database configuration
    const char* host = "localhost";
    const char* user = "root";
    const char* password = "";
    const char* database = "hospital_appointment_db";
    unsigned int port = 3306;
    
public:
    DatabaseManager();
    ~DatabaseManager();
    
    // Connection management
    bool connect();
    void disconnect();
    bool checkConnection();
    bool executeQuery(const std::string& query);
    int getLastInsertId();
    std::string escapeString(const std::string& str);
    
    // Authentication
    int authenticatePatient(const std::string& email, const std::string& password);
    int authenticateDoctor(const std::string& email, const std::string& password);
    int authenticateStaff(const std::string& email, const std::string& password);
    int authenticateAdmin(const std::string& email, const std::string& password);
    
    // Patient operations
    bool registerPatient(const std::string& name, const std::string& phone,
                        const std::string& email, const std::string& address,
                        const std::string& dob, const std::string& gender,
                        const std::string& password);
    Patient getPatientById(int patientID);
    bool updatePatient(int patientID, const std::string& name, const std::string& phone,
                      const std::string& email, const std::string& address);
    std::vector<Patient> getAllPatients();
    
    // Doctor operations
    Doctor getDoctorById(int doctorID);
    std::vector<Doctor> getAllDoctors(bool approvedOnly = true);
    bool approveDoctor(int doctorID);
    
    // Staff operations
    Staff getStaffById(int staffID);
    std::vector<Staff> getAllStaff(bool approvedOnly = true);
    bool approveStaff(int staffID);
    
    // Admin operations
    Admin getAdminById(int adminID);
    
    // Appointment operations
    bool createAppointment(int patientID, int doctorID, const std::string& date,
                          const std::string& time, const std::string& reason);
    std::vector<Appointment> getPatientAppointments(int patientID);
    std::vector<Appointment> getDoctorAppointments(int doctorID, const std::string& date = "");
    std::vector<Appointment> getAllAppointments(const std::string& status = "");
    bool updateAppointmentStatus(int appointmentID, const std::string& status, int staffID = 0);
    bool cancelAppointment(int appointmentID);
    bool checkDoctorAvailability(int doctorID, const std::string& date, const std::string& time);
    bool checkPatientDailyLimit(int patientID, int doctorID, const std::string& date);
    
    // Report operations
    AppointmentSummary getDailyAppointmentSummary(const std::string& date);
    std::string getMostActiveDoctor();
    std::string getMostFrequentPatient();
    
    // Activity log operations
    bool logActivity(const std::string& userType, int userID, 
                    const std::string& action, const std::string& details = "");
    std::vector<ActivityLogEntry> getActivityLogs(int limit = 50);
    int getPendingDoctorsCount();
    int getPendingStaffCount();
};

#endif // DATABASE_MANAGER_H
