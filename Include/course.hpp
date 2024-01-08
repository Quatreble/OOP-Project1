#ifndef COURSE_HPP
#define COURSE_HPP

#include <iostream>
#include <memory>
#include <string>

class Course {
private:
    std::string name;
    int academicPoints;
    bool isMandatory;

public:
    Course(std::string nameIn, int academicPointsIn, bool isMandatoryIn);

    Course(const Course& other);


    std::unique_ptr<Course> clone() const;

    int getAcademicPoints();

    std::string getName() const;
};


#endif // COURSE_HPP