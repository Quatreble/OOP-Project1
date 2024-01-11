#include<iostream>
#include <vector>

#include "course.hpp"

class Semester{
private:
    int year;
    bool winterOrSummer;
    vector<Course* > courses;

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

    bool getSeason(){
        return winterOrSummer;
    }

    void addCourse(Course* toAdd);

    

    
};