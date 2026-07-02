#include "AccountStore.h"

#include <iomanip>
#include <iostream>

#include "Exceptions.h"

AccountStore::AccountStore() : root(nullptr), count(0) {}

AccountStore::Node* AccountStore::insert(std::unique_ptr<Node>& node, const std::string& key,
                                           std::unique_ptr<Account> account) {
    if (!node) {
        node = std::make_unique<Node>(key, std::move(account));
        ++count;
        return node.get();
    }
    if (key < node->key) {
        return insert(node->left, key, std::move(account));
    } else if (key > node->key) {
        return insert(node->right, key, std::move(account));
    }
    throw InvalidTransactionError("An account with ID '" + key + "' already exists.");
}

AccountStore::Node* AccountStore::find(Node* node, const std::string& key) const {
    if (!node) {
        return nullptr;
    }
    if (key == node->key) {
        return node;
    }
    if (key < node->key) {
        return find(node->left.get(), key);
    }
    return find(node->right.get(), key);
}

void AccountStore::addAccount(std::unique_ptr<Account> account) {
    const std::string key = account->getAccountNumber();
    insert(root, key, std::move(account));
}

Account* AccountStore::findAccount(const std::string& accountId) const {
    Node* node = find(root.get(), accountId);
    if (!node) {
        throw AccountNotFoundError("No account found with ID '" + accountId + "'.");
    }
    return node->account.get();
}

void AccountStore::transferFunds(const std::string& fromId, const std::string& toId,
                                   double amount) {
    if (amount <= 0.0) {
        throw InvalidTransactionError("Transfer amount must be positive.");
    }
    if (fromId == toId) {
        throw InvalidTransactionError("Cannot transfer funds to the same account.");
    }

    Account* fromAccount = findAccount(fromId);
    Account* toAccount = findAccount(toId);

    // 1) Deduction -- may throw InsufficientFundsError; if it does,
    //    nothing below runs and no balance has been touched.
    fromAccount->withdraw(amount, TransactionType::Transfer);

    // 2) Addition -- guaranteed safe here since amount > 0 was already
    //    validated above (see header comment on transferFunds).
    toAccount->deposit(amount, TransactionType::Transfer);

    // 3) Transaction logging -- both legs already recorded themselves
    //    as TransactionType::Transfer via steps 1 and 2, so both
    //    ledgers reflect the movement with no separate step needed.
}

void AccountStore::inorderPrint(const Node* node) const {
    if (!node) {
        return;
    }
    inorderPrint(node->left.get());
    std::cout << "  [" << node->key << "] " << std::left << std::setw(14)
               << node->account->getAccountHolder() << " (" << std::setw(8)
               << node->account->getAccountType() << ") Balance: $" << std::right
               << std::fixed << std::setprecision(2) << node->account->getBalance() << "\n";
    inorderPrint(node->right.get());
}

void AccountStore::printAllAccounts() const {
    std::cout << "--- All Accounts (in-order BST traversal, sorted by ID) ---\n";
    inorderPrint(root.get());
}

std::size_t AccountStore::getAccountCount() const {
    return count;
}
