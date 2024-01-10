#include "secretary.hpp"

//////Secretary class functions
Secretary::Secretary(const string& dep, int sem, int reqPoints)
: depName(dep), depSemesters(sem), endSemester(false), pointsToGraduate(reqPoints)//, numOfMandatory(0)
{
    readStudentsFromFile();
    readProfessorsFromFile();
    readCourseFromFile();
    SecretaryOperation();
    //cout<<"Secretary " << depName << " constructed!" <<endl;
}

Secretary::Secretary(){
    readStudentsFromFile();
    readProfessorsFromFile();
    endSemester = false;
    SecretaryOperation();
    //cout << "Secretary constructed!" << endl;

}

Secretary::~Secretary(){
    for(Person* it : depMembers){
        //cout << "Deleted " << (*it)->getFirstName() << endl;
        delete it;
    }
    //cout << "Deleted secretary " << depName << endl;
}

//NMZW O COPY D XREIAZETAI NA ASXOLOUMASTE ME TA INTS
Secretary::Secretary(const Secretary& sec) //copy constructor for deep copy 
: depName(sec.depName), depSemesters(sec.depSemesters), endSemester(sec.endSemester)
{
    for (auto it = sec.depMembers.begin(); it != sec.depMembers.end(); ++it) {
        addPerson(**it, false);
    }

}

void Secretary::addPerson(Person& p, bool printStatement){

    Person *newP = p.clone();  // here we use the virtual function clone of the base class person instead of simply creating a 'new' person, 

    if (isStudent(newP)) {
        // Now that we know p is indeed a Student, we can call Student-specific methods
        Student* studentPtr = dynamic_cast<Student*>(newP);
        //studentPtr->setSemester();
        depMembers.push_back(studentPtr);
    }
    else if (isProfessor(newP)) {
        Professor* professorPtr = dynamic_cast<Professor*>(newP);
        depMembers.push_back(professorPtr);
    }

    if (printStatement) cout << "Added " << newP->getFirstName() << " to " << depName << "!" << endl;
}

//four different findPerson functions, each searching by a different property/Person&
Person* Secretary::findPersonByFirstName(const string& name){
    for(Person* i : depMembers){
        if(i->getFirstName() == name){
            cout << "Person Found" << endl;
            return i;
        }
    }
    cout << "Person Not Found" << endl;
    return nullptr;
}

Person* Secretary::findPersonByLastName(const string& name) {
    for(Person* i : depMembers){
        if(i->getLastName() == name){
            cout << "Person Found" << endl;
            return i;
        }
    }
    cout << "Person Not Found" << endl;
    return nullptr;
}

Person* Secretary::findPersonById(const string& id){
    for(Person* i : depMembers){
        if(i->getIdCode() == id){
            cout << "Person Found" << endl;
            return i;
        }
    }
    cout << "Person Not Found" << endl;
    return nullptr;
}

Person* Secretary::findPerson(Person& p){
    for (Person* i : depMembers){
        if(p.equals(i)){
            cout << "Person found" << endl;
            return i;
        }
    }
    cout << "Person Not Found" << endl;
    return nullptr;
}

Course* Secretary::findCourse(string name){
    for (Course& course : depCourses){
        if (course.getName() == name){
            cout << "Found course \n";
            return &course;
        }
    }
    return nullptr;
}

void Secretary::changeSemester(Course&  course){
    cout << "Enter new semester: ";
    int sem;
    cin >> sem;
    course.setSemester(sem);
    cout << course.getName() << " semester changed to " << course.getSemester() << '\n';
}

void Secretary::modifyCourse(Course& course){
    string prevName = course.getName();
    cout << "Please enter new attributes\n";
    cin >> course;
    cout << "Course " << prevName << " changed to " << course.getName() << '\n';
}

