#include <iostream>
#include <string>
using namespace std;


int Person::pCount = 0;

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
    friend ostream& operator<<(std::ostream& os, const Person& p);
    friend istream& operator>>(std::istream& is, Person& p);
};

class Secretary{
    //map me pointers sta persons
};


int Person::getCount(){
    return pCount;
}

ostream& operator<<(ostream& os, const Person& p) {
    return os << "Name: " << p.firstName << " " << p.lastName << endl << "ID code: " << p.idCode << endl;
}

istream& operator>>(std::istream& is, Person& p){
    return is >> p.firstName >> p.lastName >> p.idCode;
}

int main(){
    Person elpida;
    cin >> elpida;
    cout << elpida;
 
    cout << "Number of persons: " << Person::getCount() << endl;
}