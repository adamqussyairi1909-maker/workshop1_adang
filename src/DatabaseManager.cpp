// ============================================================
// DatabaseManager.cpp - MySQL Connector C++ (JDBC API)
// Hospital Appointment Booking System
// ============================================================

#include "../include/DatabaseManager.h"
#include <iostream>
#include <algorithm>
#include <cctype>

DatabaseManager::DatabaseManager() : driver(nullptr), isConnected(false) {}

DatabaseManager::~DatabaseManager() {
    disconnect();
}

bool DatabaseManager::connect() {
    try {
        driver = sql::mysql::get_mysql_driver_instance();
        connection.reset(driver->connect(host, user, password));
        connection->setSchema(database);
        isConnected = true;
        return true;
    }
    catch (sql::SQLException& e) {
        std::cerr << "[ERROR] Database connection failed: " << e.what() << std::endl;
        isConnected = false;
        return false;
    }
}

void DatabaseManager::disconnect() {
    if (connection) {
        connection->close();
        connection.reset();
    }
    isConnected = false;
}

bool DatabaseManager::checkConnection() {
    if (!isConnected || !connection || connection->isClosed()) {
        return connect();
    }
    return true;
}

int DatabaseManager::getLastInsertId() {
    try {
        std::unique_ptr<sql::Statement> stmt(connection->createStatement());
        std::unique_ptr<sql::ResultSet> res(stmt->executeQuery("SELECT LAST_INSERT_ID()"));
        if (res->next()) {
            return res->getInt(1);
        }
    }
    catch (sql::SQLException& e) {
        std::cerr << "[ERROR] " << e.what() << std::endl;
    }
    return 0;
}

// ============================================================
// Authentication Methods
// ============================================================

int DatabaseManager::loginPatient(const std::string& email, const std::string& pwd) {
    try {
        std::unique_ptr<sql::PreparedStatement> pstmt(
            connection->prepareStatement("SELECT PatientID FROM Patient WHERE Email = ? AND Password = ?"));
        pstmt->setString(1, email);
        pstmt->setString(2, pwd);
        std::unique_ptr<sql::ResultSet> res(pstmt->executeQuery());
        if (res->next()) {
            return res->getInt("PatientID");
        }
    }
    catch (sql::SQLException& e) {
        std::cerr << "[ERROR] " << e.what() << std::endl;
    }
    return -1;
}

int DatabaseManager::loginDoctor(const std::string& email, const std::string& pwd) {
    try {
        std::unique_ptr<sql::PreparedStatement> pstmt(
            connection->prepareStatement("SELECT DoctorID FROM Doctors WHERE Email = ? AND Password = ?"));
        pstmt->setString(1, email);
        pstmt->setString(2, pwd);
        std::unique_ptr<sql::ResultSet> res(pstmt->executeQuery());
        if (res->next()) {
            return res->getInt("DoctorID");
        }
    }
    catch (sql::SQLException& e) {
        std::cerr << "[ERROR] " << e.what() << std::endl;
    }
    return -1;
}

int DatabaseManager::loginStaff(const std::string& email, const std::string& pwd) {
    try {
        std::unique_ptr<sql::PreparedStatement> pstmt(
            connection->prepareStatement("SELECT StaffID FROM Staff WHERE Email = ? AND Password = ?"));
        pstmt->setString(1, email);
        pstmt->setString(2, pwd);
        std::unique_ptr<sql::ResultSet> res(pstmt->executeQuery());
        if (res->next()) {
            return res->getInt("StaffID");
        }
    }
    catch (sql::SQLException& e) {
        std::cerr << "[ERROR] " << e.what() << std::endl;
    }
    return -1;
}

int DatabaseManager::loginAdmin(const std::string& email, const std::string& pwd) {
    try {
        std::unique_ptr<sql::PreparedStatement> pstmt(
            connection->prepareStatement("SELECT AdminID FROM Admin WHERE Email = ? AND Password = ?"));
        pstmt->setString(1, email);
        pstmt->setString(2, pwd);
        std::unique_ptr<sql::ResultSet> res(pstmt->executeQuery());
        if (res->next()) {
            return res->getInt("AdminID");
        }
    }
    catch (sql::SQLException& e) {
        std::cerr << "[ERROR] " << e.what() << std::endl;
    }
    return -1;
}

// ============================================================
// Patient Operations
// ============================================================

bool DatabaseManager::registerPatient(const std::string& name, const std::string& phone,
                                      const std::string& email, const std::string& address,
                                      const std::string& dob, const std::string& gender,
                                      const std::string& pwd) {
    try {
        std::unique_ptr<sql::PreparedStatement> pstmt(
            connection->prepareStatement(
                "INSERT INTO Patient (PatientName, PhoneNumber, Email, Address, DOB, Gender, Password) "
                "VALUES (?, ?, ?, ?, ?, ?, ?)"));
        pstmt->setString(1, name);
        pstmt->setString(2, phone);
        pstmt->setString(3, email);
        pstmt->setString(4, address);
        pstmt->setString(5, dob);
        pstmt->setString(6, gender);
        pstmt->setString(7, pwd);
        pstmt->executeUpdate();
        return true;
    }
    catch (sql::SQLException& e) {
        std::cerr << "[ERROR] " << e.what() << std::endl;
        return false;
    }
}

Patient DatabaseManager::getPatientById(int patientID) {
    Patient patient;
    try {
        std::unique_ptr<sql::PreparedStatement> pstmt(
            connection->prepareStatement("SELECT * FROM Patient WHERE PatientID = ?"));
        pstmt->setInt(1, patientID);
        std::unique_ptr<sql::ResultSet> res(pstmt->executeQuery());
        if (res->next()) {
            patient.patientID = res->getInt("PatientID");
            patient.patientName = res->getString("PatientName");
            patient.phoneNumber = res->getString("PhoneNumber");
            patient.email = res->getString("Email");
            patient.address = res->getString("Address");
            patient.dateOfBirth = res->getString("DOB");
            patient.gender = res->getString("Gender");
        }
    }
    catch (sql::SQLException& e) {
        std::cerr << "[ERROR] " << e.what() << std::endl;
    }
    return patient;
}

