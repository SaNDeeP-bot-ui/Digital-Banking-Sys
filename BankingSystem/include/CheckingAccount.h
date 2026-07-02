#ifndef CHECKING_ACCOUNT_H
#define CHECKING_ACCOUNT_H

#include "Account.h"

// Concrete Account subclass allowing the balance to go negative down to
// a $200 overdraft limit (i.e. balance may not drop below -$200).
class CheckingAccount : public Account {
private:
    static constexpr double OVERDRAFT_LIMIT = -200.0;

public:
    CheckingAccount(const std::string& accNum, const std::string& holder, double initialBalance);

    void withdraw(double amount, TransactionType logType = TransactionType::Withdrawal) override;
    std::string getAccountType() const override;
};

#endif // CHECKING_ACCOUNT_H
