-- ========================================
-- Hospital Appointment Booking System
-- Database Setup Script (Updated for Grade A)
-- Compatible with XAMPP MySQL/MariaDB
-- ========================================

-- Create database
DROP DATABASE IF EXISTS hospital_appointment_db;
CREATE DATABASE hospital_appointment_db;
USE hospital_appointment_db;

-- ========================================
-- Table: Admin
-- ========================================
CREATE TABLE Admin (
    AdminID INT PRIMARY KEY AUTO_INCREMENT,
    AdminName VARCHAR(50) NOT NULL,
    PhoneNumber VARCHAR(20) NOT NULL UNIQUE,
    Email VARCHAR(30) NOT NULL UNIQUE,
    Password VARCHAR(20) NOT NULL,
    CreatedAt TIMESTAMP DEFAULT CURRENT_TIMESTAMP
) ENGINE=InnoDB;

-- ========================================
-- Table: Doctors
-- ========================================
CREATE TABLE Doctors (
    DoctorID INT PRIMARY KEY AUTO_INCREMENT,
    DoctorName VARCHAR(50) NOT NULL,
    Specialty VARCHAR(20) NOT NULL,
    PhoneNumber VARCHAR(20) NOT NULL UNIQUE,
    Email VARCHAR(30) NOT NULL UNIQUE,
    RoomNo VARCHAR(10) NOT NULL,
    Password VARCHAR(50) NOT NULL DEFAULT 'doc123',
    IsAvailable BOOLEAN DEFAULT TRUE,
    CreatedAt TIMESTAMP DEFAULT CURRENT_TIMESTAMP
) ENGINE=InnoDB;

-- ========================================
-- Table: Staff
-- ========================================
CREATE TABLE Staff (
    StaffID INT PRIMARY KEY AUTO_INCREMENT,
    StaffName VARCHAR(50) NOT NULL,
    PhoneNumber VARCHAR(20) NOT NULL UNIQUE,
    Email VARCHAR(30) NOT NULL UNIQUE,
    Department VARCHAR(50) NOT NULL,
    Password VARCHAR(50) NOT NULL DEFAULT 'staff123',
    AdminID INT,
    IsApproved BOOLEAN DEFAULT FALSE,
    CreatedAt TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY (AdminID) REFERENCES Admin(AdminID) ON DELETE SET NULL
) ENGINE=InnoDB;

-- ========================================
-- Table: Patient
-- ========================================
CREATE TABLE Patient (
    PatientID INT PRIMARY KEY AUTO_INCREMENT,
    PatientName VARCHAR(50) NOT NULL,
    PhoneNumber VARCHAR(20) NOT NULL UNIQUE,
    Email VARCHAR(30) NOT NULL UNIQUE,
    Address VARCHAR(100) NOT NULL,
    DOB DATE NOT NULL,
    Gender VARCHAR(10) NOT NULL CHECK (Gender IN ('Male', 'Female')),
    Password VARCHAR(50) NOT NULL,
    CreatedAt TIMESTAMP DEFAULT CURRENT_TIMESTAMP
) ENGINE=InnoDB;

-- ========================================
-- Table: Appointment
-- ========================================
CREATE TABLE Appointment (
    AppointmentID INT PRIMARY KEY AUTO_INCREMENT,
    Status VARCHAR(20) NOT NULL DEFAULT 'Pending',
    AppointmentTime TIME NOT NULL,
    AppointmentDate DATE NOT NULL,
    Reason VARCHAR(100),
    PatientID INT NOT NULL,
    DoctorID INT NOT NULL,
    StaffID INT,
    CreatedAt TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY (PatientID) REFERENCES Patient(PatientID) ON DELETE CASCADE,
    FOREIGN KEY (DoctorID) REFERENCES Doctors(DoctorID) ON DELETE CASCADE,
    FOREIGN KEY (StaffID) REFERENCES Staff(StaffID) ON DELETE SET NULL
) ENGINE=InnoDB;

-- ========================================
-- Table: ActivityLog
-- ========================================
CREATE TABLE ActivityLog (
    LogID INT PRIMARY KEY AUTO_INCREMENT,
    UserType VARCHAR(20) NOT NULL,
    UserID INT NOT NULL,
    Action VARCHAR(100) NOT NULL,
    Details VARCHAR(255),
    LogTime TIMESTAMP DEFAULT CURRENT_TIMESTAMP
) ENGINE=InnoDB;

-- ========================================
-- Insert Sample Data (IDs start from 1)
-- ========================================

-- Insert Admin
INSERT INTO Admin (AdminName, PhoneNumber, Email, Password) VALUES
('System Admin', '0123456789', 'admin@hospital.com', 'admin123');

-- Insert Doctors (IDs: 1-4)
INSERT INTO Doctors (DoctorName, Specialty, PhoneNumber, Email, RoomNo, Password, IsAvailable) VALUES
('Dr. Noor Aini', 'Cardiology', '0111111111', 'noor@hospital.com', 'R101', 'doc123', TRUE),
('Dr. Ahmad Razak', 'Pediatrics', '0122222222', 'ahmad@hospital.com', 'R102', 'doc123', TRUE),
('Dr. Siti Rahmah', 'Dermatology', '0133333333', 'siti@hospital.com', 'R103', 'doc123', TRUE),
('Dr. Lim Wei Keat', 'Orthopedics', '0144444444', 'lim@hospital.com', 'R104', 'doc123', TRUE);

