#include "person.hpp"
#include "secretary.hpp"
#include "course.hpp"

using namespace std;

// there are three ways to create a Secretary object: 1) by calling an empty constructor and 
// then later setting the name and adding people, 2) by calling a constructor and giving the name of the department and
// then later adding people and 3) by using the overloaded operator >> as following: cin >> <secretary name> which allows
// the user to enter the department name and initialize it with as many students/Professor members as desired.

// **NOTE THAT when using the overloaded operator >> to add Person objects to a Secretary object, if it was not previously empty
// this function will simply add the new Person objects to the Secretary without deleting the already existing ones

// Since base class Person is an abstract class, it can not be instantiated. Only child classes Student and Professor can  

int main(){
  
    // // create a student and a Professor member
    // Student s1;
    // Professor f1("Ioannis", "Tampouris", "sdi2200181");
    // cout << endl;

    // // set properties
    // s1.setFirstName("Elpida");
    // s1.setLastName("Stergiou");
    // s1.setIdCode("sdi2200173");

    // // get properties and print, print using overloaded operator <<
    // cout << "First Name: " << s1.getFirstName() << endl;
    // cout << "Last Name: " << s1.getLastName() << endl;
    // cout << "ID Code: " << s1.getIdCode() << endl;
    // cout << f1;
    // cout << endl;

    // //create two secretaries
    // Secretary sec1("Computer Science");
    // Secretary sec2;
    // sec2.setSecName("Mathematics");
    // cout << endl;

    // //add people to secretary using overloaded operator +
    // sec1 + s1;
    // sec1 + f1;
    // cout << endl;

    // //try to find a person in secretary by given id code, by person&
    // sec1.findPersonById("sdi2200152");
    // sec1.findPerson(s1);
    // cout << endl;

    // //remove person from secretary
    // sec1.removePerson(s1);
    // cout << endl;


    // Student s2("Nikos", "Damdhmopoulos", "sdi220172");
    // Professor f2("Giannis", "Famelhs", "sdi2200170");
    // cout << endl;

    // // overloaded operators + and += for secretary
    // sec1 + s2;
    // sec2 + f2;
    // cout << endl;
    // sec2 += sec1;
    // cout << endl;

    // // copy constructor, = and << operator overload for Secretary class
    // Secretary sec3 = sec1; 
    // cout << "(sec3) " << sec3 << endl;

    // // >> operator overload, create Secretary with user-input properties then print it
    // cin >> sec1;                 // **NOTE THAT when using the overloaded operator >> to add Person objects to a Secretary object, if it was not previously empty
    // cout <<"(sec1) " << sec1;                // this function will simply add the new Person objects to the Secretary without deleting the already existing ones
    // cout << endl;                // (this is what happens in this case, where in sec1 there already where 2 Person objects)

    // // number of total Person objects allocated on stack(created outside of a Secretary object)
    // cout << "Number of Person instances: " << Person::getCount() << endl;
    // cout << endl;


    //Secretary sec1("Computer Science");
    // Course firstCourse("math",5,true);
    // Student s2("Nikos", "Damdhmopoulos", "sdi220172");
    // sec1 + s2;
    // sec1.addCourse(firstCourse,2);

    Secretary mySec("Department of Informatics and Telecomunications");

}


// testarw
// testarw k egw <3