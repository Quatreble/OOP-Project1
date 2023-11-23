#include <iostream>
#include <string>
using namespace std;

class Person{
public:
    string first_name;
    string last_name;
    int age;
    string id_code;
public:
    Person(string f_name, string l_name)
    : first_name(f_name), last_name(l_name)
    {}

    ~Person(){
        cout << "Destructed!";
    }


};

int main(){
    Person elpida("elpida", "stergiou");
    cout << elpida.first_name << " " << elpida.last_name << endl;
}