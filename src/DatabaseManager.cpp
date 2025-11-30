// ============================================================
// DatabaseManager.cpp - MySQL Database Implementation
// Hospital Appointment Booking System
// ============================================================

#include "../include/DatabaseManager.h"
#include <stdexcept>

DatabaseManager::DatabaseManager() : connection(nullptr), isConnected(false) {}

DatabaseManager::~DatabaseManager() {
    disconnect();
}

bool DatabaseManager::connect() {
    try {
        connection = mysql_init(nullptr);
        
        if (connection == nullptr) {
            std::cerr << "[ERROR] MySQL initialization failed!" << std::endl;
            return false;
        }
        
        unsigned int timeout = 10;
        mysql_options(connection, MYSQL_OPT_CONNECT_TIMEOUT, &timeout);
        
        if (mysql_real_connect(connection, host, user, password, 
                               database, port, nullptr, 0) == nullptr) {
            std::cerr << "[ERROR] Database connection failed: " << mysql_error(connection) << std::endl;
            mysql_close(connection);
            connection = nullptr;
            return false;
        }
        
        mysql_set_character_set(connection, "utf8mb4");
        isConnected = true;
        return true;
        
    } catch (const std::exception& e) {
        std::cerr << "[ERROR] Connection exception: " << e.what() << std::endl;
        return false;
    }
}

void DatabaseManager::disconnect() {
    if (connection != nullptr) {
        mysql_close(connection);
        connection = nullptr;
        isConnected = false;
    }
}

bool DatabaseManager::checkConnection() {
    if (!isConnected || connection == nullptr) {
        return connect();
    }
    if (mysql_ping(connection) != 0) {
        disconnect();
        return connect();
    }
    return true;
}

bool DatabaseManager::executeQuery(const std::string& query) {
    if (!checkConnection()) return false;
    return (mysql_query(connection, query.c_str()) == 0);
}

int DatabaseManager::getLastInsertId() {
    return (int)mysql_insert_id(connection);
}

std::string DatabaseManager::escapeString(const std::string& str) {
    if (!checkConnection()) return str;
    char* escaped = new char[str.length() * 2 + 1];
    mysql_real_escape_string(connection, escaped, str.c_str(), str.length());
    std::string result(escaped);
    delete[] escaped;
    return result;
}

// ============================================================
// Authentication Methods
// ============================================================

int DatabaseManager::authenticatePatient(const std::string& email, const std::string& password) {
    std::string query = "SELECT PatientID FROM Patient WHERE Email = '" + 
                       escapeString(email) + "' AND Password = '" + 
                       escapeString(password) + "'";
    
    if (!executeQuery(query)) return -1;
    
    MYSQL_RES* result = mysql_store_result(connection);
    if (result == nullptr) return -1;
    
    MYSQL_ROW row = mysql_fetch_row(result);
    int patientID = (row != nullptr) ? std::stoi(row[0]) : -1;
    
    mysql_free_result(result);
    return patientID;
}

int DatabaseManager::authenticateDoctor(const std::string& email, const std::string& password) {
    std::string query = "SELECT DoctorID FROM Doctors WHERE Email = '" + 
                       escapeString(email) + "' AND Password = '" + 
                       escapeString(password) + "' AND IsApproved = TRUE";
    
    if (!executeQuery(query)) return -1;
    
    MYSQL_RES* result = mysql_store_result(connection);
    if (result == nullptr) return -1;
    
    MYSQL_ROW row = mysql_fetch_row(result);
    int doctorID = (row != nullptr) ? std::stoi(row[0]) : -1;
    
    mysql_free_result(result);
    return doctorID;
}

int DatabaseManager::authenticateStaff(const std::string& email, const std::string& password) {
    std::string query = "SELECT StaffID FROM Staff WHERE Email = '" + 
                       escapeString(email) + "' AND Password = '" + 
                       escapeString(password) + "' AND IsApproved = TRUE";
    
    if (!executeQuery(query)) return -1;
    
    MYSQL_RES* result = mysql_store_result(connection);
    if (result == nullptr) return -1;
    
    MYSQL_ROW row = mysql_fetch_row(result);
    int staffID = (row != nullptr) ? std::stoi(row[0]) : -1;
    
    mysql_free_result(result);
    return staffID;
}

