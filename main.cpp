#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

#include "person.h"
#include "secretary.h"

using namespace std;

// testarw
// testarw k egw <3

int main(){
    Student elpida("Elpida", "Stergiou", "sdi2200173");
    Faculty imoani("Imoani", "Tampouriri", "sdi2200181");
    Student gkiannhs("gkiannhs", "xortarakhs", "sdi2200162");
    cout << "Total people constructed: " << Person::getCount() << endl;
    Secretary sec("dit");
    Secretary seccy("pleb");


    seccy.addPerson(gkiannhs);
    sec.addPerson(elpida);
    sec.addPerson(imoani);
    // sec.findPersonById("sdi2200174");
    // sec.findPerson(imoani);
    // sec.printSecSize();
    //sec.removePerson(elpida);
    // sec.editFirstName(imoani, "ioanni");
    // sec.findPersonByFirstName("Imoani");
    // sec.findPersonByFirstName("ioanni");
    cout << sec;
    sec += gkiannhs;
    cout << sec;

}