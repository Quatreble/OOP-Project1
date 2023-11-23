#include <iostream>
#include <string>
using namespace std;

class Secretary{
//contains map with pointers to Person instances
};

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

    ~Person(){
        cout << "Destructed " << firstName << " " << lastName << "!" << '\n';
        pCount--;
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

int main(){
    Person student;
    cin >> student;
    cout << student;
    cout << "Number of persons: " << Person::getCount() << endl;
}