#include "Ledger.h"

#include <iomanip>
#include <iostream>

Ledger::Ledger() : head(nullptr), tail(nullptr), count(0), nextTransactionId(1) {}

Ledger::~Ledger() {
    // shared_ptr's default recursive destruction (node destroys its
    // 'next', which destroys its 'next', ...) can blow the stack for a
    // very long ledger. To manage memory explicitly and safely, we walk
    // the chain iteratively and release each 'next' link by hand before
    // the shared_ptr destructors run, turning the recursive teardown
    // into an O(n) iterative one.
    std::shared_ptr<Node> current = head;
    while (current) {
        std::shared_ptr<Node> nextNode = current->next;
        current->next.reset();
        current = nextNode;
    }
    head.reset();
    tail.reset();
}

void Ledger::addTransaction(TransactionType type, double amount) {
    Transaction t(nextTransactionId++, type, amount);
    auto newNode = std::make_shared<Node>(t);

    if (!head) {
        head = newNode;
        tail = newNode;
    } else {
        newNode->prev = tail;  // weak_ptr<Node> = shared_ptr<Node>, no cycle
        tail->next = newNode;
        tail = newNode;
    }
    ++count;
}

void Ledger::printHistory() const {
    if (!head) {
        std::cout << "    (no transactions recorded)\n";
        return;
    }

    std::shared_ptr<Node> current = head;
    while (current) {
        const Transaction& t = current->data;
        std::cout << "    [#" << t.transactionId << "] "
                   << std::left << std::setw(10) << transactionTypeToString(t.type)
                   << " | Amount: $" << std::right << std::fixed << std::setprecision(2)
                   << std::setw(10) << t.amount
                   << " | Time: " << t.getTimestampString() << "\n";
        current = current->next;
    }
}

std::size_t Ledger::getSize() const {
    return count;
}
