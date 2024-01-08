#include "person.hpp"
#include "secretary.hpp"
#include "Semester.hpp"

//////Secretary class functions
Secretary::Secretary(const string& dep)
: department(dep)
{
    for (int i = 1; i <= 8; ++i){
        semesters.push_back(Semester(i));
    }
    //cout<<"Secretary " << department << " constructed!" <<endl;
    printMenu();
}

Secretary::Secretary(){
    for (int i = 1; i <= 8; ++i){
        semesters.push_back(Semester(i));
    }
    //cout << "Secretary constructed!" << endl;
    printMenu();

}

Secretary::~Secretary(){
    for(Person* it : myVec){
        //cout << "Deleted " << (*it)->getFirstName() << endl;
        delete it;
    }
    cout << "Deleted secretary " << department << endl;
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

    // // Check if the cast was successful
    // if (isStudent(&p)) {
    //     // Now that we know p is indeed a Student, we can call Student-specific methods
    //     newS->setSemester();
    // }

    Person *newP = p.clone();  // here we use the virtual function clone of the base class person instead of simply creating a 'new' person, 

    myVec.push_back(newP);
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
            //cout << "Enter Name, Surname and ID Code: " << endl;
            is >> f;
            sec.addPerson(f);
        }
        cout << "Enter s for student, f for Professor, 0 to stop adding people:" << endl;
        is >> type;
    }
    return is;
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

void Secretary::addCourse(const Course& course,int semNum){
    if (semNum >= 1 && semNum <= 8){
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
    cout << "TYPE 0 TO EXIT\n";
    SecretaryOperation();
}

void Secretary::SecretaryOperation(){
    int op;
    while (true){
        cin >> op;
        if (op == 0){
            cout << "GOODBYE <3";
            return;
        }
        else if (op < 0 || op > 9){
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
                Professor peepee;
                cin >> peepee;
              //  addProfessor(peepee);
            }
            else if (op == 2){
                cout << "INPUT PROFESSOR ID: ";
                string id;
                cin >> id;
                Professor prof = findPersonrById(id);

                
            }
            else if (op == 3){
                
            }
        }
    }
    
}

