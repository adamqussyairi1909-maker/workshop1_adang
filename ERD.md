# 🗂️ ENTITY RELATIONSHIP DIAGRAM (ERD)
## Hospital Appointment Booking System

---

## 📊 MERMAID ERD (Copy to https://mermaid.live)

```mermaid
erDiagram
    Admin ||--o{ Staff : approves
    Patient ||--o{ Appointment : books
    Doctors ||--o{ Appointment : attends
    Staff ||--o{ Appointment : approves
    
    Admin {
        INT AdminID PK
        VARCHAR AdminName
        VARCHAR PhoneNumber UK
        VARCHAR Email UK
        VARCHAR Password
        TIMESTAMP CreatedAt
    }
    
    Doctors {
        INT DoctorID PK
        VARCHAR DoctorName
        VARCHAR Specialty
        VARCHAR PhoneNumber UK
        VARCHAR Email UK
        VARCHAR RoomNo
        VARCHAR Password
        BOOLEAN IsAvailable
        TIMESTAMP CreatedAt
    }
    
    Staff {
        INT StaffID PK
        VARCHAR StaffName
        VARCHAR PhoneNumber UK
        VARCHAR Email UK
        VARCHAR Department
        VARCHAR Password
        INT AdminID FK
        BOOLEAN IsApproved
        TIMESTAMP CreatedAt
    }
    
    Patient {
        INT PatientID PK
        VARCHAR PatientName
        VARCHAR PhoneNumber UK
        VARCHAR Email UK
        VARCHAR Address
        DATE DOB
        VARCHAR Gender
        VARCHAR Password
        TIMESTAMP CreatedAt
    }
    
    Appointment {
        INT AppointmentID PK
        VARCHAR Status
        TIME AppointmentTime
        DATE AppointmentDate
        VARCHAR Reason
        INT Duration
        DECIMAL ConsultationFee
        DECIMAL MedicineFee
        DECIMAL TotalCost
        INT PatientID FK
        INT DoctorID FK
        INT StaffID FK
        TIMESTAMP CreatedAt
    }
    
    ActivityLog {
        INT LogID PK
        VARCHAR UserType
        INT UserID
        VARCHAR Action
        VARCHAR Details
        TIMESTAMP LogTime
    }
```

---

## 🔗 PLANTUML ERD (Copy to https://plantuml.com)

```plantuml
@startuml Hospital_ERD

' Define entities with attributes
entity "Admin" as admin {
  * AdminID : INT <<PK>>
  --
  AdminName : VARCHAR(50)
  PhoneNumber : VARCHAR(20) <<UK>>
  Email : VARCHAR(30) <<UK>>
  Password : VARCHAR(20)
  CreatedAt : TIMESTAMP
}

entity "Doctors" as doctors {
  * DoctorID : INT <<PK>>
  --
  DoctorName : VARCHAR(50)
  Specialty : VARCHAR(20)
  PhoneNumber : VARCHAR(20) <<UK>>
  Email : VARCHAR(30) <<UK>>
  RoomNo : VARCHAR(10)
  Password : VARCHAR(50)
  IsAvailable : BOOLEAN
  CreatedAt : TIMESTAMP
}

entity "Staff" as staff {
  * StaffID : INT <<PK>>
  --
  StaffName : VARCHAR(50)
  PhoneNumber : VARCHAR(20) <<UK>>
  Email : VARCHAR(30) <<UK>>
  Department : VARCHAR(50)
  Password : VARCHAR(50)
  AdminID : INT <<FK>>
  IsApproved : BOOLEAN
  CreatedAt : TIMESTAMP
}

entity "Patient" as patient {
  * PatientID : INT <<PK>>
  --
  PatientName : VARCHAR(50)
  PhoneNumber : VARCHAR(20) <<UK>>
  Email : VARCHAR(30) <<UK>>
  Address : VARCHAR(100)
  DOB : DATE
  Gender : VARCHAR(10)
  Password : VARCHAR(50)
  CreatedAt : TIMESTAMP
}

entity "Appointment" as appointment {
  * AppointmentID : INT <<PK>>
  --
  Status : VARCHAR(20)
  AppointmentTime : TIME
  AppointmentDate : DATE
  Reason : VARCHAR(100)
  Duration : INT
  ConsultationFee : DECIMAL(10,2)
  MedicineFee : DECIMAL(10,2)
  TotalCost : DECIMAL(10,2)
  PatientID : INT <<FK>>
  DoctorID : INT <<FK>>
  StaffID : INT <<FK>>
  CreatedAt : TIMESTAMP
}

entity "ActivityLog" as log {
  * LogID : INT <<PK>>
  --
  UserType : VARCHAR(20)
  UserID : INT
  Action : VARCHAR(100)
  Details : VARCHAR(255)
  LogTime : TIMESTAMP
}

' Define relationships
admin ||--o{ staff : "approves"
patient ||--o{ appointment : "books"
doctors ||--o{ appointment : "attends"
staff ||--o{ appointment : "approves"

@enduml
```

---

## 🎨 ASCII ART ERD (Horizontal Layout)

