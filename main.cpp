#include <iostream>
#include <string>
#include <map>
#include <vector>
using namespace std;

class Person{
protected:
    static int pCount;

    string firstName;
    string lastName;
    string idCode;
public:
    Person()
    : firstName(""), lastName(""), idCode("")
    {
        pCount++;
    }

    Person(string fName, string lName, string id)
    : firstName(fName), lastName(lName), idCode(id)
    {
        pCount++;
    }

    Person(const Person& p) //pros to paron den auksanei to pcount
    : firstName(p.firstName), lastName(p.lastName), idCode(p.idCode)
    {}

    virtual ~Person(){
     //   cout << "Destructed " << firstName << " " << lastName << '\n';
        // pCount--;
    }

    static int getCount();
    string getFirstName();
    string getLastName();
    string getIdCode();

    friend ostream& operator<<(std::ostream& os, const Person& p);
    friend istream& operator>>(std::istream& is, Person& p);
};

class Student : public Person {
public:
    Student()
    : Person()
    {
        cout << "Constructed faculty!" << endl;
    }

    Student(string fName, string lName, string id)
    : Person(fName, lName, id)
    {
        cout << "Constructed student!" << endl;
    }

    Student(const Person& p)
    : Person(p)
    {}

};

class Faculty : public Person {
public:
    Faculty()
    : Person()
    {
        cout << "Constructed faculty!" << endl;
    }

    Faculty(string fName, string lName, string id)
    : Person(fName, lName, id)
    {
        cout << "Constructed faculty!" << endl;
    }

    Faculty(const Person& p)
    : Person(p)
    {}
};

class Secretary {
private:    
    string department;
    // we choose to use a vector instead of a map since we would like to be able 
    // to search with all Person's properties using linear iteration. Since all takes place
    // in memory there is no significant performance hit to go through every Person  ( O(n) )
    //ennoeitai egw to grapsa auto^
    vector<Person *> myVec;
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

    // bool findPerson(const string& id){
    //     auto it = myMap.find(id);
    //     if(it != myMap.end()){
    //         Person *p = it->second;

    //         cout << "Person found!" << endl;
    //         return true;
    //     }
    //     cout << "Person not found:(" << endl;
    //     return false;
    // }

    void removePerson(const Person &p){;
        //check that it is not null?
        //prwta erase apto vec k meta delete to pointer
        
        //AKOU EXW KAEI MAUTO GIA TWRA PAW NANI
        
        // myVec.erase();
        // delete *it;
        // cout << "Removed person vohtheia" << endl;
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

/*
an thelame na exoume vec me students k faculty eswterika tou secretary
    vector<Student *> getStudents(){
        vector<Student *> students = new vector<Student *>;
        return students;
    }
*/

};

// testarw
// testarw k egw <3

int main(){
    Student elpida("Elpida", "Stergiou", "sdi2200173");
    Faculty imoani;
    cout << "Total people constructed: " << Person::getCount() << endl;
    Secretary sec("dit");

    sec.addPerson(elpida);
    sec.findPersonById("sdi2200174");
    sec.printSecSize();
    // sec.removePerson(elpida);

}

int Person::pCount = 0;

int Person::getCount(){
    return pCount;
}

string Person::getFirstName(){
    return firstName;
}

string Person::getLastName(){
    return lastName;
}

string Person::getIdCode(){
    return idCode;
}

ostream& operator<<(ostream& os, const Person& p) {
    return os << "Name: " << p.firstName << " " << p.lastName << endl << "ID code: " << p.idCode << endl;
}

istream& operator>>(std::istream& is, Person& p){
    return is >> p.firstName >> p.lastName >> p.idCode;
}