int DatabaseManager::authenticateAdmin(const std::string& email, const std::string& password) {
    std::string query = "SELECT AdminID FROM Admin WHERE Email = '" + 
                       escapeString(email) + "' AND Password = '" + 
                       escapeString(password) + "'";
    
    if (!executeQuery(query)) return -1;
    
    MYSQL_RES* result = mysql_store_result(connection);
    if (result == nullptr) return -1;
    
    MYSQL_ROW row = mysql_fetch_row(result);
    int adminID = (row != nullptr) ? std::stoi(row[0]) : -1;
    
    mysql_free_result(result);
    return adminID;
}

// ============================================================
// Patient Operations
// ============================================================

bool DatabaseManager::registerPatient(const std::string& name, const std::string& phone,
                                      const std::string& email, const std::string& address,
                                      const std::string& dob, const std::string& gender,
                                      const std::string& password) {
    std::string query = "INSERT INTO Patient (PatientName, PhoneNumber, Email, "
                       "Address, DOB, Gender, Password) VALUES ('" +
                       escapeString(name) + "', '" + escapeString(phone) + "', '" +
                       escapeString(email) + "', '" + escapeString(address) + "', '" +
                       escapeString(dob) + "', '" + escapeString(gender) + "', '" +
                       escapeString(password) + "')";
    return executeQuery(query);
}

Patient DatabaseManager::getPatientById(int patientID) {
    Patient patient;
    std::string query = "SELECT * FROM Patient WHERE PatientID = " + std::to_string(patientID);
    
    if (!executeQuery(query)) return patient;
    
    MYSQL_RES* result = mysql_store_result(connection);
    if (result == nullptr) return patient;
    
    MYSQL_ROW row = mysql_fetch_row(result);
    if (row != nullptr) {
        patient.patientID = std::stoi(row[0]);
        patient.patientName = row[1] ? row[1] : "";
        patient.phoneNumber = row[2] ? row[2] : "";
        patient.email = row[3] ? row[3] : "";
        patient.address = row[4] ? row[4] : "";
        patient.dob = row[5] ? row[5] : "";
        patient.gender = row[6] ? row[6] : "";
    }
    
    mysql_free_result(result);
    return patient;
}

bool DatabaseManager::updatePatient(int patientID, const std::string& name, const std::string& phone,
                                   const std::string& email, const std::string& address) {
    std::string query = "UPDATE Patient SET PatientName = '" + escapeString(name) +
                       "', PhoneNumber = '" + escapeString(phone) +
                       "', Email = '" + escapeString(email) +
                       "', Address = '" + escapeString(address) +
                       "' WHERE PatientID = " + std::to_string(patientID);
    return executeQuery(query);
}

std::vector<Patient> DatabaseManager::getAllPatients() {
    std::vector<Patient> patients;
    std::string query = "SELECT * FROM Patient ORDER BY PatientName";
    
    if (!executeQuery(query)) return patients;
    
    MYSQL_RES* result = mysql_store_result(connection);
    if (result == nullptr) return patients;
    
    MYSQL_ROW row;
    while ((row = mysql_fetch_row(result)) != nullptr) {
        Patient p;
        p.patientID = std::stoi(row[0]);
        p.patientName = row[1] ? row[1] : "";
        p.phoneNumber = row[2] ? row[2] : "";
        p.email = row[3] ? row[3] : "";
        p.address = row[4] ? row[4] : "";
        p.dob = row[5] ? row[5] : "";
        p.gender = row[6] ? row[6] : "";
        patients.push_back(p);
    }
    
    mysql_free_result(result);
    return patients;
}

// ============================================================
// Doctor Operations
// ============================================================

Doctor DatabaseManager::getDoctorById(int doctorID) {
    Doctor doctor;
    std::string query = "SELECT * FROM Doctors WHERE DoctorID = " + std::to_string(doctorID);
    
    if (!executeQuery(query)) return doctor;
    
    MYSQL_RES* result = mysql_store_result(connection);
    if (result == nullptr) return doctor;
    
    MYSQL_ROW row = mysql_fetch_row(result);
    if (row != nullptr) {
        doctor.doctorID = std::stoi(row[0]);
        doctor.doctorName = row[1] ? row[1] : "";
        doctor.specialty = row[2] ? row[2] : "";
        doctor.phoneNumber = row[3] ? row[3] : "";
        doctor.email = row[4] ? row[4] : "";
        doctor.roomNo = row[5] ? row[5] : "";
        doctor.isApproved = (row[7] && std::string(row[7]) == "1");
    }
    
    mysql_free_result(result);
    return doctor;
}

