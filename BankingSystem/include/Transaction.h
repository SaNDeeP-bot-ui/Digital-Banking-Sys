#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <ctime>
#include <iomanip>
#include <sstream>
#include <string>

// The three kinds of activity that can be recorded in an account's Ledger.
enum class TransactionType {
    Deposit,
    Withdrawal,
    Transfer
};

inline std::string transactionTypeToString(TransactionType type) {
    switch (type) {
        case TransactionType::Deposit:    return "Deposit";
        case TransactionType::Withdrawal: return "Withdrawal";
        case TransactionType::Transfer:   return "Transfer";
    }
    return "Unknown";
}

// A single immutable record of banking activity. Instances of Transaction
// are stored by value inside Ledger's linked-list nodes.
struct Transaction {
    long transactionId;
    TransactionType type;
    double amount;
    std::time_t timestamp;

    Transaction(long id, TransactionType t, double amt)
        : transactionId(id), type(t), amount(amt), timestamp(std::time(nullptr)) {}

    // Human-readable "YYYY-MM-DD HH:MM:SS" rendering of the timestamp.
    std::string getTimestampString() const {
        std::ostringstream oss;
        std::tm tmSnapshot{};
        std::tm* tmPtr = std::localtime(&timestamp);
        if (tmPtr != nullptr) {
            tmSnapshot = *tmPtr;
        }
        oss << std::put_time(&tmSnapshot, "%Y-%m-%d %H:%M:%S");
        return oss.str();
    }
};

#endif // TRANSACTION_H
