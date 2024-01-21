#pragma once

#include<iostream>
#include <vector>
#include <unordered_map>
#include <string>

#include "course.hpp"

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

    Semester(int y, bool season)
    :year(y), winterOrSummer(season)
    {}

    Semester(){}

    ~Semester(){
        for(auto& it: courseStuds){
            for(auto element: it.second){
                //delete element->stud;
                delete element;
            }
        }
    }

    Semester(const Semester& other)
    : year(other.year), winterOrSummer(other.winterOrSummer) {}

    void printSem(){
        cout << "Year: " << year << endl << "Semester: " << (winterOrSummer?"winter":"summer") << endl;
    }

    int getYear(){
        return year;
    }

    void addProfToCourse(Course* c, Professor* p, bool print = true);

    void addStudToCourse(Course* c, Student* s, bool print = true);

    bool getSeason(){
        return winterOrSummer;
    }

    friend istream& operator>>(istream& is, Semester& semester);

    StudentCourseInstance* isRegistered(Course* course, Student* stud);

    bool gradeStud(StudentCourseInstance* sci);
    
    void printPassed(Course* course);

    void printProfStats(Professor* prof);

    void printStudStats(Student* stud);

    void setYear(int yearIn);
    void setSeason(bool season);

    void eraseCourse(string code);

};