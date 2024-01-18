#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <utility>
#include <unordered_map>
#include "json.hpp"

#include "course.hpp"
class Course;
class Semester;

using json = nlohmann::json;
using namespace std;

struct SemesterGradeInstance {
    int grade, year;
    bool isWinter;
    NLOHMANN_DEFINE_TYPE_INTRUSIVE(SemesterGradeInstance,
                                grade,
                                year,
                                isWinter)

};

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
    int currentSemester;
    int registrationYear;
    int currentPoints = 0;
    int mandatoryPassed = 0;
    unordered_map<string, SemesterGradeInstance*> coursesWithGrades;

public:
    Student();
    Student(string fName, string lName, string id, int regYear);
    Student(const Person& p)
    : Person(p)
    {}

    ~Student(){}

    void deleteCoursesWithGrades();

    virtual Student* clone() override;

    //for now we just check equality of the super-class Person
    virtual bool equals(Student* s);

    void setReg(int regYear){
        registrationYear = regYear;
    }

    int getSemesterCount();
    int getReg() {return registrationYear;}
    int getAcademicPoints();
    int getMandatoryPassed() {return mandatoryPassed;}

    void incrAcademicPoints(int p);

    void studAddCourse(Course& course);
    void studentChangeGrade(Course& course);

    void printGrades();

    void addCourseWithGrade(Course* course, SemesterGradeInstance* semGrade);

    int getCourseGrade(Course* course);

    void to_json(json& j, const Student& student, bool printCoursesWithGrades = true) {
        j = json{
            {"firstName", student.firstName},
            {"lastName", student.lastName},
            {"idCode", student.idCode},
            {"registrationYear", student.registrationYear}
            // Add other fields as necessary
        };

        if (printCoursesWithGrades == true){
            // Serialize coursesWithGrades
            json coursesJson;
            for (const auto& pair : student.coursesWithGrades) {
                const std::string& courseName = pair.first;
                const SemesterGradeInstance* gradeInstance = pair.second;
                coursesJson[courseName] = *gradeInstance;
            }
            j["coursesWithGrades"] = coursesJson;
        }
    }

    friend void from_json(const json& j, Student& student);

    unordered_map<string, SemesterGradeInstance*> getCoursesWithGrades();

    void eraseCourse(string name);

};

struct StudentCourseInstance {
    Student* stud;
    int grade = -1;

    nlohmann::json to_json() const {
        nlohmann::json j = {
            {"grade", grade}
        };

        nlohmann::json studentJson;
        stud->to_json(studentJson, *stud, false);
        j["student"] = studentJson;   
        return j;
    }

    // Declare from_json as a friend function
    friend void from_json(const nlohmann::json& j, StudentCourseInstance& sci);
};



class Professor : public Person {
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

};