-- Insert Staff (IDs: 1-2)
INSERT INTO Staff (StaffName, PhoneNumber, Email, Department, Password, AdminID, IsApproved) VALUES
('Sarah Ibrahim', '0155555555', 'sarah@hospital.com', 'Reception', 'staff123', 1, TRUE),
('Muthu Kumar', '0166666666', 'muthu@hospital.com', 'Nurse', 'staff123', 1, TRUE);

-- Insert Patients (IDs: 1-5) - Test Data
INSERT INTO Patient (PatientName, PhoneNumber, Email, Address, DOB, Gender, Password) VALUES
('Muhammad Hafiz', '0199999999', 'hafiz@email.com', '789 Jalan Sejahtera, Shah Alam', '2000-03-10', 'Male', 'patient123'),
('ADAM QUSSYAIRI BIN YUSOFF', '0195114255', 'adam@gmail.com', 'No 49 durian tunggal', '2005-09-19', 'Male', 'adam0113'),
('Siti Khadijah', '0188888888', 'khadijah@email.com', '456 Jalan Damai, PJ', '1988-12-20', 'Female', 'patient123'),
('Ahmad Fauzi', '0177777777', 'fauzi@email.com', '123 Jalan Merdeka, KL', '1995-05-15', 'Male', 'patient123'),
('Nurul Aina', '0166666666', 'aina@email.com', '321 Jalan Bahagia, Melaka', '1992-08-25', 'Female', 'patient123');

-- Insert Sample Appointments (for testing)
INSERT INTO Appointment (Status, AppointmentTime, AppointmentDate, Reason, PatientID, DoctorID, StaffID) VALUES
('Confirmed', '10:30:00', CURDATE(), 'Fever and cough', 1, 1, 1),
('Pending', '11:00:00', DATE_ADD(CURDATE(), INTERVAL 1 DAY), 'Regular checkup', 2, 1, NULL),
('Completed', '14:00:00', DATE_SUB(CURDATE(), INTERVAL 1 DAY), 'Skin rash', 3, 3, 1),
('Confirmed', '09:00:00', DATE_ADD(CURDATE(), INTERVAL 2 DAY), 'Back pain', 4, 4, 1),
('Pending', '15:30:00', DATE_ADD(CURDATE(), INTERVAL 3 DAY), 'Follow-up', 5, 2, NULL),
('Cancelled', '10:00:00', DATE_SUB(CURDATE(), INTERVAL 2 DAY), 'Cancelled appointment', 1, 2, 1);

-- Insert Sample Activity Logs
INSERT INTO ActivityLog (UserType, UserID, Action, Details) VALUES
('Admin', 1, 'Login', 'Successful login'),
('Patient', 1, 'Registration', 'New patient registered'),
('Patient', 1, 'Login', 'Successful login'),
('Doctor', 1, 'Login', 'Successful login'),
('Staff', 1, 'Login', 'Successful login'),
('Staff', 1, 'Approve Appointment', 'ID: 1');

-- ========================================
-- Views for Grade A Requirements
-- ========================================

-- View: Daily Appointment Summary (GROUP BY with Aggregation)
CREATE VIEW vw_DailyAppointmentSummary AS
SELECT 
    AppointmentDate,
    COUNT(*) AS TotalAppointments,
    SUM(CASE WHEN Status = 'Confirmed' THEN 1 ELSE 0 END) AS Confirmed,
    SUM(CASE WHEN Status = 'Pending' THEN 1 ELSE 0 END) AS Pending,
    SUM(CASE WHEN Status = 'Completed' THEN 1 ELSE 0 END) AS Completed,
    SUM(CASE WHEN Status = 'Cancelled' THEN 1 ELSE 0 END) AS Cancelled
FROM Appointment
GROUP BY AppointmentDate;

-- View: Doctor Appointment Count (JOIN with GROUP BY)
CREATE VIEW vw_DoctorAppointmentCount AS
SELECT 
    d.DoctorID,
    d.DoctorName,
    d.Specialty,
    COUNT(a.AppointmentID) AS TotalAppointments,
    SUM(CASE WHEN a.Status = 'Confirmed' THEN 1 ELSE 0 END) AS ConfirmedCount,
    AVG(CASE WHEN a.Status = 'Completed' THEN 1 ELSE 0 END) AS CompletionRate
FROM Doctors d
LEFT JOIN Appointment a ON d.DoctorID = a.DoctorID
GROUP BY d.DoctorID, d.DoctorName, d.Specialty;

-- View: Patient Appointment History (JOIN)
CREATE VIEW vw_PatientAppointmentHistory AS
SELECT 
    p.PatientID,
    p.PatientName,
    a.AppointmentID,
    a.AppointmentDate,
    a.AppointmentTime,
    a.Status,
    d.DoctorName,
    d.Specialty
FROM Patient p
JOIN Appointment a ON p.PatientID = a.PatientID
JOIN Doctors d ON a.DoctorID = d.DoctorID;

-- View: Monthly Statistics (Aggregation with Date Functions)
CREATE VIEW vw_MonthlyStatistics AS
SELECT 
    YEAR(AppointmentDate) AS Year,
    MONTH(AppointmentDate) AS Month,
    COUNT(*) AS TotalAppointments,
    SUM(CASE WHEN Status = 'Completed' THEN 1 ELSE 0 END) AS Completed,
    AVG(CASE WHEN Status = 'Completed' THEN 1.0 ELSE 0.0 END) * 100 AS CompletionPercentage
FROM Appointment
GROUP BY YEAR(AppointmentDate), MONTH(AppointmentDate);

SELECT 'Database setup completed successfully! All IDs start from 1.' AS Message;
