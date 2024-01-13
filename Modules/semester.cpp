#include "semester.hpp"

void Semester::addCourse(Course* toAdd){
    cout << "Enter academic year of course: ";
    int year;
    cin >> year;
    courses.push_back(make_pair(toAdd, year));
}

void Semester::addProfToCourse(Course* c, Professor* p){
    if(courseProfs.find(c) == courseProfs.end()){
        courseProfs[c] = vector<Professor*>();
    }
    courseProfs[c].push_back(p);
    cout << *p << "is now professor of " << c->getName() << '\n';
}

bool Semester::courseBelongs(Course& course){
    for(auto pair: courses){
        if(*(pair.first) == course){
            return true;
        }
    }
    cout << "COURSE NOT IN THIS SEMESTER" << endl;
    return false;
}