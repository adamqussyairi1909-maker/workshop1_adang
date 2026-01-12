# 📊 DATA DICTIONARY
## Hospital Appointment Booking System

---

## 📋 TABLE: Admin
**Description:** Stores administrator information for system management

| Column Name  | Data Type    | Constraints                    | Description                          |
|--------------|--------------|--------------------------------|--------------------------------------|
| AdminID      | INT          | PRIMARY KEY, AUTO_INCREMENT    | Unique identifier for admin          |
| AdminName    | VARCHAR(50)  | NOT NULL                       | Full name of administrator           |
| PhoneNumber  | VARCHAR(20)  | NOT NULL, UNIQUE               | Contact phone number                 |
| Email        | VARCHAR(30)  | NOT NULL, UNIQUE               | Email address for login              |
| Password     | VARCHAR(20)  | NOT NULL                       | Password for authentication          |
| CreatedAt    | TIMESTAMP    | DEFAULT CURRENT_TIMESTAMP      | Account creation timestamp           |

---

## 👨‍⚕️ TABLE: Doctors
**Description:** Stores doctor/physician information and availability status

| Column Name  | Data Type    | Constraints                    | Description                          |
|--------------|--------------|--------------------------------|--------------------------------------|
| DoctorID     | INT          | PRIMARY KEY, AUTO_INCREMENT    | Unique identifier for doctor         |
| DoctorName   | VARCHAR(50)  | NOT NULL                       | Full name of doctor                  |
| Specialty    | VARCHAR(20)  | NOT NULL                       | Medical specialty area               |
| PhoneNumber  | VARCHAR(20)  | NOT NULL, UNIQUE               | Contact phone number                 |
| Email        | VARCHAR(30)  | NOT NULL, UNIQUE               | Email address for login              |
| RoomNo       | VARCHAR(10)  | NOT NULL                       | Consultation room number             |
| Password     | VARCHAR(50)  | NOT NULL, DEFAULT 'doc123'     | Password for authentication          |
| IsAvailable  | BOOLEAN      | DEFAULT TRUE                   | Doctor availability status           |
| CreatedAt    | TIMESTAMP    | DEFAULT CURRENT_TIMESTAMP      | Account creation timestamp           |

---

## 👥 TABLE: Staff
**Description:** Stores hospital staff information (nurses, receptionists, etc.)

| Column Name  | Data Type    | Constraints                    | Description                          |
|--------------|--------------|--------------------------------|--------------------------------------|
| StaffID      | INT          | PRIMARY KEY, AUTO_INCREMENT    | Unique identifier for staff          |
| StaffName    | VARCHAR(50)  | NOT NULL                       | Full name of staff member            |
| PhoneNumber  | VARCHAR(20)  | NOT NULL, UNIQUE               | Contact phone number                 |
| Email        | VARCHAR(30)  | NOT NULL, UNIQUE               | Email address for login              |
| Department   | VARCHAR(50)  | NOT NULL                       | Department (Reception, Nurse, etc.)  |
| Password     | VARCHAR(50)  | NOT NULL, DEFAULT 'staff123'   | Password for authentication          |
| AdminID      | INT          | FOREIGN KEY → Admin(AdminID)   | Admin who approved this staff        |
| IsApproved   | BOOLEAN      | DEFAULT FALSE                  | Staff approval status                |
| CreatedAt    | TIMESTAMP    | DEFAULT CURRENT_TIMESTAMP      | Account creation timestamp           |

---

## 🏥 TABLE: Patient
**Description:** Stores patient information and demographics

| Column Name  | Data Type    | Constraints                    | Description                          |
|--------------|--------------|--------------------------------|--------------------------------------|
| PatientID    | INT          | PRIMARY KEY, AUTO_INCREMENT    | Unique identifier for patient        |
| PatientName  | VARCHAR(50)  | NOT NULL                       | Full name of patient                 |
| PhoneNumber  | VARCHAR(20)  | NOT NULL, UNIQUE               | Contact phone number                 |
| Email        | VARCHAR(30)  | NOT NULL, UNIQUE               | Email address for login              |
| Address      | VARCHAR(100) | NOT NULL                       | Residential address                  |
| DOB          | DATE         | NOT NULL                       | Date of birth                        |
| Gender       | VARCHAR(10)  | NOT NULL, CHECK (Male/Female)  | Patient gender                       |
| Password     | VARCHAR(50)  | NOT NULL                       | Password for authentication          |
| CreatedAt    | TIMESTAMP    | DEFAULT CURRENT_TIMESTAMP      | Account creation timestamp           |

---

## 📅 TABLE: Appointment
**Description:** Stores appointment bookings with cost breakdown

