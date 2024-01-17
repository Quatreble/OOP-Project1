#include "secretary.hpp"

//////Secretary class functions
Secretary::Secretary(const string& dep, int sem, int reqPoints)
: depName(dep), depSemesters(sem), pointsToGraduate(reqPoints)
{
    readStudentsFromFile();
    readProfessorsFromFile();
    readCourseFromFile();
    SecretaryOperation();
    //cout<<"Secretary " << depName << " constructed!" <<endl;
}

Secretary::Secretary(){
    readStudentsFromFile();
    readProfessorsFromFile();
    SecretaryOperation();
    //cout << "Secretary constructed!" << endl;

}

Secretary::~Secretary(){
    // for(Person* it : depMembers){
    //     delete it;
    // }
    //cout << "Deleted secretary " << depName << endl;
}

Secretary::Secretary(const Secretary& sec) //copy constructor for deep copy 
: depName(sec.depName), depSemesters(sec.depSemesters), pointsToGraduate(sec.pointsToGraduate), numOfMandatory(sec.numOfMandatory)
{
    for (auto& element : depStudents) {
        addPerson(*element.second, false);
    }
    for (auto& element : depProfessors) {
        addPerson(*element.second, false);
    }
    for (auto& element : depCourses) {
        addCourse(*element.second);
    }
}

void Secretary::printMenu(){
    cout << "\tECLASS\n";
    cout << "1. PROFESSOR OPTIONS\n";
    cout << "2. STUDENT OPTIONS\n";
    cout << "3. COURSE OPTIONS\n";
    cout << "4. REGISTER STUDENT TO COURSE\n";
    cout << "5. SET PROFESSOR TO COURSE\n";
    cout << "6. GRADE STUDENTS\n";
    cout << "7. PRINT STUDENTS WHO PASSED A COURSE\n";
    cout << "8. PRINT PROFESSOR COURSES STATS\n";
    cout << "9. GET GRADES\n";
    cout << "TYPE 0 TO EXIT\n";
}

void Secretary::SecretaryOperation(){
    int op;
    while (true){
        this_thread::sleep_for(chrono::seconds(1));
        printMenu();
        cin >> op;
        if (op == 0){
            cout << "\tGOODBYE\n";
            return;
        }
        else if (op < 0 || op > 12){
            cout << "WRONG INPUT\n";
            cout << "INPUT AGAIN\n";
            continue;
        }
        else if (op == 1){
            cout << "1. ADD PROFESSOR\n";
            cout << "2. MODIFY PROFESSOR\n";
            cout << "3. REMOVE PROFESSOR\n";
            cin >> op;
            if (op == 1){
                addProfessor();
            }
            else if (op == 2){
                modifyProfessor();
            }
            else if (op == 3){
                deleteProfessor();
            }
        }
        else if (op == 2){
            cout << "1. ADD STUDENT\n";
            cout << "2. MODIFY STUDENT\n";
            cout << "3. REMOVE STUDENT\n";
            cin >> op;
            if (op == 1){
                addStudent();
            }
            else if (op == 2){
                modifyStudent();
            }
            else if (op == 3){
                deleteStudent();
            }
        }
        else if (op == 3){
            cout << "1. ADD COURSE\n";
            cout << "2. MODIFY COURSE\n";
            cout << "3. REMOVE COURSE\n";
            cin >> op;
            if (op == 1){
                Course course;
                cin >> course;
                addCourse(course);
            }
            else if (op == 2){
                modifyCourse();
            }
            else if (op == 3){
                deleteCourse();
            }

        }
        else if(op == 4){
            registerStudentToCourse();
        }
        else if (op == 5){
            setCourseProf();
        }
        else if (op == 6){
            gradeStudents();
        }
        else if (op == 7){
            printStudentsWhoPassed();
        }
        else if (op == 8){
            printProfStats();
        }
        else if (op == 9){
            getGrades();
        }
    }
    
}