bool DatabaseManager::updatePatient(int patientID, const std::string& name, const std::string& phone,
                                   const std::string& email, const std::string& address) {
    try {
        std::unique_ptr<sql::PreparedStatement> pstmt(
            connection->prepareStatement(
                "UPDATE Patient SET PatientName = ?, PhoneNumber = ?, Email = ?, Address = ? WHERE PatientID = ?"));
        pstmt->setString(1, name);
        pstmt->setString(2, phone);
        pstmt->setString(3, email);
        pstmt->setString(4, address);
        pstmt->setInt(5, patientID);
        pstmt->executeUpdate();
        return true;
    }
    catch (sql::SQLException& e) {
        std::cerr << "[ERROR] " << e.what() << std::endl;
        return false;
    }
}

bool DatabaseManager::deletePatient(int patientID) {
    try {
        // Delete appointments first
        std::unique_ptr<sql::PreparedStatement> pstmt1(
            connection->prepareStatement("DELETE FROM Appointment WHERE PatientID = ?"));
        pstmt1->setInt(1, patientID);
        pstmt1->executeUpdate();
        
        // Delete patient
        std::unique_ptr<sql::PreparedStatement> pstmt2(
            connection->prepareStatement("DELETE FROM Patient WHERE PatientID = ?"));
        pstmt2->setInt(1, patientID);
        pstmt2->executeUpdate();
        return true;
    }
    catch (sql::SQLException& e) {
        std::cerr << "[ERROR] " << e.what() << std::endl;
        return false;
    }
}

std::vector<Patient> DatabaseManager::searchPatients(const std::string& search) {
    std::vector<Patient> patients;
    try {
        std::string query = "SELECT * FROM Patient";
        if (!search.empty()) {
            query += " WHERE PatientName LIKE ? OR Email LIKE ? OR PhoneNumber LIKE ?";
        }
        query += " ORDER BY PatientName";
        
        std::unique_ptr<sql::PreparedStatement> pstmt(connection->prepareStatement(query));
        if (!search.empty()) {
            std::string searchPattern = "%" + search + "%";
            pstmt->setString(1, searchPattern);
            pstmt->setString(2, searchPattern);
            pstmt->setString(3, searchPattern);
        }
        std::unique_ptr<sql::ResultSet> res(pstmt->executeQuery());
        while (res->next()) {
            Patient p;
            p.patientID = res->getInt("PatientID");
            p.patientName = res->getString("PatientName");
            p.phoneNumber = res->getString("PhoneNumber");
            p.email = res->getString("Email");
            p.address = res->getString("Address");
            p.dateOfBirth = res->getString("DOB");
            p.gender = res->getString("Gender");
            patients.push_back(p);
        }
    }
    catch (sql::SQLException& e) {
        std::cerr << "[ERROR] " << e.what() << std::endl;
    }
    return patients;
}

// ============================================================
// Doctor Operations
// ============================================================

Doctor DatabaseManager::getDoctorById(int doctorID) {
    Doctor doctor;
    try {
        std::unique_ptr<sql::PreparedStatement> pstmt(
            connection->prepareStatement("SELECT * FROM Doctors WHERE DoctorID = ?"));
        pstmt->setInt(1, doctorID);
        std::unique_ptr<sql::ResultSet> res(pstmt->executeQuery());
        if (res->next()) {
            doctor.doctorID = res->getInt("DoctorID");
            doctor.doctorName = res->getString("DoctorName");
            doctor.specialty = res->getString("Specialty");
            doctor.phoneNumber = res->getString("PhoneNumber");
            doctor.email = res->getString("Email");
            doctor.roomNo = res->getString("RoomNo");
            doctor.isAvailable = res->getBoolean("IsAvailable");
        }
    }
    catch (sql::SQLException& e) {
        std::cerr << "[ERROR] " << e.what() << std::endl;
    }
    return doctor;
}

std::vector<Doctor> DatabaseManager::getAllDoctors(bool availableOnly) {
    std::vector<Doctor> doctors;
    try {
        std::string query = "SELECT * FROM Doctors";
        if (availableOnly) query += " WHERE IsAvailable = TRUE";
        query += " ORDER BY DoctorName";
        
        std::unique_ptr<sql::Statement> stmt(connection->createStatement());
        std::unique_ptr<sql::ResultSet> res(stmt->executeQuery(query));
        while (res->next()) {
            Doctor d;
            d.doctorID = res->getInt("DoctorID");
            d.doctorName = res->getString("DoctorName");
            d.specialty = res->getString("Specialty");
            d.phoneNumber = res->getString("PhoneNumber");
            d.email = res->getString("Email");
            d.roomNo = res->getString("RoomNo");
            d.isAvailable = res->getBoolean("IsAvailable");
            doctors.push_back(d);
        }
    }
    catch (sql::SQLException& e) {
        std::cerr << "[ERROR] " << e.what() << std::endl;
    }
    return doctors;
}

