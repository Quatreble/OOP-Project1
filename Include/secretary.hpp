#pragma once

// we choose to use a vector instead of a map since we would like to be able 
                                  // to search with all Person's properties using linear iteration. Since all takes place
                                  // in memory there is no significant performance hit to go through every Person  ( O(n) )

#include <vector>
#include <chrono>
#include <thread>
#include <fstream>
#include <sstream>
#include <memory>

#include "person.hpp"
#include "course.hpp"
#include "semester.hpp"

#define CURR_SEM std::pair<int, char>(2023, 'W')

class Person;

using namespace std;

class Secretary {
private:  
    string depName;
    int depSemesters;
    //bool endSemester;
    unordered_map<string, Professor*> depProfessors; 
    unordered_map<string, Student*> depStudents; 
    unordered_map<string, Course*> depCourses;  
    vector<Semester*> semesters;
    int pointsToGraduate;
    int numOfMandatory = 0;
    json jCourses;
    json jProfessors;
    json jStudents;
    json jSemesters;

public:
    Secretary(const string& dep, int sem, int reqPoints);
    Secretary();
    ~Secretary();
    Secretary(const Secretary& sec);

    void printMenu();
    void SecretaryOperation();

    void addPerson(Student& s, bool printStatement = true, bool manualAdd = true);
    void addPerson(Professor& p, bool printStatement = true, bool manualAdd = true);
    void addProfessor();
    void addStudent();
    void addCourse(Course& course,bool manualAdd = true);

    Semester* getCurrSem();
    
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
    Course* findCourse(string code);

    void printSecSize();

    void setSecName(const string& dep);
    const string& getSecName();

    int getNumOfMandatory() { return numOfMandatory; }

    static Student* isStudent(Person *p);
    static Professor* isProfessor(Person *p);

    Secretary& operator+(Student& p);
    Secretary& operator+(Professor& p);
    Secretary& operator+=(const Secretary& sec);
    Secretary& operator=(const Secretary& sec);

    friend ostream& operator<<(ostream& os, Secretary& secretary);
    friend istream& operator>>(istream& is, Secretary& secretary);

    void createSemester();
    Semester* addSemester(Semester& toAdd);

    void setCourseProf();
    void registerStudentToCourse();
    void gradeStudents();
    void printStudentsWhoPassed();
    void printProfStats();
    void getGrades();
    void getGradesCurr();

    void printGraduates();

    Student* readAndValidateStudent();
    Professor* readAndValidateProfessor();
    Course* readAndValidateCourse();
    Semester* readAndValidateSemester();


    void readStudentsFromFile();
    void readProfessorsFromFile();
    void readCourseFromFile();
    void readSemesterFromFile();

    void printStudentToFile(Student& student);
    void printProfessorToFile(Professor& professor);
    void printCourseToFile(Course& course);
    void printSemesterToFile(Semester& semester);

    void jsonModifyProf(Professor& prof, string id);
    void jsonModifyStud(Student& stud, string id);
    void jsonModifyCourse(Course& course, string code);

    void jsonRemoveProfessor(Professor& prof);
    void jsonRemoveStudent(Student& stud);
    void jsonRemoveCourse(Course& course);

};
