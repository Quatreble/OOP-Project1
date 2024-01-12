#include "course.hpp"

Course::Course() : registered(0)
{}

Course::Course(string nameIn, int academicPointsIn, bool isMandatoryIn, int semesterIn)
    : name(nameIn), academicPoints(academicPointsIn), isMandatory(isMandatoryIn), registered(0) {
    cout << "Course Created\n";
}

// Course::Course(const Course& other)
//     : name(other.name), academicPoints(other.academicPoints), isMandatory(other.isMandatory), semester(other.semester), registered(other.registered) {
//     //cout << "Course Copied\n";
// }

Course::~Course(){
   // cout << "Course Destroyed!\n";
}

unique_ptr<Course> Course::clone() const{
    return make_unique<Course>(*this);
}

void Course::setName(string name){
    this->name = name;
}

void Course::setAcademicPoints(int points){
    academicPoints = points;
}

void Course::setMand(string c){
    if ("y" == c || "Y" == c){
        isMandatory = true;
    }
    else{
        isMandatory = false;
    }
}

string Course::getName() const{
    return name;
}

int Course::getAcademicPoints() const{
    return academicPoints;
}

bool Course::getMand() const{
        return isMandatory;
}

// int Course::passedNumber(){
//     return studentsWhoPassed.size();
// }
int Course::registeredNumber(){
    return registered;
}

void Course::incrRegistered(){
    ++registered;
}

// void Course::addStudentsWhoPassed(Student& stud){
//     studentsWhoPassed.push_back(stud);
// }

// void Course::printStudentsWhoPassed(){
//     if (studentsWhoPassed.empty()){
//         cout << "No students have passed the course\n";
//         return;
//     }   

//     ofstream outFile("students-passed-"+getName()+"-"+to_string(getSemester())+".txt");

//     // Check if the file stream is open
//     if (outFile.is_open()) {
//         // Write to the file
//         for (Student& student : studentsWhoPassed){
//             outFile << student;
//         }
//         // Close the file
//         outFile.close();

//         cout << "File created successfully." << endl;
//     } else {
//         // Error message if file couldn't be opened
//         cerr << "Unable to open file for writing." << endl;
//     }

//     for (Student& student : studentsWhoPassed){
//         cout << student;
//     }
// }

istream& operator>>(istream& is, Course& course) {
    cout << "Please enter course name: ";
    is >> course.name;
    is.ignore(numeric_limits<streamsize>::max(), '\n');
    
    cout << "Please enter course code: ";
    is >> course.code;
    is.ignore(numeric_limits<streamsize>::max(), '\n');

    cout << "Enter course academic points: ";
    is >> course.academicPoints;
    is.ignore(numeric_limits<streamsize>::max(), '\n');  

    string choice;
    do {
        cout << "Is the course mandatory? (y/n): ";
        is >> choice;
        is.ignore(numeric_limits<streamsize>::max(), '\n'); 

        if (choice == "y" || choice == "Y") {
            course.isMandatory = true;
            break;
        } else if (choice == "n" || choice == "N") {
            course.isMandatory = false;
            break;
        } else {
            cout << "Invalid input. Please enter 'y' or 'n'.\n";
        }
    } while (true);

    return is;
}

bool Course::operator==(const Course& other) const {
    return name == other.name && academicPoints == other.academicPoints && code == other.code  && isMandatory == other.isMandatory && registered == other.registered;
}



