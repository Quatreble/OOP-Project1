#pragma once
using namespace std;

class Secretary {
private:    
    vector<Person *> myVec;
// we choose to use a vector instead of a map since we would like to be able 
// to search with all Person's properties using linear iteration. Since all takes place
// in memory there is no significant performance hit to go through every Person  ( O(n) )
//ennoeitai egw to grapsa auto^
    string department;
public:
    Secretary(const string& dep)
    : department(dep)
    {
        cout<<"Secretary constructed! : "<< department <<endl;
        cout<<"People in secretary: " << myVec.size()<<endl;
    }

    ~Secretary(){
        for(auto it = myVec.begin(); it != myVec.end(); ++it){
            cout << "Deleted " << (*it)->getFirstName() << endl;
            delete *it;
        }
        cout << "Deleted secretary " << department << endl;
    }

    Secretary(const Secretary& Sec)
    : department(Sec.department), myVec(Sec.myVec)
    {}

    void addPerson(Person& p){
        Person *newP;
        if(isStudent(&p)){
            newP = new Student(p);
        }
        if(isFaculty(&p)){
            newP = new Faculty(p);
        }
        myVec.push_back(newP);
        cout << "Added " << newP->getFirstName() << "!" << endl;
    }


    Person* findPersonByFirstName(const string& name){
        for(auto i = myVec.begin(); i != myVec.end(); ++i){
            if((*i)->getFirstName() == name){
                return *i;
            }
        }
        cout << "Not found" << endl;
        return nullptr;
    }

    Person* findPersonByLastName(const string& name) {
        for(auto i = myVec.begin(); i != myVec.end(); ++i){
            if((*i)->getLastName() == name){
                return *i;
            }
        }
        cout << "Not found" << endl;
        return nullptr;
    }

    Person* findPersonById(const string& id){
        for(auto i = myVec.begin(); i != myVec.end(); ++i){
            if((*i)->getIdCode() == id){
                return *i;
            }
        }
        cout << "Not found" << endl;
        return nullptr;
    }

    Person* findPerson(Person& p){
        for (auto i = myVec.begin(); i != myVec.end(); ++i){
            if(p.personEqual(*i)){
                cout << "Person found!" << endl;
                return *i;
            }
        }
        cout << "Person not found:(" << endl;
        return nullptr;
    }

    void removePerson(Person& p){;
    //check that it is not null?
        for (auto i = myVec.begin(); i != myVec.end(); ++i){
            if(p.personEqual(*i)){
                myVec.erase(i);    //prwta erase apto vec k meta delete to pointer
                delete *i;
                cout << "People in secretary after deletion: " << myVec.size() << endl;
                return;
            }
        }
    }

    void editFirstName(Person& p, const string& newName){
        Person* ptr = findPerson(p);
        if(ptr!=nullptr){
            ptr->setFirstName(newName);
        }
       // findPerson(p)->setFirstName(newName);
    }
    void editLastName(Person& p, const string& newName){
        Person* ptr = findPerson(p);
        if(ptr!=nullptr){
            ptr->setLastName(newName);
        }
    }
    void editIDName(Person& p, const string& newId){
        Person* ptr = findPerson(p);
        if(ptr!=nullptr){
            ptr->setIdCode(newId);
        }
    }

    void printSecSize(){
        cout << "People in secretary: " << myVec.size() << endl;
    }

    static bool isStudent(Person *p){
        return dynamic_cast<Student *> (p) != nullptr;
    }

    static bool isFaculty(Person *p){
        return dynamic_cast<Faculty *> (p) != nullptr;
    }

    friend ostream& operator<<(ostream& os, Secretary& secretary);

    Secretary operator+=(Person& p){
        addPerson(p);
        return *this;
    }

/*
an thelame na exoume vec me students k faculty eswterika tou secretary
    vector<Student *> getStudents(){
        vector<Student *> students = new vector<Student *>;
        return students;
    }
*/

};

//agnoeis

// Secretary operator+=(Secretary& sec1, const Secretary& sec2){
//    Secretary result(sec1);
//     for (auto it = sec2.myVec.begin(); it != sec2.myVec.end(); ++it){
//         result.myVec.push_back(*it);
//     }
//     return result;
// }

// Secretary operator+(const Secretary& secretary,const Person& person){
//     addPerson(person)
// }


ostream& operator<<(ostream& os,Secretary& secretary){
    os << "Secretary " << "(" << secretary.department << ") :" << endl;
    for (auto it = secretary.myVec.begin(); it != secretary.myVec.end(); ++it){
        os << "   " << (*it)->getFirstName() << " " << (*it)->getLastName() << "  ID: " << (*it)->getIdCode() << endl; 
    }
    return os;
}