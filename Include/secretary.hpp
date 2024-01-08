#pragma once

#include <vector>
#include "course.hpp"
class Person;
class Semester;

using namespace std;

class Secretary {
private:  
    bool SemesterStart = false;
    string department;
    vector<Person *> myVec;  // we choose to use a vector instead of a map since we would like to be able 
                             // to search with all Person's properties using linear iteration. Since all takes place
                             // in memory there is no significant performance hit to go through every Person  ( O(n) )
    vector<Semester> semesters;
    public:
    Secretary(const string& dep);
    Secretary();
    ~Secretary();
    Secretary(const Secretary& sec);

    void addPerson(Person& p, bool printStatement = true);

    Person* findPersonByFirstName(const string& name);
    Person* findPersonByLastName(const string& name);
    Person* findPersonById(const string& id);
    Person* findPerson(Person& p);

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

    Semester* getSemester(int num);

    void addCourse(const Course& c, int semester);

    bool startSemester();

    void printMenu();

    void SecretaryOperation();
};
