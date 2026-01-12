// ============================================================
// PointerExamples.h - Pointer Demonstrations
// Hospital Appointment Booking System - Workshop Examples
// ============================================================

#ifndef POINTER_EXAMPLES_H
#define POINTER_EXAMPLES_H

#include <iostream>
#include <string>
#include <vector>
#include "Models.h"
#include "ConsoleUtils.h"

class PointerExamples {
private:
    ConsoleUtils& console;

public:
    PointerExamples(ConsoleUtils& consoleRef) : console(consoleRef) {}

    // ============================================================
    // EXAMPLE 1: Basic Pointers
    // ============================================================
    void demonstrateBasicPointers() {
        console.showHeader("BASIC POINTERS DEMONSTRATION");
        
        // Regular variable
        int patientCount = 5;
        std::cout << "  Patient Count: " << patientCount << std::endl;
        std::cout << "  Address of patientCount: " << &patientCount << std::endl;
        
        // Pointer to the variable
        int* ptr = &patientCount;  // ptr stores the address of patientCount
        std::cout << "\n  Pointer ptr points to: " << ptr << std::endl;
        std::cout << "  Value at pointer: " << *ptr << std::endl;  // Dereference
        
        // Modify through pointer
        *ptr = 10;
        std::cout << "\n  After *ptr = 10:" << std::endl;
        std::cout << "  patientCount: " << patientCount << std::endl;
        std::cout << "  *ptr: " << *ptr << std::endl;
        
        console.pause();
    }

    // ============================================================
    // EXAMPLE 2: Dynamic Memory Allocation
    // ============================================================
    void demonstrateDynamicAllocation() {
        console.showHeader("DYNAMIC MEMORY ALLOCATION");
        
        // Allocate single integer
        int* age = new int;
        *age = 25;
        std::cout << "  Patient Age (dynamic): " << *age << std::endl;
        std::cout << "  Address: " << age << std::endl;
        delete age;  // Free memory
        std::cout << "  Memory freed!" << std::endl;
        
        // Allocate array dynamically
        std::cout << "\n  Creating dynamic array of appointments..." << std::endl;
        int size = 5;
        int* appointmentIDs = new int[size];
        
        for (int i = 0; i < size; i++) {
            appointmentIDs[i] = 1000 + i;
            std::cout << "  Appointment[" << i << "] = " << appointmentIDs[i] << std::endl;
        }
        
        delete[] appointmentIDs;  // Free array memory
        std::cout << "  Array memory freed!" << std::endl;
        
        console.pause();
    }

    // ============================================================
    // EXAMPLE 3: Pointers with Structures
    // ============================================================
    void demonstrateStructurePointers() {
        console.showHeader("POINTERS WITH STRUCTURES");
        
        // Create patient on stack
        Patient patient1;
        patient1.patientID = 101;
        patient1.patientName = "John Doe";
        patient1.email = "john@example.com";
        
        // Pointer to structure
        Patient* patientPtr = &patient1;
        
        std::cout << "  Using dot operator:" << std::endl;
        std::cout << "  patient1.patientID: " << patient1.patientID << std::endl;
        std::cout << "  patient1.patientName: " << patient1.patientName << std::endl;
        
        std::cout << "\n  Using arrow operator (->):" << std::endl;
        std::cout << "  patientPtr->patientID: " << patientPtr->patientID << std::endl;
        std::cout << "  patientPtr->patientName: " << patientPtr->patientName << std::endl;
        
        // Dynamic structure allocation
        std::cout << "\n  Creating patient dynamically..." << std::endl;
        Patient* dynamicPatient = new Patient();
        dynamicPatient->patientID = 102;
        dynamicPatient->patientName = "Jane Smith";
        dynamicPatient->email = "jane@example.com";
        
        std::cout << "  Dynamic Patient ID: " << dynamicPatient->patientID << std::endl;
        std::cout << "  Dynamic Patient Name: " << dynamicPatient->patientName << std::endl;
        
        delete dynamicPatient;
        std::cout << "  Dynamic patient deleted!" << std::endl;
        
        console.pause();
    }

