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

class Person;

using namespace std;

class Secretary {
private:  
    string depName;
    int depSemesters;
    //bool endSemester;
    vector<Professor*> depProfessors; 
    vector<Student*> depStudents; 
    vector<Course*> depCourses;  
    int pointsToGraduate;
    int numOfMandatory = 0;

public:
    Secretary(const string& dep, int sem, int reqPoints);
    Secretary();
    ~Secretary();
    Secretary(const Secretary& sec);

    void addPerson(Person& p, bool printStatement = true);
    bool removePerson(Student& s);
    bool removePerson(Professor& p);
    

    void addProfessor();
    void addStudent();

    void modifyProfessor();
    void modifyStudent();

    void deleteProfessor();
    void deleteStudent();

    void modifyCourse();
    void readAndRemoveCourse();

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

    static Student* isStudent(Person *p);
    static Professor* isProfessor(Person *p);

    Secretary& operator+(Person& p);
    Secretary& operator+=(const Secretary& sec);
    Secretary& operator=(const Secretary& sec);

    friend ostream& operator<<(ostream& os, Secretary& secretary);
    friend istream& operator>>(istream& is, Secretary& secretary);


    // void changeSemester(Course&  course);
    void modifyCourse(Course& course);
    // void nextSemester();

    void addCourse(Course& course);
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
  //  void readCourseFromFile();

    void printStudentToFile(Student& student);
    void printProfessorToFile(Professor& professor);
};
