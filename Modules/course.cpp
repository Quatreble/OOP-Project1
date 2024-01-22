#include "course.hpp"

Course::Course(string nameIn, int academicPointsIn, bool isMandatoryIn)
    : name(nameIn), academicPoints(academicPointsIn), isMandatory(isMandatoryIn), registered(0) 
{
    cout << "Course Created\n";
}

unique_ptr<Course> Course::clone() const{
    return make_unique<Course>(*this);
}

bool Course::operator==(const Course& other) const {
    return name == other.name && academicPoints == other.academicPoints && code == other.code  && isMandatory == other.isMandatory && registered == other.registered;
}

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

    cout << "Enter course year: ";
    is >> course.year;
    is.ignore(numeric_limits<streamsize>::max(), '\n');  

    cout << "Enter 'w' for winter semester and 's' for summer semester: ";

    while (true) {
        is >> course.season;

        // Check if the input is one of the allowed characters
        if (course.season == "w" || course.season == "W" || course.season == "s" || course.season == "S") {
            break;
        } else {
            cout << "Invalid input. Please enter 'w' or 's': ";
            // Clear the error state and ignore the rest of the input line
            is.clear();
            is.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }

    return is;
}