//iterates vector until person with same properties is found(Person::equals function is used for this), delete Person, remove it from vector 
bool Secretary::removePerson(Person& p){
    for (auto i = depMembers.begin(); i != depMembers.end(); ++i){
        if(p.equals(*i)){
            cout << "Removed " << (*i)->getIdCode() << " from " << depName << endl;
            delete *i;
            depMembers.erase(i);       
            return true;
        }
    }
    return false;
}

void Secretary::printSecSize(){
    cout << "People in secretary: " << depMembers.size() << endl;
}

void Secretary::setSecName(const string& dep){
    depName = dep;
}

const string& Secretary::getSecName(){
    return depName;
}


//uses a dynamic cast to Student pointer to check if Person* is a Student*
bool Secretary::isStudent(Person *p){
    return dynamic_cast<Student *> (p) != nullptr;
}

//as above but for Professor
bool Secretary::isProfessor(Person *p){
    return dynamic_cast<Professor *> (p) != nullptr;
}

//Overloaded operator + to add a Person to a Secretary 
Secretary& Secretary::operator+(Person& p){
    addPerson(p);
    return *this;
}

//Overloaded operator += to add members of a secretary to the end of another// merges the two secretaries 
Secretary& Secretary::operator+=(const Secretary& sec){
    for (Person* it : sec.depMembers){
        addPerson(*it);
    }
    return *this;
}

//the member of a secretary are added to another, after it is cleared of its previous members
Secretary& Secretary::operator=(const Secretary& sec){
    if (this != &sec) {
        depName = sec.depName;
        for (Person* it : depMembers) {
            delete it;
        }
        depMembers.clear();
        for (Person* it : sec.depMembers) {
            addPerson(*it,false);
        }
    }
    return *this;
}

//overloaded operator << for output of a Secretary object
ostream& operator<<(ostream& os,Secretary& secretary){
    os << "Secretary " << secretary.depName << ":" << endl;
    for (Person* it : secretary.depMembers){
        os << (*it); 
    }
    os << endl;
    return os;
}

//overloaded operator >> that allows the user to change/input the name of the Secretary and add as many People to it as they want
istream& operator>>(istream& is, Secretary& sec){
    char type;
    cout << "Enter Department name: ";
    is >> sec.depName;
    cout << "Enter s for student, f for Professor, 0 to stop adding people:" << endl;
    is >> type;
    while(type !='0'){
        if(type == 's'){
            Student s;
            //cout << "Enter Name, Surname and ID Code: " << endl;
            is >> s;
            sec.addPerson(s);
        }
        if(type == 'f'){
            Professor f;
            cout << "Enter Name, Surname and ID Code: " << endl;
            is >> f;
            sec.addPerson(f);
        }
        cout << "Enter s for student, f for Professor, 0 to stop adding people:" << endl;
        is >> type;
    }
    return is;
}

void Secretary::addProfessor(){
    Professor f;
    //cout << "Enter Name, Surname and ID Code: " << endl;
    cin >> f;
    addPerson(f);
}

void Secretary::addStudent(){
    Student s;
    //cout << "Enter Name, Surname and ID Code: " << endl;
    cin >> s;
    addPerson(s);
}


void Secretary::addCourse(){
    Course course;
    cin >> course;
    cout << course.getName() <<" added to " << depName << " at semester " << course.getSemester() << '\n';
    depCourses.push_back(course);
    if(course.getMand()){
        ++numOfMandatory;
    }
}

void Secretary::removeCourse(Course& course){
    for (size_t i = 0; i < depCourses.size(); ++i){
        if (depCourses[i] == course){
            cout << "Erased " << course.getName() << '\n';
            depCourses.erase(depCourses.begin() + i);
            break;
        }
    }
}


