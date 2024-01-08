#include "Semester.hpp"

Semester::Semester(int numIn) : num(numIn){
    //std::cout << num <<" semester constructed!\n";
}

Semester::Semester(const Semester& other) : num(other.num) {
    for (const auto& course : other.courses) {
        courses.push_back(course);
    }
}

int Semester::getNum(){
    return num;
}