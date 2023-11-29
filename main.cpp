#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

#include "person.hpp"
#include "secretary.hpp"

using namespace std;

// testarw
// testarw k egw <3

// there are three ways to construct a secretary. 1) by calling an empty constructor and 
// then later setting the name and adding the people, 2) by calling a constructor with the name of the department and
// then adding the people and 3) by using the overloaded operator >> as following: cin >> <secretary name> which allows
// the user to enter the department name and initialize it with as many students/faculty members as they want 

int main(){

    // create student, faculty
    Student s1;
    Faculty f1("Ioannis", "Tampouris", "sdi2200181");
    cout << endl;

    //setters
    s1.setFirstName("Elpida");
    s1.setLastName("Stergiou");
    s1.setIdCode("sdi2200181");

    //getters
    cout << "First Name: " << s1.getFirstName() << endl;
    cout << "Last Name: " << s1.getLastName() << endl;
    cout << "ID Code: " << s1.getIdCode() << endl;
    cout << endl;

    //create secretaries
    Secretary sec1("Computer Science");
    Secretary sec2;
    sec2.setSecName("Mathematics");
    cout << endl;

    //adds
    sec1.addPerson(s1);
    sec1.addPerson(f1);
    cout << endl;

    //finds
    Person *foundPerson = sec1.findPersonByFirstName("Elpida");
    foundPerson = sec1.findPersonByLastName("Stergiou");
    foundPerson = sec1.findPersonById("sdi2200152");
    cout << endl;

    //remove
    sec1.removePerson(s1);
    cout << endl;

    //overloads
    Student s2("Nikos", "Damdhmopoulos", "sdi220172");
    Faculty f2("Ioannis", "Famelhs", "sdi2200170");
    cout << endl;

    // + , +=
    sec1 + s2;
    sec2 + f2;
    sec2 += sec1;
    cout << endl;

    // By copy constructor
    Secretary sec3 = sec1; 
    cout << sec3 << endl;

    //By = overload
    sec3 = sec2;
    cout << sec3 << endl;

    //Sec input, output overloads
    cin >> sec1;
    cout << sec1;
    cout << endl;

    // Student input, output overloads
    Student s3;
    cout << "Please input first Name, last name and ID" << endl;
    cin >> s3; 
    cout << s3; 
    cout << endl;

    // Faculty input, output overloads
    Faculty f3;
    cout << "Please input first Name, last name and ID" << endl;
    cin >> f3; 
    cout << f3; 
    cout << endl;

    //Final count
    cout << "Number of Person instances: " << Person::getCount() << endl;
}