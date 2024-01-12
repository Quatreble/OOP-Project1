#pragma once

#include <limits>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "json.hpp"
#include "person.hpp"

using namespace std;


class Student;

class Course {
private:
    string name;
    int academicPoints;
    bool isMandatory;
    string code;
    int registered;
   // vector<Student> studentsWhoPassed;

public:

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(Course,
                                   name,
                                   isMandatory,
                                   code)

    Course();

    Course(string nameIn, int academicPointsIn, bool isMandatoryIn, int semsterIn);

    // Course(const Course& other);

    ~Course();

    unique_ptr<Course> clone() const;

    void setName(string name);
    void setAcademicPoints(int points);
    void setMand(string c);

    string getName() const;
    int getAcademicPoints() const;
    bool getMand() const;


    void addStudentsWhoPassed(Student& stud);
    void printStudentsWhoPassed();
    int passedNumber();

    void incrRegistered();
    int registeredNumber();

    friend istream& operator>>(istream& is, Course& course);

    bool operator==(const Course& other) const;
};

