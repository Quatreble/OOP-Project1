#pragma once

#include <vector>
#include <chrono>
#include <thread>
#include <fstream>
#include <sstream>
#include <memory>

#include "person.hpp"
#include "course.hpp"
#include "semester.hpp"

using namespace std;

// this class represents the secretary of a department
// it is responsible for storing, delete, modifying, etc.
// members of the department(students and professors) and its courses
// this class makes all the calls about registering mewmbers, to courses, grading, member's data, etc.
// more specifically, by handling the program's four basic classes: Semester, Course, Student and Professor
// it implements the following operations:
// 1. add/modify/remove a student/professor/course
// 2. register student/professor to course
// 3. grade students/ get students current and past grades
// 4. get professor's teaching statistics
// 5. define who can graduate
// very importantly, the secretary is also responsible for the file-handling
// there are files which contain student, professors, course and grade data
// these are all updated and modified by the secretary class when necessary
// to facilitate file handling, the class contains four json arrays, one for each of the 4 basic classes
// it also contains three maps to store professors, students and courses respectively and a vector for the semesters
// the reason we chose an unordered map for the first three is the following:
// in the map we store as data an Object* and the associated key is each object's unique id code. This makes for a very quick code based search
// which is what we needed most. 
// a secretary's basic attributes are the following: department name, number of academic semesters, number of academic points needed to graduate, 
// number of mandatory courses and finally the year and season for which a secretary object is operating

class Secretary {
private:  
    string depName;
    int depSemesters;
    int pointsToGraduate;
    int currYear;
    char currSeason;
    int numOfMandatory = 0;

    unordered_map<string, Professor*> depProfessors; 
    unordered_map<string, Student*> depStudents; 
    unordered_map<string, Course*> depCourses;  
    vector<Semester*> semesters;

    json jCourses;
    json jProfessors;
    json jStudents;
    json jSemesters;

public:
    Secretary(const string& dep, int sem, int reqPoints);
    ~Secretary();
    Secretary(const Secretary& sec);

    void printMenu();
    void SecretaryOperation();

    void printSecSize(){cout << "PEOPLE IN SECRETARY: " << depStudents.size() + depProfessors.size() << endl; }
    void setSecName(const string& dep){ depName = dep;}
    const string& getSecName(){ return depName; }
    int getNumOfMandatory() { return numOfMandatory; }

    Semester* getCurrSem();

    void setCourseProf();
    void registerStudentToCourse();
    void gradeStudents();
    void printStudentsWhoPassed();
    void printProfStats();
    void getGrades();
    void printGraduates();

    static Student* isStudent(Person *p);
    static Professor* isProfessor(Person *p);

    void readAndAddProfessor();
    void readAndAddStudent();
    void readAndAddCourse();

    void addPerson(Student& s, bool printStatement = true, bool manualAdd = true);
    void addPerson(Professor& p, bool printStatement = true, bool manualAdd = true);
    void addCourse(Course& course,bool manualAdd = true);
    Semester* addSemester(Semester& toAdd);
    
    void modifyProfessor();
    void modifyStudent();
    void modifyCourse();

    bool removePerson(Student& s);
    bool removePerson(Professor& p);
    void removeCourse(Course& course);

    void deleteProfessor();
    void deleteStudent();
    void deleteCourse();

    Student* findStudent(const string& id);
    Professor* findProfessor(const string& id);
    Course* findCourseByCode(string code);
    Course* findCourseByName(string name);

    Student* readAndValidateStudent();
    Professor* readAndValidateProfessor();
    Course* readAndValidateCourse();
    Semester* readAndValidateSemester();

    void readStudentsFromFile();
    void readProfessorsFromFile();
    void readCourseFromFile();

    void readCoursesAndGrades(Student* stud);
    void readProfessorCourses(Professor* prof);

    void printStudentToFile(Student& student);
    void printProfessorToFile(Professor& professor);
    void printCourseToFile(Course& course);
    void printJson(string fileName, json& array);

    void jsonModifyProf(Professor& prof, string id);
    void jsonModifyStud(Student& stud, string id);
    void jsonModifyCourse(Course& course, string code);

    void jsonRemoveProfessor(Professor& prof);
    void jsonRemoveStudent(Student& stud);
    void jsonRemoveCourse(Course& course);

    void readCurrentDate();

    Secretary& operator+(Student& p);
    Secretary& operator+(Professor& p);
    Secretary& operator+=(const Secretary& sec);
    Secretary& operator=(const Secretary& sec);

    friend ostream& operator<<(ostream& os, Secretary& secretary);
    friend istream& operator>>(istream& is, Secretary& secretary);

};
