#ifndef SAVINGS_ACCOUNT_H
#define SAVINGS_ACCOUNT_H

#include "Account.h"

// Concrete Account subclass enforcing a strict $500 minimum balance.
// Runtime polymorphism: wherever code holds an Account* / Account&
// pointing at a SavingsAccount, calling withdraw() dispatches here
// automatically.
class SavingsAccount : public Account {
private:
    static constexpr double MIN_BALANCE = 500.0;

public:
    SavingsAccount(const std::string& accNum, const std::string& holder, double initialBalance);

    void withdraw(double amount, TransactionType logType = TransactionType::Withdrawal) override;
    std::string getAccountType() const override;
};

#endif // SAVINGS_ACCOUNT_H
