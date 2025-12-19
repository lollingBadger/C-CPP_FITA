#include <iostream>
#include <string>

using namespace std;

/* =======================
   Base Class: Student
   ======================= */
class Student {
protected:
    string name;

public:
    Student(const string& n) : name(n) {
        cout << "Student created: " << name << endl;
    }

    // 🔑 Virtual destructor is REQUIRED
     virtual ~Student() {
         cout << "Student base destructor called: " << name << endl;
     }

    virtual void display() const =0;
};

/* =======================
   Derived Class: SchoolStudent
   ======================= */
class SchoolStudent : public Student {
public:
    SchoolStudent(const string& n) : Student(n) {
        
        std::cout << "student Base object Created" << std::endl;
    }

    ~SchoolStudent() {
        cout << "SchoolStudent destructor called: " << name << endl;
    }

    void display() const {
        cout << "School Student: " << name << endl;
    }
};

/* =======================
   Derived Class: CollegeStudent
   ======================= */
class CollegeStudent : public Student {
public:
    CollegeStudent(const string& n) : Student(n) {}

    ~CollegeStudent() {
        cout << "CollegeStudent destructor called: " << name << endl;
    }

    void display() const {
        cout << "College Student: " << name << endl;
     }
};

/* =======================
   Derived Class: WorkingProfessional
   ======================= */
class WorkingProfessional : public Student {
public:
    WorkingProfessional(const string& n) : Student(n) {}

    ~WorkingProfessional() {
        cout << "WorkingProfessional destructor called: " << name << endl;
    }

    void display() const {
        cout << "Working Professional: " << name << endl;
    }
};

/* =======================
   Class: englishClasssBatch
   Uses RAW POINTERS intentionally
   ======================= */
class englishClasssBatch {
private:
    Student** students;
    int count;

public:
    englishClasssBatch(int c) : count(c) {
        students = new Student*[count];
        cout << "English class batch created." << endl;
    }

    void addStudent(int index, Student* student) {
        students[index] = student;
    }

     void callDisplay() {
        for (int i = 0; i < count; i++) {
            students[i]->display();  //
        }
    }
    ~englishClasssBatch() {
        cout << "Destroying English class batch..." << endl;

        for (int i = 0; i < count; i++) {
            delete students[i];  //
        }

        delete[] students;
        cout << "English class batch destroyed." << endl;
    }
};

/* =======================
   main()
   ======================= */
int main() {
    englishClasssBatch batch(3);

    batch.addStudent(0, new SchoolStudent("Alice"));
    batch.addStudent(1, new CollegeStudent("Bob"));
    batch.addStudent(2, new WorkingProfessional("Charlie"));
    batch.callDisplay();
    return 0;  // batch destructor called automatically
}
