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
    Secretary(const string& dep, int sem, int reqPoints, int currYear, char currSeason);
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
