#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
using namespace std;

class BankAccount {
protected:
    int accountNumber;
    string accountHolderName;
    double balance;

public:
    BankAccount(int accNum, const string& holderName, double initialBalance)
        : accountNumber(accNum), accountHolderName(holderName), balance(initialBalance) {}

    virtual ~BankAccount() {}

    virtual void deposit(double amount) {
        if (amount > 0) {
            balance += amount;
            cout << "Deposited: " << fixed << setprecision(2) << amount << ". New balance: " << balance << endl;
        } else {
            cout << "Invalid deposit amount.\n";
        }
    }

    virtual void withdraw(double amount) {
        if (amount > 0 && amount <= balance) {
            balance -= amount;
            cout << "Withdrawn: " << fixed << setprecision(2) << amount << ". New balance: " << balance << endl;
        } else {
            cout << "Insufficient balance or invalid amount.\n";
        }
    }

    double getBalance() const {
        return balance;
    }

    virtual void displayAccountInfo() const {
        cout << "Account Number: " << accountNumber << endl;
        cout << "Account Holder: " << accountHolderName << endl;
        cout << "Balance: " << fixed << setprecision(2) << balance << endl;
    }

    virtual void calculateInterest() const {
        cout << "Interest calculation not applicable for this account type.\n";
    }
};

class SavingsAccount : public BankAccount {
private:
    double interestRate; 

public:
    SavingsAccount(int accNum, const string& holderName, double initialBalance, double rate)
        : BankAccount(accNum, holderName, initialBalance), interestRate(rate) {}

    void calculateInterest() const override {
        double interest = balance * (interestRate / 100.0);
        cout << "Savings Account Interest: " << fixed << setprecision(2) << interest << endl;
    }
};

class CheckingAccount : public BankAccount {
private:
    double overdraftLimit;

public:
    CheckingAccount(int accNum, const string& holderName, double initialBalance, double overdraft)
        : BankAccount(accNum, holderName, initialBalance), overdraftLimit(overdraft) {}

    void withdraw(double amount) override {
        if (amount > 0 && amount <= balance + overdraftLimit) {
            balance -= amount;
            cout << "Withdrawn: " << fixed << setprecision(2) << amount << ". New balance: " << balance << endl;
        } else {
            cout << "Withdrawal exceeds overdraft limit or invalid amount.\n";
        }
    }

    void displayAccountInfo() const override {
        BankAccount::displayAccountInfo();
        cout << "Overdraft Limit: " << fixed << setprecision(2) << overdraftLimit << endl;
    }
};

class FixedDepositAccount : public BankAccount {
private:
    int term;           
    double interestRate; 

public:
    FixedDepositAccount(int accNum, const string& holderName, double initialBalance, int duration, double rate)
        : BankAccount(accNum, holderName, initialBalance), term(duration), interestRate(rate) {}

    void calculateInterest() const override {
        double interest = balance * (interestRate / 100.0) * (term / 12.0);
        cout << "Fixed Deposit Interest: " << fixed << setprecision(2) << interest << endl;
    }

    void displayAccountInfo() const override {
        BankAccount::displayAccountInfo();
        cout << "Term: " << term << " months\n";
        cout << "Interest Rate: " << fixed << setprecision(2) << interestRate << "%" << endl;
    }
};

void displayMenu() {
    cout << "\n--- Banking System Menu ---\n";
    cout << "1. Create Savings Account\n";
    cout << "2. Create Checking Account\n";
    cout << "3. Create Fixed Deposit Account\n";
    cout << "4. Deposit Money\n";
    cout << "5. Withdraw Money\n";
    cout << "6. Display Account Info\n";
    cout << "7. Calculate Interest\n";
    cout << "8. Exit\n";
    cout << "Enter your choice: ";
}

int main() {
    vector<BankAccount*> accounts;
    int choice;

    do {
        displayMenu();
        cin >> choice;

        switch (choice) {
        case 1: {
            int accNum;
            string holderName;
            double initialBalance, interestRate;

            cout << "Enter Account Number: ";
            cin >> accNum;
            cin.ignore();
            cout << "Enter Account Holder Name: ";
            getline(cin, holderName);
            cout << "Enter Initial Balance: ";
            cin >> initialBalance;
            cout << "Enter Interest Rate (%): ";
            cin >> interestRate;

            accounts.push_back(new SavingsAccount(accNum, holderName, initialBalance, interestRate));
            cout << "Savings Account created successfully!\n";
            break;
        }
        case 2: {
            int accNum;
            string holderName;
            double initialBalance, overdraftLimit;

            cout << "Enter Account Number: ";
            cin >> accNum;
            cin.ignore();
            cout << "Enter Account Holder Name: ";
            getline(cin, holderName);
            cout << "Enter Initial Balance: ";
            cin >> initialBalance;
            cout << "Enter Overdraft Limit: ";
            cin >> overdraftLimit;

            accounts.push_back(new CheckingAccount(accNum, holderName, initialBalance, overdraftLimit));
            cout << "Checking Account created successfully!\n";
            break;
        }
        case 3: {
            int accNum, term;
            string holderName;
            double initialBalance, interestRate;

            cout << "Enter Account Number: ";
            cin >> accNum;
            cin.ignore();
            cout << "Enter Account Holder Name: ";
            getline(cin, holderName);
            cout << "Enter Initial Balance: ";
            cin >> initialBalance;
            cout << "Enter Term (in months): ";
            cin >> term;
            cout << "Enter Interest Rate (%): ";
            cin >> interestRate;

            accounts.push_back(new FixedDepositAccount(accNum, holderName, initialBalance, term, interestRate));
            cout << "Fixed Deposit Account created successfully!\n";
            break;
        }
        case 4: {
            int accNum;
            double amount;
            cout << "Enter Account Number: ";
            cin >> accNum;
            cout << "Enter Deposit Amount: ";
            cin >> amount;

            for (auto account : accounts) {
                if (account->getBalance() == accNum) {
                    account->deposit(amount);
                    break;
                }
            }
            break;
        }
        case 5: {
            int accNum;
            double amount;
            cout << "Enter Account Number: ";
            cin >> accNum;
            cout << "Enter Withdrawal Amount: ";
            cin >> amount;

            for (auto account : accounts) {
                if (account->getBalance() == accNum) {
                    account->withdraw(amount);
                    break;
                }
            }
            break;
        }
        case 6: {
            for (const auto& account : accounts) {
                account->displayAccountInfo();
                cout << "---------------------\n";
            }
            break;
        }
        case 7: {
            for (const auto& account : accounts) {
                account->calculateInterest();
                cout << "---------------------\n";
            }
            break;
        }
        case 8:
            cout << "Exiting the Banking System. Goodbye!\n";
            break;
        default:
            cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != 8);

    for (auto account : accounts) {
        delete account;
    }

    return 0;
}