| Column Name      | Data Type      | Constraints                       | Description                          |
|------------------|----------------|-----------------------------------|--------------------------------------|
| AppointmentID    | INT            | PRIMARY KEY, AUTO_INCREMENT       | Unique identifier for appointment    |
| Status           | VARCHAR(20)    | NOT NULL, DEFAULT 'Pending'       | Status (Pending/Confirmed/Completed) |
| AppointmentTime  | TIME           | NOT NULL                          | Scheduled time (HH:MM:SS)            |
| AppointmentDate  | DATE           | NOT NULL                          | Scheduled date (YYYY-MM-DD)          |
| Reason           | VARCHAR(100)   | NULL                              | Reason for consultation              |
| Duration         | INT            | NOT NULL, DEFAULT 30              | Consultation duration (minutes)      |
| ConsultationFee  | DECIMAL(10,2)  | NOT NULL, DEFAULT 30.00           | Fee based on duration (RM1/minute)   |
| MedicineFee      | DECIMAL(10,2)  | NOT NULL, DEFAULT 0.00            | Medicine cost based on reason        |
| TotalCost        | DECIMAL(10,2)  | NOT NULL, DEFAULT 30.00           | Total = Consultation + Medicine      |
| PatientID        | INT            | NOT NULL, FK → Patient(PatientID) | Patient who booked                   |
| DoctorID         | INT            | NOT NULL, FK → Doctors(DoctorID)  | Assigned doctor                      |
| StaffID          | INT            | NULL, FK → Staff(StaffID)         | Staff who approved (if any)          |
| CreatedAt        | TIMESTAMP      | DEFAULT CURRENT_TIMESTAMP         | Booking creation timestamp           |

---

## 📝 TABLE: ActivityLog
**Description:** Tracks all user activities in the system for audit trail

| Column Name  | Data Type     | Constraints                    | Description                          |
|--------------|---------------|--------------------------------|--------------------------------------|
| LogID        | INT           | PRIMARY KEY, AUTO_INCREMENT    | Unique identifier for log entry      |
| UserType     | VARCHAR(20)   | NOT NULL                       | Type (Admin/Doctor/Staff/Patient)    |
| UserID       | INT           | NOT NULL                       | ID of user who performed action      |
| Action       | VARCHAR(100)  | NOT NULL                       | Action performed (Login, Book, etc.) |
| Details      | VARCHAR(255)  | NULL                           | Additional details about action      |
| LogTime      | TIMESTAMP     | DEFAULT CURRENT_TIMESTAMP      | When action occurred                 |

---

## 📊 DATABASE VIEWS

### 1️⃣ vw_DailyAppointmentSummary
**Purpose:** Daily appointment statistics grouped by date
- **Columns:** AppointmentDate, TotalAppointments, Confirmed, Pending, Completed, Cancelled

### 2️⃣ vw_DoctorAppointmentCount
**Purpose:** Doctor workload with appointment counts and completion rates
- **Columns:** DoctorID, DoctorName, Specialty, TotalAppointments, ConfirmedCount, CompletionRate

### 3️⃣ vw_PatientAppointmentHistory
**Purpose:** Complete patient appointment history with doctor details
- **Columns:** PatientID, PatientName, AppointmentID, AppointmentDate, AppointmentTime, Status, DoctorName, Specialty

### 4️⃣ vw_MonthlyStatistics
**Purpose:** Monthly aggregated statistics with completion percentages
- **Columns:** Year, Month, TotalAppointments, Completed, CompletionPercentage

---

## 🔗 RELATIONSHIPS

```
Admin (1) ──────→ (0..N) Staff
   ↓                        ↓
   └─────────────────────────┘
                ↓
            Approves

Patient (1) ──────→ (0..N) Appointment
Doctor (1)  ──────→ (0..N) Appointment
Staff (1)   ──────→ (0..N) Appointment (approval)

All Users ──────→ ActivityLog (audit trail)
```

---

## 💰 COST CALCULATION FORMULA

```
ConsultationFee = Duration (minutes) × RM1.00
MedicineFee = Based on appointment reason (predefined rates)
TotalCost = ConsultationFee + MedicineFee
```

**Medicine Fee Table:**
- Fever/Cough/Cold: RM25.00
- Skin Rash/Dermatology: RM35.00
- Back/Joint Pain: RM40.00
- Checkup/Follow-up: RM0.00

---

## 🔐 CONSTRAINTS SUMMARY

| Constraint Type | Tables             | Purpose                          |
|-----------------|--------------------|---------------------------------|
| PRIMARY KEY     | All tables         | Unique identification            |
| FOREIGN KEY     | Staff, Appointment | Referential integrity            |
| UNIQUE          | All users (Email)  | Prevent duplicate accounts       |
| NOT NULL        | Critical fields    | Data completeness                |
| CHECK           | Patient (Gender)   | Valid values only                |
| DEFAULT         | Timestamps         | Auto-populate creation time      |

---

**Generated:** 2026-01-12  
**Database Engine:** InnoDB (MySQL/MariaDB)  
**Total Tables:** 6  
**Total Views:** 4  
**Character Set:** UTF-8


