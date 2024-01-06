#include "course.hpp"

class Course{
private:
    int semester,academicPoints;
    bool isMandatory;
public:
    Course(int semesterIn,int academicPointsIn,bool isMandatoryIn) : semester(semesterIn), academicPoints(academicPointsIn), isMandatory(isMandatoryIn)
    {
        std::cout << "Course Created";
    }

    void semesterChange(int newSemester){
        semester = newSemester;
    }
};