void Secretary::addPerson(Student& s, bool printStatement, bool manualAdd){
        Student* stud = s.clone();
        auto check = depStudents.emplace(stud->getIdCode(),stud);
        if (check.second && manualAdd){
            printStudentToFile(*stud);
        }
        else if (!check.second && manualAdd){
            cout << "ID already exists\n";
            printStatement = false;
        }
        if (printStatement) cout << "Added " << stud->getFirstName() << " to " << depName << "!" << endl;
}

void Secretary::addPerson(Professor& p, bool printStatement, bool manualAdd){
        Professor* prof = p.clone();
        auto check = depProfessors.emplace(prof->getIdCode(),prof);
        if (check.second && manualAdd){
            printProfessorToFile(*prof);
        }
        else if (!check.second && manualAdd){
            cout << "ID already exists\n";
            printStatement = false;
        }
        if (printStatement) cout << "Added " << prof->getFirstName() << " to " << depName << "!" << endl;
}

void Secretary::addProfessor(){
    Professor f;
    //cout << "Enter Name, Surname and ID Code: " << endl;
    cin >> f;
    addPerson(f,true);
}

void Secretary::addStudent(){
    Student s;
    //cout << "Enter Name, Surname and ID Code: " << endl;
    cin >> s;
    addPerson(s,true);
}

void Secretary::addCourse(Course& c,bool manualAdd){
    Course* courseptr = new Course(c);
    auto check = depCourses.emplace(courseptr->getCode(),courseptr);
    if (check.second && manualAdd){
        printCourseToFile(*courseptr);
    }
    else if (!check.second && manualAdd){
        cout << "Code already exists\n";
    }
    if(courseptr->getMand()){
        ++numOfMandatory;
    }
}

void Secretary::modifyProfessor(){
    cout << "INPUT PROFESSOR ID: ";
    string id;
    cin >> id;
    Professor* prof = findProfessor(id);
    if (prof!=nullptr) {  //Check if person is professor
        string prevName = prof->getFirstName() + " " + prof->getLastName();
        string prevId = prof->getIdCode();
        auto it = depProfessors.find(prof->getIdCode());
        depProfessors.erase(it);
        cout << "Please enter the new attributes of the professor\n";
        cin >> *prof;
        depProfessors[prof->getIdCode()] = prof;
        jsonModifyProf(*prof,prevId);
        cout << "Professor " << prevName << " changed to " << prof->getFirstName() << " " << prof->getLastName() << '\n';

    } else{ //if person is found but isnt a professor
        cout << "The person with ID " << id << " is not a Professor.\n";
    }
}

void Secretary::modifyStudent(){
    cout << "INPUT STUDENT ID: ";
    string id;
    cin >> id;
    Student* stud = findStudent(id);
    if (stud!=nullptr) {  //Check if person is student
        string prevName = stud->getFirstName() + " " + stud->getLastName();
        string prevId = stud->getIdCode();
        auto it = depStudents.find(stud->getIdCode());
        depStudents.erase(it);
        cout << "Please enter the new attributes of the student\n";
        cin >> *stud;
        depStudents[stud->getIdCode()] = stud;
        jsonModifyStud(*stud,prevId);
        cout << "Student " << prevName << " changed to " << stud->getFirstName() << " " << stud->getLastName() << '\n';

    } else{ //if person is found but isnt a professor
        cout << "The person with ID " << id << " is not a Student.\n";
    }
}

