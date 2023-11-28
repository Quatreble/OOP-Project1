#pragma once
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
        // cout << "Destructed " << firstName << " " << lastName << '\n';
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
