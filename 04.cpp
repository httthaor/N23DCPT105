#include <iostream>
#include <string>
#include <vector>
#include <memory>  // Thêm thý vi?n này
#include <algorithm>
#include <fstream>
#include <sstream>
#include <map>
#include <stdexcept>

class Borrowable {
public:
    virtual void borrow() = 0;
    virtual void returnItem() = 0;
    virtual bool isAvailable() const = 0;
    virtual std::string getTitle() const = 0;
    virtual ~Borrowable() = default;
};

class Book : public Borrowable {
private:
    std::string title;
    std::string author;
    std::string ISBN;
    bool available;
public:
    Book(std::string t, std::string a, std::string i)
        : title(std::move(t)), author(std::move(a)), ISBN(std::move(i)), available(true) {}

    void borrow() override {
        if (available) {
            available = false;
            std::cout << "Sach " << title << " da duoc muon" << std::endl;
        } else {
            std::cout << "Sach " << title << " khong co san de muon." << std::endl;
        }
    }

    void returnItem() override {
        available = true;
        std::cout << "Sach " << title << " da duoc tra lai." << std::endl;
    }

    bool isAvailable() const override {
        return available;
    }

    std::string getTitle() const override { return title; }
    std::string getAuthor() const { return author; }
    std::string getISBN() const { return ISBN; }
};

class DVD : public Borrowable {
private:
    std::string title;
    std::string director;
    int duration;
    bool available;
public:
    DVD(std::string t, std::string d, int dur)
        : title(std::move(t)), director(std::move(d)), duration(dur), available(true) {}

    void borrow() override {
        if (available) {
            available = false;
            std::cout << "DVD " << title << " da duoc muon." << std::endl;
        } else {
            std::cout << "DVD " << title << " khong co san de muon." << std::endl;
        }
    }

    void returnItem() override {
        available = true;
        std::cout << "DVD " << title << " da duoc tra lai." << std::endl;
    }

    bool isAvailable() const override {
        return available;
    }

    std::string getTitle() const override { return title; }
    
    std::string getDirector() const { return director; } // Thêm phýõng th?c l?y tên ð?o di?n
    
    int getDuration() const { return duration; } // Thêm phýõng th?c l?y th?i gian
};

class Magazine : public Borrowable {
private:
    std::string title;
    std::string issueDate;
    bool available;
public:
    Magazine(std::string t, std::string date)
        : title(std::move(t)), issueDate(std::move(date)), available(true) {}

    void borrow() override {
        if (available) {
            available = false;
            std::cout << "Tap chi " << title << " da duoc muon." << std::endl;
        } else {
            std::cout << "Tap chi " << title << " khong co san de muon." << std::endl;
        }
    }

    void returnItem() override {
        available = true;
        std::cout << "Tap chi " << title << " da duoc tra lai." << std::endl;
    }

    bool isAvailable() const override {
        return available;
    }

    std::string getTitle() const override { return title; }

    std::string getIssueDate() const { return issueDate; } // Thêm phýõng th?c l?y ngày phát hành
};

class User {
private:
    std::string username;
    std::string password;
    int borrowedItems;
    const int maxBorrow = 5;
public:
    User(std::string u, std::string p)
        : username(std::move(u)), password(std::move(p)), borrowedItems(0) {}

    bool login(const std::string& u, const std::string& p) const {
        return username == u && password == p;
    }

    void borrowItem() {
        if (borrowedItems < maxBorrow) {
            borrowedItems++;
            std::cout << username << " da muon mot cuc. Tong so cuc da muon: " << borrowedItems << std::endl;
        } else {
            throw std::runtime_error("Vuot qua gioi han muon.");
        }
    }

    void returnItem() {
        if (borrowedItems > 0) {
            borrowedItems--;
            std::cout << username << " da tra mot cuc. Tong so muc con lai: " << borrowedItems << std::endl;
        }
    }

    int getBorrowedItems() const {
        return borrowedItems;
    }

    std::string getUsername() const {
        return username;
    }
};

class Library {
private:
    std::vector<std::unique_ptr<Borrowable>> items;
public:
    void addItem(std::unique_ptr<Borrowable> item) {
        items.push_back(std::move(item));
    }

    void searchByAuthor(const std::string& author) {
        for (const auto& item : items) {
            if (auto book = dynamic_cast<Book*>(item.get())) {
                if (book->getAuthor() == author) {
                    std::cout << "Tim thay sach: " << book->getTitle() << " cua tac gia " << author << std::endl;
                }
            }
        }
    }

    void sortItems() {
        std::sort(items.begin(), items.end(), [](const std::unique_ptr<Borrowable>& a, const std::unique_ptr<Borrowable>& b) {
            return a->getTitle() < b->getTitle();
        });
        std::cout << "Sap xep muc hoan thanh." << std::endl;
    }

