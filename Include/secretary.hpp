#pragma once

#include <vector>
#include "course.hpp"
class Person;

using namespace std;

class Secretary {
private:  
    string department;
    int semesters;
    bool endSemester;
    vector<Person *> myVec;  // we choose to use a vector instead of a map since we would like to be able 
                             // to search with all Person's properties using linear iteration. Since all takes place
                             // in memory there is no significant performance hit to go through every Person  ( O(n) )
    vector<Course> courses;  
    public:
    Secretary(const string& dep, int sem);
    Secretary();
    ~Secretary();
    Secretary(const Secretary& sec);

    void addPerson(Person& p, bool printStatement = true);

    Person* findPersonByFirstName(const string& name);
    Person* findPersonByLastName(const string& name);
    Person* findPersonById(const string& id);
    Person* findPerson(Person& p);

    Course* findCourse(string name);

    bool removePerson(Person& p);

    void printSecSize();
    void setSecName(const string& dep);
    const string& getSecName();

    static bool isStudent(Person *p);
    static bool isProfessor(Person *p);

    Secretary& operator+(Person& p);
    Secretary& operator+=(const Secretary& sec);
    Secretary& operator=(const Secretary& sec);

    friend ostream& operator<<(ostream& os, Secretary& secretary);
    friend istream& operator>>(istream& is, Secretary& secretary);

    void addProfessor();
    void addStudent();

    void changeSemester(Course&  course);
    void modifyCourse(Course& course);

    void addCourse();
    void removeCourse(Course& course);

    void printMenu();
    void printExamsMenu();

    void SecretaryOperation();

    Student* readAndFindStudent();
    Professor* readAndFindProfessor();
    Course* readAndFindCourse();

    void nextSemester();

    void readStudentsFromFile();
    void readProfessorsFromFile();
    void readCourseFromFile();
};
