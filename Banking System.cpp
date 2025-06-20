#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <ctime>

class Date {
public:
    int day, month, year;

    Date() {
        time_t t = time(nullptr);
        tm localTime;
        localtime_s(&localTime, &t); // Use localtime_s for safety
        day = localTime.tm_mday;
        month = localTime.tm_mon + 1; // tm_mon is 0-11
        year = localTime.tm_year + 1900; // tm_year is years since 1900
    }

    std::string toString() const {
        return std::to_string(day) + "/" + std::to_string(month) + "/" + std::to_string(year);
    }
};

class Transaction {
public:
    std::string type; // Type of transaction (Deposit, Withdrawal, Transfer)
    double amount;    // Amount of transaction
    Date date;       // Date of transaction

    Transaction(std::string t, double amt) : type(t), amount(amt), date(Date()) {}
};

class Account {
private:
    std::string accountNumber;
    double balance;
    std::vector<Transaction> transactionHistory;

public:
    Account(std::string accNum) : accountNumber(accNum), balance(0.0) {}

    std::string getAccountNumber() const {
        return accountNumber;
    }

    double getBalance() const {
        return balance;
    }

    void deposit(double amount) {
        balance += amount;
        transactionHistory.emplace_back("Deposit", amount);
    }

    bool withdraw(double amount) {
        if (amount <= balance) {
            balance -= amount;
            transactionHistory.emplace_back("Withdrawal", amount);
            return true;
        }
        std::cout << "Insufficient balance for withdrawal of $" << amount << ".\n";
        return false;
    }

    bool transfer(Account& toAccount, double amount) {
        if (withdraw(amount)) {
            toAccount.deposit(amount);
            transactionHistory.emplace_back("Transfer to " + toAccount.getAccountNumber(), amount);
            return true;
        }
        return false;
    }

    void printTransactionHistory() const {
        std::cout << "Transaction History for Account " << accountNumber << ":\n";
        std::cout << std::setw(10) << "Type"
            << " | " << std::setw(8) << "Amount"
            << " | " << "Date\n";
        std::cout << std::string(35, '-') << '\n';
        for (const auto& transaction : transactionHistory) {
            std::cout << std::setw(10) << transaction.type
                << " | " << std::setw(8) << transaction.amount
                << " | " << transaction.date.toString() << '\n';
        }
    }

    void displayAccountInfo() const {
        std::cout << "Account Number: " << accountNumber << "\n";
        std::cout << "Current Balance: $" << std::fixed << std::setprecision(2) << balance << "\n";
    }
};

class Customer {
private:
    std::string name;
    std::string customerID;
    std::vector<Account> accounts;

public:
    Customer(std::string n, std::string id) : name(n), customerID(id) {}

    void addAccount(const std::string& accountNumber) {
        accounts.emplace_back(accountNumber);
    }

    void displayAccounts() const {
        std::cout << "Accounts for Customer " << name << " (ID: " << customerID << "):\n";
        for (const auto& account : accounts) {
            account.displayAccountInfo();
            std::cout << "-------------------------\n";
        }
    }

    Account* getAccount(const std::string& accountNumber) {
        for (auto& account : accounts) {
            if (account.getAccountNumber() == accountNumber) {
                return &account;
            }
        }
        return nullptr; // Account not found
    }

    std::string getCustomerID() const { // Getter for customerID
        return customerID;
    }
};

class BankingSystem {
private:
    std::vector<Customer> customers;

public:
    void addCustomer(const std::string& name, const std::string& customerID) {
        customers.emplace_back(name, customerID);
    }

    Customer* findCustomer(const std::string& customerID) {
        for (auto& customer : customers) {
            if (customer.getCustomerID() == customerID) { // Use getter
                return &customer;
            }
        }
        return nullptr; // Customer not found
    }

    void showMenu() {
        std::cout << "\n--- Banking System Menu ---\n";
        std::cout << "1. Create Customer\n";
        std::cout << "2. Create Account\n";
        std::cout << "3. Deposit\n";
        std::cout << "4. Withdraw\n";
        std::cout << "5. Transfer\n";
        std::cout << "6. View Account Info\n";
        std::cout << "7. View Transaction History\n";
        std::cout << "8. Exit\n";
        std::cout << "Choose an option: ";
    }
};

