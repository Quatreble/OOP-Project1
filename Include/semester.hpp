#include <iostream>
#include <vector>
#include "course.hpp"


class Semester{
private:
    int num;
public:
    std::vector<Course> courses;  

    Semester(int numIn);

    Semester(const Semester& other);

    int getNum();


};