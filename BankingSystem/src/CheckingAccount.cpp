#include "CheckingAccount.h"

#include <iomanip>
#include <sstream>

#include "Exceptions.h"

CheckingAccount::CheckingAccount(const std::string& accNum, const std::string& holder,
                                   double initialBalance)
    : Account(accNum, holder, initialBalance) {}

void CheckingAccount::withdraw(double amount, TransactionType logType) {
    if (amount <= 0.0) {
        throw InvalidTransactionError("Withdrawal amount must be positive.");
    }
    if (balance - amount < OVERDRAFT_LIMIT) {
        std::ostringstream oss;
        oss << std::fixed << std::setprecision(2)
            << "Withdrawal of $" << amount << " denied for Checking account [" << accountNumber
            << "]: balance would drop below the -$" << (-OVERDRAFT_LIMIT)
            << " overdraft limit (current balance: $" << balance << ").";
        throw InsufficientFundsError(oss.str());
    }
    balance -= amount;
    recordTransaction(logType, amount);
}

std::string CheckingAccount::getAccountType() const {
    return "Checking";
}