int main() {
    BankingSystem bank;
    int choice;

    while (true) {
        bank.showMenu();
        std::cin >> choice;

        if (choice == 1) {
            std::string name, id;
            std::cout << "Enter customer name: ";
            std::cin >> name;
            std::cout << "Enter customer ID: ";
            std::cin >> id;
            bank.addCustomer(name, id);
            std::cout << "Customer created successfully.\n";
        }
        else if (choice == 2) {
            std::string id, accountNumber;
            std::cout << "Enter customer ID: ";
            std::cin >> id;
            Customer* customer = bank.findCustomer(id);
            if (customer) {
                std::cout << "Enter new account number: ";
                std::cin >> accountNumber;
                customer->addAccount(accountNumber);
                std::cout << "Account created successfully.\n";
            }
            else {
                std::cout << "Customer not found.\n";
            }
        }
        else if (choice == 3) {
            std::string id, accountNumber;
            double amount;
            std::cout << "Enter customer ID: ";
            std::cin >> id;
            Customer* customer = bank.findCustomer(id);
            if (customer) {
                std::cout << "Enter account number: ";
                std::cin >> accountNumber;
                Account* account = customer->getAccount(accountNumber);
                if (account) {
                    std::cout << "Enter amount to deposit: ";
                    std::cin >> amount;
                    account->deposit(amount);
                    std::cout << "Deposit successful.\n";
                }
                else {
                    std::cout << "Account not found.\n";
                }
            }
            else {
                std::cout << "Customer not found.\n";
            }
        }
        else if (choice == 4) {
            std::string id, accountNumber;
            double amount;
            std::cout << "Enter customer ID: ";
            std::cin >> id;
            Customer* customer = bank.findCustomer(id);
            if (customer) {
                std::cout << "Enter account number: ";
                std::cin >> accountNumber;
                Account* account = customer->getAccount(accountNumber);
                if (account) {
                    std::cout << "Enter amount to withdraw: ";
                    std::cin >> amount;
                    if (account->withdraw(amount)) {
                        std::cout << "Withdrawal successful.\n";
                    }
                }
                else {
                    std::cout << "Account not found.\n";
                }
            }
            else {
                std::cout << "Customer not found.\n";
            }
        }
        else if (choice == 5) {
            std::string fromID, fromAccount, toID, toAccount;
            double amount;
            std::cout << "Enter sender customer ID: ";
            std::cin >> fromID;
            Customer* fromCustomer = bank.findCustomer(fromID);
            if (fromCustomer) {
                std::cout << "Enter sender account number: ";
                std::cin >> fromAccount;
                Account* fromAcc = fromCustomer->getAccount(fromAccount);
                if (fromAcc) {
                    std::cout << "Enter receiver customer ID: ";
                    std::cin >> toID;
                    Customer* toCustomer = bank.findCustomer(toID);
                    if (toCustomer) {
                        std::cout << "Enter receiver account number: ";
                        std::cin >> toAccount;
                        Account* toAcc = toCustomer->getAccount(toAccount);
                        if (toAcc) {
                            std::cout << "Enter amount to transfer: ";
                            std::cin >> amount;
                            if (fromAcc->transfer(*toAcc, amount)) {
                                std::cout << "Transfer successful.\n";
                            }
                        }
                        else {
                            std::cout << "Receiver account not found.\n";
                        }
                    }
                    else {
                        std::cout << "Receiver customer not found.\n";
                    }
                }
                else {
                    std::cout << "Sender account not found.\n";
                }
            }
            else {
                std::cout << "Sender customer not found.\n";
            }
        }
        else if (choice == 6) {
            std::string id;
            std::cout << "Enter customer ID: ";
            std::cin >> id;
            Customer* customer = bank.findCustomer(id);
            if (customer) {
                customer->displayAccounts();
            }
            else {
                std::cout << "Customer not found.\n";
            }
        }
        else if (choice == 7) {
            std::string id, accountNumber;
            std::cout << "Enter customer ID: ";
            std::cin >> id;
            Customer* customer = bank.findCustomer(id);
            if (customer) {
                std::cout << "Enter account number: ";
                std::cin >> accountNumber;
                Account* account = customer->getAccount(accountNumber);
                if (account) {
                    account->printTransactionHistory();
                }
                else {
                    std::cout << "Account not found.\n";
                }
            }
            else {
                std::cout << "Customer not found.\n";
            }
        }
        else if (choice == 8) {
            std::cout << "Exiting the banking system. Goodbye!\n";
            break;
        }
        else {
            std::cout << "Invalid choice. Please try again.\n";
        }
    }

    return 0;
}