bool DatabaseManager::addDoctor(const std::string& name, const std::string& specialty,
                               const std::string& room, const std::string& phone,
                               const std::string& email, const std::string& password) {
    try {
        std::unique_ptr<sql::PreparedStatement> pstmt(
            connection->prepareStatement(
                "INSERT INTO Doctors (DoctorName, Specialty, RoomNo, PhoneNumber, Email, Password, IsAvailable) "
                "VALUES (?, ?, ?, ?, ?, ?, TRUE)"));
        pstmt->setString(1, name);
        pstmt->setString(2, specialty);
        pstmt->setString(3, room);
        pstmt->setString(4, phone);
        pstmt->setString(5, email);
        pstmt->setString(6, password);
        pstmt->executeUpdate();
        return true;
    }
    catch (sql::SQLException& e) {
        std::cerr << "[ERROR] " << e.what() << std::endl;
        return false;
    }
}

bool DatabaseManager::updateDoctor(int doctorID, const std::string& name, const std::string& specialty,
                                  const std::string& room, const std::string& phone) {
    try {
        std::unique_ptr<sql::PreparedStatement> pstmt(
            connection->prepareStatement(
                "UPDATE Doctors SET DoctorName = ?, Specialty = ?, RoomNo = ?, PhoneNumber = ? WHERE DoctorID = ?"));
        pstmt->setString(1, name);
        pstmt->setString(2, specialty);
        pstmt->setString(3, room);
        pstmt->setString(4, phone);
        pstmt->setInt(5, doctorID);
        pstmt->executeUpdate();
        return true;
    }
    catch (sql::SQLException& e) {
        std::cerr << "[ERROR] " << e.what() << std::endl;
        return false;
    }
}

bool DatabaseManager::updateDoctorAvailability(int doctorID, bool isAvailable) {
    try {
        std::unique_ptr<sql::PreparedStatement> pstmt(
            connection->prepareStatement("UPDATE Doctors SET IsAvailable = ? WHERE DoctorID = ?"));
        pstmt->setBoolean(1, isAvailable);
        pstmt->setInt(2, doctorID);
        pstmt->executeUpdate();
        return true;
    }
    catch (sql::SQLException& e) {
        std::cerr << "[ERROR] " << e.what() << std::endl;
        return false;
    }
}

bool DatabaseManager::deleteDoctor(int doctorID) {
    try {
        // Cancel appointments first
        std::unique_ptr<sql::PreparedStatement> pstmt1(
            connection->prepareStatement("UPDATE Appointment SET Status = 'Cancelled' WHERE DoctorID = ?"));
        pstmt1->setInt(1, doctorID);
        pstmt1->executeUpdate();
        
        // Delete doctor
        std::unique_ptr<sql::PreparedStatement> pstmt2(
            connection->prepareStatement("DELETE FROM Doctors WHERE DoctorID = ?"));
        pstmt2->setInt(1, doctorID);
        pstmt2->executeUpdate();
        return true;
    }
    catch (sql::SQLException& e) {
        std::cerr << "[ERROR] " << e.what() << std::endl;
        return false;
    }
}

// ============================================================
// Staff Operations
// ============================================================

Staff DatabaseManager::getStaffById(int staffID) {
    Staff staff;
    try {
        std::unique_ptr<sql::PreparedStatement> pstmt(
            connection->prepareStatement("SELECT * FROM Staff WHERE StaffID = ?"));
        pstmt->setInt(1, staffID);
        std::unique_ptr<sql::ResultSet> res(pstmt->executeQuery());
        if (res->next()) {
            staff.staffID = res->getInt("StaffID");
            staff.staffName = res->getString("StaffName");
            staff.department = res->getString("Department");
            staff.phoneNumber = res->getString("PhoneNumber");
            staff.email = res->getString("Email");
        }
    }
    catch (sql::SQLException& e) {
        std::cerr << "[ERROR] " << e.what() << std::endl;
    }
    return staff;
}

std::vector<Staff> DatabaseManager::getAllStaff() {
    std::vector<Staff> staffList;
    try {
        std::unique_ptr<sql::Statement> stmt(connection->createStatement());
        std::unique_ptr<sql::ResultSet> res(stmt->executeQuery("SELECT * FROM Staff ORDER BY StaffName"));
        while (res->next()) {
            Staff s;
            s.staffID = res->getInt("StaffID");
            s.staffName = res->getString("StaffName");
            s.department = res->getString("Department");
            s.phoneNumber = res->getString("PhoneNumber");
            s.email = res->getString("Email");
            staffList.push_back(s);
        }
    }
    catch (sql::SQLException& e) {
        std::cerr << "[ERROR] " << e.what() << std::endl;
    }
    return staffList;
}

bool DatabaseManager::addStaff(const std::string& name, const std::string& department,
                              const std::string& phone, const std::string& email,
                              const std::string& password) {
    try {
        std::unique_ptr<sql::PreparedStatement> pstmt(
            connection->prepareStatement(
                "INSERT INTO Staff (StaffName, Department, PhoneNumber, Email, Password) "
                "VALUES (?, ?, ?, ?, ?)"));
        pstmt->setString(1, name);
        pstmt->setString(2, department);
        pstmt->setString(3, phone);
        pstmt->setString(4, email);
        pstmt->setString(5, password);
        pstmt->executeUpdate();
        return true;
    }
    catch (sql::SQLException& e) {
        std::cerr << "[ERROR] " << e.what() << std::endl;
        return false;
    }
}

bool DatabaseManager::deleteStaff(int staffID) {
    try {
        std::unique_ptr<sql::PreparedStatement> pstmt(
            connection->prepareStatement("DELETE FROM Staff WHERE StaffID = ?"));
        pstmt->setInt(1, staffID);
        pstmt->executeUpdate();
        return true;
    }
    catch (sql::SQLException& e) {
        std::cerr << "[ERROR] " << e.what() << std::endl;
        return false;
    }
}

// ============================================================
// Admin Operations
// ============================================================

