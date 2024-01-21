#include "semester.hpp"

void Semester::addProfToCourse(Course* c, Professor* p, bool print){
    courseProfs.emplace(c,vector<Professor*>());
    for (Professor* prof : courseProfs[c]){
        if (prof->getIdCode() == p->getIdCode()){
            cout << "Professor already set to this course\n";
            return;
        }
    }
    courseProfs[c].push_back(p);
    if (print)
        cout << p->getFirstName() << " " << p->getLastName() << " " << "is now professor of " << c->getName() << '\n';
}

void Semester::addStudToCourse(Course* c, Student* s, bool print){
    courseStuds.emplace(c,vector<StudentCourseInstance*>());
    StudentCourseInstance* sci = new StudentCourseInstance;
    sci->stud = s;
    sci->grade = -1;
    courseStuds[c].push_back(sci);
    if (print){
        cout << s->getFirstName() << " " << s->getLastName() << " is now registered to " << c->getName() << '\n';
    }
}

istream& operator>>(istream& is, Semester& semester) {
    std::cout << "Enter year of Semester: ";
    is >> semester.year;

    cout << "Enter W for winter semester, S for summer: ";
    char sem;
    is >> sem;

    semester.winterOrSummer = (sem == 'W' || sem == 'w');

    return is;
}

StudentCourseInstance* Semester::isRegistered(Course* course, Student* stud){
    for (auto& element : courseStuds){
        if (element.first->getCode() == course->getCode()){
            for (StudentCourseInstance* sci : element.second){
                if (sci->stud->getIdCode() == stud->getIdCode()){
                    return sci;
                }
            }
        }
    }
    return nullptr;
}

bool Semester::gradeStud(StudentCourseInstance* sci){
    if (sci->grade == -1){
        cout << "Enter desired grade: ";
        int grade;
        cin >> grade;
        sci->grade = grade;
        return true;
    }
    else{
        cout << "Student already graded\n";
        return false;
    }
}

void Semester::printPassed(Course* course) {
    string fileName = course->getName() + "-" + to_string(year) + ".json";
    cout << "Course: " << course->getName() << '\n';
    cout << "Students passed: \n";
    for (StudentCourseInstance* sci : courseStuds[course]) {
        if (sci->grade >= 5) {
            cout << sci->stud->getFirstName() << " " << sci->stud->getLastName() << " " << sci->stud->getIdCode() << '\n';
            passedJson[course].push_back(sci->to_json());
        }
    }

    // Write updated information to the file
    ofstream file(fileName);
    if (file.is_open()) {
        file << passedJson[course].dump(4);
        file.close();
    } else {
        cout << "Failed to open file for writing.\n";
    }
}



void Semester::printProfStats(Professor* prof){
    int count;
    for (auto& element : courseProfs){
        for (Professor* profTemp : element.second){
            if (profTemp == prof){
                cout << "\nCourse: " << element.first->getName() << '\n';
                cout << "Course academic points: " << element.first->getAcademicPoints() << '\n';
                cout << "Students registered to course: "<< courseStuds[element.first].size() << '\n';
                count = 0;
                for (auto& sci : courseStuds[element.first]){
                    if (sci->grade >= 5){
                        ++count;
                    }
                }
                cout << "Students passed: " << count << "\n";
                if (!courseStuds[element.first].empty())
                    cout << "Percentage of students who passed: " << static_cast<float>(count)/courseStuds[element.first].size()*100 << "%\n";
            }
        }
    }
}

void Semester::printStudStats(Student* stud){
    for (auto& element : courseStuds){
        for (StudentCourseInstance*  sci : element.second){
            if (sci->stud == stud){
                if (sci->grade == -1){
                    cout << "Course: " << element.first->getName() << " has not been graded yet\n";
                }
                else{
                    cout << "Course: " << element.first->getName() << ", Grade: " <<  sci->grade << '\n';
                }
            }
        }
    }
}


void Semester::setYear(int yearIn){
    year = yearIn;
}

void Semester::setSeason(bool season){
    winterOrSummer = season;
}

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