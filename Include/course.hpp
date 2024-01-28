#pragma once

#include <limits>
#include <memory>
#include <string>

#include "person.hpp"

using namespace std;

class Student;


// this class represents a course of secretary
// its basic attributes are the following:
// name, unique id code, academic points, mandatory status,
// **academic year and academic season(winter semester or summer semester)
// the year and season attributes in a Course class instance can be changed
// and their use is the following: they define the academic semester
// for which the course is designated. eg, a course might be meant for students in their 3rd 
// academic semester(year 2, winter season)
// of course, as was mentioned, this is something that may be changed, for example a course
// used to be for first year students and is now for second year students 
class Course {
private:
    string name;
    int academicPoints;
    bool isMandatory;
    string code;
    int year;
    string season;

public:
    //  NLOHMANN_DEFINE_TYPE_INTRUSIVE macro declaration.
    // this registers the member variables for automatic json serialization and deserialization
    NLOHMANN_DEFINE_TYPE_INTRUSIVE(Course,
                                   name,
                                   isMandatory,
                                   academicPoints,
                                   code,
                                   year,
                                   season)

    Course(){}

    Course(string nameIn, string codeIn, int academicPointsIn, bool isMandatoryIn);

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
    bool getSeason() const {return season == "w"; }

    bool operator==(const Course& other) const;

    friend istream& operator>>(istream& is, Course& course);

};