Admin DatabaseManager::getAdminById(int adminID) {
    Admin admin;
    try {
        std::unique_ptr<sql::PreparedStatement> pstmt(
            connection->prepareStatement("SELECT AdminID, AdminName, PhoneNumber, Email FROM Admin WHERE AdminID = ?"));
        pstmt->setInt(1, adminID);
        std::unique_ptr<sql::ResultSet> res(pstmt->executeQuery());
        if (res->next()) {
            admin.adminID = res->getInt("AdminID");
            admin.adminName = res->getString("AdminName");
            admin.phoneNumber = res->getString("PhoneNumber");
            admin.email = res->getString("Email");
        }
    }
    catch (sql::SQLException& e) {
        std::cerr << "[ERROR] " << e.what() << std::endl;
    }
    return admin;
}

// ============================================================
// Appointment Operations
// ============================================================

bool DatabaseManager::createAppointment(int patientID, int doctorID, const std::string& date,
                                        const std::string& time, const std::string& reason,
                                        int duration, double consultationFee, double medicineFee) {
    try {
        double totalCost = consultationFee + medicineFee;
        std::unique_ptr<sql::PreparedStatement> pstmt(
            connection->prepareStatement(
                "INSERT INTO Appointment (PatientID, DoctorID, AppointmentDate, AppointmentTime, Reason, Duration, ConsultationFee, MedicineFee, TotalCost, Status) "
                "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, 'Pending')"));
        pstmt->setInt(1, patientID);
        pstmt->setInt(2, doctorID);
        pstmt->setString(3, date);
        pstmt->setString(4, time);
        pstmt->setString(5, reason);
        pstmt->setInt(6, duration);
        pstmt->setDouble(7, consultationFee);
        pstmt->setDouble(8, medicineFee);
        pstmt->setDouble(9, totalCost);
        pstmt->executeUpdate();
        return true;
    }
    catch (sql::SQLException& e) {
        std::cerr << "[ERROR] " << e.what() << std::endl;
        return false;
    }
}

double DatabaseManager::calculateConsultationFee(int duration) {
    // RM1 per minute
    return static_cast<double>(duration);
}

double DatabaseManager::calculateMedicineFee(const std::string& reason) {
    // Calculate medicine fee based on appointment reason
    std::string lowerReason = reason;
    std::transform(lowerReason.begin(), lowerReason.end(), lowerReason.begin(), ::tolower);
    
    // Common conditions and their medicine fees (in RM)
    if (lowerReason.find("fever") != std::string::npos || 
        lowerReason.find("flu") != std::string::npos || 
        lowerReason.find("cough") != std::string::npos ||
        lowerReason.find("cold") != std::string::npos) {
        return 25.00; // Basic medication for common cold/flu
    }
    else if (lowerReason.find("pain") != std::string::npos ||
             lowerReason.find("headache") != std::string::npos ||
             lowerReason.find("backache") != std::string::npos) {
        return 40.00; // Pain medication
    }
    else if (lowerReason.find("allergy") != std::string::npos ||
             lowerReason.find("rash") != std::string::npos ||
             lowerReason.find("itch") != std::string::npos ||
             lowerReason.find("skin") != std::string::npos) {
        return 35.00; // Antihistamines and topical treatments
    }
    else if (lowerReason.find("infection") != std::string::npos ||
             lowerReason.find("throat") != std::string::npos) {
        return 50.00; // Antibiotics
    }
    else if (lowerReason.find("diabetes") != std::string::npos ||
             lowerReason.find("blood pressure") != std::string::npos ||
             lowerReason.find("hypertension") != std::string::npos ||
             lowerReason.find("heart") != std::string::npos) {
        return 80.00; // Chronic condition medication
    }
    else if (lowerReason.find("checkup") != std::string::npos ||
             lowerReason.find("check up") != std::string::npos ||
             lowerReason.find("follow-up") != std::string::npos ||
             lowerReason.find("follow up") != std::string::npos ||
             lowerReason.find("screening") != std::string::npos) {
        return 0.00; // No medicine needed for routine checkups
    }
    else {
        return 30.00; // Default medicine fee for general conditions
    }
}

std::vector<Appointment> DatabaseManager::getPatientAppointments(int patientID) {
    std::vector<Appointment> appointments;
    try {
        std::unique_ptr<sql::PreparedStatement> pstmt(
            connection->prepareStatement(
                "SELECT a.*, p.PatientName, d.DoctorName FROM Appointment a "
                "JOIN Patient p ON a.PatientID = p.PatientID "
                "JOIN Doctors d ON a.DoctorID = d.DoctorID "
                "WHERE a.PatientID = ? ORDER BY a.AppointmentDate DESC, a.AppointmentTime DESC"));
        pstmt->setInt(1, patientID);
        std::unique_ptr<sql::ResultSet> res(pstmt->executeQuery());
        while (res->next()) {
            Appointment a;
            a.appointmentID = res->getInt("AppointmentID");
            a.status = res->getString("Status");
            a.appointmentTime = res->getString("AppointmentTime");
            a.appointmentDate = res->getString("AppointmentDate");
            a.reason = res->getString("Reason");
            a.duration = res->getInt("Duration");
            a.consultationFee = res->getDouble("ConsultationFee");
            a.medicineFee = res->getDouble("MedicineFee");
            a.totalCost = res->getDouble("TotalCost");
            a.patientID = res->getInt("PatientID");
            a.doctorID = res->getInt("DoctorID");
            a.patientName = res->getString("PatientName");
            a.doctorName = res->getString("DoctorName");
            appointments.push_back(a);
        }
    }
    catch (sql::SQLException& e) {
        std::cerr << "[ERROR] " << e.what() << std::endl;
    }
    return appointments;
}

