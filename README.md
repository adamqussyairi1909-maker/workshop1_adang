# Hospital Appointment Booking System

A comprehensive C++ hospital appointment management system with role-based access control, built with Visual Studio 2022, XAMPP, and MySQL Connector C++.

## 🎯 Features

### ✅ Complete UI/UX Enhancement
- **Clear Step-by-Step Instructions**: Every input has `>>` yellow prompts
- **Progress Indicators**: Multi-step forms show "STEP 1 OF 4" progress
- **Visual Confirmation**: `[OK]` green checkmarks after selections
- **Boxed Headers**: Context boxes at the top of each screen
- **Consistent Formatting**: All inputs follow the same pattern
- **Color-Coded Status**: Green (success), Yellow (warning), Red (error), Cyan (info)

### 🔐 Role-Based Access Control (RBAC)
The system automatically detects user roles after login and provides appropriate dashboards:

#### **Patient Dashboard**
- ✅ Book New Appointment (3-step wizard)
- ✅ View My Appointments (with status legend)
- ✅ Cancel Appointment (with confirmation)
- ✅ Update Personal Details

#### **Doctor Dashboard**
- ✅ Today's Appointments
- ✅ All Appointments
- ✅ Complete Appointment (mark as done)
- ✅ View Patient Info
- ✅ Update Availability Status

#### **Staff Dashboard**
- ✅ View All Appointments
- ✅ Approve Appointments (process pending requests)
- ✅ Search Patient Records
- ✅ View Doctors List
- ✅ Generate Reports

#### **Admin Dashboard**
- ✅ Manage Patients (view/delete)
- ✅ Manage Doctors (add/edit/delete)
- ✅ Manage Staff (add/delete)
- ✅ System Logs (activity history)
- ✅ Statistics (system overview)

## 🎨 UI/UX Standards

### Input Pattern
```
  >> Enter your choice:
     Your input: _
```

### Multi-Step Forms
```
  ================================================
  STEP 1 OF 4: PERSONAL INFORMATION
  ================================================

  >> Enter your full name:
     Full Name: _
```

### Confirmation Dialogs
```
  +-----------------------------------+
  | Data Review Box                   |
  +-----------------------------------+

  >> Confirm action:
     Type Y to confirm, N to cancel: _
```

### Success/Error Messages
```
  +-----------------------------------------+
  |       OPERATION SUCCESSFUL!             |
  +-----------------------------------------+
```

## 🛡️ Security & Validation

### Input Validation
- ✅ **Email**: Checks for @ and domain (e.g., .com)
- ✅ **Phone**: 10-12 digits only
- ✅ **Date**: YYYY-MM-DD format, prevents past dates
- ✅ **Time**: HH:MM format (24-hour)
- ✅ **Password**: Minimum 6 characters, confirmation required

### Business Rules
- ✅ **Daily Limit**: 1 appointment per doctor per day per patient
- ✅ **Time Slot Check**: Prevents double-booking
- ✅ **Status Workflow**: Pending → Confirmed → Completed
- ✅ **Role Restrictions**: Each user sees only their authorized features

## 📁 Project Structure

```
workshop1_adang/
├── include/               # Header files
│   ├── Models.h          # Data structures
│   ├── ConsoleUtils.h    # UI utilities
│   ├── DatabaseManager.h # Database operations
│   ├── AuthModule.h      # Authentication
│   ├── PatientModule.h   # Patient features
│   ├── DoctorModule.h    # Doctor features
│   ├── StaffModule.h     # Staff features
│   └── AdminModule.h     # Admin features
├── src/                  # Implementation files
│   ├── main.cpp          # Main entry point
│   ├── DatabaseManager.cpp
│   ├── Utilities.cpp
│   ├── AuthModule.cpp
│   ├── PatientModule.cpp
│   ├── DoctorModule.cpp
│   ├── StaffModule.cpp
│   └── AdminModule.cpp
├── database_setup.sql    # Database schema
├── SETUP_GUIDE.txt       # Setup instructions
└── README.md            # This file
```

## 🚀 Setup Instructions

