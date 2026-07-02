# Core Banking Engine (C++17) - Interactive Terminal

A lightweight, high-performance digital banking backend engineered in modern C++. This project was built to demonstrate raw computer science fundamentals by intentionally bypassing standard library abstractions (`std::map`, `std::list`) in favor of custom-engineered data structures, strict explicit memory management, and an interactive command-line interface.

## Class Architecture & Hierarchy

```text
Banking Core Engine
│
├── AccountStore (Custom Binary Search Tree)
│
├── AccountFactory (Factory Design Pattern)
│
└── Account (Abstract Base Class)
    ├── SavingsAccount (Enforces $500 minimum balance limit)
    └── CheckingAccount (Allows $200 overdraft facility)
         │
         └── Ledger (Custom Doubly Linked List per account)
```
## Core Computer Science Concepts Demonstrated

* **Custom Algorithmic Data Structures:**
  * **Binary Search Tree (BST):** Implemented for O(log n) average-time complexity account retrieval and storage, completely replacing `std::map`.
  * **Doubly Linked List:** Engineered from scratch to maintain chronological transaction histories without relying on `std::list`.
* **Modern C++ Memory Management:**
  * Utilizes `std::unique_ptr` for strict, exclusive ownership of polymorphic `Account` nodes within the BST to guarantee zero memory leaks.
  * Implements explicit `std::shared_ptr` (forward) and `std::weak_ptr` (backward) pairs in the custom Ledger to completely prevent circular reference memory leaks.
* **Strict Object-Oriented Programming (OOP):**
  * **Runtime Polymorphism:** Core functions like `transferFunds` operate safely on the abstract `Account` base class, allowing the C++ virtual table to dynamically apply specific withdrawal rules at runtime.
  * **Factory Design Pattern:** `AccountFactory` abstracts and centralizes the secure instantiation of derived account types using strict enum typing (`AccountType`).
  * **Exception Handling:** Robust `try-catch` blocks and custom exception classes (`InsufficientFundsError`, `AccountNotFoundError`) ensure the interactive terminal never crashes during invalid user inputs or structural business rule breaches.

## Build & Run Instructions

This project is entirely self-contained and requires zero external dependencies or database configurations. 

**Requirements:** Any standard C++ compiler (`g++`, `clang++`) supporting the C++17 standard, and `make`.

```bash
# 1. Clone the repository and navigate to the root directory
cd Digital-Banking-Sys

# 2. Compile the project using the included Makefile
make

# 3. Launch the interactive banking terminal
./banking_system.exe
```
(Note: On Mac/Linux, run ./banking_system instead of the .exe)

## Sample Execution 
```
 ============================================
       DIGITAL BANKING TERMINAL             
============================================
  1. Create New Account
  2. Deposit Funds
  3. Withdraw Funds
  4. Transfer Funds
  5. View Account & Transaction Ledger
  6. Exit System
--------------------------------------------
Enter your choice (1-6): 1
Enter Account Type (Savings / Checking): Savings
Enter New Account ID (e.g., ACC1003): ACC1003
Enter Account Holder Name: John Doe
Enter Initial Deposit Amount: $2000

[SUCCESS] Savings account ACC1003 created successfully.

Enter your choice (1-6): 4
Enter Sender Account ID: ACC1003
Enter Receiver Account ID: ACC1002
Enter Transfer Amount: $1600

[TRANSACTION FAILED] Transaction denied. Savings Account must maintain a minimum balance of $500.
```