std::vector<Appointment> DatabaseManager::getDoctorAppointments(int doctorID, const std::string& date) {
    std::vector<Appointment> appointments;
    try {
        std::string query = "SELECT a.*, p.PatientName, d.DoctorName FROM Appointment a "
                           "JOIN Patient p ON a.PatientID = p.PatientID "
                           "JOIN Doctors d ON a.DoctorID = d.DoctorID "
                           "WHERE a.DoctorID = ?";
        if (!date.empty()) {
            query += " AND a.AppointmentDate = ?";
        }
        query += " ORDER BY a.AppointmentDate, a.AppointmentTime";
        
        std::unique_ptr<sql::PreparedStatement> pstmt(connection->prepareStatement(query));
        pstmt->setInt(1, doctorID);
        if (!date.empty()) {
            pstmt->setString(2, date);
        }
        std::unique_ptr<sql::ResultSet> res(pstmt->executeQuery());
        while (res->next()) {
            Appointment a;
            a.appointmentID = res->getInt("AppointmentID");
            a.status = res->getString("Status");
            a.appointmentTime = res->getString("AppointmentTime");
            a.appointmentDate = res->getString("AppointmentDate");
            a.reason = res->getString("Reason");
            a.duration = res->getInt("Duration");
            a.consultationFee = res->getDouble("ConsultationFee");
            a.medicineFee = res->getDouble("MedicineFee");
            a.totalCost = res->getDouble("TotalCost");
            a.patientID = res->getInt("PatientID");
            a.doctorID = res->getInt("DoctorID");
            a.patientName = res->getString("PatientName");
            a.doctorName = res->getString("DoctorName");
            appointments.push_back(a);
        }
    }
    catch (sql::SQLException& e) {
        std::cerr << "[ERROR] " << e.what() << std::endl;
    }
    return appointments;
}

std::vector<Appointment> DatabaseManager::getDoctorAllAppointments(int doctorID) {
    return getDoctorAppointments(doctorID, "");
}

std::vector<Appointment> DatabaseManager::getAllAppointments() {
    std::vector<Appointment> appointments;
    try {
        std::unique_ptr<sql::Statement> stmt(connection->createStatement());
        std::unique_ptr<sql::ResultSet> res(stmt->executeQuery(
            "SELECT a.*, p.PatientName, d.DoctorName FROM Appointment a "
            "JOIN Patient p ON a.PatientID = p.PatientID "
            "JOIN Doctors d ON a.DoctorID = d.DoctorID "
            "ORDER BY a.AppointmentDate DESC, a.AppointmentTime"));
        while (res->next()) {
            Appointment a;
            a.appointmentID = res->getInt("AppointmentID");
            a.status = res->getString("Status");
            a.appointmentTime = res->getString("AppointmentTime");
            a.appointmentDate = res->getString("AppointmentDate");
            a.reason = res->getString("Reason");
            a.duration = res->getInt("Duration");
            a.consultationFee = res->getDouble("ConsultationFee");
            a.medicineFee = res->getDouble("MedicineFee");
            a.totalCost = res->getDouble("TotalCost");
            a.patientID = res->getInt("PatientID");
            a.doctorID = res->getInt("DoctorID");
            a.patientName = res->getString("PatientName");
            a.doctorName = res->getString("DoctorName");
            appointments.push_back(a);
        }
    }
    catch (sql::SQLException& e) {
        std::cerr << "[ERROR] " << e.what() << std::endl;
    }
    return appointments;
}

std::vector<Appointment> DatabaseManager::getPendingAppointments() {
    std::vector<Appointment> appointments;
    try {
        std::unique_ptr<sql::Statement> stmt(connection->createStatement());
        std::unique_ptr<sql::ResultSet> res(stmt->executeQuery(
            "SELECT a.*, p.PatientName, d.DoctorName FROM Appointment a "
            "JOIN Patient p ON a.PatientID = p.PatientID "
            "JOIN Doctors d ON a.DoctorID = d.DoctorID "
            "WHERE a.Status = 'Pending' "
            "ORDER BY a.AppointmentDate, a.AppointmentTime"));
        while (res->next()) {
            Appointment a;
            a.appointmentID = res->getInt("AppointmentID");
            a.status = res->getString("Status");
            a.appointmentTime = res->getString("AppointmentTime");
            a.appointmentDate = res->getString("AppointmentDate");
            a.reason = res->getString("Reason");
            a.duration = res->getInt("Duration");
            a.consultationFee = res->getDouble("ConsultationFee");
            a.medicineFee = res->getDouble("MedicineFee");
            a.totalCost = res->getDouble("TotalCost");
            a.patientID = res->getInt("PatientID");
            a.doctorID = res->getInt("DoctorID");
            a.patientName = res->getString("PatientName");
            a.doctorName = res->getString("DoctorName");
            appointments.push_back(a);
        }
    }
    catch (sql::SQLException& e) {
        std::cerr << "[ERROR] " << e.what() << std::endl;
    }
    return appointments;
}

std::vector<Appointment> DatabaseManager::getAppointmentsByDateRange(const std::string& startDate, const std::string& endDate) {
    std::vector<Appointment> appointments;
    try {
        std::unique_ptr<sql::PreparedStatement> pstmt(
            connection->prepareStatement(
                "SELECT a.*, p.PatientName, d.DoctorName FROM Appointment a "
                "JOIN Patient p ON a.PatientID = p.PatientID "
                "JOIN Doctors d ON a.DoctorID = d.DoctorID "
                "WHERE a.AppointmentDate BETWEEN ? AND ? "
                "ORDER BY a.AppointmentDate DESC, a.AppointmentTime"));
        pstmt->setString(1, startDate);
        pstmt->setString(2, endDate);
        std::unique_ptr<sql::ResultSet> res(pstmt->executeQuery());
        while (res->next()) {
            Appointment a;
            a.appointmentID = res->getInt("AppointmentID");
            a.status = res->getString("Status");
            a.appointmentTime = res->getString("AppointmentTime");
            a.appointmentDate = res->getString("AppointmentDate");
            a.reason = res->getString("Reason");
            a.duration = res->getInt("Duration");
            a.consultationFee = res->getDouble("ConsultationFee");
            a.medicineFee = res->getDouble("MedicineFee");
            a.totalCost = res->getDouble("TotalCost");
            a.patientID = res->getInt("PatientID");
            a.doctorID = res->getInt("DoctorID");
            a.patientName = res->getString("PatientName");
            a.doctorName = res->getString("DoctorName");
            appointments.push_back(a);
        }
    }
    catch (sql::SQLException& e) {
        std::cerr << "[ERROR] " << e.what() << std::endl;
    }
    return appointments;
}

