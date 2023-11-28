#include <iostream>
#include <string>
#include <vector>

#include "person.h"
#include "secretary.h"

using namespace std;

// testarw
// testarw k egw <3

int main(){
    Student elpida("Elpida", "Stergiou", "sdi2200173");
    Faculty imoani;
    cout << "Total people constructed: " << Person::getCount() << endl;
    Secretary sec("dit");

    sec.addPerson(elpida);
    sec.findPersonById("sdi2200174");
    sec.printSecSize();
    // sec.removePerson(elpida);

}