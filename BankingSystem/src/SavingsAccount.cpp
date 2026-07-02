#include "SavingsAccount.h"

#include <iomanip>
#include <sstream>

#include "Exceptions.h"

SavingsAccount::SavingsAccount(const std::string& accNum, const std::string& holder,
                                 double initialBalance)
    : Account(accNum, holder, initialBalance) {
    if (initialBalance < MIN_BALANCE) {
        std::ostringstream oss;
        oss << std::fixed << std::setprecision(2)
            << "Savings account [" << accNum << "] requires a minimum opening balance of $"
            << MIN_BALANCE << " (provided: $" << initialBalance << ").";
        throw InvalidTransactionError(oss.str());
    }
}

void SavingsAccount::withdraw(double amount, TransactionType logType) {
    if (amount <= 0.0) {
        throw InvalidTransactionError("Withdrawal amount must be positive.");
    }
    if (balance - amount < MIN_BALANCE) {
        std::ostringstream oss;
        oss << std::fixed << std::setprecision(2)
            << "Withdrawal of $" << amount << " denied for Savings account [" << accountNumber
            << "]: balance would drop below the $" << MIN_BALANCE
            << " minimum (current balance: $" << balance << ").";
        throw InsufficientFundsError(oss.str());
    }
    balance -= amount;
    recordTransaction(logType, amount);
}

std::string SavingsAccount::getAccountType() const {
    return "Savings";
}
