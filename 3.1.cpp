#include <iostream>
#include <string>
#include <vector>


class Account {
protected:
    std::string accountNumber;
    std::string ownerName;
    double balance;
public:
    Account(std::string number, std::string name, double initialBalance)
        : accountNumber(number), ownerName(name), balance(initialBalance) {}

    
    std::string getAccountNumber() const {
        return accountNumber;
    }

    virtual void deposit(double amount) {
        if (amount > 0) {
            balance += amount;
            std::cout << "Da nap " << amount << " vao tai khoan." << std::endl;
        } else {
            std::cout << "So tien khong hop le." << std::endl;
        }
    }

    virtual bool withdraw(double amount) {
        if (amount > 0 && balance >= amount) {
            balance -= amount;
            std::cout << "Da rut " << amount << " tu tai khoan." << std::endl;
            return true;
        } else {
            std::cout << "Khong the rut tien. Vuot qua gioi han hoac so tien khong hop le." << std::endl;
            return false;
        }
    }

    virtual void displayInfo() const {
        std::cout << "So tai khoan: " << accountNumber << std::endl;
        std::cout << "Chu tai khoan: " << ownerName << std::endl;
        std::cout << "So du: " << balance << std::endl;
    }

    bool transfer(Account* toAccount, double amount) {
        if (this->withdraw(amount)) {
            toAccount->deposit(amount);
            std::cout << "Da chuyen " << amount << " tu tai khoan " << this->accountNumber 
                      << " sang tai khoan " << toAccount->getAccountNumber() << std::endl;
            return true;
        }
        return false;
    }

    virtual ~Account() {}
};

// L?p SavingsAccount k? th?a t? Account
class SavingsAccount : public Account {
private:
    double interestRate;
public:
    SavingsAccount(std::string number, std::string name, double initialBalance, double rate)
        : Account(number, name, initialBalance), interestRate(rate) {}

    void applyInterest() {
        double interest = balance * interestRate;
        deposit(interest);
        std::cout << "Da cong lai: " << interest << std::endl;
    }

    void displayInfo() const override {
        Account::displayInfo();
        std::cout << "Lai suat: " << (interestRate * 100) << "%" << std::endl;
    }
};

// L?p CheckingAccount k? th?a t? Account
class CheckingAccount : public Account {
private:
    double overdraftLimit;
public:
    CheckingAccount(std::string number, std::string name, double initialBalance, double limit)
        : Account(number, name, initialBalance), overdraftLimit(limit) {}

    bool withdraw(double amount) override {
        if (amount > 0 && (balance + overdraftLimit) >= amount) {
            balance -= amount;
            std::cout << "Da rut " << amount << " tu tai khoan." << std::endl;
            return true;
        } else {
            std::cout << "Khong the rut tien. Vuot qua gioi han thau chi hoac so tien khong hop le." << std::endl;
            return false;
        }
    }

    void displayInfo() const override {
        Account::displayInfo();
        std::cout << "Gioi han thau chi: " << overdraftLimit << std::endl;
    }
};

// L?p FixedDepositAccount k? th?a t? Account
class FixedDepositAccount : public Account {
private:
    int term; // th?i h?n tính theo tháng
    double interestRate;
public:
    FixedDepositAccount(std::string number, std::string name, double initialBalance, int _term, double rate)
        : Account(number, name, initialBalance), term(_term), interestRate(rate) {}

    void applyInterest() {
        if (term <= 0) {
            double interest = balance * interestRate;
            deposit(interest);
            std::cout << "Da cong lai: " << interest << std::endl;
        } else {
            std::cout << "Tai khoan chua den han." << std::endl;
        }
    }

    void reduceTerm(int months) {
        term -= months;
        if (term < 0) term = 0;
    }

    void displayInfo() const override {
        Account::displayInfo();
        std::cout << "Thoi han con lai: " << term << " thang" << std::endl;
        std::cout << "Lai suat: " << (interestRate * 100) << "%" << std::endl;
    }
};

// L?p Bank qu?n l? tài kho?n
class Bank {
private:
    std::vector<Account*> accounts;
public:
    void addAccount(Account* account) {
        accounts.push_back(account);
    }

    void displayAllAccounts() const {
        for (const auto& account : accounts) {
            account->displayInfo();
            std::cout << "------------------------" << std::endl;
        }
    }

    Account* findAccount(const std::string& accountNumber) const {
        for (const auto& account : accounts) {
            if (account->getAccountNumber() == accountNumber) {
                return account;
            }
        }
        return nullptr;
    }

    ~Bank() {
        for (auto account : accounts) {
            delete account;
        }
    }
};

// Hi?n th? menu cho ngý?i dùng
void displayMenu() {
    std::cout << "================ MENU ================" << std::endl;
    std::cout << "1. Hien thi tat ca tai khoan" << std::endl;
    std::cout << "2. Nap tien vao tai khoan" << std::endl;
    std::cout << "3. Rut tien tu tai khoan" << std::endl;
    std::cout << "4. Chuyen tien" << std::endl;
    std::cout << "5. Thoat" << std::endl;
    std::cout << "======================================" << std::endl;
}

// Giao di?n ngý?i dùng
void userInterface(Bank& bank) {
    int choice;
    do {
        displayMenu();
        std::cout << "Nhap lua chon: ";
        std::cin >> choice;

        if (choice == 1) {
            bank.displayAllAccounts();
        } 
        else if (choice == 2) {
            std::string accountNumber;
            double amount;
            std::cout << "Nhap so tai khoan: ";
            std::cin >> accountNumber;
            std::cout << "Nhap so tien can nap: ";
            std::cin >> amount;

            Account* account = bank.findAccount(accountNumber);
            if (account) {
                account->deposit(amount);
            } else {
                std::cout << "Khong tim thay tai khoan." << std::endl;
            }
        } 
        else if (choice == 3) {
            std::string accountNumber;
            double amount;
            std::cout << "Nhap so tai khoan: ";
            std::cin >> accountNumber;
            std::cout << "Nhap so tien can rut: ";
            std::cin >> amount;

            Account* account = bank.findAccount(accountNumber);
            if (account) {
                account->withdraw(amount);
            } else {
                std::cout << "Khong tim thay tai khoan." << std::endl;
            }
        } 
        else if (choice == 4) {
            std::string fromAccountNumber, toAccountNumber;
            double amount;
            std::cout << "Nhap so tai khoan gui: ";
            std::cin >> fromAccountNumber;
            std::cout << "Nhap so tai khoan nhan: ";
            std::cin >> toAccountNumber;
            std::cout << "Nhap so tien can chuyen: ";
            std::cin >> amount;

            Account* fromAccount = bank.findAccount(fromAccountNumber);
            Account* toAccount = bank.findAccount(toAccountNumber);

            if (fromAccount && toAccount) {
                fromAccount->transfer(toAccount, amount);
            } else {
                std::cout << "Khong tim thay mot trong hai tai khoan." << std::endl;
            }
        }
    } while (choice != 5);
}

int main() {
    Bank myBank;
    myBank.addAccount(new SavingsAccount("SA001", "Nguyen Van A", 1000000, 0.05));
    myBank.addAccount(new CheckingAccount("CA001", "Tran Thi B", 2000000, 500000));
    myBank.addAccount(new FixedDepositAccount("FD001", "Le Van C", 5000000, 12, 0.07));

    userInterface(myBank);
    return 0;
}


