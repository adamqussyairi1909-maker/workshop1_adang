// ============================================================
// ConsoleUtils.h - HCI Console Utilities
// Hospital Appointment Booking System
// Provides colored output and formatted display functions
// ============================================================

#ifndef CONSOLE_UTILS_H
#define CONSOLE_UTILS_H

#include <windows.h>
#include <iostream>
#include <string>
#include <iomanip>
#include <cctype>
#include <stdexcept>
#include <conio.h>

// Console color codes for Windows
enum ConsoleColor {
    BLACK = 0,
    DARK_BLUE = 1,
    DARK_GREEN = 2,
    DARK_CYAN = 3,
    DARK_RED = 4,
    DARK_MAGENTA = 5,
    DARK_YELLOW = 6,
    GRAY = 7,
    DARK_GRAY = 8,
    BLUE = 9,
    GREEN = 10,
    CYAN = 11,
    RED = 12,
    MAGENTA = 13,
    YELLOW = 14,
    WHITE = 15
};

class ConsoleUtils {
private:
    HANDLE hConsole;
    
public:
    ConsoleUtils() {
        hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    }
    
    // Set console text color
    void setColor(int color) {
        SetConsoleTextAttribute(hConsole, color);
    }
    
    // Reset to default color
    void resetColor() {
        SetConsoleTextAttribute(hConsole, WHITE);
    }
    
    // Clear console screen
    void clearScreen() {
        system("cls");
    }
    
    // Print centered text
    void printCentered(const std::string& text, int width = 50) {
        int padding = (width - (int)text.length()) / 2;
        if (padding > 0) {
            std::cout << std::string(padding, ' ');
        }
        std::cout << text << std::endl;
    }
    
    // Print header with decoration
    void printHeader(const std::string& title) {
        std::string border(50, '=');
        setColor(CYAN);
        std::cout << "\n" << border << std::endl;
        setColor(YELLOW);
        printCentered(title);
        setColor(CYAN);
        std::cout << border << std::endl;
        resetColor();
    }
    
    // Print sub-header
    void printSubHeader(const std::string& title) {
        std::string border(50, '-');
        setColor(DARK_CYAN);
        std::cout << border << std::endl;
        setColor(WHITE);
        printCentered(title);
        setColor(DARK_CYAN);
        std::cout << border << std::endl;
        resetColor();
    }
    
    // Print success message
    void printSuccess(const std::string& message) {
        setColor(GREEN);
        std::cout << "\n[SUCCESS] " << message << std::endl;
        resetColor();
    }
    
    // Print error message
    void printError(const std::string& message) {
        setColor(RED);
        std::cout << "\n[ERROR] " << message << std::endl;
        resetColor();
    }
    
    // Print warning message
    void printWarning(const std::string& message) {
        setColor(YELLOW);
        std::cout << "\n[WARNING] " << message << std::endl;
        resetColor();
    }
    
    // Print info message
    void printInfo(const std::string& message) {
        setColor(CYAN);
        std::cout << "\n[INFO] " << message << std::endl;
        resetColor();
    }
    
    // Print menu option
    void printMenuOption(int number, const std::string& option) {
        setColor(YELLOW);
        std::cout << "  [" << number << "] ";
        setColor(WHITE);
        std::cout << option << std::endl;
    }
    
    // Print menu option with letter
    void printMenuOptionLetter(char letter, const std::string& option) {
        setColor(YELLOW);
        std::cout << "  [" << letter << "] ";
        setColor(WHITE);
        std::cout << option << std::endl;
    }
    
    // Print table row
    void printTableRow(const std::string& col1, const std::string& col2, 
                       const std::string& col3 = "", const std::string& col4 = "") {
        std::cout << "| " << std::left << std::setw(15) << col1 
                  << "| " << std::setw(20) << col2;
        if (!col3.empty()) std::cout << "| " << std::setw(15) << col3;
        if (!col4.empty()) std::cout << "| " << std::setw(15) << col4;
        std::cout << "|" << std::endl;
    }
    
    // Print table separator
    void printTableSeparator(int columns = 2) {
        std::string sep = "+";
        for (int i = 0; i < columns; i++) {
            sep += std::string(17, '-') + "+";
        }
        std::cout << sep << std::endl;
    }
    