std::vector<Appointment> DatabaseManager::getTodayAppointments() {
    std::vector<Appointment> appointments;
    try {
        std::unique_ptr<sql::Statement> stmt(connection->createStatement());
        std::unique_ptr<sql::ResultSet> res(stmt->executeQuery(
            "SELECT a.*, p.PatientName, d.DoctorName FROM Appointment a "
            "JOIN Patient p ON a.PatientID = p.PatientID "
            "JOIN Doctors d ON a.DoctorID = d.DoctorID "
            "WHERE a.AppointmentDate = CURDATE() "
            "ORDER BY a.AppointmentTime"));
        while (res->next()) {
            Appointment a;
            a.appointmentID = res->getInt("AppointmentID");
            a.status = res->getString("Status");
            a.appointmentTime = res->getString("AppointmentTime");
            a.appointmentDate = res->getString("AppointmentDate");
            a.reason = res->getString("Reason");
            a.duration = res->getInt("Duration");
            a.consultationFee = res->getDouble("ConsultationFee");
            a.medicineFee = res->getDouble("MedicineFee");
            a.totalCost = res->getDouble("TotalCost");
            a.patientID = res->getInt("PatientID");
            a.doctorID = res->getInt("DoctorID");
            a.patientName = res->getString("PatientName");
            a.doctorName = res->getString("DoctorName");
            appointments.push_back(a);
        }
    }
    catch (sql::SQLException& e) {
        std::cerr << "[ERROR] " << e.what() << std::endl;
    }
    return appointments;
}

std::vector<Appointment> DatabaseManager::getWeeklyAppointments() {
    std::vector<Appointment> appointments;
    try {
        std::unique_ptr<sql::Statement> stmt(connection->createStatement());
        std::unique_ptr<sql::ResultSet> res(stmt->executeQuery(
            "SELECT a.*, p.PatientName, d.DoctorName FROM Appointment a "
            "JOIN Patient p ON a.PatientID = p.PatientID "
            "JOIN Doctors d ON a.DoctorID = d.DoctorID "
            "WHERE a.AppointmentDate >= DATE_SUB(CURDATE(), INTERVAL 7 DAY) "
            "AND a.AppointmentDate <= CURDATE() "
            "ORDER BY a.AppointmentDate DESC, a.AppointmentTime"));
        while (res->next()) {
            Appointment a;
            a.appointmentID = res->getInt("AppointmentID");
            a.status = res->getString("Status");
            a.appointmentTime = res->getString("AppointmentTime");
            a.appointmentDate = res->getString("AppointmentDate");
            a.reason = res->getString("Reason");
            a.duration = res->getInt("Duration");
            a.consultationFee = res->getDouble("ConsultationFee");
            a.medicineFee = res->getDouble("MedicineFee");
            a.totalCost = res->getDouble("TotalCost");
            a.patientID = res->getInt("PatientID");
            a.doctorID = res->getInt("DoctorID");
            a.patientName = res->getString("PatientName");
            a.doctorName = res->getString("DoctorName");
            appointments.push_back(a);
        }
    }
    catch (sql::SQLException& e) {
        std::cerr << "[ERROR] " << e.what() << std::endl;
    }
    return appointments;
}

std::vector<Appointment> DatabaseManager::getMonthlyAppointments() {
    std::vector<Appointment> appointments;
    try {
        std::unique_ptr<sql::Statement> stmt(connection->createStatement());
        std::unique_ptr<sql::ResultSet> res(stmt->executeQuery(
            "SELECT a.*, p.PatientName, d.DoctorName FROM Appointment a "
            "JOIN Patient p ON a.PatientID = p.PatientID "
            "JOIN Doctors d ON a.DoctorID = d.DoctorID "
            "WHERE a.AppointmentDate >= DATE_SUB(CURDATE(), INTERVAL 30 DAY) "
            "AND a.AppointmentDate <= CURDATE() "
            "ORDER BY a.AppointmentDate DESC, a.AppointmentTime"));
        while (res->next()) {
            Appointment a;
            a.appointmentID = res->getInt("AppointmentID");
            a.status = res->getString("Status");
            a.appointmentTime = res->getString("AppointmentTime");
            a.appointmentDate = res->getString("AppointmentDate");
            a.reason = res->getString("Reason");
            a.duration = res->getInt("Duration");
            a.consultationFee = res->getDouble("ConsultationFee");
            a.medicineFee = res->getDouble("MedicineFee");
            a.totalCost = res->getDouble("TotalCost");
            a.patientID = res->getInt("PatientID");
            a.doctorID = res->getInt("DoctorID");
            a.patientName = res->getString("PatientName");
            a.doctorName = res->getString("DoctorName");
            appointments.push_back(a);
        }
    }
    catch (sql::SQLException& e) {
        std::cerr << "[ERROR] " << e.what() << std::endl;
    }
    return appointments;
}

