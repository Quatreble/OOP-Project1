#ifndef COURSE_HPP
#define COURSE_HPP

#include <iostream>
#include <memory>
#include <string>
#include <vector>
class Student;

class Course {
private:
    std::string name;
    int academicPoints;
    bool isMandatory;
    int semester;
    std::vector<Student> studentsPassed;
    int registered;

public:
    Course();

    Course(std::string nameIn, int academicPointsIn, bool isMandatoryIn, int semsterIn);

    ~Course();

    Course(const Course& other);


    std::unique_ptr<Course> clone() const;

    int getAcademicPoints();

    std::string getName() const;

    void setSemester(int semNum);
    int getSemester() const;

    void addStudentsWhoPassed(Student& stud);
    void printStudentsWhoPassed();
    void incRegistered();
    int passedNumber();
    int registeredNumber();

    friend std::istream& operator>>(std::istream& is, Course& course);
    bool operator==(const Course& other) const;
};


#endif // COURSE_HPP