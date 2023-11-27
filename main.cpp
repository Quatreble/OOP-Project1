#include <iostream>
#include <string>
#include <map>
using namespace std;

class Person{
private:
    static int pCount;
    string firstName;
    string lastName;
    int age;
    string idCode;
public:
    Person()
    : firstName(""), lastName(""), idCode("")
    {
        cout << "Constructed!" << endl;
        pCount++;
    }

    Person(string fName, string lName, string id)
    : firstName(fName), lastName(lName), idCode(id)
    {
        pCount++;
    }

    Person(const Person& p) //pros to paron den auksanei to pcount
    : firstName(p.firstName), lastName(p.lastName), age(p.age), idCode(p.idCode)
    {}

    ~Person(){
        cout << "Destructed " << firstName << " " << lastName << "!" << '\n';
        // pCount--;
    }

    static int getCount();
    string getFirstName();
    string getLastName();
    string getIdCode();
    int getAge();

    void setAge(int age);

    friend ostream& operator<<(std::ostream& os, const Person& p);
    friend istream& operator>>(std::istream& is, Person& p);
};

class Secretary{
private:
    map<string, Person*> myMap;
public:
    Secretary(){
        cout<<"Secretary constructed!"<<endl;
        cout<<"People in secretary: " << myMap.size()<<endl;
    }

    ~Secretary(){
        for(auto it = myMap.begin(); it != myMap.end(); ++it){
            cout << "deleted " << it->first << endl;
            delete it->second;

        }
    }

    void addPerson(const Person& p){
        Person* newP = new Person(p);
        myMap.insert(make_pair(newP->getIdCode(), newP));
    }

    bool findPerson(const string& id){
        auto it = myMap.find(id);
        if(it != myMap.end()){
            cout << "Person found!" << endl;
            return true;
        }
        cout << "Person not found:(" << endl;
        return false;
    }

    void removePerson(string id){
        auto it = myMap.find(id);
        delete it->second;
        myMap.erase(id);
        cout << "removed person vohtheia" << endl;
    }

    void printSecSize(){
        cout << "People in secretary: " << myMap.size() << endl;
    }
};

//testarw
//testarw k egw <3

int main(){
    Person elpida("Elpida", "Stergiou", "sdi2200173");
    Secretary sec;
    sec.addPerson(elpida);
    sec.findPerson("sdi2200173");
    sec.printSecSize();
    sec.removePerson(elpida.getIdCode());
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

int Person::getAge(){
    return age;
}

void Person::setAge(int age){
    this->age = age;
}

ostream& operator<<(ostream& os, const Person& p) {
    return os << "Name: " << p.firstName << " " << p.lastName << endl << "ID code: " << p.idCode << endl;
}

istream& operator>>(std::istream& is, Person& p){
    return is >> p.firstName >> p.lastName >> p.idCode;
}

