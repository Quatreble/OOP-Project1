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

    virtual ~Person(){}

    void setFirstName(const string& name) {firstName = name; }
    void setLastName(const string& name) {lastName = name; }
    void setIdCode(const string& id) {idCode = id; }

    string getFirstName() {return firstName; }
    string getLastName() {return lastName; }
    string getIdCode() {return idCode; }
    static int getCount() {return pCount; }


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
    Student():Person(){};
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

    int getSemesterCount(){return currentSemester; }
    int getReg() {return registrationYear;}
    int getAcademicPoints(){return currentPoints; }
    int getMandatoryPassed() {return mandatoryPassed;}

    void incrAcademicPoints(int p){currentPoints += p; }

    void studAddCourse(Course& course);
    void studentChangeGrade(Course& course);

    void printGrades();

    void addCourseWithGrade(Course* course, SemesterGradeInstance* semGrade);
    int getCourseGrade(Course* course);
    void eraseCourse(string name);

    void to_json(json& j, const Student& student, bool printCoursesWithGrades = true) {
        j = json{
            {"firstName", student.firstName},
            {"lastName", student.lastName},
            {"idCode", student.idCode},
            {"registrationYear", student.registrationYear},
            {"mandatoryPassed", student.mandatoryPassed},
            {"currentPoints", student.currentPoints}
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

    friend void from_json(const nlohmann::json& j, StudentCourseInstance& sci);
};



class Professor : public Person {
private:
    unordered_map <string, vector<pair<int,bool>>> profCourses;

public:

    Professor():Person(){};
    Professor(string fName, string lName, string id);
    Professor(const Person& p)
    : Person(p)
    {}

    virtual Professor* clone() override;

    //for now we just check equality of the super-class Person
    virtual bool equals(Professor* f);

    unordered_map<string, vector<pair<int,bool>>> getProfCourses(){return profCourses; }

    friend void to_json(json& j, const Professor& p) {
        j = json{{"firstName", p.firstName}, {"lastName", p.lastName}, {"idCode", p.idCode}};
        json courseMap = json::object();
        for (const auto& coursePair : p.profCourses) {
            const string& courseCode = coursePair.first;
            const auto& semesterPairs = coursePair.second;
            json semArray = json::array();
            for (const auto& pair : semesterPairs) {
                int year = pair.first;
                bool isWinter = pair.second;
                semArray.push_back({{"Year", year}, {"isWinter", isWinter}});
            }
            courseMap[courseCode] = semArray;
        }
        j["profCourses"] = courseMap;
    }

    friend void from_json(const json& j, Professor& p);

    void addCourse(string courseCode, int year, bool season);

    void eraseCourse(string code);
};
