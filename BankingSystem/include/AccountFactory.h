#ifndef ACCOUNT_FACTORY_H
#define ACCOUNT_FACTORY_H

#include <memory>
#include <string>

#include "Account.h"

// The kinds of account the factory knows how to build. Using an enum
// class (rather than a raw string/int) makes createAccount() type-safe
// at compile time.
enum class AccountType {
    Savings,
    Checking
};

// Factory Design Pattern: callers ask for an account by *type*, never by
// concrete class name, and never call `new SavingsAccount(...)` /
// `new CheckingAccount(...)` themselves. This centralizes construction
// logic in one place and returns ownership via std::unique_ptr<Account>,
// so the caller immediately owns the object through the abstract base
// class interface with automatic, exception-safe cleanup.
class AccountFactory {
public:
    static std::unique_ptr<Account> createAccount(AccountType type,
                                                    const std::string& accountId,
                                                    const std::string& accountHolder,
                                                    double initialDeposit);
};

#endif // ACCOUNT_FACTORY_H
