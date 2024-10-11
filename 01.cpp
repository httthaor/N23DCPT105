#include <iostream>
#include <string>
using namespace std;
class Student {
private:
string name;
int age;
float gpa;
public:
Student(string n, int a, float g) : name(n), age(a), gpa(g) {}
string getName() const { return name; }
int getAge() const { return age; }
float getGPA() const { return gpa; }

void setName(string n) { name = n; }
void setAge(int a) { age = a; }
void setGPA(float g) { gpa = g; }

void displayInfo() const {
cout << "Ten: " << name << endl;
cout << "Tuoi: " << age << endl;
cout << "GPA: " << gpa << endl;
}
};
int main() {
Student s1("Nguyen Van A", 20, 3.5);

cout << "Thong tin sinh vien:" << endl;
s1.displayInfo();

// Thay ð?i GPA c?a sinh viên
s1.setGPA(3.8);
// Hi?n th? thông tin sau khi c?p nh?t
cout << "\n Thong tin sau khi cap nhat GPA:" << endl;
s1.displayInfo();
return 0;
}
