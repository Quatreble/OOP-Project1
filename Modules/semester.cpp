#include "semester.hpp"

void Semester::addCourse(Course* toAdd){
    courses.push_back(toAdd);
}

void Semester::addProfToCourse(Course* c, Professor* p){
    string key = c->getCode();
    if(courseProfs.find(key) == courseProfs.end()){
        courseProfs[key] = vector<Professor*>();
    }
    courseProfs[key].push_back(p);
    cout << *p << "is now professor of " << c->getName() << '\n';
}

bool Semester::courseBelongs(Course& course){
    for(auto c: courses){
        if(*c == course){
            return true;
        }
    }
    cout << "COURSE NOT IN THIS SEMESTER" << endl;
    return false;
}