std::vector<Doctor> DatabaseManager::getAllDoctors(bool approvedOnly) {
    std::vector<Doctor> doctors;
    std::string query = "SELECT * FROM Doctors";
    if (approvedOnly) query += " WHERE IsApproved = TRUE";
    query += " ORDER BY DoctorName";
    
    if (!executeQuery(query)) return doctors;
    
    MYSQL_RES* result = mysql_store_result(connection);
    if (result == nullptr) return doctors;
    
    MYSQL_ROW row;
    while ((row = mysql_fetch_row(result)) != nullptr) {
        Doctor d;
        d.doctorID = std::stoi(row[0]);
        d.doctorName = row[1] ? row[1] : "";
        d.specialty = row[2] ? row[2] : "";
        d.phoneNumber = row[3] ? row[3] : "";
        d.email = row[4] ? row[4] : "";
        d.roomNo = row[5] ? row[5] : "";
        d.isApproved = (row[7] && std::string(row[7]) == "1");
        doctors.push_back(d);
    }
    
    mysql_free_result(result);
    return doctors;
}

bool DatabaseManager::approveDoctor(int doctorID) {
    std::string query = "UPDATE Doctors SET IsApproved = TRUE WHERE DoctorID = " +
                       std::to_string(doctorID);
    return executeQuery(query);
}

// ============================================================
// Staff Operations
// ============================================================

Staff DatabaseManager::getStaffById(int staffID) {
    Staff staff;
    std::string query = "SELECT * FROM Staff WHERE StaffID = " + std::to_string(staffID);
    
    if (!executeQuery(query)) return staff;
    
    MYSQL_RES* result = mysql_store_result(connection);
    if (result == nullptr) return staff;
    
    MYSQL_ROW row = mysql_fetch_row(result);
    if (row != nullptr) {
        staff.staffID = std::stoi(row[0]);
        staff.staffName = row[1] ? row[1] : "";
        staff.phoneNumber = row[2] ? row[2] : "";
        staff.email = row[3] ? row[3] : "";
        staff.role = row[4] ? row[4] : "";
        staff.isApproved = (row[7] && std::string(row[7]) == "1");
    }
    
    mysql_free_result(result);
    return staff;
}

std::vector<Staff> DatabaseManager::getAllStaff(bool approvedOnly) {
    std::vector<Staff> staffList;
    std::string query = "SELECT * FROM Staff";
    if (approvedOnly) query += " WHERE IsApproved = TRUE";
    query += " ORDER BY StaffName";
    
    if (!executeQuery(query)) return staffList;
    
    MYSQL_RES* result = mysql_store_result(connection);
    if (result == nullptr) return staffList;
    
    MYSQL_ROW row;
    while ((row = mysql_fetch_row(result)) != nullptr) {
        Staff s;
        s.staffID = std::stoi(row[0]);
        s.staffName = row[1] ? row[1] : "";
        s.phoneNumber = row[2] ? row[2] : "";
        s.email = row[3] ? row[3] : "";
        s.role = row[4] ? row[4] : "";
        s.isApproved = (row[7] && std::string(row[7]) == "1");
        staffList.push_back(s);
    }
    
    mysql_free_result(result);
    return staffList;
}

bool DatabaseManager::approveStaff(int staffID) {
    std::string query = "UPDATE Staff SET IsApproved = TRUE WHERE StaffID = " +
                       std::to_string(staffID);
    return executeQuery(query);
}

// ============================================================
// Admin Operations
// ============================================================

