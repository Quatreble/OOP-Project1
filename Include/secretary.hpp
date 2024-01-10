#pragma once

#include <vector>
#include <chrono>
#include <thread>
#include <fstream>
#include <sstream>

#include "person.hpp"
#include "course.hpp"

class Person;

using namespace std;

class Secretary {
private:  
    string depName;
    int depSemesters;
    bool endSemester;
    vector<Person *> depMembers;  // we choose to use a vector instead of a map since we would like to be able 
                             // to search with all Person's properties using linear iteration. Since all takes place
                             // in memory there is no significant performance hit to go through every Person  ( O(n) )
    vector<Course> depCourses;  
    int pointsToGraduate;
    int numOfMandatory = 0;

public:
    Secretary(const string& dep, int sem, int reqPoints);
    Secretary();
    ~Secretary();
    Secretary(const Secretary& sec);

    void addPerson(Person& p, bool printStatement = true);
    bool removePerson(Person& p);

    void addProfessor();
    void addStudent();

    Person* findPersonByFirstName(const string& name);
    Person* findPersonByLastName(const string& name);
    Person* findPersonById(const string& id);
    Person* findPerson(Person& p);

    Course* findCourse(string name);


    void printSecSize();
    void setSecName(const string& dep);

    const string& getSecName();
    int getNumOfMandatory(){
        return numOfMandatory;
    }

    static bool isStudent(Person *p);
    static bool isProfessor(Person *p);

    Secretary& operator+(Person& p);
    Secretary& operator+=(const Secretary& sec);
    Secretary& operator=(const Secretary& sec);

    friend ostream& operator<<(ostream& os, Secretary& secretary);
    friend istream& operator>>(istream& is, Secretary& secretary);


    void changeSemester(Course&  course);
    void modifyCourse(Course& course);
    void nextSemester();

    void addCourse();
    void removeCourse(Course& course);

    void printMenu();
    void printExamsMenu();
    void printGraduates();

    void SecretaryOperation();

    Student* readAndFindStudent();
    Professor* readAndFindProfessor();
    Course* readAndFindCourse();

    void readStudentsFromFile();
    void readProfessorsFromFile();
    void readCourseFromFile();
};
