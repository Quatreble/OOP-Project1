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
    os << "Name: " << p.firstName << " " << p.lastName << ", " << "ID code: " << p.idCode;
    // Student* stud = dynamic_cast<Student *> (&p);
    // if (stud != nullptr){
    //     os << ", year of registration: " << stud->getReg();
    // }
    os << endl;
    return os;
}

istream& operator>>(std::istream& is, Person& p){
    cout << "Enter first Name, last Name and ID code: " << endl;
    is >> p.firstName >> p.lastName >> p.idCode;
    Student* stud = dynamic_cast<Student *> (&p);
    if (stud != nullptr){
        cout << "Enter year of registration: ";
        int reg;
        cin >> reg;
        stud->setReg(reg);
    }
    return is;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////Student class functions///////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Student::Student()
: Person()
{
    //cout << "Constructed student!" << endl;
}

Student::Student(string fName, string lName, string id, int regYear)
: Person(fName, lName, id), registrationYear(regYear)
{
    //cout << "Constructed student!" << endl;
}

void Student::deleteCoursesWithGrades(){
    for(auto& it: coursesWithGrades){
        delete it.second;
    }
}

//dynamically allocates and returns a copy of Student 
Student* Student::clone(){
    return new Student(*this);
}

//for now we just check equality of the super-class Person
bool Student::equals(Student* s) {
    return Person::equals(s);
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

// void Student::studAddCourse(Course& course){
//     // if (course.getSemester() > currentSemester){
//     //     cout << "Student can't register. Lesson is at bigger semester\n";
//     //     return;
//     // }
//     for (auto& element : coursesWithGrades){
//         const Course& vecCourse = element.first;
//         if (course == vecCourse){
//             cout << "Student already registered\n";
//             return;
//         }
//     }
//     course.incrRegistered();
//     coursesWithGrades.push_back(make_pair(course,-1));

//     cout << "Student " << getFirstName() << " " << getLastName() << " is now registered in " << course.getName() << '\n';
// }

// void Student::studentChangeGrade(Course& course){
//     for (auto& element : coursesWithGrades){
//         Course& c = element.first;
//         if (c == course){
//             int grade;
//             bool error;
//             do {
//                 error = false;
//                 cout << "Enter desired grade: ";
//                 cin >> grade;
//                 if (grade < 0 || grade > 10){
//                     cout << "Wrong input";
//                     error = true;
//                 }
//             }while (error);

//             if (element.second == -1){
//                 if (grade >= 5){
//                     course.addStudentsWhoPassed(*this);
//                     incrAcademicPoints(course.getAcademicPoints());
//                     if(course.getMand()){
//                         ++mandatoryPassed;
//                     }
//                 }
//                 element.second = grade;
//                 cout << "Grade was changed to " << element.second << '\n';
//             }
//             else{
//                 cout << "Student already graded\n";
//             }
//             return;
//         }
//     }
//     cout << "Student not registered to course\n";

// }

void Student::printGrades(){
    for (auto& element : coursesWithGrades){
        cout << "Course: " << element.first<< ", Grade: " << element.second->grade << '\n';
    }
}

void Student::addCourseWithGrade(Course* course, SemesterGradeInstance* semGrade){
    coursesWithGrades[course->getName()] = semGrade;
}

int Student::getCourseGrade(Course* course){
    for (auto& element : coursesWithGrades){
        if (element.first == course->getName()){
            return element.second->grade;
        }
    }
    return -1;
}

void from_json(const json& j, Student& student) {
    // Deserialize the basic student data
    j.at("firstName").get_to(student.firstName);
    j.at("lastName").get_to(student.lastName);
    j.at("idCode").get_to(student.idCode);
    j.at("registrationYear").get_to(student.registrationYear);
    // Add other fields as necessary

    // Deserialize coursesWithGrades
    if (j.contains("coursesWithGrades")) {
        const json& coursesJson = j.at("coursesWithGrades");
        for (const auto& item : coursesJson.items()) {
            string courseName = item.key();
            SemesterGradeInstance* gradeInstance = new SemesterGradeInstance;
            from_json(item.value(), *gradeInstance);

            // Assign the new gradeInstance to the student's coursesWithGrades map
            student.coursesWithGrades[courseName] = gradeInstance;
        }
    }
}

void from_json(const nlohmann::json& j, StudentCourseInstance& sci) {
    Student temp_student;
    j.at("student").get_to(temp_student);
    sci.stud = new Student(temp_student);
    j.at("grade").get_to(sci.grade);
}

unordered_map<string, SemesterGradeInstance*> Student::getCoursesWithGrades() { 
    return coursesWithGrades; 
}

void Student::eraseCourse(string name){
    for (auto& element : coursesWithGrades){
        if (element.first == name){
            coursesWithGrades.erase(name);
        }
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

