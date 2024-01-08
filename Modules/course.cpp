#include "course.hpp"
#include <limits>

Course::Course(){};

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

void Course::setSemester(int semNum){
    semester = semNum;
}


std::istream& operator>>(std::istream& is, Course& course) {
    std::cout << "Please enter course name: ";
    std::string name;
    is >> course.name;
    is.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::cout << "Enter course academic points: ";
    is >> course.academicPoints;
    is.ignore(std::numeric_limits<std::streamsize>::max(), '\n');  // Clear newline after integer

    std::string choice;
    do {
        std::cout << "Is the course mandatory? (y/n): ";
        is >> choice;
        is.ignore(std::numeric_limits<std::streamsize>::max(), '\n');  // Clear any leftover characters

        if (choice == "y" || choice == "Y") {
            course.isMandatory = true;
            break;
        } else if (choice == "n" || choice == "N") {
            course.isMandatory = false;
            break;
        } else {
            std::cout << "Invalid input. Please enter 'y' or 'n'.\n";
        }
    } while (true);

    return is;
}


