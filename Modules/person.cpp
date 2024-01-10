#include "person.hpp"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////Person class functions///////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Person::Person()                           
{
    pCount++;
}

Person::Person(string fName, string lName, string id)
: firstName(fName), lastName(lName), idCode(id)
{
    pCount++;
}

Person::~Person() {}

void Person::setFirstName(const string& name){
    firstName = name;
}

void Person::setLastName(const string& name){
    lastName = name;
}

void Person::setIdCode(const string& id){
    idCode = id;
}

string Person::getFirstName() {
    return firstName;
}

string Person::getLastName() {
    return lastName;
}

string Person::getIdCode() {
    return idCode;
}

//sub-classes should override this and add logic specific to their fields
bool Person::equals(Person* p){
    return (firstName == p->firstName && lastName == p->lastName && idCode == p->idCode); 
}

//initialize static member
int Person::pCount = 0;

int Person::getCount(){
    return pCount;
}

//overloaded operators <<, >> for input and output of Person objects
ostream& operator<<(ostream& os, const Person& p) {
    return os << "Name: " << p.firstName << " " << p.lastName << ", " << "ID code: " << p.idCode << endl;
}

istream& operator>>(std::istream& is, Person& p){
    if (dynamic_cast<Student *> (&p) != nullptr){
        Student* stud = dynamic_cast<Student *> (&p);
        cout << "Enter semester: ";
        int sem;
        cin >> sem;
        stud->setSemester(sem);
    }
    cout << "Enter first Name, last Name and ID code: " << endl;
    return is >> p.firstName >> p.lastName >> p.idCode;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////Student class functions///////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Student::Student()
: Person()
{
    //cout << "Constructed student!" << endl;
}

Student::Student(string fName, string lName, string id)
: Person(fName, lName, id)
{
    //cout << "Constructed student!" << endl;
}

//dynamically allocates and returns a copy of Student 
Student* Student::clone(){
    return new Student(*this);
}

//for now we just check equality of the super-class Person
bool Student::equals(Student* s) {
    return Person::equals(s);
}

void Student::setSemester(int sem, bool next){
    if (next){
        currentSemester++;
        return;
    }
     currentSemester = sem;
}

int Student::getSemesterCount() {
    return currentSemester;
}

int Student::getAcademicPoints(){
    return currentPoints;
}

void Student::incrAcademicPoints(int p){
    currentPoints += p;
}

void Student::studAddCourse(Course& course){
    if (course.getSemester() > currentSemester){
        cout << "Student can't register. Lesson is at bigger semester\n";
        return;
    }
    for (auto& element : coursesWithGrades){
        const Course& vecCourse = element.first;
        if (course == vecCourse){
            cout << "Student already registered\n";
            return;
        }
    }
    course.incrRegistered();
    coursesWithGrades.push_back(make_pair(course,-1));

    cout << "Student " << getFirstName() << " " << getLastName() << " is now registered in " << course.getName() << '\n';
}

void Student::studentChangeGrade(Course& course){
    for (auto& element : coursesWithGrades){
        Course& c = element.first;
        if (c == course){
            int grade;
            bool error;
            do {
                error = false;
                cout << "Enter desired grade: ";
                cin >> grade;
                if (grade < 0 || grade > 10){
                    cout << "Wrong input";
                    error = true;
                }
            }while (error);

            if (element.second == -1){
                if (grade >= 5){
                    course.addStudentsWhoPassed(*this);
                    incrAcademicPoints(course.getAcademicPoints());
                    if(course.getMand()){
                        ++mandatoryPassed;
                    }
                }
                element.second = grade;
                cout << "Grade was changed to " << element.second << '\n';
            }
            else{
                cout << "Student already graded\n";
            }
            return;
        }
    }
    cout << "Student not registered to course\n";

}

void Student::printGrades(bool semesterOnly){
    bool found = false;
    for (auto& element : coursesWithGrades){
        const Course& course = element.first;
        int grade = element.second;
        if (grade != -1 && !semesterOnly){
            cout << "COURSE: " << course.getName() << "   GRADE: " << grade << '\n';
            found = true;
        }
        else if (grade != -1 && course.getSemester() == currentSemester){
            cout << "COURSE: " << course.getName() << "   GRADE: " << grade << '\n';
            found = true;
        }
    }
    if (found == false){
        cout << "No Grades Found\n";
    }
}

void Student::printGradesToto(){
    for (auto& element : coursesWithGrades){
        cout << "THE GRADE FOR COURSE " << element.first.getName()<< " IS " << element.second << endl;
    }
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////Professor class functions/////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Professor::Professor()
: Person()
{
    //cout << "Constructed Professor!" << endl;
}

Professor::Professor(string fName, string lName, string id)
: Person(fName, lName, id)
{
    //cout << "Constructed Professor!" << endl;
}

//dynamically allocates and returns a copy of Professor 
Professor* Professor::clone(){
    return new Professor(*this);
}

//for now we just check equality of the super-class Person
bool Professor::equals(Professor* f) {
    return Professor::equals(f);
}

void Professor::profAddCourse(Course& course){
    for (Course& c : profCourses){
        if (c == course){
            cout << "Professor " << lastName << " is already assigned to this course\n";
            return;
        }
    }
    profCourses.push_back(course);
    cout << "Professor " << firstName << " " << lastName << " now teaches " << course.getName() << '\n';
}

bool Professor::teachesCourse(Course& course){
    for (Course& c : profCourses){
        if (c == course){
            return true;
        }
    }
    return false;
}

void Professor::printStats(int sem){
    if(profCourses.empty()){
        cout << "The professor has no courses\n";
        return;
    }
    float perc;
    for(Course& c : profCourses){
        if(c.getSemester() == sem){
            perc = c.passedNumber() / c.registeredNumber() * 100.0;
            cout << "Course name: " << c.getName() << endl;
            cout << "Course academic points: " << c.getAcademicPoints() << endl;
            cout << "Number of registered students for current semester: " << c.registeredNumber() << endl;
            cout << "Number of students who passed for current semester: " << c.passedNumber() << endl;
            cout << "Percentage of students who passed: " << perc << "%" << '\n';
        }
    }
}


// void courseRegister(){
//     if (!currDepartment->startSemester()){
//         std::cout << "Registrations not possible at the current moment\n";
//     }
// }
