-- ========================================
-- Hospital Appointment Booking System
-- Database Setup Script
-- Compatible with XAMPP MySQL
-- ========================================

-- Create database
DROP DATABASE IF EXISTS hospital_appointment_db;
CREATE DATABASE hospital_appointment_db;
USE hospital_appointment_db;

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
    Password VARCHAR(50) NOT NULL DEFAULT 'doctor123',
    IsApproved BOOLEAN DEFAULT FALSE,
    CreatedAt TIMESTAMP DEFAULT CURRENT_TIMESTAMP
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
-- Table: Staff
-- ========================================
CREATE TABLE Staff (
    StaffID INT PRIMARY KEY AUTO_INCREMENT,
    StaffName VARCHAR(50) NOT NULL,
    PhoneNumber VARCHAR(20) NOT NULL UNIQUE,
    Email VARCHAR(30) NOT NULL UNIQUE,
    Role VARCHAR(10) NOT NULL,
    Password VARCHAR(50) NOT NULL DEFAULT 'staff123',
    AdminID INT,
    IsApproved BOOLEAN DEFAULT FALSE,
    CreatedAt TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY (AdminID) REFERENCES Admin(AdminID) ON DELETE SET NULL
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
-- Table: Payment
-- ========================================
CREATE TABLE Payment (
    PaymentID INT PRIMARY KEY AUTO_INCREMENT,
    Amount DECIMAL(10,2) NOT NULL CHECK (Amount >= 0),
    PaymentDate DATE NOT NULL,
    PaymentMethod VARCHAR(30) NOT NULL CHECK (PaymentMethod IN ('Cash', 'Card', 'Online')),
    AppointmentID INT NOT NULL,
    CreatedAt TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY (AppointmentID) REFERENCES Appointment(AppointmentID) ON DELETE CASCADE
) ENGINE=InnoDB;

-- ========================================
-- Table: ActivityLog (for Admin monitoring)
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
-- Table: DoctorSchedule (for availability)
-- ========================================
CREATE TABLE DoctorSchedule (
    ScheduleID INT PRIMARY KEY AUTO_INCREMENT,
    DoctorID INT NOT NULL,
    DayOfWeek VARCHAR(10) NOT NULL,
    StartTime TIME NOT NULL,
    EndTime TIME NOT NULL,
    IsAvailable BOOLEAN DEFAULT TRUE,
    FOREIGN KEY (DoctorID) REFERENCES Doctors(DoctorID) ON DELETE CASCADE
) ENGINE=InnoDB;

-- ========================================
-- Insert Sample Data
-- ========================================

-- Insert Admin
INSERT INTO Admin (AdminName, PhoneNumber, Email, Password) VALUES
('System Admin', '0123456789', 'admin@hospital.com', 'admin123');

-- Insert Doctors
INSERT INTO Doctors (DoctorName, Specialty, PhoneNumber, Email, RoomNo, Password, IsApproved) VALUES
('Dr. Noor Aini', 'Cardiology', '0111111111', 'noor@hospital.com', 'R101', 'doc123', TRUE),
('Dr. Ahmad Razak', 'Pediatrics', '0122222222', 'ahmad@hospital.com', 'R102', 'doc123', TRUE),
('Dr. Siti Rahmah', 'Dermatology', '0133333333', 'siti@hospital.com', 'R103', 'doc123', FALSE),
('Dr. Lim Wei Keat', 'Orthopedics', '0144444444', 'lim@hospital.com', 'R104', 'doc123', TRUE);

-- Insert Staff
INSERT INTO Staff (StaffName, PhoneNumber, Email, Role, Password, AdminID, IsApproved) VALUES
('Sarah Ibrahim', '0155555555', 'sarah@hospital.com', 'Reception', 'staff123', 1, TRUE),
('Muthu Kumar', '0166666666', 'muthu@hospital.com', 'Nurse', 'staff123', 1, TRUE);

-- Insert Patients
INSERT INTO Patient (PatientName, PhoneNumber, Email, Address, DOB, Gender, Password) VALUES
('Adam Qussyairi', '0177777777', 'adam@email.com', '123 Jalan Merdeka, KL', '1995-05-15', 'Male', 'patient123'),
('Siti Khadijah', '0188888888', 'khadijah@email.com', '456 Jalan Damai, PJ', '1988-12-20', 'Female', 'patient123'),
('Muhammad Hafiz', '0199999999', 'hafiz@email.com', '789 Jalan Sejahtera, Shah Alam', '2000-03-10', 'Male', 'patient123');

-- Insert Doctor Schedules
INSERT INTO DoctorSchedule (DoctorID, DayOfWeek, StartTime, EndTime, IsAvailable) VALUES
(1, 'Monday', '09:00:00', '17:00:00', TRUE),
(1, 'Tuesday', '09:00:00', '17:00:00', TRUE),
(1, 'Wednesday', '09:00:00', '13:00:00', TRUE),
(2, 'Monday', '08:00:00', '16:00:00', TRUE),
(2, 'Thursday', '08:00:00', '16:00:00', TRUE),
(2, 'Friday', '08:00:00', '12:00:00', TRUE),
(3, 'Tuesday', '10:00:00', '18:00:00', TRUE),
(3, 'Wednesday', '10:00:00', '18:00:00', TRUE),
(4, 'Monday', '09:00:00', '17:00:00', TRUE),
(4, 'Wednesday', '09:00:00', '17:00:00', TRUE),
(4, 'Friday', '09:00:00', '17:00:00', TRUE);

-- Insert Sample Appointments
INSERT INTO Appointment (Status, AppointmentTime, AppointmentDate, Reason, PatientID, DoctorID, StaffID) VALUES
('Confirmed', '10:30:00', '2025-11-05', 'Fever and cough', 1, 1, 1),
('Pending', '11:00:00', '2025-11-05', 'Regular checkup', 3, 1, NULL),
('Completed', '14:00:00', '2025-11-01', 'Skin rash', 2, 3, 1),
('Cancelled', '09:00:00', '2025-10-28', 'Back pain', 1, 4, 1);

-- Insert Sample Payments
INSERT INTO Payment (Amount, PaymentDate, PaymentMethod, AppointmentID) VALUES
(150.00, '2025-11-05', 'Card', 1),
(100.00, '2025-11-01', 'Cash', 3);

-- ========================================
-- Useful Views for Reports
-- ========================================

-- View: Daily Appointment Summary
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

-- View: Doctor Appointment Count
CREATE VIEW vw_DoctorAppointmentCount AS
SELECT 
    d.DoctorID,
    d.DoctorName,
    d.Specialty,
    COUNT(a.AppointmentID) AS TotalAppointments
FROM Doctors d
LEFT JOIN Appointment a ON d.DoctorID = a.DoctorID
GROUP BY d.DoctorID, d.DoctorName, d.Specialty;

-- View: Patient Appointment History
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

SELECT 'Database setup completed successfully!' AS Message;

