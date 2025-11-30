// ============================================================
// DatabaseManager.cpp - MySQL Connector C++ (JDBC API)
// Hospital Appointment Booking System
// ============================================================

#include "../include/DatabaseManager.h"
#include <iostream>

DatabaseManager::DatabaseManager() : driver(nullptr), isConnected(false) {}

DatabaseManager::~DatabaseManager() {
    disconnect();
}

bool DatabaseManager::connect() {
    try {
        driver = get_driver_instance();
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

int DatabaseManager::authenticatePatient(const std::string& email, const std::string& pwd) {
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

int DatabaseManager::authenticateDoctor(const std::string& email, const std::string& pwd) {
    try {
        std::unique_ptr<sql::PreparedStatement> pstmt(
            connection->prepareStatement("SELECT DoctorID FROM Doctors WHERE Email = ? AND Password = ? AND IsApproved = TRUE"));
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

int DatabaseManager::authenticateStaff(const std::string& email, const std::string& pwd) {
    try {
        std::unique_ptr<sql::PreparedStatement> pstmt(
            connection->prepareStatement("SELECT StaffID FROM Staff WHERE Email = ? AND Password = ? AND IsApproved = TRUE"));
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

int DatabaseManager::authenticateAdmin(const std::string& email, const std::string& pwd) {
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
            patient.dob = res->getString("DOB");
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

std::vector<Patient> DatabaseManager::getAllPatients() {
    std::vector<Patient> patients;
    try {
        std::unique_ptr<sql::Statement> stmt(connection->createStatement());
        std::unique_ptr<sql::ResultSet> res(stmt->executeQuery("SELECT * FROM Patient ORDER BY PatientName"));
        while (res->next()) {
            Patient p;
            p.patientID = res->getInt("PatientID");
            p.patientName = res->getString("PatientName");
            p.phoneNumber = res->getString("PhoneNumber");
            p.email = res->getString("Email");
            p.address = res->getString("Address");
            p.dob = res->getString("DOB");
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
            doctor.isApproved = res->getBoolean("IsApproved");
        }
    }
    catch (sql::SQLException& e) {
        std::cerr << "[ERROR] " << e.what() << std::endl;
    }
    return doctor;
}

std::vector<Doctor> DatabaseManager::getAllDoctors(bool approvedOnly) {
    std::vector<Doctor> doctors;
    try {
        std::string query = "SELECT * FROM Doctors";
        if (approvedOnly) query += " WHERE IsApproved = TRUE";
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
            d.isApproved = res->getBoolean("IsApproved");
            doctors.push_back(d);
        }
    }
    catch (sql::SQLException& e) {
        std::cerr << "[ERROR] " << e.what() << std::endl;
    }
    return doctors;
}

bool DatabaseManager::approveDoctor(int doctorID) {
    try {
        std::unique_ptr<sql::PreparedStatement> pstmt(
            connection->prepareStatement("UPDATE Doctors SET IsApproved = TRUE WHERE DoctorID = ?"));
        pstmt->setInt(1, doctorID);
        pstmt->executeUpdate();
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
            staff.phoneNumber = res->getString("PhoneNumber");
            staff.email = res->getString("Email");
            staff.role = res->getString("Role");
            staff.isApproved = res->getBoolean("IsApproved");
        }
    }
    catch (sql::SQLException& e) {
        std::cerr << "[ERROR] " << e.what() << std::endl;
    }
    return staff;
}

std::vector<Staff> DatabaseManager::getAllStaff(bool approvedOnly) {
    std::vector<Staff> staffList;
    try {
        std::string query = "SELECT * FROM Staff";
        if (approvedOnly) query += " WHERE IsApproved = TRUE";
        query += " ORDER BY StaffName";
        
        std::unique_ptr<sql::Statement> stmt(connection->createStatement());
        std::unique_ptr<sql::ResultSet> res(stmt->executeQuery(query));
        while (res->next()) {
            Staff s;
            s.staffID = res->getInt("StaffID");
            s.staffName = res->getString("StaffName");
            s.phoneNumber = res->getString("PhoneNumber");
            s.email = res->getString("Email");
            s.role = res->getString("Role");
            s.isApproved = res->getBoolean("IsApproved");
            staffList.push_back(s);
        }
    }
    catch (sql::SQLException& e) {
        std::cerr << "[ERROR] " << e.what() << std::endl;
    }
    return staffList;
}

bool DatabaseManager::approveStaff(int staffID) {
    try {
        std::unique_ptr<sql::PreparedStatement> pstmt(
            connection->prepareStatement("UPDATE Staff SET IsApproved = TRUE WHERE StaffID = ?"));
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
                                        const std::string& time, const std::string& reason) {
    try {
        std::unique_ptr<sql::PreparedStatement> pstmt(
            connection->prepareStatement(
                "INSERT INTO Appointment (PatientID, DoctorID, AppointmentDate, AppointmentTime, Reason, Status) "
                "VALUES (?, ?, ?, ?, ?, 'Pending')"));
        pstmt->setInt(1, patientID);
        pstmt->setInt(2, doctorID);
        pstmt->setString(3, date);
        pstmt->setString(4, time);
        pstmt->setString(5, reason);
        pstmt->executeUpdate();
        return true;
    }
    catch (sql::SQLException& e) {
        std::cerr << "[ERROR] " << e.what() << std::endl;
        return false;
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

std::vector<Appointment> DatabaseManager::getAllAppointments(const std::string& status) {
    std::vector<Appointment> appointments;
    try {
        std::string query = "SELECT a.*, p.PatientName, d.DoctorName FROM Appointment a "
                           "JOIN Patient p ON a.PatientID = p.PatientID "
                           "JOIN Doctors d ON a.DoctorID = d.DoctorID";
        if (!status.empty()) {
            query += " WHERE a.Status = ?";
        }
        query += " ORDER BY a.AppointmentDate DESC, a.AppointmentTime";
        
        std::unique_ptr<sql::PreparedStatement> pstmt(connection->prepareStatement(query));
        if (!status.empty()) {
            pstmt->setString(1, status);
        }
        std::unique_ptr<sql::ResultSet> res(pstmt->executeQuery());
        while (res->next()) {
            Appointment a;
            a.appointmentID = res->getInt("AppointmentID");
            a.status = res->getString("Status");
            a.appointmentTime = res->getString("AppointmentTime");
            a.appointmentDate = res->getString("AppointmentDate");
            a.reason = res->getString("Reason");
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

bool DatabaseManager::updateAppointmentStatus(int appointmentID, const std::string& status, int staffID) {
    try {
        std::string query = "UPDATE Appointment SET Status = ?";
        if (staffID > 0) {
            query += ", StaffID = ?";
        }
        query += " WHERE AppointmentID = ?";
        
        std::unique_ptr<sql::PreparedStatement> pstmt(connection->prepareStatement(query));
        pstmt->setString(1, status);
        if (staffID > 0) {
            pstmt->setInt(2, staffID);
            pstmt->setInt(3, appointmentID);
        } else {
            pstmt->setInt(2, appointmentID);
        }
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
// Report Operations
// ============================================================

AppointmentSummary DatabaseManager::getDailyAppointmentSummary(const std::string& date) {
    AppointmentSummary summary;
    try {
        std::string query = "SELECT Status, COUNT(*) as cnt FROM Appointment ";
        if (!date.empty()) {
            query += "WHERE AppointmentDate = ? ";
        }
        query += "GROUP BY Status";
        
        std::unique_ptr<sql::PreparedStatement> pstmt(connection->prepareStatement(query));
        if (!date.empty()) {
            pstmt->setString(1, date);
        }
        std::unique_ptr<sql::ResultSet> res(pstmt->executeQuery());
        while (res->next()) {
            std::string status = res->getString("Status");
            int count = res->getInt("cnt");
            summary.total += count;
            if (status == "Confirmed") summary.confirmed = count;
            else if (status == "Pending") summary.pending = count;
            else if (status == "Completed") summary.completed = count;
            else if (status == "Cancelled") summary.cancelled = count;
        }
    }
    catch (sql::SQLException& e) {
        std::cerr << "[ERROR] " << e.what() << std::endl;
    }
    return summary;
}

std::string DatabaseManager::getMostActiveDoctor() {
    try {
        std::unique_ptr<sql::Statement> stmt(connection->createStatement());
        std::unique_ptr<sql::ResultSet> res(stmt->executeQuery(
            "SELECT d.DoctorName, COUNT(*) as cnt FROM Appointment a "
            "JOIN Doctors d ON a.DoctorID = d.DoctorID "
            "WHERE a.Status != 'Cancelled' "
            "GROUP BY a.DoctorID ORDER BY cnt DESC LIMIT 1"));
        if (res->next()) {
            return res->getString("DoctorName");
        }
    }
    catch (sql::SQLException& e) {
        std::cerr << "[ERROR] " << e.what() << std::endl;
    }
    return "N/A";
}

std::string DatabaseManager::getMostFrequentPatient() {
    try {
        std::unique_ptr<sql::Statement> stmt(connection->createStatement());
        std::unique_ptr<sql::ResultSet> res(stmt->executeQuery(
            "SELECT p.PatientName, COUNT(*) as cnt FROM Appointment a "
            "JOIN Patient p ON a.PatientID = p.PatientID "
            "GROUP BY a.PatientID ORDER BY cnt DESC LIMIT 1"));
        if (res->next()) {
            return res->getString("PatientName");
        }
    }
    catch (sql::SQLException& e) {
        std::cerr << "[ERROR] " << e.what() << std::endl;
    }
    return "N/A";
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

std::vector<ActivityLogEntry> DatabaseManager::getActivityLogs(int limit) {
    std::vector<ActivityLogEntry> logs;
    try {
        std::unique_ptr<sql::PreparedStatement> pstmt(
            connection->prepareStatement("SELECT * FROM ActivityLog ORDER BY LogTime DESC LIMIT ?"));
        pstmt->setInt(1, limit);
        std::unique_ptr<sql::ResultSet> res(pstmt->executeQuery());
        while (res->next()) {
            ActivityLogEntry log;
            log.logID = res->getInt("LogID");
            log.userType = res->getString("UserType");
            log.userID = res->getInt("UserID");
            log.action = res->getString("Action");
            log.details = res->getString("Details");
            log.logTime = res->getString("LogTime");
            logs.push_back(log);
        }
    }
    catch (sql::SQLException& e) {
        std::cerr << "[ERROR] " << e.what() << std::endl;
    }
    return logs;
}

int DatabaseManager::getPendingDoctorsCount() {
    try {
        std::unique_ptr<sql::Statement> stmt(connection->createStatement());
        std::unique_ptr<sql::ResultSet> res(stmt->executeQuery("SELECT COUNT(*) as cnt FROM Doctors WHERE IsApproved = FALSE"));
        if (res->next()) {
            return res->getInt("cnt");
        }
    }
    catch (sql::SQLException& e) {
        std::cerr << "[ERROR] " << e.what() << std::endl;
    }
    return 0;
}

int DatabaseManager::getPendingStaffCount() {
    try {
        std::unique_ptr<sql::Statement> stmt(connection->createStatement());
        std::unique_ptr<sql::ResultSet> res(stmt->executeQuery("SELECT COUNT(*) as cnt FROM Staff WHERE IsApproved = FALSE"));
        if (res->next()) {
            return res->getInt("cnt");
        }
    }
    catch (sql::SQLException& e) {
        std::cerr << "[ERROR] " << e.what() << std::endl;
    }
    return 0;
}
