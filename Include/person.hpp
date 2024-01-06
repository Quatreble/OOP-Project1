#pragma once

#include <iostream>
#include <string>

using namespace std;

class Person {
protected:
    static int pCount; //counter for every Person object that is instantiated 
    string firstName;
    string lastName;  //properties of a Person object
    string idCode;
public:
    Person();
    Person(string fName, string lName, string id);
    Person(const Person& p) 
    : firstName(p.firstName), lastName(p.lastName), idCode(p.idCode)
    {}

    virtual ~Person();

    static int getCount();

    void setFirstName(const string& name);
    void setLastName(const string& name);
    void setIdCode(const string& id);

    string getFirstName();
    string getLastName();
    string getIdCode();

    //sub-classes should override this and add logic specific to their fields
    virtual bool equals(Person* p);

    //sub-classes should implement that so we can dynamically create copies
    virtual Person* clone() = 0;

    friend ostream& operator<<(std::ostream& os, const Person& p); 
    friend istream& operator>>(std::istream& is, Person& p);
};

class Student : public Person {
public:
    Student();
    Student(string fName, string lName, string id);
    Student(const Person& p)
    : Person(p)
    {}

    virtual Student* clone() override;

    //for now we just check equality of the super-class Person
    virtual bool equals(Student* s);
};

class Professor : public Person {
public:
    Professor();
    Professor(string fName, string lName, string id);
    Professor(const Person& p)
    : Person(p)
    {}

    virtual Professor* clone() override;

    //for now we just check equality of the super-class Person
    virtual bool equals(Professor* f);
};