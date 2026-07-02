#ifndef LEDGER_H
#define LEDGER_H

#include <cstddef>
#include <memory>

#include "Transaction.h"

// Ledger is a hand-rolled doubly linked list (std::list is intentionally
// NOT used) that stores the full transaction history for one Account.
//
// Memory management design:
//   - Node::next is a std::shared_ptr<Node>. Ownership of the chain flows
//     forward: each node keeps the next node alive.
//   - Node::prev is a std::weak_ptr<Node>. If prev were also a
//     shared_ptr, every adjacent pair of nodes would own each other,
//     creating a reference cycle that the shared_ptr control block can
//     never free -> a classic memory leak. weak_ptr breaks that cycle
//     while still allowing backward traversal via prev.lock().
class Ledger {
private:
    struct Node {
        Transaction data;
        std::shared_ptr<Node> next;
        std::weak_ptr<Node> prev;

        explicit Node(const Transaction& t) : data(t), next(nullptr) {}
    };

    std::shared_ptr<Node> head;
    std::shared_ptr<Node> tail;
    std::size_t count;
    long nextTransactionId;

public:
    Ledger();
    ~Ledger();

    // Rule of Five, applied deliberately rather than left implicit:
    // Copying a Ledger would shallow-copy the shared_ptr chain, meaning
    // two "independent" ledgers would silently share and mutate the same
    // underlying nodes -- so copying is disabled outright. Moving is safe
    // (it simply steals the head/tail pointers) so it is left available.
    Ledger(const Ledger&) = delete;
    Ledger& operator=(const Ledger&) = delete;
    Ledger(Ledger&&) noexcept = default;
    Ledger& operator=(Ledger&&) noexcept = default;

    // Appends a new transaction of the given type/amount to the tail of
    // the list. O(1) thanks to the retained tail pointer.
    void addTransaction(TransactionType type, double amount);

    // Prints every recorded transaction, oldest first.
    void printHistory() const;

    std::size_t getSize() const;
};

#endif // LEDGER_H
