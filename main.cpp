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
    // Student elpida("Elpida", "Stergiou", "sdi2200173");
    // Faculty imoani("Imoani", "Tampouriri", "sdi2200181");
    // Student gkiannhs("gkiannhs", "xortarakhs", "sdi2200162");
    // Faculty loko("xatzhko", "kolakhs", "sdi8800175");
    // cout << "Total people constructed: " << Person::getCount() << endl;
    // Secretary sec("dit");
    // Secretary seccy("pleb");


    // seccy.addPerson(gkiannhs);
    // sec.addPerson(elpida);
    // sec.addPerson(imoani);


    // //(sec.findPersonById("sdi2200173"))->setFirstName("sdf");
    // // sec.findPersonById("sdi2200174");
    // // sec.findPerson(imoani);
    // // sec.printSecSize();
    // //sec.removePerson(elpida);
    // // sec.editFirstName(imoani, "ioanni");
    // // sec.findPersonByFirstName("Imoani");
    // // sec.findPersonByFirstName("ioanni");
    // cout << sec;
    // sec += seccy;
    // cout << sec;
    // sec = sec + loko;
    // cout << sec;
    // sec = seccy;
    // cout << sec;

    Secretary sec;
    cin >> sec;
   // cout << stud1;
    
}