```
┌─────────────────────┐
│      ADMIN          │
├─────────────────────┤
│ PK: AdminID         │
│     AdminName       │
│ UK: PhoneNumber     │
│ UK: Email           │
│     Password        │
│     CreatedAt       │
└──────────┬──────────┘
           │ 1
           │ approves
           │ N
           ▼
┌─────────────────────┐          ┌─────────────────────┐          ┌─────────────────────┐
│      STAFF          │          │      PATIENT        │          │      DOCTORS        │
├─────────────────────┤          ├─────────────────────┤          ├─────────────────────┤
│ PK: StaffID         │          │ PK: PatientID       │          │ PK: DoctorID        │
│     StaffName       │          │     PatientName     │          │     DoctorName      │
│ UK: PhoneNumber     │          │ UK: PhoneNumber     │          │     Specialty       │
│ UK: Email           │          │ UK: Email           │          │ UK: PhoneNumber     │
│     Department      │          │     Address         │          │ UK: Email           │
│     Password        │          │     DOB             │          │     RoomNo          │
│ FK: AdminID         │          │     Gender          │          │     Password        │
│     IsApproved      │          │     Password        │          │     IsAvailable     │
│     CreatedAt       │          │     CreatedAt       │          │     CreatedAt       │
└──────────┬──────────┘          └──────────┬──────────┘          └──────────┬──────────┘
           │ N                              │ 1                              │ 1
           │ approves                       │ books                          │ attends
           │                                │                                │
           │                                ▼                                ▼
           └─────────────────────►  ┌─────────────────────┐  ◄──────────────┘
                                    │    APPOINTMENT      │
                                    ├─────────────────────┤
                                    │ PK: AppointmentID   │
                                    │     Status          │
                                    │     AppointmentTime │
                                    │     AppointmentDate │
                                    │     Reason          │
                                    │     Duration        │
                                    │     ConsultationFee │
                                    │     MedicineFee     │
                                    │     TotalCost       │
                                    │ FK: PatientID       │
                                    │ FK: DoctorID        │
                                    │ FK: StaffID         │
                                    │     CreatedAt       │
                                    └─────────────────────┘

                                    ┌─────────────────────┐
                                    │   ACTIVITYLOG       │
                                    ├─────────────────────┤
                                    │ PK: LogID           │
                                    │     UserType        │
                                    │     UserID          │
                                    │     Action          │
                                    │     Details         │
                                    │     LogTime         │
                                    └─────────────────────┘
                                    (Tracks all user actions)
```

---

## 📋 DETAILED RELATIONSHIP TABLE

| Relationship | From Entity | To Entity   | Cardinality | Description                           |
|--------------|-------------|-------------|-------------|---------------------------------------|
| Approves     | Admin       | Staff       | 1:N         | One admin approves many staff         |
| Books        | Patient     | Appointment | 1:N         | One patient books many appointments   |
| Attends      | Doctors     | Appointment | 1:N         | One doctor attends many appointments  |
| Approves     | Staff       | Appointment | 1:N         | One staff approves many appointments  |
| Logs         | All Users   | ActivityLog | N:N         | All users generate activity logs      |

---

## 🔑 KEY LEGEND

- **PK** = Primary Key (Unique identifier)
- **FK** = Foreign Key (References another table)
- **UK** = Unique Key (Must be unique, no duplicates)
- **1:N** = One-to-Many relationship
- **N:1** = Many-to-One relationship
- **N:N** = Many-to-Many relationship

---

## 💡 BUSINESS RULES

1. **Admin → Staff**
   - One admin can approve multiple staff accounts
   - Staff must be approved by an admin before accessing system
   - If admin is deleted, staff.AdminID becomes NULL

2. **Patient → Appointment**
   - One patient can book multiple appointments
   - Each appointment belongs to exactly one patient
   - If patient is deleted, all their appointments are deleted (CASCADE)

3. **Doctor → Appointment**
   - One doctor can have multiple appointments
   - Each appointment is assigned to exactly one doctor
   - If doctor is deleted, all their appointments are deleted (CASCADE)

4. **Staff → Appointment**
   - One staff can approve multiple appointments
   - Appointments can exist without staff approval (NULL allowed)
   - If staff is deleted, appointment.StaffID becomes NULL

5. **Cost Calculation**
   - ConsultationFee = Duration × RM1.00 per minute
   - MedicineFee = Based on appointment reason (predefined rates)
   - TotalCost = ConsultationFee + MedicineFee

---

## 📊 CARDINALITY SUMMARY

```
Admin (1) ────────── approves ──────────► Staff (Many)
                                            │
                                            │ N:1
                                            │
Patient (1) ──────── books ────────────► Appointment (Many)
                                            ▲
                                            │ N:1
                                            │
Doctor (1) ───────── attends ──────────────┘
                                            ▲
                                            │ N:1 (optional)
                                            │
Staff (1) ────────── approves ──────────────┘

All Users ─────────── logs ──────────────► ActivityLog (Many)
```

---

## 🎯 NORMALIZATION LEVEL

**✅ Third Normal Form (3NF)**

1. **1NF:** All tables have atomic values (no repeating groups)
2. **2NF:** All non-key attributes depend on entire primary key
3. **3NF:** No transitive dependencies (no non-key depends on another non-key)

**Integrity Features:**
- Referential integrity via Foreign Keys
- Entity integrity via Primary Keys
- Domain integrity via CHECK constraints
- User-defined integrity via UNIQUE constraints

---

**Generated:** 2026-01-12  
**Database:** MySQL/MariaDB (InnoDB)  
**Total Entities:** 6  
**Total Relationships:** 4  
**Normalization:** 3NF