Admin DatabaseManager::getAdminById(int adminID) {
    Admin admin;
    std::string query = "SELECT AdminID, AdminName, PhoneNumber, Email FROM Admin "
                       "WHERE AdminID = " + std::to_string(adminID);
    
    if (!executeQuery(query)) return admin;
    
    MYSQL_RES* result = mysql_store_result(connection);
    if (result == nullptr) return admin;
    
    MYSQL_ROW row = mysql_fetch_row(result);
    if (row != nullptr) {
        admin.adminID = std::stoi(row[0]);
        admin.adminName = row[1] ? row[1] : "";
        admin.phoneNumber = row[2] ? row[2] : "";
        admin.email = row[3] ? row[3] : "";
    }
    
    mysql_free_result(result);
    return admin;
}

// ============================================================
// Appointment Operations
// ============================================================

bool DatabaseManager::createAppointment(int patientID, int doctorID, const std::string& date,
                                        const std::string& time, const std::string& reason) {
    std::string query = "INSERT INTO Appointment (PatientID, DoctorID, "
                       "AppointmentDate, AppointmentTime, Reason, Status) VALUES (" +
                       std::to_string(patientID) + ", " + std::to_string(doctorID) +
                       ", '" + escapeString(date) + "', '" + escapeString(time) +
                       "', '" + escapeString(reason) + "', 'Pending')";
    return executeQuery(query);
}

std::vector<Appointment> DatabaseManager::getPatientAppointments(int patientID) {
    std::vector<Appointment> appointments;
    std::string query = "SELECT a.*, p.PatientName, d.DoctorName FROM Appointment a "
                       "JOIN Patient p ON a.PatientID = p.PatientID "
                       "JOIN Doctors d ON a.DoctorID = d.DoctorID "
                       "WHERE a.PatientID = " + std::to_string(patientID) +
                       " ORDER BY a.AppointmentDate DESC, a.AppointmentTime DESC";
    
    if (!executeQuery(query)) return appointments;
    
    MYSQL_RES* result = mysql_store_result(connection);
    if (result == nullptr) return appointments;
    
    MYSQL_ROW row;
    while ((row = mysql_fetch_row(result)) != nullptr) {
        Appointment a;
        a.appointmentID = std::stoi(row[0]);
        a.status = row[1] ? row[1] : "";
        a.appointmentTime = row[2] ? row[2] : "";
        a.appointmentDate = row[3] ? row[3] : "";
        a.reason = row[4] ? row[4] : "";
        a.patientID = row[5] ? std::stoi(row[5]) : 0;
        a.doctorID = row[6] ? std::stoi(row[6]) : 0;
        a.staffID = row[7] ? std::stoi(row[7]) : 0;
        a.patientName = row[9] ? row[9] : "";
        a.doctorName = row[10] ? row[10] : "";
        appointments.push_back(a);
    }
    
    mysql_free_result(result);
    return appointments;
}

std::vector<Appointment> DatabaseManager::getDoctorAppointments(int doctorID, const std::string& date) {
    std::vector<Appointment> appointments;
    std::string query = "SELECT a.*, p.PatientName, d.DoctorName FROM Appointment a "
                       "JOIN Patient p ON a.PatientID = p.PatientID "
                       "JOIN Doctors d ON a.DoctorID = d.DoctorID "
                       "WHERE a.DoctorID = " + std::to_string(doctorID);
    if (!date.empty()) {
        query += " AND a.AppointmentDate = '" + escapeString(date) + "'";
    }
    query += " ORDER BY a.AppointmentDate, a.AppointmentTime";
    
    if (!executeQuery(query)) return appointments;
    
    MYSQL_RES* result = mysql_store_result(connection);
    if (result == nullptr) return appointments;
    
    MYSQL_ROW row;
    while ((row = mysql_fetch_row(result)) != nullptr) {
        Appointment a;
        a.appointmentID = std::stoi(row[0]);
        a.status = row[1] ? row[1] : "";
        a.appointmentTime = row[2] ? row[2] : "";
        a.appointmentDate = row[3] ? row[3] : "";
        a.reason = row[4] ? row[4] : "";
        a.patientID = row[5] ? std::stoi(row[5]) : 0;
        a.doctorID = row[6] ? std::stoi(row[6]) : 0;
        a.patientName = row[9] ? row[9] : "";
        a.doctorName = row[10] ? row[10] : "";
        appointments.push_back(a);
    }
    
    mysql_free_result(result);
    return appointments;
}

