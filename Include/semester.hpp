#include<iostream>
#include <vector>
#include <unordered_map>

#include "course.hpp"

class Semester{
private:
    int year;
    bool winterOrSummer;
    unordered_map<Course*,int> courses;
    unordered_map<Course* , vector<Professor* > > courseProfs;
    unordered_map<Course* , vector<StudentCourseInstance* > > courseStuds;
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

    int getCourseYear(Course& course){
        for (auto& element : courses){
            if (*(element.first) == course){
                return element.second;
            }
        }
        return -1;
    }

    bool courseBelongs(Course& course);

    void addProfToCourse(Course* c, Professor* p);

    void addStudToCourse(Course* c, Student* s);

    bool getSeason(){
        return winterOrSummer;
    }

    void addCourse(Course* toAdd);

};