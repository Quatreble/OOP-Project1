#pragma once
using namespace std;

class Secretary {
private:    
    string department;
    vector<Person *> myVec;  // we choose to use a vector instead of a map since we would like to be able 
                             // to search with all Person's properties using linear iteration. Since all takes place
public:                      // in memory there is no significant performance hit to go through every Person  ( O(n) )
    Secretary(const string& dep)
    : department(dep)
    {
        cout<<"Secretary " << department << " constructed!" <<endl;
       // cout<<"People in secretary: " << myVec.size()<<endl;
    }

    Secretary(){
        cout << "Secretary constructed!" << endl;
    }

    ~Secretary(){
        for(Person* it : myVec){
            //cout << "Deleted " << (*it)->getFirstName() << endl;
            delete it;
        }
        myVec.clear();
        cout << "Deleted secretary " << department << endl;
    }

    Secretary(const Secretary& sec)
    : department(sec.department)
    {
        for (auto it = sec.myVec.begin(); it != sec.myVec.end(); ++it) {
            addPerson(**it, false);
        }
    }

    void addPerson(Person& p, bool printStatement = true){
        Person *newP = p.clone();  // here we use the virtual function clone of the base class person instead of simply creating a 'new' person, 
        //if (isStudent(&p)) {           // because it automatically creates either a Student or a Faculty accordingly.
        //    newP = new Student(p);     // The lines which are commented out are a different implementation of this, using the
        //}                              // functions isStudent and isFaculty in order to distinguish between the types of person.
        //if(isFaculty(&p)){
        //    newP = new Faculty(p);
        //}
        myVec.push_back(newP);
        if (printStatement) cout << "Added " << newP->getFirstName() << " to " << department << "!" << endl;
    }


    Person* findPersonByFirstName(const string& name){
        for(Person* i : myVec){
            if(i->getFirstName() == name){
                cout << "Person Found" << endl;
                return i;
            }
        }
        cout << "Person Not Found" << endl;
        return nullptr;
    }

    Person* findPersonByLastName(const string& name) {
        for(Person* i : myVec){
            if(i->getLastName() == name){
                cout << "Person Found" << endl;
                return i;
            }
        }
        cout << "Person Not Found" << endl;
        return nullptr;
    }

    Person* findPersonById(const string& id){
        for(Person* i : myVec){
            if(i->getIdCode() == id){
                cout << "Person Found" << endl;
                return i;
            }
        }
        cout << "Person Not Found" << endl;
        return nullptr;
    }

    Person* findPerson(Person& p){
        for (Person* i : myVec){
            if(p.equals(i)){
                cout << "Person found" << endl;
                return i;
            }
        }
        cout << "Person Not Found" << endl;
        return nullptr;
    }

    bool removePerson(Person& p){
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

    void printSecSize(){
        cout << "People in secretary: " << myVec.size() << endl;
    }

    void setSecName(const string& dep){
        department = dep;
    }

    const string& getSecName(){
        return department;
    }

    static bool isStudent(Person *p){
        return dynamic_cast<Student *> (p) != nullptr;
    }

    static bool isFaculty(Person *p){
        return dynamic_cast<Faculty *> (p) != nullptr;
    }

    Secretary& operator+(Person& p){
        addPerson(p);
        return *this;
    }

    Secretary& operator+=(const Secretary& sec){
        for (Person* it : myVec){
           addPerson(*it);
        }
        return *this;
    }

    Secretary& operator=(const Secretary& sec){
        if (this != &sec) {
            department = sec.department;
            for (Person* it : myVec) {
                delete it;
            }
            myVec.clear();
            for (Person* it : myVec) {
                addPerson(*it,false);
            }
        }
        return *this;
    }

    friend ostream& operator<<(ostream& os, Secretary& secretary);
    friend istream& operator>>(istream& is, Secretary& secretary);

};

ostream& operator<<(ostream& os,Secretary& secretary){
    os << "Secretary " << secretary.department << ":" << endl;
    for (Person* it : secretary.myVec){
        os << (*it); 
    }
    os << endl;
    return os;
}

istream& operator>>(istream& is, Secretary& sec){
    char type;
    cout << "Enter Department name: ";
    is >> sec.department;
    cout << "Enter s for student, f for faculty, 0 to stop adding people:" << endl;
    is >> type;
    while(type !='0'){
        if(type == 's'){
            Student s;
            //cout << "Enter Name, Surname and ID Code: " << endl;
            is >> s;
            sec.addPerson(s);
        }
        if(type == 'f'){
            Faculty f;
            //cout << "Enter Name, Surname and ID Code: " << endl;
            is >> f;
            sec.addPerson(f);
        }
        cout << "Enter s for student, f for faculty, 0 to stop adding people:" << endl;
        is >> type;
    }
    return is;
}