void Secretary::printMenu(){
    cout << "\tECLASS\n";
    cout << "1. PROFESSOR OPTIONS\n";
    cout << "2. STUDENT OPTIONS\n";
    cout << "3. COURSE OPTIONS\n";
    cout << "4. SET COURSE PROFESSOR\n";
    cout << "5. REGISTER TO COURSE(STUDENT ONLY)\n";
    cout << "6. STUDENTS WHO PASSED A COURSE\n";
    cout << "7. GET COURSE STATS(PROF ONLY)\n";
    cout << "8. GET GRADES(STUDENT ONLY)\n";
    cout << "9. WHO GRADUATES?\n";
    cout << "10. PRINT DEPARTMENT MEMBERS\n";
    cout << "11. END SEMESTER\n";
    cout << "12. PRINT DEPARTMENT INFO\n";
    cout << "TYPE 0 TO EXIT\n";
}

void Secretary::SecretaryOperation(){
    int op;
    while (true){
        this_thread::sleep_for(chrono::seconds(1));
        printMenu();
        cin >> op;
        if (op == 0){
            cout << "\tGOODBYE\n";
            return;
        }
        else if (op < 0 || op > 12){
            cout << "WRONG INPUT\n";
            cout << "INPUT AGAIN\n";
            continue;
        }
        else if (op == 1){
            cout << "1. ADD PROFESSOR\n";
            cout << "2. MODIFY PROFESSOR\n";
            cout << "3. REMOVE PROFESSOR\n";
            cin >> op;
            if (op == 1){
                addProfessor();
            }
            else if (op == 2){
                cout << "INPUT PROFESSOR ID: ";
                string id;
                cin >> id;
                Person* person = findPersonById(id);
                if (isProfessor(person)) {  //Check if person is professor
                    Professor* prof = dynamic_cast<Professor*>(person);
                    string prevName = prof->getFirstName() + " " + prof->getLastName();
                    cout << "Please enter the new attributes of the professor\n";
                    cin >> *prof;
                    cout << "Professor " << prevName << " changed to " << prof->getFirstName() << " " << prof->getLastName() << '\n';

                } else if (person != nullptr) { //if person is found but isnt a professor
                    cout << "The person with ID " << id << " is not a Professor.\n";
                }

                
            }
            else if (op == 3){
                cout << "INPUT PROFESSOR ID: ";
                string id;
                cin >> id;
                Person* person = findPersonById(id);
                if (isProfessor(person)) {  //same as above
                    removePerson(*person);
                } else if (person != nullptr) {
                    cout << "The person with ID " << id << " is not a Professor.\n";
                }
            }
        }
        else if (op == 2){
            cout << "1. ADD STUDENT\n";
            cout << "2. MODIFY STUDENT\n";
            cout << "3. REMOVE STUDENT\n";
            cin >> op;
            if (op == 1){
                addStudent();
            }
            else if (op == 2){
                cout << "INPUT STUDENT ID: ";
                string id;
                cin >> id;
                Person* person = findPersonById(id);
                if (isStudent(person)) { //check if person is student
                    Student* stud = dynamic_cast<Student*>(person);
                    string prevName = stud->getFirstName() + " " + stud->getLastName();
                    cout << "Please enter the new attributes of the student\n";
                    cin >> *stud;
                    cout << "Student " << prevName << " changed to " << stud->getFirstName() << " " << stud->getLastName() << '\n';

                } else if (person != nullptr) { //if person is found and isnt a student 
                    cout << "The person with ID " << id << " is not a Student.\n";
                }
            }
            else if (op == 3){
                cout << "INPUT STUDENT ID: ";
                string id;
                cin >> id;
                Person* person = findPersonById(id);
                if (isStudent(person)) {    //same as above
                    removePerson(*person);
                } else if (person != nullptr){
                    cout << "The person with ID " << id << " is not a Student.\n";
                }
            }
        }
        else if (op == 3){
            cout << "1. ADD COURSE\n";
            cout << "2. MODIFY COURSE\n";
            cout << "3. REMOVE COURSE\n";
            cin >> op;
            if (op == 1){
                addCourse();
            }
            else if (op == 2){
                cout << "1. CHANGE SEMESTER\n";
                cout << "2. MODIFY ALL ATTRIBUTES\n";
                cin >> op;
                if (op == 1){
                    string name;
                    cout << "Enter Course name: ";
                    cin >> name;
                    Course* course = findCourse(name);
                    if (course != nullptr){
                        changeSemester(*course);
                    }
                    else 
                        cout << "Course not found!\n";
                }
                else if (op == 2){
                    string name;
                    cout << "Enter Course name: ";
                    cin >> name;     
                    Course* course = findCourse(name); 
                    if (course != nullptr){
                        modifyCourse(*course);
                    }
                    else   
                        cout << "Course not found!\n";            
                }
            }
            else if (op == 3){
                string name;
                cout << "Enter Course name: ";
                cin >> name;     
                Course* course = findCourse(name);
                if (course != nullptr){
                    removeCourse(*course);
                } 
                else
                    cout << "Course not found!\n";
            }

        }
        else if (op == 4){
            cout << "Enter the course name: \n";
            string name;
            cin >> name;
            Course* course = findCourse(name);
            if (course != nullptr){
                cout << "Enter professor id: ";
                string id;
                cin >> id;
                Person* person = findPersonById(id);
                if (isProfessor(person)){
                    Professor* prof = dynamic_cast<Professor*>(person);
                    prof->profAddCourse(*course);
                }
                else
                    cout << "The person with ID " << id << " is not a Professor.\n";
            }
            else
                cout << "Course not found!\n";
        }
        else if (op == 5){
            cout << "Enter the course name: \n";
            string name;
            cin >> name;
            Course* course = findCourse(name);
            if (course != nullptr){
                cout << "Enter Student id: ";
                string id;
                cin >> id;
                Person* person = findPersonById(id);
                if (isStudent(person)){
                    Student* stud = dynamic_cast<Student*>(person);
                    stud->studAddCourse(*course);
                }
                else{
                    cout << "The person with ID " << id << " is not a Student.\n";
                }
            }
            else
                cout << "Course not found!\n";
        }
        else if(op == 6){
            Course* course = readAndFindCourse();
            course->printStudentsWhoPassed();
        }
        else if(op == 7){
            Professor* prof = readAndFindProfessor();
            if(prof != nullptr){
                cout << "Enter semester for stats: ";
                int sem;
                cin >> sem;
                prof->printStats(sem);
            }
        }
        else if(op == 8){
            Student* stud = readAndFindStudent();
            if (stud != nullptr){
                stud->printGrades();
            }
        }
        else if(op == 9){
            printGraduates();
        }
        else if (op == 10){
            cout << *this;
        }
        else if (op == 12){
            cout << "Department name: " <<depName << '\n';
            cout << "Number of Semesters: " << depSemesters << '\n';
            cout << "Required academic points for degree: " << pointsToGraduate << '\n';
            cout << "Number of mandatory courses: " << numOfMandatory << '\n';
        }
        else if (op == 11){
            endSemester = true;
            while (endSemester == true){
                this_thread::sleep_for(chrono::seconds(1));
                printExamsMenu();
                cin >> op;
                if (op == 1){
                    cout << "Enter the course name: \n";
                    string name;
                    cin >> name;
                    Course* course = findCourse(name);
                    if (course != nullptr){
                        cout << "Enter professor id: ";
                        string id;
                        cin >> id;
                        Person* person = findPersonById(id);
                        Professor* prof = dynamic_cast<Professor*>(person);
                        if (isProfessor(person) && prof->teachesCourse(*course)){
                            Student* stud = readAndFindStudent();
                            if (stud != nullptr){
                                stud->studentChangeGrade(*course);
                                stud->printGradesToto();
                            }
                        }
                        else{
                            cout << "Incorrect search\n";
                        }
                    }
                }
                else if (op == 2){
                    Student* stud = readAndFindStudent();
                    if (stud != nullptr){
                        stud->printGrades(true);
                    }
                }
                else if (op == 4){
                    nextSemester();
                }
            }
        }
    }
    
}

