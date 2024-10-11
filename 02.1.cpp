#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <fstream>
#include <sstream>

class Employee {
protected:
    int id;
    std::string name;
    int age;
    double salary;
public:
    Employee(int _id, std::string _name, int _age, double _salary)
        : id(_id), name(_name), age(_age), salary(_salary) {}

    virtual void displayInfo() const {
        std::cout << "ID: " << id << ", Ten: " << name
                  << ", Tuoi: " << age << ", Luong: " << salary << std::endl;
    }

    virtual double getSalary() const {
        return salary;
    }

    void increaseSalary(double amount) {
        salary += amount;
    }

    friend class Company;
};

class Manager : public Employee {
private:
    int teamSize;
public:
    Manager(int _id, std::string _name, int _age, double _salary, int _teamSize)
        : Employee(_id, _name, _age, _salary), teamSize(_teamSize) {}

    void displayInfo() const override {
        Employee::displayInfo();
        std::cout << "So nhan vien quan ly: " << teamSize << std::endl;
    }

    double getSalary() const override {
        return salary + (teamSize * 1000);
    }
};

class HourlyEmployee : public Employee {
private:
    int hoursWorked;
    double hourlyRate;
public:
    HourlyEmployee(int _id, std::string _name, int _age, int _hoursWorked, double _hourlyRate)
        : Employee(_id, _name, _age, 0), hoursWorked(_hoursWorked), hourlyRate(_hourlyRate) {}

    double getSalary() const override {
        return hoursWorked * hourlyRate;
    }

    void displayInfo() const override {
        Employee::displayInfo();
        std::cout << "So gio lam viec: " << hoursWorked << ", Luong theo gio: " << hourlyRate << std::endl;
    }
};

class Company {
private:
    std::vector<Employee*> employees;
public:
    void addEmployee(Employee* emp) {
        employees.push_back(emp);
    }

    void displayAllEmployees() const {
        for (const auto& emp : employees) {
            emp->displayInfo();
            std::cout << "------------------------" << std::endl;
        }
    }

    Employee* findEmployeeByID(int id) const {
        for (const auto& emp : employees) {
            if (emp->id == id) {
                return emp;
            }
        }
        return nullptr;
    }

    Employee* findEmployeeByName(const std::string& name) const {
        for (const auto& emp : employees) {
            if (emp->name == name) {
                return emp;
            }
        }
        return nullptr;
    }

    void sortEmployeesBySalary() {
        std::sort(employees.begin(), employees.end(), [](Employee* a, Employee* b) {
            return a->getSalary() > b->getSalary();
        });
    }

    void sortEmployeesByAge() {
        std::sort(employees.begin(), employees.end(), [](Employee* a, Employee* b) {
            return a->age > b->age;
        });
    }

    double getTotalSalary() const {
        double total = 0;
        for (const auto& emp : employees) {
            total += emp->getSalary();
        }
        return total;
    }

    void saveToFile(const std::string& filename) const {
        std::ofstream file(filename);
        if (file.is_open()) {
            for (const auto& emp : employees) {
                file << emp->id << "," << emp->name << "," << emp->age << "," << emp->getSalary() << std::endl;
            }
            file.close();
        }
    }

    void loadFromFile(const std::string& filename) {
        std::ifstream file(filename);
        if (file.is_open()) {
            employees.clear();
            std::string line;
            while (std::getline(file, line)) {
                std::stringstream ss(line);
                int id, age;
                double salary;
                std::string name;
                ss >> id >> name >> age >> salary;
                employees.push_back(new Employee(id, name, age, salary));
            }
            file.close();
        }
    }

    ~Company() {
        for (auto emp : employees) {
            delete emp;
        }
    }
};

int main() {
    Company myCompany;
    myCompany.addEmployee(new Employee(1, "Nguyen Van A", 30, 10000000));
    myCompany.addEmployee(new Employee(2, "Tran Thi B", 25, 8000000));
    myCompany.addEmployee(new Manager(3, "Le Van C", 40, 20000000, 5));
    myCompany.addEmployee(new HourlyEmployee(4, "Pham Thi D", 22, 120, 100000));

    std::cout << "Thong tin tat ca nhan vien:" << std::endl;
    myCompany.displayAllEmployees();

    std::cout << "Tong luong cong ty: " << myCompany.getTotalSalary() << std::endl;

    std::cout << "Sap xep theo luong:" << std::endl;
    myCompany.sortEmployeesBySalary();
    myCompany.displayAllEmployees();

    std::cout << "Luu danh sach nhan vien vao file." << std::endl;
    myCompany.saveToFile("employees.txt");

    return 0;
}

