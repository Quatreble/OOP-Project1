#include "semester.hpp"

void Semester::addCourse(Course* toAdd){
    cout << "Enter academic year of course: ";
    int year;
    cin >> year;
    courses.emplace(toAdd, year);
}

void Semester::addProfToCourse(Course* c, Professor* p){
    courseProfs.emplace(c, vector<Professor*>());
    courseProfs[c].push_back(p);
    cout << *p << "is now professor of " << c->getName() << '\n';
}

void Semester::addStudToCourse(Course* c, Student* s){
    courseStuds.emplace(c,vector<StudentCourseInstance*>());
    StudentCourseInstance* sci = new StudentCourseInstance;
    sci->stud = s;
    sci->grade = -1;
    courseStuds[c].push_back(sci);
    cout << *s << "is now registered to " << c->getName() << '\n';
}

bool Semester::courseBelongs(Course& course){
    for(auto& pair: courses){
        if(*(pair.first) == course){
            return true;
        }
    }
    cout << "COURSE NOT IN THIS SEMESTER" << endl;
    return false;
}