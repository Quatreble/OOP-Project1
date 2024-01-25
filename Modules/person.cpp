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

//sub-classes should override this and add logic specific to their fields
bool Person::equals(Person* p){
    return (firstName == p->firstName && lastName == p->lastName && idCode == p->idCode); 
}

//initialize static member
int Person::pCount = 0;

//overloaded operators <<, >> for input and output of Person objects
ostream& operator<<(ostream& os, const Person& p) {
    os << "NAME: " << p.firstName << " " << p.lastName << ", " << "ID: " << p.idCode;
    os << '\n';
    return os;
}

istream& operator>>(std::istream& is, Person& p){
    cout << "ENTER FIRST NAME, LAST NAME AND ID CODE: " << endl;
    is >> p.firstName >> p.lastName >> p.idCode;
    Student* stud = dynamic_cast<Student *> (&p);
    if (stud != nullptr){
        cout << "ENTER YEAR OF REGISTRATION: ";
        int reg;
        cin >> reg;
        stud->setReg(reg);
    }
    return is;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////Student class functions///////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Student::Student(string fName, string lName, string id, int regYear)
: Person(fName, lName, id), registrationYear(regYear)
{}

void Student::deleteCoursesWithGrades(){
    for(auto& it: coursesWithGrades){
        delete it.second;
    }
}

//dynamically allocates and returns a copy of Student 
Student* Student::clone(){
    try {
        return new Student(*this);
    }
    catch(const bad_alloc &e){
        cerr << "Memory allocation failed: " << e.what() << '\n';
        return nullptr;
    }
}

//for now we just check equality of the super-class Person
bool Student::equals(Student* s) {
    return Person::equals(s) && registrationYear == s->getReg();
}

void Student::printGrades(){
    for (auto& element : coursesWithGrades){
        cout << "COURSE: " << element.first<< ", GRADE: " << element.second->grade << '\n';
    }
}

void Student::addCourseWithGrade(Course* course, SemesterGradeInstance* semGrade){
    coursesWithGrades[course->getName()] = semGrade;
    if (semGrade->grade >= 5){
        mandatoryPassed++;
        currentPoints+= course->getAcademicPoints();
    }
}

int Student::getCourseGrade(Course* course){
    for (auto& element : coursesWithGrades){
        if (element.first == course->getName()){
            return element.second->grade;
        }
    }
    return -1;
}

void Student::eraseCourse(string name){
    for (auto& element : coursesWithGrades){
        if (element.first == name){
            coursesWithGrades.erase(name);
            if(element.second->grade >= 5){
                mandatoryPassed--;
            }
        }
    }
}

void from_json(const json& j, Student& student) {
    // Deserialize the basic student data
    j.at("firstName").get_to(student.firstName);
    j.at("lastName").get_to(student.lastName);
    j.at("idCode").get_to(student.idCode);
    j.at("registrationYear").get_to(student.registrationYear);
    j.at("mandatoryPassed").get_to(student.mandatoryPassed);
    j.at("currentPoints").get_to(student.currentPoints);

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

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////Professor class functions/////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Professor::Professor(string fName, string lName, string id)
: Person(fName, lName, id)
{}

//dynamically allocates and returns a copy of Professor 
Professor* Professor::clone(){
    try {
        return new Professor(*this);
    }
    catch(const bad_alloc &e){
        cerr << "Memory allocation failed: " << e.what() << '\n';
        return nullptr;
    }
}

//for now we just check equality of the super-class Person
bool Professor::equals(Professor* f) {
    return Person::equals(f);
}

void from_json(const json& j, Professor& p) {
    j.at("firstName").get_to(p.firstName);
    j.at("lastName").get_to(p.lastName);
    j.at("idCode").get_to(p.idCode);
    json courseMap = j.at("profCourses");
    for (const auto& courseItem : courseMap.items()) {
        const string& courseCode = courseItem.key();
        const json& semArray = courseItem.value();
        vector<pair<int, bool>> semesterPairs;
        for (const auto& item : semArray) {
            int year = item.at("Year");
            bool isWinter = item.at("isWinter");
            semesterPairs.emplace_back(year, isWinter);
        }
        p.profCourses[courseCode] = semesterPairs;
    }
}

void Professor::addCourse(string CourseCode, int year, bool season){
    profCourses[CourseCode].push_back(make_pair(year,season));
}

void Professor::eraseCourse(string code){
    for (auto& element : profCourses){
        if (element.first == code){
            profCourses.erase(code);
        }
    }
}