    // Get password input (masked)
    std::string getPasswordInput() {
        std::string password;
        char ch;
        while ((ch = _getch()) != '\r') { // Enter key
            if (ch == '\b') { // Backspace
                if (!password.empty()) {
                    password.pop_back();
                    std::cout << "\b \b";
                }
            } else {
                password += ch;
                std::cout << '*';
            }
        }
        std::cout << std::endl;
        return password;
    }
    
    // Pause and wait for key press
    void pauseScreen() {
        setColor(DARK_GRAY);
        std::cout << "\n  Press any key to continue...";
        resetColor();
        (void)_getch();  // Cast to void to suppress warning
    }
    
    // Get validated integer input with STRICT validation
    int getIntInput(const std::string& prompt, int min, int max) {
        std::string input;
        int value;
        
        while (true) {
            setColor(CYAN);
            std::cout << prompt;
            resetColor();
            
            // Read entire line as string
            std::getline(std::cin, input);
            
            // Trim leading and trailing whitespace
            input.erase(0, input.find_first_not_of(" \t\n\r"));
            input.erase(input.find_last_not_of(" \t\n\r") + 1);
            
            // Check if empty
            if (input.empty()) {
                printError("Input cannot be empty. Please enter a number between " + 
                          std::to_string(min) + " and " + std::to_string(max) + ".");
                continue;
            }
            
            // STRICT: Check if input contains ONLY digits (and optional leading minus)
            bool isValidInteger = true;
            bool hasSign = false;
            
            for (size_t i = 0; i < input.length(); i++) {
                char c = input[i];
                
                // Allow minus sign only at the beginning
                if (c == '-' || c == '+') {
                    if (i == 0 && !hasSign) {
                        hasSign = true;
                        continue;
                    } else {
                        isValidInteger = false;
                        break;
                    }
                }
                // Allow only digits
                else if (!std::isdigit(static_cast<unsigned char>(c))) {
                    isValidInteger = false;
                    break;
                }
            }
            
            // Reject if contains invalid characters
            if (!isValidInteger) {
                printError("Invalid input! Only whole numbers are allowed. Please enter a number between " + 
                          std::to_string(min) + " and " + std::to_string(max) + ".");
                continue;
            }
            
            // Check for leading zeros (except for single zero)
            if (input.length() > 1 && input[0] == '0') {
                printError("Invalid input! Numbers cannot have leading zeros. Please enter a number between " + 
                          std::to_string(min) + " and " + std::to_string(max) + ".");
                continue;
            }
            
            // Convert to integer
            try {
                value = std::stoi(input);
                
                // Check range
                if (value < min || value > max) {
                    printError("Number out of range! Please enter a number between " + 
                              std::to_string(min) + " and " + std::to_string(max) + ".");
                    continue;
                }
                
                return value;
            }
            catch (const std::out_of_range&) {
                printError("Number too large! Please enter a number between " + 
                          std::to_string(min) + " and " + std::to_string(max) + ".");
                continue;
            }
            catch (const std::invalid_argument&) {
                printError("Invalid input! Please enter a valid number between " + 
                          std::to_string(min) + " and " + std::to_string(max) + ".");
                continue;
            }
        }
    }
    
    // Get string input with STRICT validation
    std::string getStringInput(const std::string& prompt, bool allowEmpty = false, 
                               size_t maxLength = 255, bool trimWhitespace = true) {
        std::string input;
        while (true) {
            setColor(CYAN);
            std::cout << prompt;
            resetColor();
            std::getline(std::cin, input);
            
            // Trim leading and trailing whitespace if requested
            if (trimWhitespace) {
                input.erase(0, input.find_first_not_of(" \t\n\r"));
                input.erase(input.find_last_not_of(" \t\n\r") + 1);
            }
            
            // Check if empty
            if (!allowEmpty && input.empty()) {
                printError("This field cannot be empty. Please enter a value.");
                continue;
            }
            
            // Check maximum length
            if (input.length() > maxLength) {
                printError("Input too long! Maximum length is " + std::to_string(maxLength) + " characters.");
                continue;
            }
            
            // Check for control characters (security)
            bool hasControlChar = false;
            for (char c : input) {
                if (std::iscntrl(static_cast<unsigned char>(c)) && c != '\n' && c != '\r' && c != '\t') {
                    hasControlChar = true;
                    break;
                }
            }
            
            if (hasControlChar) {
                printError("Invalid characters detected. Please enter valid text only.");
                continue;
            }
            
            return input;
        }
    }
    
