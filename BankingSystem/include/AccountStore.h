#ifndef ACCOUNT_STORE_H
#define ACCOUNT_STORE_H

#include <cstddef>
#include <memory>
#include <string>

#include "Account.h"

// AccountStore is a hand-rolled Binary Search Tree keyed by account
// number (std::map / std::unordered_map are intentionally NOT used).
//
// Ownership: each Node owns its Account via std::unique_ptr<Account>,
// and owns its left/right children via std::unique_ptr<Node>. This
// means destroying the tree (or any subtree) automatically and
// recursively destroys every Account and every descendant Node -- no
// manual delete anywhere, and no leaks even if an exception unwinds
// through the tree.
//
// Because Account is abstract, std::unique_ptr<Account> is exactly the
// mechanism that lets one owning slot transparently hold either a
// SavingsAccount or a CheckingAccount (or any future subclass) while
// still calling the correct overridden virtual methods -- classic
// runtime polymorphism combined with RAII ownership.
class AccountStore {
private:
    struct Node {
        std::string key;
        std::unique_ptr<Account> account;
        std::unique_ptr<Node> left;
        std::unique_ptr<Node> right;

        Node(const std::string& k, std::unique_ptr<Account> acc)
            : key(k), account(std::move(acc)), left(nullptr), right(nullptr) {}
    };

    std::unique_ptr<Node> root;
    std::size_t count;

    // Recursive helpers. Average-case O(log n) for a reasonably
    // balanced tree of n accounts; worst case O(n) for an adversarial
    // insertion order, same as any unbalanced BST.
    Node* insert(std::unique_ptr<Node>& node, const std::string& key,
                 std::unique_ptr<Account> account);
    Node* find(Node* node, const std::string& key) const;
    void inorderPrint(const Node* node) const;

public:
    AccountStore();
    ~AccountStore() = default;

    // The store is the single owner of every Account in the system;
    // copying it would require deep-cloning a polymorphic tree of
    // accounts, which is not a meaningful operation here, so it is
    // disabled.
    AccountStore(const AccountStore&) = delete;
    AccountStore& operator=(const AccountStore&) = delete;

    void addAccount(std::unique_ptr<Account> account);

    // Returns a raw, non-owning pointer for observation/mutation by the
    // caller. The AccountStore retains ownership; the caller must never
    // delete this pointer. Throws AccountNotFoundError if no account
    // with that ID exists.
    Account* findAccount(const std::string& accountId) const;

    // Moves 'amount' from account 'fromId' to account 'toId', logging
    // the movement as a Transfer in both accounts' ledgers. The
    // withdrawal is attempted first and, if it throws (e.g. balance
    // policy violation), no state has changed yet -- the deposit step
    // never runs. Since 'amount' is validated as positive up front and
    // deposit() only ever throws for non-positive amounts, the deposit
    // step is guaranteed not to throw once the withdrawal has
    // succeeded, so the transfer can never leave the two accounts in an
    // inconsistent (money-vanished) state.
    void transferFunds(const std::string& fromId, const std::string& toId, double amount);

    void printAllAccounts() const;
    std::size_t getAccountCount() const;
};

#endif // ACCOUNT_STORE_H