void Secretary::printExamsMenu(){
    cout << "\tSEMESTER END MENU\n";
    cout << "1. GRADE STUDENTS\n";
    cout << "2. PRINT STUDENT'S GRADES\n";
    cout << "4. START NEXT SEMESTER\n";
}

void Secretary::printGraduates(){
    for (Person* person : depMembers){
        if (isStudent(person)){
            Student* stud = dynamic_cast<Student*>(person);
            if(stud->getSemesterCount() >= depSemesters && stud->getMandatoryPassed()==getNumOfMandatory() && stud->getAcademicPoints() >= pointsToGraduate){
                cout << *stud;
            }
        }
    }
}

Student* Secretary::readAndFindStudent(){
    cout << "Enter Student id: ";
    string id;
    cin >> id;
    Person* person = findPersonById(id);
    if (isStudent(person)){
        return dynamic_cast<Student*>(person);
    }
    else{
        cout << "The person with ID " << id << " is not a Student.\n";
        return nullptr;
    }
}

Professor* Secretary::readAndFindProfessor(){
    cout << "Enter Professor id: ";
    string id;
    cin >> id;
    Person* person = findPersonById(id);
    if (isProfessor(person)){
        return dynamic_cast<Professor*>(person);
    }
    else{
        cout << "The person with ID " << id << " is not a Professor.\n";
        return nullptr;
    }
}

