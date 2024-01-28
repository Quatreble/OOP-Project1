#include "course.hpp"

// constructor, initializes course attributes
Course::Course(string nameIn, string codeIn, int academicPointsIn, bool isMandatoryIn)
    : name(nameIn), academicPoints(academicPointsIn), isMandatory(isMandatoryIn), code(codeIn), registered(0) 
{
    cout << "COURSE CREATED\n";
}

// dynamically allocates a copy of Course and returns unique pointer to it
unique_ptr<Course> Course::clone() const{
    return make_unique<Course>(*this);
}

// equality operator overload, compares all attributes of course
bool Course::operator==(const Course& other) const {
    return name == other.name && academicPoints == other.academicPoints && code == other.code  && isMandatory == other.isMandatory && registered == other.registered;
}

// input operator overload, read a Course object
istream& operator>>(istream& is, Course& course) {
    cout << "PLEASE ENTER COURSE NAME: "; // read name
    is >> course.name;
    is.ignore(numeric_limits<streamsize>::max(), '\n'); // ignore anything else written on that line before reading next
    
    cout << "PLEASE ENTER COURSE CODE: "; // read code
    is >> course.code;
    is.ignore(numeric_limits<streamsize>::max(), '\n');
    

    cout << "ENTER COURSE ACADEMIC POINTS: "; // read academic points
    is >> course.academicPoints;
    is.ignore(numeric_limits<streamsize>::max(), '\n');  

    string choice; // read isMandatory. read repeatedly until input is as expected(y/n)
    do {
        cout << "IS COURSE MANDATORY (y/n): ";
        is >> choice;
        is.ignore(numeric_limits<streamsize>::max(), '\n'); 

        if (choice == "y" || choice == "Y") {
            course.isMandatory = true;
            break;
        } else if (choice == "n" || choice == "N") {
            course.isMandatory = false;
            break;
        } else {
            cout << "INVALID INPUT. PLEASE ENTER 'Y' OR 'N'\n";
        }
    } while (true);

    cout << "ENTER COURSE YEAR: "; // read course's academic year
    is >> course.year;
    is.ignore(numeric_limits<streamsize>::max(), '\n');  

    cout << "ENTER 'W' FOR WINTER SEMESTER, 'S' FOR SUMMER: ";

    while (true) { // read course's season repeatedly until input is as expected(w/s)
        is >> course.season;

        // check if the input is one of the allowed characters
        if (course.season == "w" || course.season == "W" || course.season == "s" || course.season == "S") {
            break;
        } else {
            cout << "INVALID INPUT. PLEASE ENTER 'W' OR 'S': ";
            // clear the error state and ignore the rest of the input line
            is.clear();
            is.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }

    return is;
}