void Secretary::modifyCourse(){
    Course* course = readAndValidateCourse();
    if (course == nullptr) return;
    cout << "1. MODIFY ALL COURSES ATTRIBUTES\n";
    cout << "2. CHANGE COURSE YEAR AND SEMESTER\n";
    int op;
    cin >> op;
    string prevCode = course->getCode();
    if (op == 1){
        auto it = depCourses.find(course->getCode());
        depCourses.erase(it);
        cout << "Please enter new attributes\n";
        cin >> *course;
        depCourses[course->getCode()] = course;
    }
    else if (op == 2){
        cout << "Enter course year\n";
        int year;
        cin >> year;
        course->setYear(year);
        string season;
        cout << "Enter 'w' for winter semester and 's' for summer semester: ";
        while (true) {
            cin >> season;
            if (season == "w" || season == "W" || season == "s" || season == "S") {
                break;
            } else {
                cout << "Invalid input. Please enter 'w' for winter or 's' for summer: ";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
        }
        course->setSemester(season);
    }
    cout << "Course was changed\n";
    jsonModifyCourse(*course,prevCode);
}

//iterates vector until person with same properties is found(Person::equals function is used for this), delete Person, remove it from vector 
bool Secretary::removePerson(Student& s){
    auto it = depStudents.find(s.getIdCode());
    if(it != depStudents.end()){
        cout << "Removed " << it->first << " from " << depName << endl;
        delete it->second;
        depStudents.erase(it);       
        return true;
    }
    return false;
}

bool Secretary::removePerson(Professor& p){
    auto it = depProfessors.find(p.getIdCode());
    if(it != depProfessors.end()){
        cout << "Removed " << it->first << " from " << depName << endl;
        delete it->second;
        depProfessors.erase(it);       
        return true;
    }
    return false;
}

void Secretary::removeCourse(Course& course){
    auto it = depCourses.find(course.getCode());
    if(it != depCourses.end()){
        cout << "Erased " << course.getName() << '\n';
        delete it->second;
        depCourses.erase(it);
        return;
    }
    
}

void Secretary::deleteProfessor(){
    cout << "INPUT PROFESSOR ID: ";
    string id;
    cin >> id;
    Professor* prof = findProfessor(id);
    if (prof != nullptr) {  //same as above
        jsonRemoveProfessor(*prof);
        removePerson(*prof);
    } else{
        cout << "The person with ID " << id << " is not a Professor.\n";
    }
}

void Secretary::deleteStudent(){
    cout << "INPUT STUDENT ID: ";
    string id;
    cin >> id;
    Student* stud = findStudent(id);
    if (stud != nullptr) {  //same as above
        jsonRemoveStudent(*stud);
        removePerson(*stud);
    } else{
        cout << "The person with ID " << id << " is not a student.\n";
    }
}


void Secretary::deleteCourse(){
    string name;
    cout << "Enter Course code: ";
    cin >> name;     
    Course* course = findCourse(name);
    if (course != nullptr){
        jsonRemoveCourse(*course);
        removeCourse(*course);
    } 
    else
        cout << "Course not found!\n";
}

Student* Secretary::findStudent(const string& id){
    auto itStud = depStudents.find(id);
    if (itStud != depStudents.end()){
        cout << "Student found" << endl;
        return itStud->second;
    }
    //cout << "Student Not Found" << endl;
    return nullptr;
}

Professor* Secretary::findProfessor(const string& id){
    auto itProf = depProfessors.find(id);
    if (itProf != depProfessors.end()){
        cout << "Professor found" << endl;
        return itProf->second;
    }
   // cout << "Professor Not Found" << endl;
    return nullptr;
}


Course* Secretary::findCourse(string code){
    for (auto& element : depCourses){
        if (element.second->getCode() == code){
            cout << "Found course \n";
            return element.second;
        }
    }
    return nullptr;
}

void Secretary::printSecSize(){
    cout << "People in secretary: " << depStudents.size() + depProfessors.size() << endl;
}

void Secretary::setSecName(const string& dep){
    depName = dep;
}

const string& Secretary::getSecName(){
    return depName;
}


//uses a dynamic cast to Student pointer to check if Person* is a Student*
Student* Secretary::isStudent(Person *p){
    return dynamic_cast<Student *> (p);
}

//as above but for Professor
Professor* Secretary::isProfessor(Person *p){
    return dynamic_cast<Professor *> (p);
}

//Overloaded operator + to add a Person to a Secretary 
Secretary& Secretary::operator+(Student& s){
    addPerson(s);
    return *this;
}

Secretary& Secretary::operator+(Professor& p){
    addPerson(p);
    return *this;
}

//Overloaded operator += to add members of a secretary to the end of another// merges the two secretaries 
Secretary& Secretary::operator+=(const Secretary& sec){
    for (const auto& element : sec.depStudents){
        addPerson(*element.second);
    }
    for (const auto& element : sec.depProfessors){
        addPerson(*element.second);
    }
    return *this;
}

//the member of a secretary are added to another, after it is cleared of its previous members
Secretary& Secretary::operator=(const Secretary& sec){
    if (this != &sec) {
        depName = sec.depName;
        depSemesters = sec.depSemesters;
        pointsToGraduate = sec.pointsToGraduate;
        numOfMandatory = sec.numOfMandatory;
        jCourses = sec.jCourses;
        jProfessors = sec.jProfessors;
        jStudents = sec.jStudents;

        for (auto& element : depStudents) {
            delete element.second;
        }
        depStudents.clear();
        for (auto& element : depProfessors) {
            delete element.second;
        }
        depProfessors.clear();
        for (auto& element : sec.depStudents) {
            addPerson(*element.second,false);
        }
        for (auto& element : sec.depProfessors) {
            addPerson(*element.second,false);
        }
        
    }
    return *this;
}

//overloaded operator << for output of a Secretary object
ostream& operator<<(ostream& os,Secretary& secretary){
    os << "Secretary " << secretary.depName << ":" << endl;
    for (const auto& element : secretary.depStudents){
        os << "Student:" << (*element.second); 
    }
    for (const auto& element : secretary.depProfessors){
        os << "Professor:" << (*element.second); 
    }
    os << endl;
    return os;
}

//overloaded operator >> that allows the user to change/input the name of the Secretary and add as many People to it as they want
istream& operator>>(istream& is, Secretary& sec){
    char type;
    cout << "Enter Department name: ";
    is >> sec.depName;
    cout << "Enter s for student, f for Professor, 0 to stop adding people:" << endl;
    is >> type;
    while(type !='0'){
        if(type == 's'){
            Student s;
            //cout << "Enter Name, Surname and ID Code: " << endl;
            is >> s;
            sec.addPerson(s);
        }
        if(type == 'f'){
            Professor f;
            cout << "Enter Name, Surname and ID Code: " << endl;
            is >> f;
            sec.addPerson(f);
        }
        cout << "Enter s for student, f for Professor, 0 to stop adding people:" << endl;
        is >> type;
    }
    return is;
}

void Secretary::createSemester(){
    Semester sem;
    cin >> sem;
    sem.printSem();
    addSemester(sem);
}

Semester* Secretary::addSemester(Semester& toAdd){
    Semester* semptr = new Semester(toAdd);
    semesters.push_back(semptr);
    printSemesterToFile(toAdd);
    return semptr;
}

void Secretary::setCourseProf(){
    Semester* sem = readAndValidateSemester();
    Course* course = readAndValidateCourse();
    if(course != nullptr){
        Professor* prof = readAndValidateProfessor();
        if(prof==nullptr) return;
        sem->addProfToCourse(course, prof);
    }
}

void Secretary::registerStudentToCourse(){
    Student* stud = readAndValidateStudent();
    if (stud == nullptr) return;
    Semester* sem = readAndValidateSemester();
    Course* course = readAndValidateCourse();
    if (course == nullptr) return;
    if(sem->getSeason() != course->getSeason() || CURR_SEM.first < sem->getYear() || sem->getYear() - stud->getReg() < course->getYear() || sem->isRegistered(course, stud) != nullptr){
        cout << "STUDENT CAN'T REGISTER TO THIS COURSE\n";
        return;
    }
    if (stud->getCourseGrade(course) >= 5){
        cout << "Student has already passed this course\n";
        return;
    }
    sem->addStudToCourse(course, stud);
}

void Secretary::gradeStudents(){
    StudentCourseInstance* sci = nullptr;
    Semester* sem  = nullptr;
    Student* stud = readAndValidateStudent();
    if (stud == nullptr) return;
    Course* course = readAndValidateCourse();
    if (course == nullptr) return;
    for (Semester* sem : semesters){
        StudentCourseInstance* sciTemp = sem->isRegistered(course, stud);
        if (sciTemp != nullptr && sciTemp->grade == -1){
            sci = sciTemp;
        }
    }
    if (sci != nullptr){
        sem->gradeStud(sci);
        stud->addCourseWithGrade(course, sci->grade);
        return;
    }
    if (sem == nullptr){
        cout << "Student not registered to that course or is already graded\n";
        return;
    }

    
}

void Secretary::printStudentsWhoPassed(){
    Semester* sem = readAndValidateSemester();
    Course* course = readAndValidateCourse();
    if (course == nullptr) return;
    sem->printPassed(course);
}

void Secretary::printProfStats(){
    Professor* prof = readAndValidateProfessor();
    if (prof == nullptr) return;
    Semester* sem = readAndValidateSemester();
    sem->printProfStats(prof);
}

void Secretary::getGrades(){
    Student* stud = readAndValidateStudent();
    cout << "1. GET GRADES FOR CURRENT SEMESTER\n";
    cout << "2. GET GRADES HISTORY\n";
    int op;
    cin >> op;
    if (op == 1){
        Semester* sem = getCurrSem();
        sem->printStudStats(stud);
    }
    if (op == 2){
        stud->printGrades();
    }
}

Semester* Secretary::getCurrSem(){
    int year = CURR_SEM.first;
    char sem = CURR_SEM.second;
    bool isWinter = false;
    if(sem == 'W' || sem == 'w'){
        isWinter = true;
    }
    for(auto& semester: semesters){
        if(semester->getYear() == year && semester->getSeason() == isWinter){
            return semester;
        }
    }
    return nullptr;
}

void Secretary::printGraduates(){
    for (auto& element : depStudents){
        Student* stud = element.second;
        if(stud->getSemesterCount() >= depSemesters && stud->getMandatoryPassed()==getNumOfMandatory() && stud->getAcademicPoints() >= pointsToGraduate){
            cout << *stud;
        }
    }
}

Student* Secretary::readAndValidateStudent(){
    cout << "Enter Student id: ";
    string id;
    cin >> id;
    Student* stud= findStudent(id);
    if (stud != nullptr){
        return stud;
    }
    else{
        cout << "The person with ID " << id << " is not a Student.\n";
        return nullptr;
    }
}

Professor* Secretary::readAndValidateProfessor(){
    cout << "Enter Professor id: ";
    string id;
    cin >> id;
    Professor* prof = findProfessor(id);
    if (prof != nullptr){
        return prof;
    }
    else{
        cout << "The person with ID " << id << " is not a Professor.\n";
        return nullptr;
    }
}

Course* Secretary::readAndValidateCourse(){
    cout << "Enter the course code: ";
    string code;
    cin >> code;
    Course* course = findCourse(code);
    if (course == nullptr){
        cout << "Course not found\n";
    }
    return course;
}

Semester* Secretary::readAndValidateSemester(){
    Semester sem;
    cin >> sem;
    for(auto semptr : semesters){
        if(semptr->getYear() == sem.getYear() && semptr->getSeason() == sem.getSeason()){
            return semptr;
        }
    }
    return addSemester(sem);
}

void Secretary::readStudentsFromFile(){
    ifstream f("studentinfo.json");
    if(f.is_open()){
        jStudents = json::parse(f);
        Student stud;
        for(auto& item: jStudents){
            item.get_to(stud);
            //cout << stud;
            addPerson(stud, false, false);
        }
        f.close();
    }
    else{
        cerr << "Unable to open file\n"; 
    }
}

void Secretary::readProfessorsFromFile(){
    ifstream f("profinfo.json");
    if(f.is_open()){
        jProfessors = json::parse(f);
        Professor prof;
        for(auto& item: jProfessors){
            item.get_to(prof);
            //cout << prof;
            addPerson(prof, false, false);
        }
        f.close();
    }
    else{
        cerr << "Unable to open file\n"; 
    }   
}

void Secretary::readCourseFromFile(){
    ifstream f("courseinfo.json");
    if(f.is_open()){
        jCourses = json::parse(f);
        Course course;
        for(auto& item: jCourses){
            item.get_to(course);
            addCourse(course, false);
            if (course.getMand()){
                numOfMandatory++;
            }
        }
        f.close();
    }
    else{
        cerr << "Unable to open file\n"; 
    }
}

void Secretary::printStudentToFile(Student& student){
    jStudents.push_back(student);
    ofstream f("studentinfo.json");
    if(f.is_open()){
        f << jStudents.dump(4);
        f.close();
    }
    else{
        cerr << "Could not open file for writing\n";
    }
}

void Secretary::printProfessorToFile(Professor& professor){
    jProfessors.push_back(professor);
    ofstream f("profinfo.json");
    if(f.is_open()){
        f << jProfessors.dump(4);
        f.close();
    }
    else{
        cerr << "Could not open file for writing\n";
    }
}

void Secretary::printCourseToFile(Course& course){
    jCourses.push_back(course);
    ofstream f("courseinfo.json");
    if(f.is_open()){
        f << jCourses.dump(4);
        f.close();
    }
    else{
        cerr << "Could not open file for writing\n";
    }
}

void Secretary::printSemesterToFile(Semester& sem){
    jSemesters.push_back(sem);
    ofstream f("semesterinfo.json");
    if(f.is_open()){
        f << jSemesters.dump(4);
        f.close();
    }
    else{
        cerr << "Could not open file for writing\n";
    }
}

void Secretary::jsonModifyProf(Professor& prof, string id){
    for (auto& element : jProfessors){
        if (element["idCode"] == id){
            element = prof;
            break;
        }
    }
        
    ofstream f("profinfo.json");
    if(f.is_open()){
        f << jProfessors.dump(4);            // Writes the JSON array to the file
        f.close();
    }
    else{
        cerr << "Could not open file for writing\n";
    }
}

void Secretary::jsonModifyStud(Student& stud, string id){
    for (auto& element : jStudents){
        if (element["idCode"] == id){
            element = stud;
            break;
        }
    }
        
    ofstream f("studentinfo.json");
    if(f.is_open()){
        f << jStudents.dump(4);            // Writes the JSON array to the file
        f.close();
    }
    else{
        cerr << "Could not open file for writing\n";
    }
}

void Secretary::jsonModifyCourse(Course& course, string code){
    for (auto& element : jCourses){
        if (element["code"] == code){
            element = course;
            break;
        }
    }
        
    ofstream f("courseinfo.json");
    if(f.is_open()){
        f << jCourses.dump(4);            // Writes the JSON array to the file
        f.close();
    }
    else{
        cerr << "Could not open file for writing\n";
    }
}

void Secretary::jsonRemoveProfessor(Professor& prof){
    for (auto it = jProfessors.begin(); it != jProfessors.end(); ++it) {
        if ((*it)["idCode"] == prof.getIdCode()) {
            it = jProfessors.erase(it);
            break;
        }
    }

    ofstream f("profinfo.json");
    if(f.is_open()){
        f << jProfessors.dump(4);            // Writes the JSON array to the file
        f.close();
    }
    else{
        cerr << "Could not open file for writing\n";
    }

}

void Secretary::jsonRemoveStudent(Student& stud){
    for (auto it = jStudents.begin(); it != jStudents.end(); ++it) {
        if ((*it)["idCode"] == stud.getIdCode()) {
            it = jStudents.erase(it);
            break;
        }
    }

    ofstream f("studentinfo.json");
    if(f.is_open()){
        f << jStudents.dump(4);            // Writes the JSON array to the file
        f.close();
    }
    else{
        cerr << "Could not open file for writing\n";
    }
}

void Secretary::jsonRemoveCourse(Course& course){
    for (auto it = jCourses.begin(); it != jCourses.end(); ++it) {
        if ((*it)["code"] == course.getCode()) {
            it = jCourses.erase(it);
            break;
        }
    }

    ofstream f("courseinfo.json");
    if(f.is_open()){
        f << jCourses.dump(4);            // Writes the JSON array to the file
        f.close();
    }
    else{
        cerr << "Could not open file for writing\n";
    }
}
