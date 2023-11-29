#pragma once

using namespace std;

class Person {
protected:
    static int pCount;
    string firstName;
    string lastName;
    string idCode;
public:
    Person()                           
    // : firstName(""), lastName(""), idCode("")
    {
        pCount++;
    }

    Person(string fName, string lName, string id)
    : firstName(fName), lastName(lName), idCode(id)
    {
        pCount++;
    }

    Person(const Person& p) 
    : firstName(p.firstName), lastName(p.lastName), idCode(p.idCode)
    {}

    virtual ~Person()
    {}

    static int getCount();

    void setFirstName(const string& name){
        firstName = name;
    }
    void setLastName(const string& name){
        lastName = name;
    }
    void setIdCode(const string& id){
        idCode = id;
    }

    string getFirstName() {
        return firstName;
    }

    string getLastName() {
        return lastName;
    }

    string getIdCode() {
        return idCode;
    }

    //sub-classes should override this and add logic specific to their fields
    virtual bool equals(Person* p){
        return (firstName == p->firstName && lastName == p->lastName && idCode == p->idCode); 
    }

    //sub-classes should implement that so we can dynamically create copies
    virtual Person* clone() = 0;

    friend ostream& operator<<(std::ostream& os, const Person& p);
    friend istream& operator>>(std::istream& is, Person& p);
};

class Student : public Person {
public:
    Student()
    : Person()
    {
        cout << "Constructed student!" << endl;
    }

    Student(string fName, string lName, string id)
    : Person(fName, lName, id)
    {
        cout << "Constructed student!" << endl;
    }

    Student(const Person& p)
    : Person(p)
    {}

    virtual Student* clone() override{
        return new Student(*this);
    }

    //for now we just check equality of the super-class Person
    virtual bool equals(Student* s) {
        return Person::equals(s);
    }
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

    virtual Faculty* clone() override{
        return new Faculty(*this);
    }

    //for now we just check equality of the super-class Person
    virtual bool equals(Faculty* f) {
        return Person::equals(f);
    }
};

//initialize static member
int Person::pCount = 0;

int Person::getCount(){
    return pCount;
}

ostream& operator<<(ostream& os, const Person& p) {
    return os << "Name: " << p.firstName << " " << p.lastName << ", " << "ID code: " << p.idCode << endl;
}

istream& operator>>(std::istream& is, Person& p){
    cout << "Enter first Name, last Name and ID code: " << endl;
    return is >> p.firstName >> p.lastName >> p.idCode;
}