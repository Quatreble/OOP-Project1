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

// this struct is stored alongside a course code(Course class identifier)
// inside Student::coursesWithGrades. It serves as a connection between a semester, a course, and the Student's 
// corresponding grade. When the program starts and a secretary is created, in studentsinfo.json besides the students basic info
// there are are course-SemesterGradeInstance pairs that are also read and loaded. there pairs contain info about each course
// for which a student has been graded in the past.
// The macro NLOHMANN_DEFINE_TYPE_INTRUSIVE is used to facilitate serialization and 
// deserialization with the nlohmann/json library. This macro makes it possible to 
// automatically tie the member variables of this struct to corresponding fields in 
// a json object

struct SemesterGradeInstance {
    int grade, year;
    bool isWinter;
    NLOHMANN_DEFINE_TYPE_INTRUSIVE(SemesterGradeInstance,
                                grade,
                                year,
                                isWinter)

};

// abstract person class, is inherited by children classes Student and Professors. Basic attributes of 
// person are: first name, last name and a unique id code
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

// this class implements a student of a department and is a child class of person
// the unique attributes of a studnet are: the current academic semester they are in(eg semester 3), 
// their registration year, their current academic points and the number of mandatory courses they have passed
// in the class there is also an unordered map associating a course(identified by its unique code) and a SemesterGradeInstance,
// which contains the information about the semester in which they took that course and their grade for it
// this map is useful for storing and having access to a student's grade history
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
    void eraseCourse(string name, bool isMand);

    // serialization of Student
    // convert a Student object to its json represantation. to_json's functionality
    // has been described above(see StudentCourseInstance)
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

// this struct contains a Student* and an integer grade variable
// it is stored in an unordered map in Semester class where a vector containinig these structs
// is associated to a course. This makes it easy to have access to all of a course's students and their grades
// for a given semester
// because for serialization and deserialization we need to handle a Student class object,
// we dont use a macro for it but the two more specific functions: to_json and from_json
struct StudentCourseInstance {
    Student* stud;
    int grade = -1;

    // convert the current instance of SemesterGradeInstance to a json object.
    nlohmann::json to_json() const {
        nlohmann::json j = {
            {"grade", grade} // seiralize grade
        };
        // create a new empty json object
        nlohmann::json studentJson;
        stud->to_json(studentJson, *stud, false);// here we serialize the student data, for this the to_json of Student is called 
        j["student"] = studentJson;  // assign the serialized student data to the "student" field of the object
        return j;
    }

    friend void from_json(const nlohmann::json& j, StudentCourseInstance& sci);
};

// this class implements a professor of a department and is a child class of person
// it contains an unordered map which associates courses(by code) to semesters(year and season)
// this map helps us store the courses that the professor has taught through the years
// in the map are stored a string(unique id code) for the course and a year, int pair for the semester
// and not a Course* or a Semester*. That does not pose a problem, as it is very easy and fast
// to find an object of those classes given the right identifier
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

// to_json function operates as described above
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