Course* Secretary::readAndFindCourse(){
    cout << "Enter the course name: \n";
    string name;
    cin >> name;
    Course* course = findCourse(name);
    if (course == nullptr){
        cout << "Course not found\n";
    }
    return course;
}

void Secretary::nextSemester(){
    endSemester = false;
    for (Person* person : depMembers){
        if (isStudent(person)){
            Student* stud = dynamic_cast<Student*>(person);
            stud->setSemester(0,true);
        }
    }
}


void Secretary::readStudentsFromFile(){
    ifstream file("student-info.txt");
    if (file.is_open()) {
        string line;
        while (getline(file, line)) {
            Student stud;
            istringstream iss(line);
            string word;
            for (int i = 0; i <= 3; ++i) {
                iss >> word;
                if (i == 0){
                    stud.setFirstName(word);
                }
                else if (i == 1){
                    stud.setLastName(word);
                }
                else if (i == 2){
                    stud.setIdCode(word);
                }
                else if (i == 3){
                    stud.setSemester(stoi(word));
                }
            }
            addPerson(stud, false);
        }
        file.close();
    } else {
        cerr << "Unable to open file" << endl;
    }
}

void Secretary::readProfessorsFromFile(){
    ifstream file("professor-info.txt");
    if (file.is_open()) {
        string line;
        while (getline(file, line)) {
            Professor prof;
            istringstream iss(line);
            string word;
            for (int i = 0; i <= 2; ++i) {
                iss >> word;
                if (i == 0){
                    prof.setFirstName(word);
                }
                else if (i == 1){
                    prof.setLastName(word);
                }
                else if (i == 2){
                    prof.setIdCode(word);
                }

            }
            addPerson(prof, false);
        }
        file.close();
    } else {
        cerr << "Unable to open file" << endl;
    }
}

void Secretary::readCourseFromFile(){
    ifstream file("course-info.txt");
    if (file.is_open()) {
        string line;
        while (getline(file, line)) {
            Course course;
            istringstream iss(line);
            string word;
            for (int i = 0; i <= 3; ++i) {
                iss >> word;
                if (i == 0){
                    course.setName(word);
                }
                else if (i == 1){
                    course.setSemester(stoi(word));
                }
                else if (i == 2){
                    course.setAcademicPoints(stoi(word));
                }
                else if (i == 3){
                    course.setMand(word);
                }

            }
            depCourses.push_back(course);
            if(course.getMand()){
                ++numOfMandatory;
            }
        }
        file.close();
    } else {
        cerr << "Unable to open file" << endl;
    }
}