    // ============================================================
    // EXAMPLE 4: Array of Structure Pointers
    // ============================================================
    void demonstrateArrayOfPointers() {
        console.showHeader("ARRAY OF STRUCTURE POINTERS");
        
        const int MAX_DOCTORS = 3;
        Doctor* doctors[MAX_DOCTORS];  // Array of pointers
        
        std::cout << "  Creating " << MAX_DOCTORS << " doctors dynamically..." << std::endl;
        
        // Allocate each doctor
        doctors[0] = new Doctor();
        doctors[0]->doctorID = 1;
        doctors[0]->doctorName = "Dr. Ahmad";
        doctors[0]->specialty = "Cardiology";
        
        doctors[1] = new Doctor();
        doctors[1]->doctorID = 2;
        doctors[1]->doctorName = "Dr. Fatimah";
        doctors[1]->specialty = "Pediatrics";
        
        doctors[2] = new Doctor();
        doctors[2]->doctorID = 3;
        doctors[2]->doctorName = "Dr. Kumar";
        doctors[2]->specialty = "Orthopedics";
        
        // Display doctors
        std::cout << "\n  Doctor List:" << std::endl;
        std::cout << "  " << std::string(60, '-') << std::endl;
        for (int i = 0; i < MAX_DOCTORS; i++) {
            std::cout << "  ID: " << doctors[i]->doctorID 
                      << " | Name: " << doctors[i]->doctorName 
                      << " | Specialty: " << doctors[i]->specialty << std::endl;
        }
        
        // Clean up
        for (int i = 0; i < MAX_DOCTORS; i++) {
            delete doctors[i];
        }
        std::cout << "\n  All doctors deleted!" << std::endl;
        
        console.pause();
    }

    // ============================================================
    // EXAMPLE 5: Pointer to Pointer (Double Pointer)
    // ============================================================
    void demonstrateDoublePointer() {
        console.showHeader("DOUBLE POINTER DEMONSTRATION");
        
        int appointmentID = 1001;
        int* ptr1 = &appointmentID;        // Pointer to int
        int** ptr2 = &ptr1;                // Pointer to pointer
        
        std::cout << "  appointmentID: " << appointmentID << std::endl;
        std::cout << "  Address of appointmentID: " << &appointmentID << std::endl;
        
        std::cout << "\n  Single Pointer (ptr1):" << std::endl;
        std::cout << "  ptr1 points to: " << ptr1 << std::endl;
        std::cout << "  *ptr1 = " << *ptr1 << std::endl;
        
        std::cout << "\n  Double Pointer (ptr2):" << std::endl;
        std::cout << "  ptr2 points to: " << ptr2 << std::endl;
        std::cout << "  *ptr2 (address in ptr1): " << *ptr2 << std::endl;
        std::cout << "  **ptr2 (value): " << **ptr2 << std::endl;
        
        // Modify through double pointer
        **ptr2 = 2002;
        std::cout << "\n  After **ptr2 = 2002:" << std::endl;
        std::cout << "  appointmentID: " << appointmentID << std::endl;
        
        console.pause();
    }

    // ============================================================
    // EXAMPLE 6: Function with Pointer Parameters
    // ============================================================
    void swapIntegers(int* a, int* b) {
        int temp = *a;
        *a = *b;
        *b = temp;
    }
    
    void updatePatientInfo(Patient* patient, const std::string& newName) {
        if (patient != nullptr) {
            patient->patientName = newName;
        }
    }
    
    void demonstratePointerParameters() {
        console.showHeader("POINTER PARAMETERS IN FUNCTIONS");
        
        // Swap example
        int patientID1 = 101;
        int patientID2 = 202;
        
        std::cout << "  Before swap:" << std::endl;
        std::cout << "  Patient ID 1: " << patientID1 << std::endl;
        std::cout << "  Patient ID 2: " << patientID2 << std::endl;
        
        swapIntegers(&patientID1, &patientID2);
        
        std::cout << "\n  After swap:" << std::endl;
        std::cout << "  Patient ID 1: " << patientID1 << std::endl;
        std::cout << "  Patient ID 2: " << patientID2 << std::endl;
        
        // Update structure example
        Patient patient;
        patient.patientID = 303;
        patient.patientName = "Original Name";
        
        std::cout << "\n  Before update: " << patient.patientName << std::endl;
        updatePatientInfo(&patient, "Updated Name");
        std::cout << "  After update: " << patient.patientName << std::endl;
        
        console.pause();
    }

