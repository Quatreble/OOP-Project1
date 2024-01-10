#pragma once

#include <iostream>
#include <string>
#include "course.hpp"
#include <vector>
#include <utility>
class Secretary;

using namespace std;

class Person {
protected:
    static int pCount; //counter for every Person object that is instantiated 
    string firstName;
    string lastName;  //properties of a Person object
    string idCode;
public:
    Person();
    Person(string fName, string lName, string id);
    Person(const Person& p) 
    : firstName(p.firstName), lastName(p.lastName), idCode(p.idCode)
    {}

    virtual ~Person();

    static int getCount();

    void setFirstName(const string& name);
    void setLastName(const string& name);
    void setIdCode(const string& id);

    string getFirstName();
    string getLastName();
    string getIdCode();

    //sub-classes should override this and add logic specific to their fields
    virtual bool equals(Person* p);

    //sub-classes should implement that so we can dynamically create copies
    virtual Person* clone() = 0;

    friend ostream& operator<<(std::ostream& os, const Person& p); 
    friend istream& operator>>(std::istream& is, Person& p);
};

class Student : public Person {
private:
    //Semester* currSemester;
    int currSem;
    vector<pair<Course, int>> studCourses;
    int currPoints = 0;
    int mandatoryPassed = 0;
public:
    Student();
    Student(string fName, string lName, string id);
    Student(const Person& p)
    : Person(p)
    {}

    virtual Student* clone() override;
    void setSemester(int sem, bool next = false);


    //for now we just check equality of the super-class Person
    virtual bool equals(Student* s);

    int getSemesterCount();
    void incAcademicPoints(int p);
    int getAcademicPoints();
    void printGrades(bool semesterOnly = false);
    int getMandatoryPassed(){
        return mandatoryPassed;
    }

    void studAddCourse(Course& course);
    void studentChangeGrade(Course& course);

    friend istream& operator>>(std::istream& is, Person& p);
    
    void printGradesToto();
};

class Professor : public Person {
private:
    std::vector<Course> profCourses;
public:
    Professor();
    Professor(string fName, string lName, string id);
    Professor(const Person& p)
    : Person(p)
    {}



    virtual Professor* clone() override;
    void profAddCourse(Course& course);

    bool teachesCourse(Course& course);
    void printStats(int sem);

    //for now we just check equality of the super-class Person
    virtual bool equals(Professor* f);
};