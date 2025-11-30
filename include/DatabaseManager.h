// ============================================================
// DatabaseManager.h - MySQL Connector C++ (JDBC API)
// Hospital Appointment Booking System
// ============================================================

#ifndef DATABASE_MANAGER_H
#define DATABASE_MANAGER_H

// Disable warnings
#pragma warning(disable: 4267)
#pragma warning(disable: 4251)

// MySQL Connector C++ JDBC Headers
#include "mysql_driver.h"
#include "mysql_connection.h"
#include "cppconn/statement.h"
#include "cppconn/resultset.h"
#include "cppconn/prepared_statement.h"
#include "cppconn/exception.h"

#include <string>
#include <vector>
#include <memory>
#include "Models.h"

class DatabaseManager {
private:
    sql::Driver* driver;
    std::unique_ptr<sql::Connection> connection;
    bool isConnected;
    
    std::string host = "tcp://127.0.0.1:3306";
    std::string user = "root";
    std::string password = "";
    std::string database = "hospital_appointment_db";
    
public:
    DatabaseManager();
    ~DatabaseManager();
    
    // Connection
    bool connect();
    void disconnect();
    bool checkConnection();
    int getLastInsertId();
    
    // Authentication
    int loginPatient(const std::string& email, const std::string& password);
    int loginDoctor(const std::string& email, const std::string& password);
    int loginStaff(const std::string& email, const std::string& password);
    int loginAdmin(const std::string& email, const std::string& password);
    
    // Patient operations
    bool registerPatient(const std::string& name, const std::string& phone,
                        const std::string& email, const std::string& address,
                        const std::string& dob, const std::string& gender,
                        const std::string& password);
    Patient getPatientById(int patientID);
    bool updatePatient(int patientID, const std::string& name, const std::string& phone,
                      const std::string& email, const std::string& address);
    bool deletePatient(int patientID);
    std::vector<Patient> searchPatients(const std::string& search);
    
    // Doctor operations
    Doctor getDoctorById(int doctorID);
    std::vector<Doctor> getAllDoctors(bool availableOnly = false);
    bool addDoctor(const std::string& name, const std::string& specialty,
                  const std::string& room, const std::string& phone,
                  const std::string& email, const std::string& password);
    bool updateDoctor(int doctorID, const std::string& name, const std::string& specialty,
                     const std::string& room, const std::string& phone);
    bool updateDoctorAvailability(int doctorID, bool isAvailable);
    bool deleteDoctor(int doctorID);
    
    // Staff operations
    Staff getStaffById(int staffID);
    std::vector<Staff> getAllStaff();
    bool addStaff(const std::string& name, const std::string& department,
                 const std::string& phone, const std::string& email,
                 const std::string& password);
    bool deleteStaff(int staffID);
    
    // Admin operations
    Admin getAdminById(int adminID);
    
    // Appointment operations
    bool createAppointment(int patientID, int doctorID, const std::string& date,
                          const std::string& time, const std::string& reason);
    std::vector<Appointment> getPatientAppointments(int patientID);
    std::vector<Appointment> getDoctorAppointments(int doctorID, const std::string& date = "");
    std::vector<Appointment> getDoctorAllAppointments(int doctorID);
    std::vector<Appointment> getAllAppointments();
    std::vector<Appointment> getPendingAppointments();
    bool updateAppointmentStatus(int appointmentID, const std::string& status);
    bool cancelAppointment(int appointmentID);
    bool checkDoctorAvailability(int doctorID, const std::string& date, const std::string& time);
    bool checkPatientDailyLimit(int patientID, int doctorID, const std::string& date);
    
    // Activity logging
    bool logActivity(const std::string& userType, int userID, 
                    const std::string& action, const std::string& details = "");
    std::vector<ActivityLog> getActivityLogs(int limit = 50);
};

#endif // DATABASE_MANAGER_H
