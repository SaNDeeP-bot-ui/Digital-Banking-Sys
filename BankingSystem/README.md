# In-Memory Digital Banking System

A dependency-free, modern C++17 banking simulator demonstrating abstract
class hierarchies, runtime polymorphism, the Factory design pattern, and
hand-rolled data structures (a doubly linked list and a binary search
tree) built with strict, explicit memory management.

## Class Hierarchy

```
std::runtime_error
 ├── InsufficientFundsError      (Exceptions.h)
 ├── AccountNotFoundError        (Exceptions.h)
 └── InvalidTransactionError     (Exceptions.h)

Account                          (abstract base class, Account.h/.cpp)
 ├── SavingsAccount               (SavingsAccount.h/.cpp)  — $500 minimum balance
 └── CheckingAccount              (CheckingAccount.h/.cpp) — -$200 overdraft limit

AccountFactory                   (AccountFactory.h/.cpp)
 └── createAccount() -> std::unique_ptr<Account>   (Factory Pattern)

Ledger                           (Ledger.h/.cpp)
 └── private Node { Transaction data;
                     shared_ptr<Node> next;
                     weak_ptr<Node>   prev; }        (custom Doubly Linked List)

AccountStore                     (AccountStore.h/.cpp)
 └── private Node { string key;
                     unique_ptr<Account> account;
                     unique_ptr<Node>    left;
                     unique_ptr<Node>    right; }     (custom Binary Search Tree)

Transaction                      (Transaction.h, plain struct)
```

## OOP & Data Structure Concepts

| Concept | Where it appears |
|---|---|
| **Abstract class / pure virtual functions** | `Account` declares `withdraw()` and `getAccountType()` as pure virtual — it can never be instantiated directly. |
| **Inheritance & runtime polymorphism** | `SavingsAccount` and `CheckingAccount` override `withdraw()`; code calling `Account::withdraw()` through a base pointer dispatches to the correct override at runtime. |
| **Factory design pattern** | `AccountFactory::createAccount()` is the only place `new SavingsAccount`/`new CheckingAccount` is ever written; callers only ever ask for an `AccountType`. |
| **Encapsulation** | `Account`'s fields (`accountNumber`, `accountHolder`, `balance`, `ledger`) are `protected`; all external access goes through public accessor methods. |
| **`std::unique_ptr` (exclusive ownership)** | `AccountFactory` returns `unique_ptr<Account>`; `AccountStore::Node` owns its `Account` and its `left`/`right` children via `unique_ptr`, giving automatic, exception-safe cleanup of the whole tree. |
| **`std::shared_ptr` / `std::weak_ptr` (shared ownership, cycle avoidance)** | `Ledger::Node::next` is `shared_ptr` (owns forward); `Ledger::Node::prev` is `weak_ptr` (observes backward) — this deliberately breaks the reference cycle a `shared_ptr`-only doubly linked list would otherwise create. |
| **Rule of Five, applied explicitly** | `Ledger` and `Account` both explicitly delete/default their copy/move constructors and assignment operators, with comments explaining *why* each choice was made, rather than relying on compiler defaults. |
| **Custom Doubly Linked List (`O(1)` append)** | `Ledger` — no `std::list` used. |
| **Custom Binary Search Tree (`O(log n)` avg. insert/search)** | `AccountStore` — no `std::map` / `std::unordered_map` used; accounts are keyed and ordered by account number. |
| **Custom exception hierarchy** | `InsufficientFundsError`, `AccountNotFoundError`, `InvalidTransactionError` all derive from `std::runtime_error` for both specific and generic `catch` handling. |

## How to Compile and Run

```
make
./banking_system
```

`make clean` removes all compiled objects and the `banking_system` binary.

## Sample Output

```
============================================================
 In-Memory Digital Banking System
============================================================

============================================================
 Step 1: Creating accounts via AccountFactory
============================================================
Created Savings account for Alice Johnson [ACC-ALICE-001] -- opening balance: $1500.00
Created Checking account for Bob Smith [ACC-BOB-002] -- opening balance: $100.00
--- All Accounts (in-order BST traversal, sorted by ID) ---
  [ACC-ALICE-001] Alice Johnson  (Savings ) Balance: $1500.00
  [ACC-BOB-002] Bob Smith      (Checking) Balance: $100.00

============================================================
 Step 2: Transfer $300 from Alice to Bob
============================================================
Transfer succeeded: $300.00 moved from ACC-ALICE-001 to ACC-BOB-002.
--- All Accounts (in-order BST traversal, sorted by ID) ---
  [ACC-ALICE-001] Alice Johnson  (Savings ) Balance: $1200.00
  [ACC-BOB-002] Bob Smith      (Checking) Balance: $400.00

============================================================
 Step 3: Alice attempts to withdraw $800 (breaches $500 Savings minimum)
============================================================
Caught InsufficientFundsError: Withdrawal of $800.00 denied for Savings account
[ACC-ALICE-001]: balance would drop below the $500.00 minimum (current balance: $1200.00).

============================================================
 Step 4: Bob attempts to withdraw $700 (breaches -$200 Checking overdraft limit)
============================================================
Caught InsufficientFundsError: Withdrawal of $700.00 denied for Checking account
[ACC-BOB-002]: balance would drop below the -$200.00 overdraft limit (current balance: $400.00).

============================================================
 Step 6: Ledger history (proves only the successful transfer was recorded)
============================================================
--- Ledger for [ACC-ALICE-001] Alice Johnson (Savings) ---
    [#1] Transfer   | Amount: $    300.00 | Time: 2026-07-02 18:27:38
--- Ledger for [ACC-BOB-002] Bob Smith (Checking) ---
    [#1] Transfer   | Amount: $    300.00 | Time: 2026-07-02 18:27:38
```

Note that each ledger contains exactly **one** entry — the successful
$300 transfer — even though two withdrawal attempts were made
afterward. Because `SavingsAccount::withdraw()` and
`CheckingAccount::withdraw()` throw *before* calling
`recordTransaction()`, a failed withdrawal never touches the ledger,
which is exactly what the custom doubly linked list is shown proving
above.

## Verification

This project was compiled with `g++ -std=c++17 -Wall -I include` and
produces **zero warnings** (it is also clean under `-Wextra`). The
compiled binary was additionally run under `valgrind --leak-check=full
--show-leak-kinds=all`, reporting:

```
HEAP SUMMARY:
    in use at exit: 0 bytes in 0 blocks
    total heap usage: 38 allocs, 38 frees, 86,749 bytes allocated
All heap blocks were freed -- no leaks are possible
ERROR SUMMARY: 0 errors from 0 contexts
```

confirming the `shared_ptr`/`weak_ptr` ledger chain and the
`unique_ptr`-owned BST both release every byte they allocate.
