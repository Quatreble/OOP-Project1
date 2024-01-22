#pragma once

#include <limits>
#include <memory>
#include <string>

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
    int year;
    string season;

public:

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(Course,
                                   name,
                                   isMandatory,
                                   academicPoints,
                                   code,
                                   year,
                                   season)

    Course() : registered(0){}

    Course(string nameIn, int academicPointsIn, bool isMandatoryIn);

    //Course(const Course& other);

    unique_ptr<Course> clone() const;

    void setName(string name) {this->name = name; }
    void setAcademicPoints(int points) {academicPoints = points; }
    void setMand(string c) {isMandatory = (c == "y" || c == "Y"); }
    void setYear(int newYear) {year = newYear; }
    void setSemester(string newSeason) {season = newSeason; }

    string getName() const {return name; }
    int getAcademicPoints() const {return academicPoints; }
    bool getMand() const {return isMandatory; }
    string getCode() const {return code; }
    int getYear() const {return year; }
    int registeredNumber() const {return registered; }
    bool getSeason() const {return season == "w"; }

    void incrRegistered(){ ++registered; }

    bool operator==(const Course& other) const;

    friend istream& operator>>(istream& is, Course& course);

};

