#include "AccountFactory.h"

#include "CheckingAccount.h"
#include "Exceptions.h"
#include "SavingsAccount.h"

std::unique_ptr<Account> AccountFactory::createAccount(AccountType type,
                                                          const std::string& accountId,
                                                          const std::string& accountHolder,
                                                          double initialDeposit) {
    switch (type) {
        case AccountType::Savings:
            return std::make_unique<SavingsAccount>(accountId, accountHolder, initialDeposit);
        case AccountType::Checking:
            return std::make_unique<CheckingAccount>(accountId, accountHolder, initialDeposit);
    }
    // Defensive: only reachable if AccountType is extended without
    // updating this switch.
    throw InvalidTransactionError("AccountFactory: unrecognized account type requested.");
}
