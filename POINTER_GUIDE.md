# C++ Pointer Guide - Workshop Documentation
## Hospital Appointment Booking System

---

## Table of Contents
1. [Introduction to Pointers](#1-introduction-to-pointers)
2. [Basic Pointer Syntax](#2-basic-pointer-syntax)
3. [Dynamic Memory Allocation](#3-dynamic-memory-allocation)
4. [Pointers with Structures](#4-pointers-with-structures)
5. [Array of Pointers](#5-array-of-pointers)
6. [Double Pointers](#6-double-pointers)
7. [Pointer Parameters](#7-pointer-parameters)
8. [Pointer Arithmetic](#8-pointer-arithmetic)
9. [Best Practices](#9-best-practices)

---

## 1. Introduction to Pointers

### What is a Pointer?
A pointer is a variable that stores the memory address of another variable.

### Why Use Pointers?
- **Dynamic memory allocation**: Create variables at runtime
- **Efficient parameter passing**: Pass large structures by reference
- **Data structures**: Build linked lists, trees, graphs
- **Memory management**: Control when and how memory is allocated/freed

---

## 2. Basic Pointer Syntax

### Declaration and Initialization

```cpp
// Declare a regular variable
int patientID = 101;

// Declare a pointer
int* ptr;

// Initialize pointer with address of variable
ptr = &patientID;  // & = "address of" operator

// Access value through pointer
int value = *ptr;  // * = "dereference" operator
```

### Key Operators
- `&` - **Address-of operator**: Gets memory address of a variable
- `*` - **Dereference operator**: Accesses value at a memory address

### Example in Our System

```cpp
void example1() {
    // Patient appointment count
    int appointmentCount = 5;
    
    std::cout << "Value: " << appointmentCount << std::endl;
    std::cout << "Address: " << &appointmentCount << std::endl;
    
    // Create pointer to appointmentCount
    int* ptr = &appointmentCount;
    
    std::cout << "Pointer stores: " << ptr << std::endl;        // Address
    std::cout << "Value at pointer: " << *ptr << std::endl;     // Value (5)
    
    // Modify through pointer
    *ptr = 10;
    std::cout << "New value: " << appointmentCount << std::endl; // 10
}
```

**Output:**
```
Value: 5
Address: 0x7ffeeb9cc4ac
Pointer stores: 0x7ffeeb9cc4ac
Value at pointer: 5
New value: 10
```

---

## 3. Dynamic Memory Allocation

### Using `new` and `delete`

```cpp
// Allocate single variable
int* age = new int;
*age = 25;
delete age;  // Free memory

// Allocate array
int* appointmentIDs = new int[5];
for (int i = 0; i < 5; i++) {
    appointmentIDs[i] = 1000 + i;
}
delete[] appointmentIDs;  // Note: delete[] for arrays
```

### Example: Dynamic Patient Creation

```cpp
void example2() {
    // Create patient dynamically
    Patient* patient = new Patient();
    patient->patientID = 101;
    patient->patientName = "Ahmad Ali";
    patient->email = "ahmad@example.com";
    
    std::cout << "Patient ID: " << patient->patientID << std::endl;
    std::cout << "Patient Name: " << patient->patientName << std::endl;
    
    // Important: Free memory when done
    delete patient;
}
```

**Key Points:**
- `new` allocates memory on the heap
- `delete` frees the memory
- Always `delete` what you `new`
- Use `delete[]` for arrays

---

## 4. Pointers with Structures

### Stack vs Heap Allocation

```cpp
// Stack allocation (automatic)
Patient patient1;
patient1.patientID = 101;
patient1.patientName = "John";

// Heap allocation (dynamic)
Patient* patient2 = new Patient();
patient2->patientID = 102;
patient2->patientName = "Jane";

delete patient2;
```

### Accessing Structure Members

```cpp
// Dot operator (.)
Patient patient;
patient.patientID = 101;

// Arrow operator (->)
Patient* ptr = &patient;
ptr->patientID = 102;

// Equivalent to:
(*ptr).patientID = 102;
```

### Example: Doctor Management

```cpp
void example3() {
    // Stack allocation
    Doctor doctor1;
    doctor1.doctorID = 1;
    doctor1.doctorName = "Dr. Ahmad";
    doctor1.specialty = "Cardiology";
    
    // Pointer to stack variable
    Doctor* ptr = &doctor1;
    std::cout << "Specialty: " << ptr->specialty << std::endl;
    
    // Dynamic allocation
    Doctor* doctor2 = new Doctor();
    doctor2->doctorID = 2;
    doctor2->doctorName = "Dr. Fatimah";
    doctor2->specialty = "Pediatrics";
    
    std::cout << "Doctor: " << doctor2->doctorName << std::endl;
    
    delete doctor2;
}
```

---

## 5. Array of Pointers

### Creating and Managing Multiple Objects

```cpp
const int MAX_DOCTORS = 3;
Doctor* doctors[MAX_DOCTORS];  // Array of pointers

// Allocate each doctor
for (int i = 0; i < MAX_DOCTORS; i++) {
    doctors[i] = new Doctor();
}

// Use doctors
doctors[0]->doctorName = "Dr. Ahmad";
doctors[1]->doctorName = "Dr. Siti";
doctors[2]->doctorName = "Dr. Kumar";

// Clean up
for (int i = 0; i < MAX_DOCTORS; i++) {
    delete doctors[i];
}
```

### Example: Patient List

```cpp
void example4() {
    const int PATIENT_COUNT = 3;
    Patient* patients[PATIENT_COUNT];
    
    // Create patients
    patients[0] = new Patient();
    patients[0]->patientID = 101;
    patients[0]->patientName = "Ali";
    
    patients[1] = new Patient();
    patients[1]->patientID = 102;
    patients[1]->patientName = "Sara";
    
    patients[2] = new Patient();
    patients[2]->patientID = 103;
    patients[2]->patientName = "Ahmad";
    
    // Display all patients
    for (int i = 0; i < PATIENT_COUNT; i++) {
        std::cout << patients[i]->patientID << ": " 
                  << patients[i]->patientName << std::endl;
    }
    
    // Clean up
    for (int i = 0; i < PATIENT_COUNT; i++) {
        delete patients[i];
    }
}
```

---

## 6. Double Pointers (Pointer to Pointer)

### Concept

```cpp
int value = 42;
int* ptr1 = &value;      // Pointer to int
int** ptr2 = &ptr1;      // Pointer to pointer

std::cout << value << std::endl;   // 42
std::cout << *ptr1 << std::endl;   // 42
std::cout << **ptr2 << std::endl;  // 42
```

### Visual Representation

```
+--------+       +--------+       +--------+
| value  |  <--  |  ptr1  |  <--  |  ptr2  |
|   42   |       | 0x1000 |       | 0x2000 |
+--------+       +--------+       +--------+
```

### Example: Appointment ID Management

```cpp
void example5() {
    int appointmentID = 1001;
    int* ptr1 = &appointmentID;
    int** ptr2 = &ptr1;
    
    std::cout << "Original ID: " << appointmentID << std::endl;
    std::cout << "*ptr1: " << *ptr1 << std::endl;
    std::cout << "**ptr2: " << **ptr2 << std::endl;
    
    // Modify through double pointer
    **ptr2 = 2002;
    
    std::cout << "After modification: " << appointmentID << std::endl;
}
```

**Output:**
```
Original ID: 1001
*ptr1: 1001
**ptr2: 1001
After modification: 2002
```

---

## 7. Pointer Parameters in Functions

### Pass by Value vs Pass by Pointer

```cpp
// Pass by value (copy)
void modifyValue(int x) {
    x = 10;  // Only modifies copy
}

// Pass by pointer (reference)
void modifyPointer(int* x) {
    *x = 10;  // Modifies original
}

int main() {
    int value = 5;
    
    modifyValue(value);
    std::cout << value << std::endl;  // 5 (unchanged)
    
    modifyPointer(&value);
    std::cout << value << std::endl;  // 10 (changed)
}
```

### Example: Swap Function

```cpp
void swapIntegers(int* a, int* b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

void example6() {
    int patientID1 = 101;
    int patientID2 = 202;
    
    std::cout << "Before: " << patientID1 << ", " << patientID2 << std::endl;
    swapIntegers(&patientID1, &patientID2);
    std::cout << "After: " << patientID1 << ", " << patientID2 << std::endl;
}
```

**Output:**
```
Before: 101, 202
After: 202, 101
```

### Example: Update Structure

```cpp
void updatePatient(Patient* patient, const std::string& newName) {
    if (patient != nullptr) {
        patient->patientName = newName;
    }
}

void example7() {
    Patient patient;
    patient.patientID = 101;
    patient.patientName = "Old Name";
    
    std::cout << "Before: " << patient.patientName << std::endl;
    updatePatient(&patient, "New Name");
    std::cout << "After: " << patient.patientName << std::endl;
}
```

---

## 8. Pointer Arithmetic

### Basic Operations

```cpp
int arr[] = {10, 20, 30, 40, 50};
int* ptr = arr;  // Points to arr[0]

std::cout << *ptr << std::endl;      // 10
std::cout << *(ptr + 1) << std::endl; // 20
std::cout << *(ptr + 2) << std::endl; // 30

ptr++;  // Move to next element
std::cout << *ptr << std::endl;      // 20
```

### Example: Appointment Time Slots

```cpp
void example8() {
    int timeSlots[] = {9, 10, 11, 14, 15};
    int* ptr = timeSlots;
    
    std::cout << "Available Time Slots:" << std::endl;
    for (int i = 0; i < 5; i++) {
        std::cout << *(ptr + i) << ":00" << std::endl;
    }
    
    // Alternative using pointer increment
    ptr = timeSlots;
    for (int i = 0; i < 5; i++) {
        std::cout << *ptr << ":00" << std::endl;
        ptr++;
    }
}
```

---

## 9. Best Practices

### 1. Always Initialize Pointers

```cpp
// Good
int* ptr = nullptr;

// Bad
int* ptr;  // Uninitialized - dangerous!
```

### 2. Check for NULL Before Use

```cpp
Doctor* doctor = getDoctorById(101);

if (doctor != nullptr) {
    std::cout << doctor->doctorName << std::endl;
} else {
    std::cout << "Doctor not found!" << std::endl;
}
```

### 3. Always Delete What You New

```cpp
// Allocate
Patient* patient = new Patient();

// Use patient...

// Free memory
delete patient;
patient = nullptr;  // Good practice
```

### 4. Use `delete[]` for Arrays

```cpp
// Allocate array
int* arr = new int[10];

// Use array...

// Free array
delete[] arr;  // Note: delete[] not delete
```

### 5. Avoid Memory Leaks

```cpp
// Bad - memory leak
void badFunction() {
    Patient* patient = new Patient();
    // Forgot to delete!
}  // Memory leaked

// Good - proper cleanup
void goodFunction() {
    Patient* patient = new Patient();
    // Use patient...
    delete patient;
}
```

### 6. Avoid Dangling Pointers

```cpp
int* ptr = new int(42);
delete ptr;
ptr = nullptr;  // Set to nullptr after delete

// Bad
// *ptr = 10;  // Accessing deleted memory!

// Good
if (ptr != nullptr) {
    *ptr = 10;
}
```

---

## Complete Example: Hospital Patient Management

```cpp
class PatientManager {
private:
    Patient** patients;  // Dynamic array of pointers
    int capacity;
    int count;
    
public:
    PatientManager(int maxPatients) {
        capacity = maxPatients;
        count = 0;
        patients = new Patient*[capacity];
        
        // Initialize all pointers to nullptr
        for (int i = 0; i < capacity; i++) {
            patients[i] = nullptr;
        }
    }
    
    ~PatientManager() {
        // Delete all patients
        for (int i = 0; i < count; i++) {
            delete patients[i];
        }
        // Delete array
        delete[] patients;
    }
    
    bool addPatient(int id, const std::string& name) {
        if (count >= capacity) {
            return false;
        }
        
        patients[count] = new Patient();
        patients[count]->patientID = id;
        patients[count]->patientName = name;
        count++;
        return true;
    }
    
    Patient* findPatient(int id) {
        for (int i = 0; i < count; i++) {
            if (patients[i]->patientID == id) {
                return patients[i];
            }
        }
        return nullptr;
    }
    
    void displayAll() {
        for (int i = 0; i < count; i++) {
            std::cout << "ID: " << patients[i]->patientID 
                      << ", Name: " << patients[i]->patientName << std::endl;
        }
    }
};

// Usage
void example9() {
    PatientManager manager(100);
    
    manager.addPatient(101, "Ahmad");
    manager.addPatient(102, "Fatimah");
    manager.addPatient(103, "Kumar");
    
    manager.displayAll();
    
    Patient* patient = manager.findPatient(102);
    if (patient != nullptr) {
        std::cout << "Found: " << patient->patientName << std::endl;
    }
    
    // Destructor automatically cleans up
}
```

---

## Summary

### Key Concepts
1. **Pointers store memory addresses**
2. **Use `&` to get address, `*` to dereference**
3. **`new` allocates, `delete` frees**
4. **`->` accesses structure members through pointers**
5. **Always initialize pointers (use `nullptr`)**
6. **Check for `nullptr` before dereferencing**
7. **Always `delete` what you `new`**
8. **Use `delete[]` for arrays**

### Common Mistakes to Avoid
- ❌ Dereferencing uninitialized pointers
- ❌ Dereferencing `nullptr`
- ❌ Memory leaks (forgetting to `delete`)
- ❌ Using `delete` instead of `delete[]` for arrays
- ❌ Accessing memory after `delete` (dangling pointer)
- ❌ Double deletion

### When to Use Pointers
- ✅ Dynamic memory allocation
- ✅ Large structures (avoid copying)
- ✅ Returning multiple values from functions
- ✅ Building complex data structures
- ✅ Interfacing with C libraries

---

## Running the Examples

To run these examples in your Hospital Appointment System:

1. **Compile your project** in Visual Studio 2022
2. **Run the application**
3. **Select option 6** from the main menu: "View Pointer Examples (Workshop)"
4. **Choose an example** (1-9) to see it in action
5. **Study the output** and compare with the code

---

**End of Pointer Guide**