std::vector<Appointment> DatabaseManager::getAllAppointments(const std::string& status) {
    std::vector<Appointment> appointments;
    std::string query = "SELECT a.*, p.PatientName, d.DoctorName FROM Appointment a "
                       "JOIN Patient p ON a.PatientID = p.PatientID "
                       "JOIN Doctors d ON a.DoctorID = d.DoctorID";
    if (!status.empty()) {
        query += " WHERE a.Status = '" + escapeString(status) + "'";
    }
    query += " ORDER BY a.AppointmentDate DESC, a.AppointmentTime";
    
    if (!executeQuery(query)) return appointments;
    
    MYSQL_RES* result = mysql_store_result(connection);
    if (result == nullptr) return appointments;
    
    MYSQL_ROW row;
    while ((row = mysql_fetch_row(result)) != nullptr) {
        Appointment a;
        a.appointmentID = std::stoi(row[0]);
        a.status = row[1] ? row[1] : "";
        a.appointmentTime = row[2] ? row[2] : "";
        a.appointmentDate = row[3] ? row[3] : "";
        a.reason = row[4] ? row[4] : "";
        a.patientID = row[5] ? std::stoi(row[5]) : 0;
        a.doctorID = row[6] ? std::stoi(row[6]) : 0;
        a.patientName = row[9] ? row[9] : "";
        a.doctorName = row[10] ? row[10] : "";
        appointments.push_back(a);
    }
    
    mysql_free_result(result);
    return appointments;
}

bool DatabaseManager::updateAppointmentStatus(int appointmentID, const std::string& status, int staffID) {
    std::string query = "UPDATE Appointment SET Status = '" + escapeString(status) + "'";
    if (staffID > 0) {
        query += ", StaffID = " + std::to_string(staffID);
    }
    query += " WHERE AppointmentID = " + std::to_string(appointmentID);
    return executeQuery(query);
}

bool DatabaseManager::cancelAppointment(int appointmentID) {
    return updateAppointmentStatus(appointmentID, "Cancelled");
}

bool DatabaseManager::checkDoctorAvailability(int doctorID, const std::string& date, const std::string& time) {
    std::string query = "SELECT COUNT(*) FROM Appointment WHERE DoctorID = " +
                       std::to_string(doctorID) + " AND AppointmentDate = '" +
                       escapeString(date) + "' AND AppointmentTime = '" +
                       escapeString(time) + "' AND Status NOT IN ('Cancelled', 'Completed')";
    
    if (!executeQuery(query)) return false;
    
    MYSQL_RES* result = mysql_store_result(connection);
    if (result == nullptr) return false;
    
    MYSQL_ROW row = mysql_fetch_row(result);
    int count = (row != nullptr) ? std::stoi(row[0]) : 0;
    
    mysql_free_result(result);
    return (count == 0);
}

bool DatabaseManager::checkPatientDailyLimit(int patientID, int doctorID, const std::string& date) {
    std::string query = "SELECT COUNT(*) FROM Appointment WHERE PatientID = " +
                       std::to_string(patientID) + " AND DoctorID = " +
                       std::to_string(doctorID) + " AND AppointmentDate = '" +
                       escapeString(date) + "' AND Status NOT IN ('Cancelled')";
    
    if (!executeQuery(query)) return false;
    
    MYSQL_RES* result = mysql_store_result(connection);
    if (result == nullptr) return false;
    
    MYSQL_ROW row = mysql_fetch_row(result);
    int count = (row != nullptr) ? std::stoi(row[0]) : 0;
    
    mysql_free_result(result);
    return (count == 0);
}

// ============================================================
// Report Operations
// ============================================================

AppointmentSummary DatabaseManager::getDailyAppointmentSummary(const std::string& date) {
    AppointmentSummary summary;
    std::string query = "SELECT Status, COUNT(*) FROM Appointment ";
    if (!date.empty()) {
        query += "WHERE AppointmentDate = '" + escapeString(date) + "' ";
    }
    query += "GROUP BY Status";
    
    if (!executeQuery(query)) return summary;
    
    MYSQL_RES* result = mysql_store_result(connection);
    if (result == nullptr) return summary;
    
    MYSQL_ROW row;
    while ((row = mysql_fetch_row(result)) != nullptr) {
        std::string status = row[0] ? row[0] : "";
        int count = row[1] ? std::stoi(row[1]) : 0;
        
        summary.total += count;
        if (status == "Confirmed") summary.confirmed = count;
        else if (status == "Pending") summary.pending = count;
        else if (status == "Completed") summary.completed = count;
        else if (status == "Cancelled") summary.cancelled = count;
    }
    
    mysql_free_result(result);
    return summary;
}

