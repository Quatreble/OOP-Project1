#include<iostream>
#include <vector>

#include "course.hpp"

class Semester{
private:
    int year;
    bool winterOrSummer;
    vector<Course> courses;

public:
    Semester(int y, bool season)
    :year(y), winterOrSummer(season)
    {}

    

    
};