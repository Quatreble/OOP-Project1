#include "course.hpp"
#include <limits>
#include "person.hpp"
#include <fstream>

int Course::semCount = 0;

Course::Course() : registered(0)
{}

Course::Course(std::string nameIn, int academicPointsIn, bool isMandatoryIn, int semesterIn)
    : name(nameIn), academicPoints(academicPointsIn), isMandatory(isMandatoryIn), semester(semesterIn), registered(0) {
    std::cout << "Course Created\n";
}

Course::Course(const Course& other)
    : name(other.name), academicPoints(other.academicPoints), isMandatory(other.isMandatory), semester(other.semester), registered(other.registered) {
    //std::cout << "Course Copied\n";
}

Course::~Course(){
   // std::cout << "Course Destroyed!\n";
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

int Course::getSemester() const{
    return semester;
}

int Course::passedNumber(){
    return studentsPassed.size();
}
int Course::registeredNumber(){
    return registered;
}

void Course::addStudentsWhoPassed(Student& stud){
    studentsPassed.push_back(stud);
}

void Course::printStudentsWhoPassed(){
    if (studentsPassed.empty()){
        std::cout << "No students have passed the course\n";
        return;
    }   

    std::ofstream outFile("students-passed-"+getName()+"-"+to_string(getSemester())+".txt");

    // Check if the file stream is open
    if (outFile.is_open()) {
        // Write to the file
        for (Student& student : studentsPassed){
            outFile << student;
        }
        // Close the file
        outFile.close();

        std::cout << "File created successfully." << std::endl;
    } else {
        // Error message if file couldn't be opened
        std::cerr << "Unable to open file for writing." << std::endl;
    }

    for (Student& student : studentsPassed){
        std::cout << student;
    }
}

std::istream& operator>>(std::istream& is, Course& course) {
    std::cout << "Please enter course name: ";
    is >> course.name;
    is.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    do{
    std::cout << "Enter Course Semester: ";
    is >> course.semester;
    is.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    if (course.semester < 1 || course.semester > 8){
        std::cout << "Semester doesnt exist\n";
    }
    } while(course.semester < 1 || course.semester > 8);

    std::cout << "Enter course academic points: ";
    is >> course.academicPoints;
    is.ignore(std::numeric_limits<std::streamsize>::max(), '\n');  

    std::string choice;
    do {
        std::cout << "Is the course mandatory? (y/n): ";
        is >> choice;
        is.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 

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

void Course::setName(string name){
    this->name = name;
}

void Course::setSemester(int semNum){
    semester = semNum;
}

void Course::setAcademicPoints(int points){
    academicPoints = points;
}

void Course::setMand(std::string c){
    if ("y" == c || "Y" == c){
        isMandatory = true;
    }
    else{
        isMandatory = false;
    }
}

void Course::incRegistered(){
    ++registered;
}

bool Course::operator==(const Course& other) const {
    return name == other.name && academicPoints == other.academicPoints && semester == other.semester  && isMandatory == other.isMandatory;
}