std::string DatabaseManager::getMostActiveDoctor() {
    std::string query = "SELECT d.DoctorName, COUNT(*) as cnt FROM Appointment a "
                       "JOIN Doctors d ON a.DoctorID = d.DoctorID "
                       "WHERE a.Status != 'Cancelled' "
                       "GROUP BY a.DoctorID ORDER BY cnt DESC LIMIT 1";
    
    if (!executeQuery(query)) return "N/A";
    
    MYSQL_RES* result = mysql_store_result(connection);
    if (result == nullptr) return "N/A";
    
    MYSQL_ROW row = mysql_fetch_row(result);
    std::string doctorName = (row != nullptr && row[0]) ? row[0] : "N/A";
    
    mysql_free_result(result);
    return doctorName;
}

std::string DatabaseManager::getMostFrequentPatient() {
    std::string query = "SELECT p.PatientName, COUNT(*) as cnt FROM Appointment a "
                       "JOIN Patient p ON a.PatientID = p.PatientID "
                       "GROUP BY a.PatientID ORDER BY cnt DESC LIMIT 1";
    
    if (!executeQuery(query)) return "N/A";
    
    MYSQL_RES* result = mysql_store_result(connection);
    if (result == nullptr) return "N/A";
    
    MYSQL_ROW row = mysql_fetch_row(result);
    std::string patientName = (row != nullptr && row[0]) ? row[0] : "N/A";
    
    mysql_free_result(result);
    return patientName;
}

// ============================================================
// Activity Log Operations
// ============================================================

bool DatabaseManager::logActivity(const std::string& userType, int userID, 
                                  const std::string& action, const std::string& details) {
    std::string query = "INSERT INTO ActivityLog (UserType, UserID, Action, Details) "
                       "VALUES ('" + escapeString(userType) + "', " +
                       std::to_string(userID) + ", '" + escapeString(action) +
                       "', '" + escapeString(details) + "')";
    return executeQuery(query);
}

std::vector<ActivityLogEntry> DatabaseManager::getActivityLogs(int limit) {
    std::vector<ActivityLogEntry> logs;
    std::string query = "SELECT * FROM ActivityLog ORDER BY LogTime DESC LIMIT " +
                       std::to_string(limit);
    
    if (!executeQuery(query)) return logs;
    
    MYSQL_RES* result = mysql_store_result(connection);
    if (result == nullptr) return logs;
    
    MYSQL_ROW row;
    while ((row = mysql_fetch_row(result)) != nullptr) {
        ActivityLogEntry log;
        log.logID = std::stoi(row[0]);
        log.userType = row[1] ? row[1] : "";
        log.userID = row[2] ? std::stoi(row[2]) : 0;
        log.action = row[3] ? row[3] : "";
        log.details = row[4] ? row[4] : "";
        log.logTime = row[5] ? row[5] : "";
        logs.push_back(log);
    }
    
    mysql_free_result(result);
    return logs;
}

int DatabaseManager::getPendingDoctorsCount() {
    std::string query = "SELECT COUNT(*) FROM Doctors WHERE IsApproved = FALSE";
    if (!executeQuery(query)) return 0;
    
    MYSQL_RES* result = mysql_store_result(connection);
    if (result == nullptr) return 0;
    
    MYSQL_ROW row = mysql_fetch_row(result);
    int count = (row != nullptr) ? std::stoi(row[0]) : 0;
    
    mysql_free_result(result);
    return count;
}

int DatabaseManager::getPendingStaffCount() {
    std::string query = "SELECT COUNT(*) FROM Staff WHERE IsApproved = FALSE";
    if (!executeQuery(query)) return 0;
    
    MYSQL_RES* result = mysql_store_result(connection);
    if (result == nullptr) return 0;
    
    MYSQL_ROW row = mysql_fetch_row(result);
    int count = (row != nullptr) ? std::stoi(row[0]) : 0;
    
    mysql_free_result(result);
    return count;
}

