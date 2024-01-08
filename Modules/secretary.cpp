#include "person.hpp"
#include "secretary.hpp"
#include "semester.hpp"
#include <chrono>
#include <thread>

//////Secretary class functions
Secretary::Secretary(const string& dep)
: department(dep)
{
    for (int i = 1; i <= 8; ++i){
        semesters.push_back(Semester(i));
    }
    //cout<<"Secretary " << department << " constructed!" <<endl;
    SecretaryOperation();
}

Secretary::Secretary(){
    for (int i = 1; i <= 8; ++i){
        semesters.push_back(Semester(i));
    }
    //cout << "Secretary constructed!" << endl;
    SecretaryOperation();

}

Secretary::~Secretary(){
    for(Person* it : myVec){
        //cout << "Deleted " << (*it)->getFirstName() << endl;
        delete it;
    }
    //cout << "Deleted secretary " << department << endl;
}

Secretary::Secretary(const Secretary& sec) //copy constructor for deep copy 
: department(sec.department)
{
    for (auto it = sec.myVec.begin(); it != sec.myVec.end(); ++it) {
        addPerson(**it, false);
    }

    for (const auto semesterInstance : sec.semesters) {
        semesters.push_back(Semester(semesterInstance));
    }
}

void Secretary::addPerson(Person& p, bool printStatement){

    Person *newP = p.clone();  // here we use the virtual function clone of the base class person instead of simply creating a 'new' person, 

    if (isStudent(newP)) {
        // Now that we know p is indeed a Student, we can call Student-specific methods
        Student* studentPtr = dynamic_cast<Student*>(newP);
        studentPtr->setSemester();
        myVec.push_back(studentPtr);
    }
    else if (isProfessor(newP)) {
        Professor* professorPtr = dynamic_cast<Professor*>(newP);
        myVec.push_back(professorPtr);
    }

    if (printStatement) cout << "Added " << newP->getFirstName() << " to " << department << "!" << endl;
}

//four different findPerson functions, each searching by a different property/Person&
Person* Secretary::findPersonByFirstName(const string& name){
    for(Person* i : myVec){
        if(i->getFirstName() == name){
            cout << "Person Found" << endl;
            return i;
        }
    }
    cout << "Person Not Found" << endl;
    return nullptr;
}

Person* Secretary::findPersonByLastName(const string& name) {
    for(Person* i : myVec){
        if(i->getLastName() == name){
            cout << "Person Found" << endl;
            return i;
        }
    }
    cout << "Person Not Found" << endl;
    return nullptr;
}

Person* Secretary::findPersonById(const string& id){
    for(Person* i : myVec){
        if(i->getIdCode() == id){
            cout << "Person Found" << endl;
            return i;
        }
    }
    cout << "Person Not Found" << endl;
    return nullptr;
}

Person* Secretary::findPerson(Person& p){
    for (Person* i : myVec){
        if(p.equals(i)){
            cout << "Person found" << endl;
            return i;
        }
    }
    cout << "Person Not Found" << endl;
    return nullptr;
}

//iterates vector until person with same properties is found(Person::equals function is used for this), delete Person, remove it from vector 
bool Secretary::removePerson(Person& p){
    for (auto i = myVec.begin(); i != myVec.end(); ++i){
        if(p.equals(*i)){
            cout << "Removed " << (*i)->getIdCode() << " from " << department << endl;
            delete *i;
            myVec.erase(i);       
            return true;
        }
    }
    return false;
}

void Secretary::printSecSize(){
    cout << "People in secretary: " << myVec.size() << endl;
}

void Secretary::setSecName(const string& dep){
    department = dep;
}

const string& Secretary::getSecName(){
    return department;
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
    for (Person* it : sec.myVec){
        addPerson(*it);
    }
    return *this;
}

//the member of a secretary are added to another, after it is cleared of its previous members
Secretary& Secretary::operator=(const Secretary& sec){
    if (this != &sec) {
        department = sec.department;
        for (Person* it : myVec) {
            delete it;
        }
        myVec.clear();
        for (Person* it : sec.myVec) {
            addPerson(*it,false);
        }
    }
    return *this;
}

//overloaded operator << for output of a Secretary object
ostream& operator<<(ostream& os,Secretary& secretary){
    os << "Secretary " << secretary.department << ":" << endl;
    for (Person* it : secretary.myVec){
        os << (*it); 
    }
    os << endl;
    return os;
}

//overloaded operator >> that allows the user to change/input the name of the Secretary and add as many People to it as they want
istream& operator>>(istream& is, Secretary& sec){
    char type;
    cout << "Enter Department name: ";
    is >> sec.department;
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

Semester* Secretary::getSemester(int num){
    if (1 <= num && num <= 8){
        return &semesters[num-1];
    }
    else{
        std::cout << "semester doesnt exist. Returned first semester\n";
        return &semesters[0];
    }
}

void Secretary::addCourse(){
    cout << "Enter Courses Semester: ";
    int semNum;
    cin >> semNum;
    if (semNum >= 1 && semNum <= 8){
        Course course;
        cin >> course;
        course.setSemester(semNum);
        std::cout << course.getName() <<" added to " << department << " at semester " << semNum << '\n';
        semesters[semNum-1].courses.push_back(course);
    }
    else{
        std::cout << "Semester does not exist\n";
        return;
    }
}

bool Secretary::startSemester(){
    SemesterStart = true;
    return SemesterStart;
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
        else if (op < 0 || op > 10){
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

        }
        else if (op == 10){
            cout << *this;
        }
    }
    
}

