#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <utility>
#include <unordered_map>
#include "json.hpp"

#include "course.hpp"
class Course;

using json = nlohmann::json;
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

    friend ostream& operator<<(ostream& os, const Person& p); 
    friend istream& operator>>(istream& is, Person& p);
};

class Student : public Person {
private:
    //Semester* currentSemester;
    int currentSemester;
    int registrationYear;
    int currentPoints = 0;
    int mandatoryPassed = 0;
    unordered_map<Course*, int> coursesWithGrades;

public:

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(Student,
                                   firstName,
                                   lastName,
                                   idCode,
                                   registrationYear)

    Student();
    Student(string fName, string lName, string id, int regYear);
    Student(const Person& p)
    : Person(p)
    {}

    virtual Student* clone() override;

    //for now we just check equality of the super-class Person
    virtual bool equals(Student* s);

    void setReg(int regYear){
        registrationYear = regYear;
    }

    int getSemesterCount();
    int getReg(){
        return registrationYear;
    }
    int getAcademicPoints();
    int getMandatoryPassed(){
        return mandatoryPassed;
    }

    void incrAcademicPoints(int p);

    void studAddCourse(Course& course);
    void studentChangeGrade(Course& course);

    void printGrades();

    void addCourseWithGrade(Course* course, int grade);

    int getCourseGrade(Course* course);

};

struct StudentCourseInstance {
    Student* stud;
    int grade = -1;

    nlohmann::json to_json() const {
        return nlohmann::json{
            {"student", stud ? *stud : Student{}},
            {"grade", grade}
        };
    }

    // Declare from_json as a friend function
    friend void from_json(const nlohmann::json& j, StudentCourseInstance& sci);
};



class Professor : public Person {
private:
    vector<Course> profCourses;

public:
    NLOHMANN_DEFINE_TYPE_INTRUSIVE(Professor,
                                   firstName,
                                   lastName,
                                   idCode)

    Professor();
    Professor(string fName, string lName, string id);
    Professor(const Person& p)
    : Person(p)
    {}

    virtual Professor* clone() override;

    //for now we just check equality of the super-class Person
    virtual bool equals(Professor* f);

    void profAddCourse(Course& course);
    bool teachesCourse(Course& course);

    void printStats(int sem);

};