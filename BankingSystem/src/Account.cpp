#include "Account.h"

#include <iostream>
#include <utility>

#include "Exceptions.h"

Account::Account(std::string accNum, std::string holder, double initialBalance)
    : accountNumber(std::move(accNum)),
      accountHolder(std::move(holder)),
      balance(initialBalance) {
    if (initialBalance < 0.0) {
        throw InvalidTransactionError(
            "Initial deposit cannot be negative for account " + accountNumber);
    }
}

void Account::deposit(double amount, TransactionType logType) {
    if (amount <= 0.0) {
        throw InvalidTransactionError("Deposit amount must be positive.");
    }
    balance += amount;
    recordTransaction(logType, amount);
}

double Account::getBalance() const {
    return balance;
}

std::string Account::getAccountNumber() const {
    return accountNumber;
}

std::string Account::getAccountHolder() const {
    return accountHolder;
}

void Account::recordTransaction(TransactionType type, double amount) {
    ledger.addTransaction(type, amount);
}

void Account::displayLedger() const {
    std::cout << "--- Ledger for [" << accountNumber << "] " << accountHolder
               << " (" << getAccountType() << ") ---\n";
    ledger.printHistory();
}
