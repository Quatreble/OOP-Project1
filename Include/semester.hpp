#pragma once

#include<iostream>
#include <vector>
#include <unordered_map>
#include <string>
#include <fstream>

#include "course.hpp"

// a Semester class instance is characterized by its year and season
// for each Semester object there are two unordered maps: courseProfs and courseStuds
// in which the a semester's data is stored. Each element of courseProfs stores a Course*
// and a vector of Professor*, thus storing the course's professors(can be more than one)
// for this semester. Likewise, in courseStuds we store the students registered to each course
// for this semester, along with their grade. This is implemented through the use of the StudentCourseInstance
// struct which contains a Student* and a grade integer; 

// we chose to use unordered maps because we needed quick insertions and look-ups using a person's unique id code, 
// while the sequence of the stored elements is insignificant

// the passedJson map helps us keep track of all the students who passed a given course in a certain semester
// and then update(or create) the file in which this info is kept when the printPassed functions is called

class Semester{
private:
    int year;
    bool winterOrSummer;
    unordered_map<Course* , vector<Professor* > > courseProfs;
    unordered_map<Course*, vector<StudentCourseInstance*> > courseStuds;
    unordered_map <Course*, json> passedJson;

public:

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(Semester,
                                   year,
                                   winterOrSummer)

    Semester(int yearIn, bool seasonIn)
    :year(yearIn), winterOrSummer(seasonIn)
    {}

    Semester(){}

    ~Semester(){
        for(auto& it: courseStuds){
            for(auto element: it.second){ //deletes the StudentCourseInstance pointers
                delete element;
            }
        }
    }

    // Semester(const Semester& other)
    // : year(other.year), winterOrSummer(other.winterOrSummer) {}

    int getYear() const { return year; }
    bool getSeason() const {return winterOrSummer; }

    void setYear(int yearIn) {year = yearIn; }
    void setSeason(bool seasonIn) {winterOrSummer = seasonIn; }

    StudentCourseInstance* isRegistered(Course* course, Student* stud);

    void addStudToCourse(Course* c, Student* s, bool print = true);
    void addProfToCourse(Course* c, Professor* p, bool print = true);

    bool gradeStud(StudentCourseInstance* sci);

    void printSem();
    void printPassed(Course* course);
    void printProfStats(Professor* prof);
    void printStudGrades(Student* stud);

    void eraseCourse(string code);

    friend istream& operator>>(istream& is, Semester& semester);

};