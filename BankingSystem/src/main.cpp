#include "AccountStore.h"
#include "AccountFactory.h"
#include "Exceptions.h"
#include <iostream>
#include <string>
#include <limits>

// Helper function to clear bad input if a user types letters instead of numbers
void clearInput() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void displayMenu() {
    std::cout << "\n============================================\n";
    std::cout << "       DIGITAL BANKING TERMINAL             \n";
    std::cout << "============================================\n";
    std::cout << "  1. Create New Account\n";
    std::cout << "  2. Deposit Funds\n";
    std::cout << "  3. Withdraw Funds\n";
    std::cout << "  4. Transfer Funds\n";
    std::cout << "  5. View Account & Transaction Ledger\n";
    std::cout << "  6. Exit System\n";
    std::cout << "--------------------------------------------\n";
    std::cout << "Enter your choice (1-6): ";
}

int main() {
    AccountStore bankRegistry;

    // Pre-populate the system with two accounts so you can test transfers immediately
    bankRegistry.addAccount(AccountFactory::createAccount(AccountType::Savings, "ACC1001", "Alice Smith", 1500.0));
    bankRegistry.addAccount(AccountFactory::createAccount(AccountType::Checking, "ACC1002", "Bob Jones", 100.0));

    std::cout << "\n[SYSTEM] Booting up... Demo accounts ACC1001 and ACC1002 initialized.\n";

    int choice = 0;
    while (choice != 6) {
        displayMenu();

        // Check if the user entered a valid number
        if (!(std::cin >> choice)) {
            std::cout << "\n[ERROR] Invalid input. Please enter a number from the menu.\n";
            clearInput();
            continue; 
        }

        try {
            switch (choice) {
                case 1: {
                    std::string typeStr, id, name;
                    double initialDeposit;
                    
                    std::cout << "Enter Account Type (Savings / Checking): ";
                    std::cin >> typeStr;
                    std::cout << "Enter New Account ID (e.g., ACC1003): ";
                    std::cin >> id;
                    
                    std::cout << "Enter Account Holder Name: ";
                    std::cin.ignore(); // clear the newline character
                    std::getline(std::cin, name);
                    
                    std::cout << "Enter Initial Deposit Amount: $";
                    std::cin >> initialDeposit;

                    // --- NEW LOGIC: Convert the string input to the strict enum ---
                    AccountType actType;
                    if (typeStr == "Savings" || typeStr == "savings") {
                        actType = AccountType::Savings;
                    } else if (typeStr == "Checking" || typeStr == "checking") {
                        actType = AccountType::Checking;
                    } else {
                        throw std::invalid_argument("Invalid account type entered.");
                    }

                    // Pass the enum (actType) instead of the string
                    bankRegistry.addAccount(AccountFactory::createAccount(actType, id, name, initialDeposit));
                    std::cout << "\n[SUCCESS] " << typeStr << " account " << id << " created successfully.\n";
                    break;
                }
                case 2: {
                    std::string id;
                    double amount;
                    std::cout << "Enter Account ID: ";
                    std::cin >> id;
                    std::cout << "Enter Deposit Amount: $";
                    std::cin >> amount;

                    Account* acc = bankRegistry.findAccount(id);
                    acc->deposit(amount);
                    std::cout << "\n[SUCCESS] Deposited $" << amount << " into " << id << ".\n";
                    break;
                }
                case 3: {
                    std::string id;
                    double amount;
                    std::cout << "Enter Account ID: ";
                    std::cin >> id;
                    std::cout << "Enter Withdrawal Amount: $";
                    std::cin >> amount;

                    Account* acc = bankRegistry.findAccount(id);
                    acc->withdraw(amount);
                    std::cout << "\n[SUCCESS] Withdrew $" << amount << " from " << id << ".\n";
                    break;
                }
                case 4: {
                    std::string senderId, receiverId;
                    double amount;
                    std::cout << "Enter Sender Account ID: ";
                    std::cin >> senderId;
                    std::cout << "Enter Receiver Account ID: ";
                    std::cin >> receiverId;
                    std::cout << "Enter Transfer Amount: $";
                    std::cin >> amount;

                    bankRegistry.transferFunds(senderId, receiverId, amount);
                    std::cout << "\n[SUCCESS] Transferred $" << amount << " from " << senderId << " to " << receiverId << ".\n";
                    break;
                }
                case 5: {
                    std::string id;
                    std::cout << "Enter Account ID: ";
                    std::cin >> id;

                    Account* acc = bankRegistry.findAccount(id);
                    std::cout << "\nAccount Details: " << acc->getAccountNumber() << " | " << acc->getAccountHolder() << "\n";
                    std::cout << "Current Balance: $" << acc->getBalance() << "\n";
                    acc->displayLedger();
                    break;
                }
                case 6:
                    std::cout << "\n[SYSTEM] Shutting down digital banking core. Goodbye!\n";
                    break;
                default:
                    std::cout << "\n[ERROR] Invalid choice. Please enter a number between 1 and 6.\n";
                    break;
            }
        } 
        // This catches our custom errors (like InsufficientFundsError) without crashing the program!
        catch (const std::exception& e) {
            std::cout << "\n[TRANSACTION FAILED] " << e.what() << "\n";
        }
    }

    return 0;
}