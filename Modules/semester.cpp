#include "semester.hpp"

void Semester::addProfToCourse(Course* c, Professor* p){
    courseProfs.emplace(c,vector<Professor*>());
    for (Professor* prof : courseProfs[c]){
        if (prof->getIdCode() == p->getIdCode()){
            cout << "Professor already set to this course\n";
            return;
        }
    }
    courseProfs[c].push_back(p);
    cout << p->getFirstName() << " " << p->getLastName() << " " << "is now professor of " << c->getName() << '\n';
}

void Semester::addStudToCourse(Course* c, Student* s){
    courseStuds.emplace(c,vector<StudentCourseInstance*>());
    StudentCourseInstance* sci = new StudentCourseInstance;
    sci->stud = s;
    sci->grade = -1;
    courseStuds[c].push_back(sci);
    cout << s->getFirstName() << " " << s->getLastName() << " is now registered to " << c->getName() << '\n';
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

void Semester::printPassed(Course* course){
    for (auto& element : courseStuds){
        if (element.first->getCode() == course->getCode()){
            cout << "Course: " << course->getName() << '\n';
            cout << "Students passed: \n";
            for (StudentCourseInstance* sci : element.second){
                if (sci->grade >= 5){
                    cout << sci->stud->getFirstName() << " " << sci->stud->getLastName() << " " << sci->stud->getIdCode() << '\n';
                }
            }
        }
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
                    if (sci->grade != -1){
                        ++count;
                    }
                }
                cout << "Students graded: " << count << "\n";
            }
        }
    }
}