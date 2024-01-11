#include<iostream>
#include <vector>
#include <unordered_map>

#include "course.hpp"

class Semester{
private:
    int year;
    bool winterOrSummer;
    vector<Course* > courses;
    unordered_map<string, vector<Professor*>> courseProfs;

public:
    Semester(int y, bool season)
    :year(y), winterOrSummer(season)
    {}

    void printSem(){
        cout << "Year: " << year << endl << "Semester: " << (winterOrSummer?"winter":"summer") << endl;
    }

    int getYear(){
        return year;
    }

    bool courseBelongs(Course& course);

    void addProfToCourse(Course* c, Professor* p);

    bool getSeason(){
        return winterOrSummer;
    }

    void addCourse(Course* toAdd);

};