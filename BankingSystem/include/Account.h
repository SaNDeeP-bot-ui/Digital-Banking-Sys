#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <string>

#include "Ledger.h"

// Abstract base class for every account type in the system.
//
// Encapsulation: accountNumber/accountHolder/balance/ledger are all
// 'protected' -- visible to subclasses that need them to implement
// withdraw(), but never public. External code only ever interacts
// through the public accessor methods below.
//
// Polymorphism: withdraw() and getAccountType() are pure virtual, so
// every concrete account type is forced to supply its own balance
// policy, and callers can operate on a plain Account* / Account&
// without knowing (or caring) whether it's a SavingsAccount or a
// CheckingAccount -- the correct override is dispatched at runtime.
class Account {
protected:
    std::string accountNumber;
    std::string accountHolder;
    double balance;
    Ledger ledger;

    // Shared helper for subclasses: appends an entry to this account's
    // ledger. Kept protected (not public) because only Account and its
    // subclasses should be able to write to the ledger; outside code can
    // only ever *read* it via displayLedger().
    void recordTransaction(TransactionType type, double amount);

public:
    Account(std::string accNum, std::string holder, double initialBalance);

    // Base class of a polymorphic hierarchy managed through
    // std::unique_ptr<Account> -> destructor MUST be virtual, otherwise
    // deleting through a base pointer is undefined behavior and the
    // derived part of the object (and any resources it owns) would
    // never be cleaned up.
    virtual ~Account() = default;

    // Rule of Five, made explicit: Account instances live exclusively
    // inside AccountStore, owned by a single std::unique_ptr<Account>.
    // There is never a legitimate reason to copy or move the object
    // itself (only the owning unique_ptr moves), so both are disabled
    // to prevent accidental slicing or duplicate ownership of the
    // ledger's shared_ptr chain.
    Account(const Account&) = delete;
    Account& operator=(const Account&) = delete;
    Account(Account&&) = delete;
    Account& operator=(Account&&) = delete;

    // Pure virtual -> Account is abstract; it can never be instantiated
    // directly. logType defaults to Withdrawal so ordinary callers don't
    // need to think about it; AccountStore::transferFunds overrides it
    // to Transfer so the ledger records *why* the money moved.
    virtual void withdraw(double amount, TransactionType logType = TransactionType::Withdrawal) = 0;
    virtual std::string getAccountType() const = 0;

    // Concrete, shared by every account type.
    void deposit(double amount, TransactionType logType = TransactionType::Deposit);
    double getBalance() const;
    std::string getAccountNumber() const;
    std::string getAccountHolder() const;
    void displayLedger() const;
};

#endif // ACCOUNT_H