    // Validate email format
    bool isValidEmail(const std::string& email) {
        size_t atPos = email.find('@');
        size_t dotPos = email.rfind('.');
        return (atPos != std::string::npos && 
                dotPos != std::string::npos && 
                atPos < dotPos && 
                dotPos < email.length() - 1);
    }
    
    // Validate phone number (Malaysian format)
    bool isValidPhone(const std::string& phone) {
        if (phone.length() < 10 || phone.length() > 12) return false;
        for (char c : phone) {
            if (!isdigit(c) && c != '-' && c != '+') return false;
        }
        return true;
    }
    
    // Validate date format (YYYY-MM-DD) with STRICT validation
    bool isValidDate(const std::string& date) {
        // Trim whitespace first
        std::string trimmed = date;
        trimmed.erase(0, trimmed.find_first_not_of(" \t\n\r"));
        trimmed.erase(trimmed.find_last_not_of(" \t\n\r") + 1);
        
        // Must be exactly 10 characters
        if (trimmed.length() != 10) return false;
        
        // Must have dashes at correct positions
        if (trimmed[4] != '-' || trimmed[7] != '-') return false;
        
        // Check all other characters are digits (STRICT)
        for (int i = 0; i < 10; i++) {
            if (i != 4 && i != 7 && !std::isdigit(static_cast<unsigned char>(trimmed[i]))) {
                return false;
            }
        }
        
        try {
            int year = std::stoi(trimmed.substr(0, 4));
            int month = std::stoi(trimmed.substr(5, 2));
            int day = std::stoi(trimmed.substr(8, 2));
            
            if (year < 1900 || year > 2100) return false;
            if (month < 1 || month > 12) return false;
            if (day < 1 || day > 31) return false;
            
            return true;
        } catch (...) {
            return false;
        }
    }
    
    // Validate time format (HH:MM) with STRICT validation
    bool isValidTime(const std::string& time) {
        // Trim whitespace first
        std::string trimmed = time;
        trimmed.erase(0, trimmed.find_first_not_of(" \t\n\r"));
        trimmed.erase(trimmed.find_last_not_of(" \t\n\r") + 1);
        
        // Must be exactly 5 characters
        if (trimmed.length() != 5) return false;
        
        // Must have colon at correct position
        if (trimmed[2] != ':') return false;
        
        // Check all other characters are digits (STRICT)
        for (int i = 0; i < 5; i++) {
            if (i != 2 && !std::isdigit(static_cast<unsigned char>(trimmed[i]))) {
                return false;
            }
        }
        
        try {
            int hour = std::stoi(trimmed.substr(0, 2));
            int minute = std::stoi(trimmed.substr(3, 2));
            
            if (hour < 0 || hour > 23) return false;
            if (minute < 0 || minute > 59) return false;
            
            return true;
        } catch (...) {
            return false;
        }
    }
    
    // Display loading animation
    void showLoading(const std::string& message, int duration = 3) {
        setColor(YELLOW);
        std::cout << "\n" << message;
        for (int i = 0; i < duration; i++) {
            std::cout << ".";
            Sleep(300);
        }
        std::cout << std::endl;
        resetColor();
    }
    
    // Display welcome banner
    void displayWelcomeBanner() {
        clearScreen();
        std::cout << "\n\n";
        setColor(CYAN);
        std::cout << "      ========================================================" << std::endl;
        setColor(WHITE);
        std::cout << "                  HOSPITAL APPOINTMENT SYSTEM                 " << std::endl;
        setColor(CYAN);
        std::cout << "      ========================================================" << std::endl;
        std::cout << "\n";
        setColor(DARK_GRAY);
        std::cout << "                    [ Welcome to the System ]                 " << std::endl;
        std::cout << "\n";
        resetColor();
    }
};

#endif // CONSOLE_UTILS_H

