#include "semester.hpp"

// find given course, and if given student is registered to it return the StudentCourseInstance*, else return nullptr
StudentCourseInstance* Semester::isRegistered(Course* course, Student* stud){
    auto it = courseStuds.find(course);
    if (it != courseStuds.end()){ // try to find course in courseStuds
        for (StudentCourseInstance* sci : it->second){ // iterates course's students until you find Student
            if (sci->stud->getIdCode() == stud->getIdCode()){
                return sci; // return sci if found 
            }
        }
    }
    return nullptr;
}
// add given to student to given course
// to do this a SemesterGradeInstance must first be created with the student and their grade(initialized to -1)
void Semester::addStudToCourse(Course* c, Student* s, bool print){
    // if course is not already in courseStuds insert it as key
    courseStuds.emplace(c, vector<StudentCourseInstance*>());
    StudentCourseInstance* sci = new StudentCourseInstance; //create a new studentCourseInstance with given student and initialize grade to -1
    sci->stud = s;
    sci->grade = -1;
    courseStuds[c].push_back(sci); //insert it to to SCI vector with given course as key in courseStud
    if (print){
        cout << s->getFirstName() << " " << s->getLastName() << " IS NOW REGISTERED TO " << c->getName() << '\n';
    }
}

// same function as above, but for professors
void Semester::addProfToCourse(Course* c, Professor* p, bool print){
    courseProfs.emplace(c, vector<Professor*>());
    for (Professor* prof : courseProfs[c]){
        if (prof->getIdCode() == p->getIdCode()){
            cout << "PROFESSOR ALREADY REGISTERED TO COURSE\n";
            return;
        }
    }
    courseProfs[c].push_back(p);
    if (print)
        cout << p->getFirstName() << " " << p->getLastName() <<" IS NOW TEACHING " << c->getName() << '\n';
}

// grade a student
// if there is no grade (grade is equal to -1), allow student to be graded
bool Semester::gradeStud(StudentCourseInstance* sci){
    if (sci->grade == -1){
        cout << "ENTER GRADE: ";
        int grade;
        cin >> grade;
        sci->grade = grade;
        return true;
    }
    else{
        cout << "STUDENT HAS ALREADY BEEN GRADED\n";
        return false;
    }
}

// print semester's year and season
void Semester::printSem(){
    cout << "YEAR: " << year << endl << "SEMESTER: " << (winterOrSummer ? "WINTER" : "SUMMER") << '\n';
}

// this function creates a json file(if it doesn't already exist) with the name "course name"-"year of semester"
// and writes in it the information of each student that has passed the given course in the given semester
// along with their grade. This is easily implemented through the use of the StudentCourseInstance struct 
// and the Semester::courseStuds map
// it also prints the students
void Semester::printPassed(Course* course) {
    string fileName = course->getName() + "-" + to_string(year) + ".json";
    cout << "COURSE: " << course->getName() << '\n';
    cout << "STUDENTS THAT HAVE PASSED: \n";

    // for each registered student, if grade is above 5 print their info and put them in the passedJson array
    for (StudentCourseInstance* sci : courseStuds[course]) {
        if (sci->grade >= 5) {
            cout << *(sci->stud);
            passedJson[course].push_back(sci->to_json());
        }
    }

    // open the file and write to it the updated information
    ofstream file(fileName);
    if (file.is_open()) {
        file << passedJson[course].dump(4);
        file.close();
    } else {
        cout << "Failed to open file for writing.\n";
    }
}

// for each course that the professor teaches in this semester, print its info
// and its statistics for registered and passing students by accessing the Semester::courseProfs map  
void Semester::printProfStats(Professor* prof){
    int count;
    for (auto& element : courseProfs){ // iterate courseProfs until you find prof
        for (Professor* profTemp : element.second){
            if (profTemp == prof){ // print statistics
                cout << "\nCOURSE NAME AND ID: " << element.first->getName() << ", " << element.first->getCode() << '\n';
                cout << "ACADEMIC POINTS: " << element.first->getAcademicPoints() << '\n';
                cout << "NUMBER REGISTERED STUDENTS: "<< courseStuds[element.first].size() << '\n';
                count = 0;
                for (auto& sci : courseStuds[element.first]){
                    if (sci->grade >= 5){
                        ++count;
                    }
                }
                cout << "NUMBERS OF STUDENTS WHO PASSED: " << count << "\n";
            //print the percentage only if there are registered students, as a division by 0 would lead to error
                if (!courseStuds[element.first].empty())
                    cout << "PASSING PERCENTAGE: " << static_cast<float>(count)/courseStuds[element.first].size()*100 << "%\n";
            }
        }
    }
}

// for each course of Semester::CourseStuds, if the given student is registered and graded, print their grade 
void Semester::printStudGrades(Student* stud){
    for (auto& element : courseStuds){ // iterate courseStuds
        for (StudentCourseInstance*  sci : element.second){ // for each course iterates its students
            if (sci->stud == stud){ // if found student in course, and there is a grade(not -1), print course name and grade
                if (sci->grade == -1){
                    cout << "NO GRADE AVAILABLE FOR " << element.first->getName() << '\n';
                }
                else{
                    cout << "COURSE: " << element.first->getName() << ", GRADE: " <<  sci->grade << '\n';
                }
            }
        }
    }
}

//iterates maps courseProfs and courseStuds and erases courses that have the given code
void Semester::eraseCourse(string code){
    for (auto& element : courseProfs){
        if (element.first->getCode() == code){
            courseProfs.erase(element.first);
        }
    }
    for (auto& element : courseStuds){
        if (element.first->getCode() == code){
            courseProfs.erase(element.first);
        }
    }
}

//input operator overload for reading year and season of semester
istream& operator>>(istream& is, Semester& semester) {
    std::cout << "ENTER SEMESTER YEAR: ";
    is >> semester.year;

    cout << "ENTER 'W' FOR WINTER SEMESTER, 'S' FOR SUMMER: ";
    char sem;
    is >> sem;

    semester.winterOrSummer = (sem == 'W' || sem == 'w');

    return is;
}