### Prerequisites
- Visual Studio 2022
- XAMPP (MySQL)
- MySQL Connector C++ 9.1.0

### Step 1: Database Setup
1. Start XAMPP MySQL
2. Open phpMyAdmin (http://localhost/phpmyadmin)
3. Import `database_setup.sql`

### Step 2: Configure Visual Studio

#### Include Directories
```
C:\Users\ADAMQUSSYAIRI\Downloads\mysql-connector-c++-9.1.0-winx64 (1)\mysql-connector-c++-9.1.0-winx64\include\jdbc
```

#### Library Directories
```
C:\Users\ADAMQUSSYAIRI\Downloads\mysql-connector-c++-9.1.0-winx64 (1)\mysql-connector-c++-9.1.0-winx64\lib64\vs14
```

#### Additional Dependencies
```
mysqlcppconn.lib
```

### Step 3: Copy Required DLLs
Copy these files to your project's Debug/Release folder:
- `mysqlcppconn-9-vs14.dll`
- `mysqlcppconn.dll`

## 📊 Database Schema

### Tables
- **Patient**: Patient records and credentials
- **Doctors**: Doctor information and availability
- **Staff**: Staff accounts and departments
- **Admin**: Administrator accounts
- **Appointment**: Appointment bookings
- **ActivityLog**: System activity tracking

### Relationships
- Patient ↔ Appointment (1:N)
- Doctor ↔ Appointment (1:N)
- Staff → Appointment (approval tracking)

## 🎯 User Flow Examples

### Patient Journey
1. **Register**: 4-step form with validation
2. **Login**: Automatic role detection
3. **Dashboard**: See patient-specific options
4. **Book Appointment**: 3-step wizard
5. **View Status**: Check appointment status
6. **Cancel if Needed**: Confirmation dialog

### Staff Workflow
1. **Login**: Automatic role detection
2. **Dashboard**: Staff-specific menu
3. **View Pending**: List of appointments awaiting approval
4. **Approve/Reject**: With detailed review
5. **Search Patients**: Quick lookup
6. **Generate Reports**: System statistics

## 🔍 Testing the System

### Test Accounts (After Running database_setup.sql)

**Admin Account:**
- Email: `admin@hospital.com`
- Password: `admin123`

**Create Your Own:**
- Patient: Use "Register as Patient" option
- Staff/Doctor: Admin must create via dashboard

## 📝 Key Improvements Made

### ✅ UI/UX Enhancements
- Clear welcome boxes on every screen
- Step-by-step progress indicators
- Yellow `>>` arrows for all input prompts
- Consistent spacing and formatting
- Visual confirmations and warnings
- Status legends and help text

### ✅ HCI Elements
- **Color Coding**: Consistent color scheme throughout
- **Loading Animations**: "Verifying credentials..." feedback
- **Password Masking**: Secure password entry
- **Input Validation**: Real-time validation with error messages
- **Confirmation Dialogs**: Prevent accidental actions
- **Clear Navigation**: Always show how to go back

### ✅ Error Handling
- Database connection failures
- Invalid input formats
- Duplicate bookings
- Authorization checks
- Null/empty input handling

### ✅ Role-Based Security
- Automatic role detection after login
- Dashboard content filtered by role
- Function-level access control
- Activity logging for auditing

## 🐛 Known Issues & Solutions

### Issue: "Cannot open input file 'libmysql.lib'"
**Solution**: Change to `mysqlcppconn.lib` in Linker settings

### Issue: ASCII art corruption
**Solution**: Already fixed with simple ASCII characters

### Issue: User doesn't know what to type
**Solution**: All inputs now have `>>` yellow prompts

## 📈 Future Enhancements
- [ ] Email notifications
- [ ] Payment processing
- [ ] Medical records management
- [ ] Prescription system
- [ ] Report generation (PDF)

## 👨‍💻 Developer

**GitHub Repository**: [https://github.com/adamqussyairi1909-maker/workshop1_adang](https://github.com/adamqussyairi1909-maker/workshop1_adang)

## 📄 License

Educational project for workshop purposes.

---

**Last Updated**: November 30, 2025  
**Version**: 1.0  
**Status**: Production Ready ✅



