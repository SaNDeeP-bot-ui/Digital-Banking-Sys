#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <stdexcept>
#include <string>

// All custom banking exceptions derive from std::runtime_error so that
// callers can either catch the specific type or fall back to catching
// std::exception / std::runtime_error generically.

// Thrown when a withdrawal would violate an account's balance policy
// (Savings minimum balance or Checking overdraft limit).
class InsufficientFundsError : public std::runtime_error {
public:
    explicit InsufficientFundsError(const std::string& message)
        : std::runtime_error(message) {}
};

// Thrown when the AccountStore (BST) cannot locate an account for a
// given account number.
class AccountNotFoundError : public std::runtime_error {
public:
    explicit AccountNotFoundError(const std::string& message)
        : std::runtime_error(message) {}
};

// Thrown for structurally invalid operations: negative/zero amounts,
// invalid account types, duplicate account IDs, etc.
class InvalidTransactionError : public std::runtime_error {
public:
    explicit InvalidTransactionError(const std::string& message)
        : std::runtime_error(message) {}
};

#endif // EXCEPTIONS_H
