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
    if (stud != nullptr){ // dynamic cast to check if person is a student. if yes, read also year of registration
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

// deletes all SemesterGradeInstance* inside Student::coursesWithGrades
// this operation is not done in Student's destructor, because it would be called twice
// and there would be a double free error
// so this function is called from secretary's destructor as each Student* is deleted
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
    catch(const bad_alloc &e){ // if unable to allocate throw exception
        cerr << "MEMORY ALLOCATION FAILED: " << e.what() << '\n';
        return nullptr;
    }
}

//for now we just check equality of the super-class Person
bool Student::equals(Student* s) {
    return Person::equals(s) && registrationYear == s->getReg();
}

// print all of a student's grades
// for each SemesterGradeInstance in coursesWithGrades, print course name and grade
void Student::printGrades(){
    for (auto& element : coursesWithGrades){
        cout << "COURSE: " << element.first<< ", GRADE: " << element.second->grade << '\n';
    }
}

// adds to Student::courseWithGrades a new SemesterGradeInstance which contains a new grade
// if that grade is >= 5 and the course is mandatory , increment student's mandatoryPassed
void Student::addCourseWithGrade(Course* course, SemesterGradeInstance* sgi){
    coursesWithGrades[course->getName()] = sgi;
    if (sgi->grade >= 5 && course->getMand()){
        mandatoryPassed++;
        currentPoints+= course->getAcademicPoints();
    }
}

// iterates courseWithGrades to find course, if found return Student's grade, else return -1
int Student::getCourseGrade(Course* course){
    for (auto& element : coursesWithGrades){
        if (element.first == course->getName()){
            return element.second->grade;
        }
    }
    return -1;
}

// find course with given name in coursesWithGrades and erases it
// if student has passed it and it is mandatory, decrease mandatory passed
void Student::eraseCourse(string name, bool isMand){
    for (auto& element : coursesWithGrades){
        if (element.first == name){
            coursesWithGrades.erase(name);
            if(element.second->grade >= 5 && isMand){
                mandatoryPassed--;
            }
        }
    }
}

// deserialize a json object into a Student object
// this function takes a json object representing a student and parses it
// to fill the fields of a Student object. The json structure is expected to include the student's
// first and last name, id code, registration year, information about mandatory courses passed,
// current academic points and a list of courses with grades.
void from_json(const json& j, Student& student) {
    // deserialize basic student data
    j.at("firstName").get_to(student.firstName);
    j.at("lastName").get_to(student.lastName);
    j.at("idCode").get_to(student.idCode);
    j.at("registrationYear").get_to(student.registrationYear);
    j.at("mandatoryPassed").get_to(student.mandatoryPassed);
    j.at("currentPoints").get_to(student.currentPoints);

    // deserialize coursesWithGrades
    if (j.contains("coursesWithGrades")) { // if there are courses with grades in the student's data, load them as well
        const json& coursesJson = j.at("coursesWithGrades");
        for (const auto& item : coursesJson.items()) {
            string courseName = item.key(); // this is a mapping of course names to their corresponding grades and semesters
            SemesterGradeInstance* gradeInstance = new SemesterGradeInstance;
            from_json(item.value(), *gradeInstance); 

            // assign the new gradeInstance to the student's coursesWithGrades map
            student.coursesWithGrades[courseName] = gradeInstance;

        }
    }
}

// deserializes studentCourseInstance
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
        cerr << "MEMORY ALLOCATION FAILED: " << e.what() << '\n';
        return nullptr;
    }
}

//for now we just check equality of the super-class Person
bool Professor::equals(Professor* f) {
    return Person::equals(f);
}


// deserialize a json object into a Professor object
// this function takes a json object representing a professor and parses it
// to fill the fields of a Professor object. The expected json
// structure includes the professor's first name, last name, id code, and a
// map of courses they teach, along with the semesters and years those
// courses are taught.
void from_json(const json& j, Professor& p) {
    // assign basic attributes: first name, last name, and id code
    j.at("firstName").get_to(p.firstName);
    j.at("lastName").get_to(p.lastName);
    j.at("idCode").get_to(p.idCode);

    // extract the mapping of courses
    json courseMap = j.at("profCourses");
    for (const auto& courseItem : courseMap.items()) {
        // extract course code as key
        const string& courseCode = courseItem.key();
        const json& semArray = courseItem.value();

        // vector for year-season of semester pairs
        vector<pair<int, bool>> semesterPairs;

        // iterate semester array
        for (const auto& item : semArray) {
            // extract semester info
            int year = item.at("Year");
            bool isWinter = item.at("isWinter");

            // store semester info as pair
            semesterPairs.emplace_back(year, isWinter);
        }

        // assign the semester pairs to the respective course in profCourses
        p.profCourses[courseCode] = semesterPairs;
    }
}

// add a course code along with a semester year and season to Professor::profCourses map
// *** not a Course* or Semester* ***
void Professor::addCourse(string CourseCode, int year, bool season){
    profCourses[CourseCode].push_back(make_pair(year,season));
}

// find course by code in profCourses map and erase it
void Professor::eraseCourse(string code){
    if(profCourses.find(code) != profCourses.end()){
        profCourses.erase(code);
    }
}

