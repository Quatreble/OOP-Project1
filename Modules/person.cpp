#include "person.hpp"

//////Person class functions:
Person::Person()                           
// : firstName(""), lastName(""), idCode("")
{
    pCount++;
}

Person::Person(string fName, string lName, string id)
: firstName(fName), lastName(lName), idCode(id)
{
    pCount++;
}

Person::~Person() {}

void Person::setFirstName(const string& name){
    firstName = name;
}
void Person::setLastName(const string& name){
    lastName = name;
}
void Person::setIdCode(const string& id){
    idCode = id;
}

string Person::getFirstName() {
    return firstName;
}

string Person::getLastName() {
    return lastName;
}

string Person::getIdCode() {
    return idCode;
}

//sub-classes should override this and add logic specific to their fields
bool Person::equals(Person* p){
    return (firstName == p->firstName && lastName == p->lastName && idCode == p->idCode); 
}

//initialize static member
int Person::pCount = 0;

int Person::getCount(){
    return pCount;
}

//overloaded operators <<, >> for input and output of Person objects
ostream& operator<<(ostream& os, const Person& p) {
    return os << "Name: " << p.firstName << " " << p.lastName << ", " << "ID code: " << p.idCode << endl;
}

istream& operator>>(std::istream& is, Person& p){
    cout << "Enter first Name, last Name and ID code: " << endl;
    return is >> p.firstName >> p.lastName >> p.idCode;
}

//////Student class functions
Student::Student()
: Person()
{
    cout << "Constructed student!" << endl;
}

Student::Student(string fName, string lName, string id)
: Person(fName, lName, id)
{
    cout << "Constructed student!" << endl;
}

//dynamically allocates and returns a copy of Student 
Student* Student::clone(){
    return new Student(*this);
}

//for now we just check equality of the super-class Person
bool Student::equals(Student* s) {
    return Person::equals(s);
}

//////Professor class functions
Professor::Professor()
: Person()
{
    cout << "Constructed Professor!" << endl;
}

Professor::Professor(string fName, string lName, string id)
: Person(fName, lName, id)
{
    cout << "Constructed Professor!" << endl;
}

//dynamically allocates and returns a copy of Professor 
Professor* Professor::clone(){
    return new Professor(*this);
}

//for now we just check equality of the super-class Person
bool Professor::equals(Professor* f) {
    return Professor::equals(f);
}