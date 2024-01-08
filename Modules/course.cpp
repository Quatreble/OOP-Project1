#include "course.hpp"

Course::Course(std::string nameIn, int academicPointsIn, bool isMandatoryIn)
    : name(nameIn), academicPoints(academicPointsIn), isMandatory(isMandatoryIn) {
    std::cout << "Course Created\n";
}

Course::Course(const Course& other)
    : name(other.name), academicPoints(other.academicPoints), isMandatory(other.isMandatory) {
    //std::cout << "Course Copied\n";
}

std::unique_ptr<Course> Course::clone() const{
    return std::make_unique<Course>(*this);
}

int Course::getAcademicPoints(){
    return academicPoints;
}

std::string Course::getName() const{
    return name;
}
