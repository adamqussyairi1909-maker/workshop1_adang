# Hospital Appointment Booking System - Flowchart

## Mermaid Diagram Code

```mermaid
flowchart TD
    Start([Start System]) --> DB{Database<br/>Connected?}
    DB -->|No| Error[Show Error<br/>Exit System]
    DB -->|Yes| Menu[Main Menu]
    
    Menu --> Choice{Select Option}
    Choice -->|1| PatientLogin[Login as Patient]
    Choice -->|2| DoctorLogin[Login as Doctor]
    Choice -->|3| StaffLogin[Login as Staff]
    Choice -->|4| AdminLogin[Login as Admin]
    Choice -->|5| Register[Register Patient]
    Choice -->|6| Pointers[Pointer Examples]
    Choice -->|7| Exit([Exit System])
    
    PatientLogin --> Auth1{Valid<br/>Credentials?}
    Auth1 -->|No| Menu
    Auth1 -->|Yes| PatientDash[Patient Dashboard]
    
    DoctorLogin --> Auth2{Valid<br/>Credentials?}
    Auth2 -->|No| Menu
    Auth2 -->|Yes| DoctorDash[Doctor Dashboard]
    
    StaffLogin --> Auth3{Valid<br/>Credentials?}
    Auth3 -->|No| Menu
    Auth3 -->|Yes| StaffDash[Staff Dashboard]
    
    AdminLogin --> Auth4{Valid<br/>Credentials?}
    Auth4 -->|No| Menu
    Auth4 -->|Yes| AdminDash[Admin Dashboard]
    
    Register --> RegProcess[Enter Details<br/>Create Account]
    RegProcess --> Menu
    
    Pointers --> PtrMenu[9 Pointer Examples<br/>Interactive Learning]
    PtrMenu --> Menu
    
    PatientDash --> P1[Book Appointment<br/>View Appointments<br/>Update Profile<br/>Cancel Appointment]
    P1 --> P2{Logout?}
    P2 -->|Yes| Menu
    P2 -->|No| P1
    
    DoctorDash --> D1[View Today's Appointments<br/>View All Appointments<br/>Update Availability<br/>View Patient History]
    D1 --> D2{Logout?}
    D2 -->|Yes| Menu
    D2 -->|No| D1
    
    StaffDash --> S1[View All Appointments<br/>Approve Appointments<br/>View Patient Records]
    S1 --> S2{Logout?}
    S2 -->|Yes| Menu
    S2 -->|No| S1
    
    AdminDash --> A1[Manage Patients<br/>Manage Doctors<br/>Manage Staff<br/>Generate Reports<br/>System Statistics]
    A1 --> A2{Logout?}
    A2 -->|Yes| Menu
    A2 -->|No| A1
    
    style Start fill:#90EE90
    style Exit fill:#FFB6C6
    style Menu fill:#87CEEB
    style PatientDash fill:#FFE4B5
    style DoctorDash fill:#E0BBE4
    style StaffDash fill:#B4E7CE
    style AdminDash fill:#FFA07A
    style Error fill:#FF6B6B
```

---

## Text-Based Flowchart

```
┌─────────────────────────────────────────────────────────────────┐
│                    HOSPITAL APPOINTMENT SYSTEM                  │
└─────────────────────────────────────────────────────────────────┘
                              │
                              ▼
                    ┌──────────────────┐
                    │  Start System    │
                    └────────┬─────────┘
                             │
                             ▼
                    ┌──────────────────┐
                    │  Database        │
                    │  Connection      │
                    └────────┬─────────┘
                             │
                  ┌──────────┴──────────┐
                  │                     │
             [SUCCESS]              [FAILED]
                  │                     │
                  ▼                     ▼
         ┌─────────────────┐    ┌──────────────┐
         │   MAIN MENU     │    │  Show Error  │
         │                 │    │  Exit System │
         └────────┬────────┘    └──────────────┘
                  │
       ┌──────────┼──────────┬──────────┬──────────┬──────────┐
       │          │          │          │          │          │
       ▼          ▼          ▼          ▼          ▼          ▼
   ┌───────┐ ┌───────┐ ┌───────┐ ┌───────┐ ┌────────┐ ┌──────┐
   │Login  │ │Login  │ │Login  │ │Login  │ │Register│ │Pointer│
   │Patient│ │Doctor │ │Staff  │ │Admin  │ │Patient │ │Examples│
   └───┬───┘ └───┬───┘ └───┬───┘ └───┬───┘ └────┬───┘ └───┬──┘
       │         │         │         │          │         │
       ▼         ▼         ▼         ▼          │         │
   [Auth?]   [Auth?]   [Auth?]   [Auth?]       │         │
       │         │         │         │          │         │
       ▼         ▼         ▼         ▼          ▼         ▼
┌──────────┐┌──────────┐┌──────────┐┌──────────┐    [Back to Menu]
│ PATIENT  ││ DOCTOR   ││  STAFF   ││  ADMIN   │
│DASHBOARD ││DASHBOARD ││DASHBOARD ││DASHBOARD │
└────┬─────┘└────┬─────┘└────┬─────┘└────┬─────┘
     │           │           │           │
     ▼           ▼           ▼           ▼
┌──────────┐┌──────────┐┌──────────┐┌──────────┐
│• Book    ││• View    ││• View All││• Manage  │
│  Appoint ││  Today's ││  Appoint ││  Patients│
│• View    ││  Appoint ││• Approve ││• Manage  │
│  Appoint ││• View All││  Appoint ││  Doctors │
│• Update  ││  Appoint ││• View    ││• Manage  │
│  Profile ││• Update  ││  Records ││  Staff   │
│• Cancel  ││  Status  ││• Logout  ││• Generate│
│• Logout  ││• Logout  ││          ││  Reports │
│          ││          ││          ││• Stats   │
└──────────┘└──────────┘└──────────┘│• Logout  │
                                    └──────────┘
```

---

## Simplified Process Flow

```
START
  ↓
CONNECT TO DATABASE
  ↓
MAIN MENU (7 Options)
  ├─→ [1] Patient Login → Patient Dashboard
  │                        ├─ Book Appointment
  │                        ├─ View Appointments  
  │                        ├─ Update Profile
  │                        └─ Logout
  │
  ├─→ [2] Doctor Login → Doctor Dashboard
  │                       ├─ View Today's Appointments
  │                       ├─ View All Appointments
  │                       ├─ Update Availability
  │                       └─ Logout
  │
  ├─→ [3] Staff Login → Staff Dashboard
  │                      ├─ View All Appointments
  │                      ├─ Approve Appointments
  │                      └─ Logout
  │
  ├─→ [4] Admin Login → Admin Dashboard
  │                      ├─ Manage Patients
  │                      ├─ Manage Doctors
  │                      ├─ Manage Staff
  │                      ├─ Generate Reports
  │                      ├─ System Statistics
  │                      └─ Logout
  │
  ├─→ [5] Register Patient → Create Account → Back to Menu
  │
  ├─→ [6] Pointer Examples → 9 Interactive Demos → Back to Menu
  │
  └─→ [7] Exit → END
```