    // ============================================================
    // EXAMPLE 7: Pointer Arithmetic
    // ============================================================
    void demonstratePointerArithmetic() {
        console.showHeader("POINTER ARITHMETIC");
        
        int appointmentSlots[] = {9, 10, 11, 14, 15};
        int* ptr = appointmentSlots;
        
        std::cout << "  Appointment Time Slots (using pointer arithmetic):" << std::endl;
        std::cout << "  " << std::string(40, '-') << std::endl;
        
        for (int i = 0; i < 5; i++) {
            std::cout << "  Slot " << i << ": " << *(ptr + i) << ":00" << std::endl;
        }
        
        std::cout << "\n  Moving pointer forward:" << std::endl;
        std::cout << "  ptr points to: " << *ptr << std::endl;
        ptr++;
        std::cout << "  After ptr++: " << *ptr << std::endl;
        ptr += 2;
        std::cout << "  After ptr += 2: " << *ptr << std::endl;
        
        console.pause();
    }

    // ============================================================
    // EXAMPLE 8: Returning Pointers from Functions
    // ============================================================
    Patient* createPatient(int id, const std::string& name) {
        Patient* newPatient = new Patient();
        newPatient->patientID = id;
        newPatient->patientName = name;
        return newPatient;
    }
    
    void demonstrateReturningPointers() {
        console.showHeader("RETURNING POINTERS FROM FUNCTIONS");
        
        std::cout << "  Creating patient using function that returns pointer..." << std::endl;
        Patient* patient = createPatient(999, "Ali bin Ahmad");
        
        if (patient != nullptr) {
            std::cout << "\n  Patient Created:" << std::endl;
            std::cout << "  ID: " << patient->patientID << std::endl;
            std::cout << "  Name: " << patient->patientName << std::endl;
            
            delete patient;
            std::cout << "\n  Patient deleted!" << std::endl;
        }
        
        console.pause();
    }

    // ============================================================
    // EXAMPLE 9: Null Pointer Safety
    // ============================================================
    void demonstrateNullPointer() {
        console.showHeader("NULL POINTER SAFETY");
        
        Doctor* doctor = nullptr;
        
        std::cout << "  Checking null pointer before use..." << std::endl;
        
        if (doctor == nullptr) {
            std::cout << "  doctor is NULL - cannot access!" << std::endl;
        }
        
        // Safe allocation
        doctor = new Doctor();
        doctor->doctorID = 100;
        doctor->doctorName = "Dr. Siti";
        
        if (doctor != nullptr) {
            std::cout << "\n  doctor is now valid:" << std::endl;
            std::cout << "  ID: " << doctor->doctorID << std::endl;
            std::cout << "  Name: " << doctor->doctorName << std::endl;
        }
        
        delete doctor;
        doctor = nullptr;  // Good practice: set to nullptr after delete
        
        std::cout << "\n  After delete and setting to nullptr:" << std::endl;
        std::cout << "  doctor == nullptr: " << (doctor == nullptr ? "true" : "false") << std::endl;
        
        console.pause();
    }

    // ============================================================
    // Main Menu for Pointer Examples
    // ============================================================
    void showExamplesMenu() {
        while (true) {
            console.showHeader("POINTER EXAMPLES - C++ WORKSHOP");
            
            console.printMenuOption(1, "Basic Pointers");
            console.printMenuOption(2, "Dynamic Memory Allocation");
            console.printMenuOption(3, "Pointers with Structures");
            console.printMenuOption(4, "Array of Pointers");
            console.printMenuOption(5, "Double Pointer (Pointer to Pointer)");
            console.printMenuOption(6, "Pointer Parameters in Functions");
            console.printMenuOption(7, "Pointer Arithmetic");
            console.printMenuOption(8, "Returning Pointers from Functions");
            console.printMenuOption(9, "Null Pointer Safety");
            console.printMenuOption(0, "Back to Main Menu");
            
            int choice = console.getIntInput("Enter choice", 0, 9);
            
            switch (choice) {
                case 1: demonstrateBasicPointers(); break;
                case 2: demonstrateDynamicAllocation(); break;
                case 3: demonstrateStructurePointers(); break;
                case 4: demonstrateArrayOfPointers(); break;
                case 5: demonstrateDoublePointer(); break;
                case 6: demonstratePointerParameters(); break;
                case 7: demonstratePointerArithmetic(); break;
                case 8: demonstrateReturningPointers(); break;
                case 9: demonstrateNullPointer(); break;
                case 0: return;
                default: console.printError("Invalid choice!");
            }
        }
    }
};

#endif // POINTER_EXAMPLES_H