Appointment DatabaseManager::getAppointmentById(int appointmentID) {
    Appointment appt;
    try {
        std::unique_ptr<sql::PreparedStatement> pstmt(
            connection->prepareStatement(
                "SELECT a.*, p.PatientName, d.DoctorName FROM Appointment a "
                "JOIN Patient p ON a.PatientID = p.PatientID "
                "JOIN Doctors d ON a.DoctorID = d.DoctorID "
                "WHERE a.AppointmentID = ?"));
        pstmt->setInt(1, appointmentID);
        std::unique_ptr<sql::ResultSet> res(pstmt->executeQuery());
        if (res->next()) {
            appt.appointmentID = res->getInt("AppointmentID");
            appt.status = res->getString("Status");
            appt.appointmentTime = res->getString("AppointmentTime");
            appt.appointmentDate = res->getString("AppointmentDate");
            appt.reason = res->getString("Reason");
            appt.duration = res->getInt("Duration");
            appt.consultationFee = res->getDouble("ConsultationFee");
            appt.medicineFee = res->getDouble("MedicineFee");
            appt.totalCost = res->getDouble("TotalCost");
            appt.patientID = res->getInt("PatientID");
            appt.doctorID = res->getInt("DoctorID");
            appt.patientName = res->getString("PatientName");
            appt.doctorName = res->getString("DoctorName");
        }
    }
    catch (sql::SQLException& e) {
        std::cerr << "[ERROR] " << e.what() << std::endl;
    }
    return appt;
}

bool DatabaseManager::updateAppointmentStatus(int appointmentID, const std::string& status) {
    try {
        std::unique_ptr<sql::PreparedStatement> pstmt(
            connection->prepareStatement("UPDATE Appointment SET Status = ? WHERE AppointmentID = ?"));
        pstmt->setString(1, status);
        pstmt->setInt(2, appointmentID);
        pstmt->executeUpdate();
        return true;
    }
    catch (sql::SQLException& e) {
        std::cerr << "[ERROR] " << e.what() << std::endl;
        return false;
    }
}

bool DatabaseManager::cancelAppointment(int appointmentID) {
    return updateAppointmentStatus(appointmentID, "Cancelled");
}

bool DatabaseManager::checkDoctorAvailability(int doctorID, const std::string& date, const std::string& time) {
    try {
        std::unique_ptr<sql::PreparedStatement> pstmt(
            connection->prepareStatement(
                "SELECT COUNT(*) as cnt FROM Appointment WHERE DoctorID = ? AND AppointmentDate = ? "
                "AND AppointmentTime = ? AND Status NOT IN ('Cancelled', 'Completed')"));
        pstmt->setInt(1, doctorID);
        pstmt->setString(2, date);
        pstmt->setString(3, time);
        std::unique_ptr<sql::ResultSet> res(pstmt->executeQuery());
        if (res->next()) {
            return res->getInt("cnt") == 0;
        }
    }
    catch (sql::SQLException& e) {
        std::cerr << "[ERROR] " << e.what() << std::endl;
    }
    return false;
}

bool DatabaseManager::checkPatientDailyLimit(int patientID, int doctorID, const std::string& date) {
    try {
        std::unique_ptr<sql::PreparedStatement> pstmt(
            connection->prepareStatement(
                "SELECT COUNT(*) as cnt FROM Appointment WHERE PatientID = ? AND DoctorID = ? "
                "AND AppointmentDate = ? AND Status NOT IN ('Cancelled')"));
        pstmt->setInt(1, patientID);
        pstmt->setInt(2, doctorID);
        pstmt->setString(3, date);
        std::unique_ptr<sql::ResultSet> res(pstmt->executeQuery());
        if (res->next()) {
            return res->getInt("cnt") == 0;
        }
    }
    catch (sql::SQLException& e) {
        std::cerr << "[ERROR] " << e.what() << std::endl;
    }
    return false;
}

// ============================================================
// Activity Log Operations
// ============================================================

bool DatabaseManager::logActivity(const std::string& userType, int userID, 
                                  const std::string& action, const std::string& details) {
    try {
        std::unique_ptr<sql::PreparedStatement> pstmt(
            connection->prepareStatement(
                "INSERT INTO ActivityLog (UserType, UserID, Action, Details) VALUES (?, ?, ?, ?)"));
        pstmt->setString(1, userType);
        pstmt->setInt(2, userID);
        pstmt->setString(3, action);
        pstmt->setString(4, details);
        pstmt->executeUpdate();
        return true;
    }
    catch (sql::SQLException& e) {
        std::cerr << "[ERROR] " << e.what() << std::endl;
        return false;
    }
}

std::vector<ActivityLog> DatabaseManager::getActivityLogs(int limit) {
    std::vector<ActivityLog> logs;
    try {
        std::unique_ptr<sql::PreparedStatement> pstmt(
            connection->prepareStatement("SELECT * FROM ActivityLog ORDER BY LogTime DESC LIMIT ?"));
        pstmt->setInt(1, limit);
        std::unique_ptr<sql::ResultSet> res(pstmt->executeQuery());
        while (res->next()) {
            ActivityLog log;
            log.logID = res->getInt("LogID");
            log.userType = res->getString("UserType");
            log.userID = res->getInt("UserID");
            log.action = res->getString("Action");
            log.details = res->getString("Details");
            log.timestamp = res->getString("LogTime");
            logs.push_back(log);
        }
    }
    catch (sql::SQLException& e) {
        std::cerr << "[ERROR] " << e.what() << std::endl;
    }
    return logs;
}

// ============================================================
// Grade A: Complex Calculations (SQL Aggregations)
// ============================================================