    void borrowItem(const std::string& title) {
        auto it = std::find_if(items.begin(), items.end(), [&title](const auto& item) {
            return item->getTitle() == title && item->isAvailable();
        });

        if (it != items.end()) {
            (*it)->borrow();
        } else {
            std::cout << "Khong tim thay muc " << title << " trong thu vien hoac khong co san." << std::endl;
        }
    }

    void returnItem(const std::string& title) {
        auto it = std::find_if(items.begin(), items.end(), [&title](const auto& item) {
            return item->getTitle() == title;
        });

        if (it != items.end()) {
            (*it)->returnItem();
        } else {
            std::cout << "Khong tim thay muc " << title << " trong thu vien" << std::endl;
        }
    }

    void saveToFile(const std::string& filename) {
        std::ofstream file(filename);
        for (const auto& item : items) {
            if (auto book = dynamic_cast<Book*>(item.get())) {
                file << "Book," << book->getTitle() << "," << book->getAuthor() << "," << book->getISBN() << "\n";
            } else if (auto dvd = dynamic_cast<DVD*>(item.get())) {
                file << "DVD," << dvd->getTitle() << "," << dvd->getDirector() << "," << dvd->getDuration() << "\n";
            } else if (auto magazine = dynamic_cast<Magazine*>(item.get())) {
                file << "Magazine," << magazine->getTitle() << "," << magazine->getIssueDate() << "\n";
            }
        }
    }

    void loadFromFile(const std::string& filename) {
        std::ifstream file(filename);
        std::string line;
        while (std::getline(file, line)) {
            std::stringstream ss(line);
            std::string type;
            std::getline(ss, type, ',');
            if (type == "Book") {
                std::string title, author, isbn;
                std::getline(ss, title, ',');
                std::getline(ss, author, ',');
                std::getline(ss, isbn);
                addItem(std::make_unique<Book>(title, author, isbn));
            } else if (type == "DVD") {
                std::string title, director;
                int duration;
                std::getline(ss, title, ',');
                std::getline(ss, director, ',');
                ss >> duration;
                addItem(std::make_unique<DVD>(title, director, duration));
            } else if (type == "Magazine") {
                std::string title, issueDate;
                std::getline(ss, title, ',');
                std::getline(ss, issueDate);
                addItem(std::make_unique<Magazine>(title, issueDate));
            }
        }
    }
};

void displayMenu() {
    std::cout << "==== MENU ====\n";
    std::cout << "1. Tim kiem theo tac gia\n";
    std::cout << "2. Muon sach\n";
    std::cout << "3. Tra sach\n";
    std::cout << "4. Sap xep cac muc\n";
    std::cout << "5. Thoat\n";
    std::cout << "==============================================\n";
}

void userInterface(Library& library, User& user) {
    int choice;
    do {
        displayMenu();
        std::cout << "Nhap lua chon cua ban: ";
        std::cin >> choice;

        switch (choice) {
        case 1: {
            std::string author;
            std::cout << "Nhap ten tac gia: ";
            std::cin.ignore();
            std::getline(std::cin, author);
            library.searchByAuthor(author);
            break;
        }
        case 2: {
            std::string title;
            std::cout << "Nhap ten sach ban muon muon: ";
            std::cin.ignore();
            std::getline(std::cin, title);
            try {
                user.borrowItem();
                library.borrowItem(title);
            } catch (const std::runtime_error& e) {
                std::cout << "Loi: " << e.what() << std::endl;
            }
            break;
        }
        case 3: {
            std::string title;
            std::cout << "Nhap ten sach ban muon tra: ";
            std::cin.ignore();
            std::getline(std::cin, title);
            library.returnItem(title);
            user.returnItem();
            break;
        }
        case 4:
            library.sortItems();
            break;
        case 5:
            std::cout << "Thoat chuong trinh.\n";
            break;
        default:
            std::cout << "Lua chon khong hop le.\n";
        }
    } while (choice != 5);
}

int main() {
    Library library;
    library.addItem(std::unique_ptr<Borrowable>(new Book("Cuon sach A", "Tac gia A", "ISBN123")));
    library.addItem(std::unique_ptr<Borrowable>(new DVD("Phim B", "Dao dien B", 120)));
    library.addItem(std::unique_ptr<Borrowable>(new Magazine("Tap chí C", "Tháng 10")));


    User user("ten", "matkhau");

    library.loadFromFile("library_data.txt");

    std::string username, password;
    std::cout << "Nhap ten dang nhap: ";
    std::cin >> username;
    std::cout << "Nhap mat khau: ";
    std::cin >> password;

    if (user.login(username, password)) {
        std::cout << "Dang nhap thanh cong!\n";
        userInterface(library, user);
    } else {
        std::cout << "Dang nhap that bai.\n";
    }

    library.saveToFile("library_data.txt");

    return 0;
}

  