std::vector<DatabaseManager::DoctorStats> DatabaseManager::getDoctorStatistics() {
    std::vector<DatabaseManager::DoctorStats> stats;
    try {
        std::unique_ptr<sql::Statement> stmt(connection->createStatement());
        std::unique_ptr<sql::ResultSet> res(stmt->executeQuery(
            "SELECT d.DoctorID, d.DoctorName, "
            "COUNT(a.AppointmentID) AS TotalAppointments, "
            "SUM(CASE WHEN a.Status = 'Confirmed' THEN 1 ELSE 0 END) AS ConfirmedCount, "
            "AVG(CASE WHEN a.Status = 'Completed' THEN 1.0 ELSE 0.0 END) * 100 AS CompletionRate "
            "FROM Doctors d "
            "LEFT JOIN Appointment a ON d.DoctorID = a.DoctorID "
            "GROUP BY d.DoctorID, d.DoctorName "
            "ORDER BY TotalAppointments DESC"));
        while (res->next()) {
            DatabaseManager::DoctorStats s;
            s.doctorID = res->getInt("DoctorID");
            s.doctorName = res->getString("DoctorName");
            s.totalAppointments = res->getInt("TotalAppointments");
            s.confirmedCount = res->getInt("ConfirmedCount");
            s.completionRate = res->getDouble("CompletionRate");
            stats.push_back(s);
        }
    }
    catch (sql::SQLException& e) {
        std::cerr << "[ERROR] " << e.what() << std::endl;
    }
    return stats;
}

std::vector<DatabaseManager::MonthlyStats> DatabaseManager::getMonthlyStatistics() {
    std::vector<DatabaseManager::MonthlyStats> stats;
    try {
        std::unique_ptr<sql::Statement> stmt(connection->createStatement());
        std::unique_ptr<sql::ResultSet> res(stmt->executeQuery(
            "SELECT YEAR(AppointmentDate) AS Year, MONTH(AppointmentDate) AS Month, "
            "COUNT(*) AS TotalAppointments, "
            "SUM(CASE WHEN Status = 'Completed' THEN 1 ELSE 0 END) AS Completed, "
            "AVG(CASE WHEN Status = 'Completed' THEN 1.0 ELSE 0.0 END) * 100 AS CompletionPercentage "
            "FROM Appointment "
            "GROUP BY YEAR(AppointmentDate), MONTH(AppointmentDate) "
            "ORDER BY Year DESC, Month DESC"));
        while (res->next()) {
            DatabaseManager::MonthlyStats s;
            s.year = res->getInt("Year");
            s.month = res->getInt("Month");
            s.totalAppointments = res->getInt("TotalAppointments");
            s.completed = res->getInt("Completed");
            s.completionPercentage = res->getDouble("CompletionPercentage");
            stats.push_back(s);
        }
    }
    catch (sql::SQLException& e) {
        std::cerr << "[ERROR] " << e.what() << std::endl;
    }
    return stats;
}

std::vector<DatabaseManager::DailyStats> DatabaseManager::getDailyStatistics() {
    std::vector<DatabaseManager::DailyStats> stats;
    try {
        std::unique_ptr<sql::Statement> stmt(connection->createStatement());
        std::unique_ptr<sql::ResultSet> res(stmt->executeQuery(
            "SELECT AppointmentDate, "
            "COUNT(*) AS Total, "
            "SUM(CASE WHEN Status = 'Confirmed' THEN 1 ELSE 0 END) AS Confirmed, "
            "SUM(CASE WHEN Status = 'Pending' THEN 1 ELSE 0 END) AS Pending, "
            "SUM(CASE WHEN Status = 'Completed' THEN 1 ELSE 0 END) AS Completed, "
            "SUM(CASE WHEN Status = 'Cancelled' THEN 1 ELSE 0 END) AS Cancelled "
            "FROM Appointment "
            "GROUP BY AppointmentDate "
            "ORDER BY AppointmentDate DESC"));
        while (res->next()) {
            DatabaseManager::DailyStats s;
            s.date = res->getString("AppointmentDate");
            s.total = res->getInt("Total");
            s.confirmed = res->getInt("Confirmed");
            s.pending = res->getInt("Pending");
            s.completed = res->getInt("Completed");
            s.cancelled = res->getInt("Cancelled");
            stats.push_back(s);
        }
    }
    catch (sql::SQLException& e) {
        std::cerr << "[ERROR] " << e.what() << std::endl;
    }
    return stats;
}

DatabaseManager::RevenueStats DatabaseManager::getRevenueStatistics() {
    RevenueStats stats;
    try {
        std::unique_ptr<sql::Statement> stmt(connection->createStatement());
        std::unique_ptr<sql::ResultSet> res(stmt->executeQuery(
            "SELECT "
            "SUM(CASE WHEN Status = 'Completed' THEN TotalCost ELSE 0 END) AS TotalRevenue, "
            "SUM(CASE WHEN Status IN ('Pending', 'Confirmed') THEN TotalCost ELSE 0 END) AS PotentialRevenue, "
            "AVG(TotalCost) AS AverageCost, "
            "SUM(CASE WHEN Status = 'Completed' THEN ConsultationFee ELSE 0 END) AS TotalConsultation, "
            "SUM(CASE WHEN Status = 'Completed' THEN MedicineFee ELSE 0 END) AS TotalMedicine "
            "FROM Appointment WHERE Status != 'Cancelled'"));
        if (res->next()) {
            stats.totalRevenue = res->getDouble("TotalRevenue");
            stats.potentialRevenue = res->getDouble("PotentialRevenue");
            stats.averageCost = res->getDouble("AverageCost");
            stats.totalConsultation = res->getDouble("TotalConsultation");
            stats.totalMedicine = res->getDouble("TotalMedicine");
        }
    }
    catch (sql::SQLException& e) {
        std::cerr << "[ERROR] " << e.what() << std::endl;
    }
    return stats;
}