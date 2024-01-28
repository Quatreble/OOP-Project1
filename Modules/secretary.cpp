#include "secretary.hpp"

/////////////////////////////////////////////////////////////////////////////////////////
//////Secretary class functions//////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////


// constructor. initialize secretary's attributes,
// read Person and Course data from files, create Student, 
// Professor, Course and Semester instances accordingly and add them to secretary
// finally, read the semester for which the program is ran and call secretaryOperation() to begin operations
Secretary::Secretary(const string& dep, int sem, int reqPoints) 
: depName(dep), depSemesters(sem), pointsToGraduate(reqPoints) 
{                                                              
    readCourseFromFile();
    readStudentsFromFile();
    for (auto& element : depStudents){
        readCoursesAndGrades(element.second);
    }
    readProfessorsFromFile();
    for (auto& element : depProfessors){
        readProfessorCourses(element.second);
    }
    readCurrentDate();
    SecretaryOperation();
}

Secretary::~Secretary(){ //destructor. deletes all pointers within the secretary's structs
    for(auto& it : depStudents){
        it.second->deleteCoursesWithGrades();
        delete it.second;
    }
    for(auto& it : depProfessors){
        delete it.second;
    }
    for(auto& it : depCourses){
        delete it.second;
    }
    for(auto it: semesters){
        delete it;
    }
    cout << "    SECRETARY DELETED\n";

}



Secretary::Secretary(const Secretary& sec) //copy constructor for deep copy 
: depName(sec.depName), depSemesters(sec.depSemesters), pointsToGraduate(sec.pointsToGraduate), currYear(sec.currYear), currSeason(sec.currSeason), numOfMandatory(sec.numOfMandatory)
{
    for (auto& element : sec.depStudents) {
        addPerson(*element.second, false);
    }
    for (auto& element : sec.depProfessors) {
        addPerson(*element.second, false);
    }
    for (auto& element : sec.depCourses) {
        addCourse(*element.second);
    }
    for(auto& element: sec.semesters) {
        addSemester(*element);
    }
}

//print numbered menu of secretary's operations
void Secretary::printMenu(){
    cout << "\tECLASS\n";
    cout << "1. PROFESSOR OPTIONS\n";
    cout << "2. STUDENT OPTIONS\n";
    cout << "3. COURSE OPTIONS\n";
    cout << "4. REGISTER STUDENT TO COURSE\n";
    cout << "5. REGISTER PROFESSOR TO COURSE\n";
    cout << "6. GRADE STUDENT\n";
    cout << "7. PRINT STUDENTS WHO PASSED A COURSE\n";
    cout << "8. PRINT PROFESSOR COURSE STATISTICS\n";
    cout << "9. GET STUDENT'S GRADES\n";
    cout << "10. GET LIST OF STUDENTS WHO CAN GRADUATE\n";
    cout << "TYPE 0 TO EXIT\n";
}

// read and validate input for desired operation. call the function which implements said operation
// this repeated process is terminated when input is 0
void Secretary::SecretaryOperation(){
    int op;
    while (true){
        this_thread::sleep_for(chrono::seconds(1)); // wait a second everytime before printing the menu
        printMenu();
        cin >> op;

        if (cin.fail()) {
            cin.clear(); 
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "INVALID INPUT. PLEASE ENTER A NUMBER: " << std::endl;
        } 
        else if (op < 0 || op > 10){
            cout << "WRONG INPUT\n";
            cout << "INPUT AGAIN\n";
            continue;
        }
        else if (op == 0){
            cout << "\tGOODBYE! :)\n";
            return;
        }
        else if (op == 1){
            cout << "1. ADD PROFESSOR\n";
            cout << "2. MODIFY PROFESSOR\n";
            cout << "3. REMOVE PROFESSOR\n";
            cin >> op;

            if (op == 1){ readAndAddProfessor(); }
            else if (op == 2){ modifyProfessor(); }
            else if (op == 3){ deleteProfessor(); }
        }
        else if (op == 2){
            cout << "1. ADD STUDENT\n";
            cout << "2. MODIFY STUDENT\n";
            cout << "3. REMOVE STUDENT\n";
            cin >> op;

            if (op == 1){ readAndAddStudent(); }
            else if (op == 2){ modifyStudent(); }
            else if (op == 3){ deleteStudent(); }
        }
        else if (op == 3){
            cout << "1. ADD COURSE\n";
            cout << "2. MODIFY COURSE\n";
            cout << "3. REMOVE COURSE\n";
            cin >> op;

            if (op == 1){ readAndAddCourse(); }
            else if (op == 2){ modifyCourse(); }
            else if (op == 3){ deleteCourse(); }
        }
        else if(op == 4){ registerStudentToCourse(); }
        else if (op == 5){ setCourseProf(); }
        else if (op == 6){ gradeStudents(); }
        else if (op == 7){ printStudentsWhoPassed(); }
        else if (op == 8){ printProfStats(); }
        else if (op == 9){ getGrades(); }
        else if (op == 10){ printGraduates(); }
    }
    
}

// using the appropriate readAndValidateXX function, register given to Professor to given Course for Given Semester
void Secretary::setCourseProf(){
    Semester* sem = readAndValidateSemester(); //checks
    Course* course = readAndValidateCourse();
    if(course != nullptr){
        Professor* prof = readAndValidateProfessor();
        if(prof==nullptr) return;
        sem->addProfToCourse(course, prof); // in given semester add professor to course's professors
        prof->addCourse(course->getCode(),sem->getYear(),sem->getSeason()); // Course-Professor connection is made by and stored in Semester class
        jsonModifyProf(*prof,prof->getIdCode()); // update the professorInfo file after this registration
    }
}

// register given student to given course for given semester
// there are certain additional checks to be made for this registration:
// 1. student may not register if they have already passed this course
// 2. student can only register to a course of the current season with student's year >= course's year 
// (a student in semester 3(winter) can register to a semester 1(also winter) course) that they have not passed
// 3. obviously, the student cannot register to courses on a higher academic year and lastly,
// 4. the year of the semester for which they are trying to register must not be earlier than their registration year
void Secretary::registerStudentToCourse(){
    Student* stud = readAndValidateStudent(); // input validity checks
    if (stud == nullptr) return;
    Semester* sem = readAndValidateSemester();
    Course* course = readAndValidateCourse();
    if (course == nullptr) return; // checks for Student's registration eligibility
    if(sem->getSeason() != course->getSeason() || currYear > sem->getYear() || sem->getYear() - stud->getReg() < course->getYear() - 1 || sem->isRegistered(course, stud) != nullptr){
        cout << "STUDENT CAN'T REGISTER TO THIS COURSE\n";
        return;
    }
    if (stud->getCourseGrade(course) >= 5){
        cout << "STUDENT HAS ALREADY PASSED COURSE\n";
        return;
    }
    sem->addStudToCourse(course, stud); // again the Course-Student connection is made by and stored in Semester class
}


// this functions allows to give a grade to a given student for a given course in a semester
// the readAndValidate functions for each class operate as above
// there is also a check for whether the student is registered to the course, and whether they have already been graded or not
void Secretary::gradeStudents(){
    StudentCourseInstance* sci = nullptr;
    Semester* sem  = nullptr;
    Student* stud = readAndValidateStudent(); // read and check
    if (stud == nullptr) return;
    Course* course = readAndValidateCourse();
    if (course == nullptr) return;
    for (Semester* semTemp : semesters){ // iterate through Secretary::semesters
        StudentCourseInstance* sciTemp = semTemp->isRegistered(course, stud); // if semester exists, check if the combination of given Student and Course are stored in it
        if (sciTemp != nullptr && sciTemp->grade == -1){ // if the grade stored equals -1 (hasn't been graded yet), continue to change it
            sci = sciTemp;
            sem = semTemp;
            break;
        }
    }
    if (sci != nullptr){ // create a new SemesterGradeInstance and pass it to Student to store it along with the new grade through the function Student::addCourseWithGrade
        try{
            sem->gradeStud(sci);
            SemesterGradeInstance* semGrade = new SemesterGradeInstance;
            semGrade->grade = sci->grade;
            semGrade->year = sem->getYear();
            semGrade->isWinter = sem->getSeason();
            stud->addCourseWithGrade(course, semGrade);
            jsonModifyStud(*stud,stud->getIdCode()); // update studentInfo file which contains information about students and their registered courses' grades
            return;
        }
        catch (const bad_alloc &e){ // throw exception for bad allocation
            cerr << "MEMORY ALLOCATION FAILED: " << e.what() << '\n';
            return;
        }
    }
    if (sem == nullptr){
        cout << "STUDENT NOT REGISTERED TO COURSE OR HAS ALREADY BEEN GRADED\n";
        return;
    }

    
}

// mandatory checks and then call Semester::printPassed to print and write to a json file the info of the student that 
// passed a given course in a given semester
void Secretary::printStudentsWhoPassed(){
    Semester* sem = readAndValidateSemester();
    Course* course = readAndValidateCourse();
    if (course == nullptr) return;
    sem->printPassed(course);
}

// operates as above, Semester::printProfStats prints the statistics of a certain course that a certain professor
// is teaching in a semester 
void Secretary::printProfStats(){
    Professor* prof = readAndValidateProfessor();
    if (prof == nullptr) return;
    Semester* sem = readAndValidateSemester();
    sem->printProfStats(prof);
}

// read and validate a student, then give the following options:
// (a) get all grades for registered courses of current semester
// (b) get grade history of student(grades of all past semesters)
// call corresponding functions: Semester::printStudGrades for (a)
// Student::printGrades for (b)
// the two functions although similar are of different classes
// this is because for (a) we require data of just one semester, which is stored in a Semester class instance,
// while for (b) we require all data for past semesters that concerns a Student object
// and is stored inside it using the SemesterGradeInstance struct
void Secretary::getGrades(){
    Student* stud = readAndValidateStudent(); // checks
    if (stud == nullptr) return;
    cout << "1. GRADES FOR CURRENT SEMESTER\n";
    cout << "2. GRADE HISTORY\n";
    int op;
    cin >> op;
    if (op == 1){
        Semester* sem = getCurrSem(); // assure that there is data stored for current semester
        if (sem == nullptr){
            cout << "NO GRADES FOR THE CURRENT SEMESTER\n";
            return;
        }
        sem->printStudGrades(stud); // operation (a)
    }
    if (op == 2){
        stud->printGrades(); // operation (b)
    }
}


// print info of all students in secretary that are able to graduate
// a student may graduate if they have:
// 1. completed the years of study
// 2. passed all mandatory courses
// 3. collected the required number of academic points
void Secretary::printGraduates(){
    Student* stud;
    bool grads = false;
    for(auto& it: depStudents){ // iterate through the department's students
        stud = it.second;
        if(currYear - stud->getReg() >= depSemesters/2 && stud->getAcademicPoints() >= pointsToGraduate // required checks
        && stud->getMandatoryPassed() >= numOfMandatory){
            grads = true;
            cout << *stud; // if student passed the check, print
        } 
    }
    if(grads == false){
        cout << "NO STUDENTS ARE CURRENTLY ABLE TO GRADUATE\n";
    }
}

// iterates through the secretary's semesters, to find whether the current semester is logged/ there is info for it
// semester equality is based on their year and season
Semester* Secretary::getCurrSem(){
    int year = currYear;
    char sem = currSeason;
    bool isWinter = (sem == 'W' || sem == 'w');
    for(auto& semester: semesters){
        if(semester->getYear() == year && semester->getSeason() == isWinter){
            return semester;
        }
    }
    return nullptr;
}

//uses a dynamic cast to Student pointer to check if Person* is a Student*
Student* Secretary::isStudent(Person *p){
    return dynamic_cast<Student *> (p);
}

//as above but for Professor
Professor* Secretary::isProfessor(Person *p){
    return dynamic_cast<Professor *> (p);
}

// using the overloaded input operator, read a Professor's info from the console and add new Professor to secretary
void Secretary::readAndAddProfessor(){
    Professor f;
    cin >> f;
    addPerson(f, true);
}

//as above but for Student
void Secretary::readAndAddStudent(){
    Student s;
    cin >> s;
    addPerson(s, true);
}

//as above but for Course
void Secretary::readAndAddCourse(){
    Course c;
    cin >> c;
    addCourse(c, true);
}

// this overloaded function takes as an argument either a Student& or a Professor& and adds them to their respective
// unordered_maps Secretary::depStudents and Secretary::depProfessors (with their unique idCode as key) which store all of the department's members 
// initially the Person::clone function is called, which dynamically allocates and creates a new Student/Professor and returns a
// pointer to it which is what later gets inserted to the map
// also the studentinfo.json/profinfo.json are respectively updated to contain the new member
// there is also a check for whether this Person is already in the map, and a printStatement to let the user know
// that the addition was successful
void Secretary::addPerson(Student& s, bool printStatement, bool manualAdd){
        Student* stud = s.clone(); // clone, create pointer to be added to map
        auto check = depStudents.emplace(stud->getIdCode(),stud); // emplace, if it already exists it isn't re-added to the map
        if (check.second && manualAdd){
            printStudentToFile(*stud); // update file
        }
        else if (!check.second && manualAdd){
            cout << "ID ALREADY EXISTS\n";
            printStatement = false;
        }
        if (printStatement) cout << "ADDED " << stud->getFirstName() << " TO " << depName << "!" << endl;
}

// ^^
void Secretary::addPerson(Professor& p, bool printStatement, bool manualAdd){
        Professor* prof = p.clone();
        auto check = depProfessors.emplace(prof->getIdCode(),prof);
        if (check.second && manualAdd){
            printProfessorToFile(*prof);
        }
        else if (!check.second && manualAdd){
            cout << "ID ALREADY EXISTS\n";
            printStatement = false;
        }
        if (printStatement) cout << "ADDED " << prof->getFirstName() << " TO " << depName << "!" << endl;
}

// operates exactly like addPerson, only the dynamic allocation of the new Course is inside this funtion
// if the Courses is mandatory, increment the number of the secretary's mandatory courses by 1
void Secretary::addCourse(Course& c,bool manualAdd){
    try{
        Course* courseptr = new Course(c);
        auto check = depCourses.emplace(courseptr->getCode(),courseptr);
        if (check.second && manualAdd){
            printCourseToFile(*courseptr);
        }
        else if (!check.second && manualAdd){
            cout << "CODE ALREADY EXISTS\n";
        }
        if(courseptr->getMand()){
            ++numOfMandatory;
        }
    }
    catch(const bad_alloc& e){ // throw exception if unable to allocate space 
        cerr << "MEMORY ALLOCATION FAILED: " << e.what() << '\n';
    }
}

// same as above, semesters are stored in a vector
Semester* Secretary::addSemester(Semester& toAdd){
    try{
        Semester* semptr = new Semester(toAdd);
        semesters.push_back(semptr);
        return semptr;
    }
    catch(const bad_alloc &e){
        cerr << "MEMORY ALLOCATION FAILED: " << e.what() << '\n';
        return nullptr;
    }
}

// these two functions for modifying a professor or a student inside the secretary operate as following:
// the person's unique id code is given. If that person is found in the correspoinding map(depStudents/depProfessors)
// the previous pair of code/Person* is removed from the map, while a new one is created using the user's input and inserted to the map
// the json file which contains the student/professor info is also updated
void Secretary::modifyProfessor(){
    cout << "INPUT PROFESSOR ID: ";
    string id;
    cin >> id;
    Professor* prof = findProfessor(id);
    if (prof!=nullptr) {  // Check if person is professor and is in secretary
        string prevName = prof->getFirstName() + " " + prof->getLastName();
        string prevId = prof->getIdCode();
        auto it = depProfessors.find(prof->getIdCode()); // find pair in map with previous info
        depProfessors.erase(it); // remove it
        cout << "PLEASE ENTER THE NEW DATA: \n";
        cin >> *prof; // read new data
        depProfessors[prof->getIdCode()] = prof; // insert new data to map
        jsonModifyProf(*prof,prevId); // write to file
        cout << "PROFESSOR " << prevName << " CHANGED TO " << prof->getFirstName() << " " << prof->getLastName() << '\n';

    } else{ //if person is found but isnt a professor
        cout << "THE PERSON WITH ID " << id << " IS NOT A PROFESSOR.\n";
    }
}

// ^^
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
        cout << "PLEASE ENTER THE NEW DATA: \n";
        cin >> *stud;
        depStudents[stud->getIdCode()] = stud;
        jsonModifyStud(*stud,prevId);
        cout << "STUDENT " << prevName << " CHANGED TO " << stud->getFirstName() << " " << stud->getLastName() << '\n';

    } else{ //if person is found but isnt a professor
        cout << "THE PERSON WITH ID " << id << " IS NOT A STUDENT.\n";
    }
}

// functions operates eaxctly as the ones above, with a slight difference:
// a choice is given to the user for whether they want to modify all of the course's
// attributes, or simply change the academic semester for which it is available
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
        cout << "PLEASE ENTER THE NEW DATA: \n";
        cin >> *course;
        depCourses[course->getCode()] = course;
    }
    else if (op == 2){
        cout << "ENTER COURSE YEAR: ";
        int year;
        cin >> year;
        course->setYear(year);
        string season;
        cout << "ENTER 'W' FOR WINTER SEMESTER, 'S' FOR SUMMER: ";
        while (true) {
            cin >> season;
            if (season == "w" || season == "W" || season == "s" || season == "S") {
                break;
            } else {
                cout << "INVALID INPUT. ENTER W FOR WINTER, S FOR SUMMER: ";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
        }
        course->setSemester(season);
    }
    cout << "COURSE WAS MODIFIED\n";
    jsonModifyCourse(*course,prevCode);
}

// this overloaded function for Student and Professor
// iterates vector until person with same properties is found(Person::equals function is used for this), delete Person, remove it from vector 
// Secretary::depStudents and Secretary::depProfessors accordingly
bool Secretary::removePerson(Student& s){
    auto it = depStudents.find(s.getIdCode());
    if(it != depStudents.end()){
        cout << "REMOVED " << it->first << " FROM " << depName << endl;
        delete it->second;
        depStudents.erase(it);       
        return true;
    }
    return false;
}

// ^^
bool Secretary::removePerson(Professor& p){
    auto it = depProfessors.find(p.getIdCode());
    if(it != depProfessors.end()){
        cout << "REMOVED " << it->first << " FROM " << depName << endl;
        delete it->second;
        depProfessors.erase(it);       
        return true;
    }
    return false;
}

// same as above, but also erases any of the course's data that is stored by other objects(students, professors and semesters)
// also decrease by 1 the number of secretary's mandatory courses if needed
// updates any json with info that is needed
void Secretary::removeCourse(Course& course){
    auto it = depCourses.find(course.getCode());
    if(it != depCourses.end()){
        cout << "ERASED " << course.getName() << '\n';
        for (auto& element : depStudents){ // erase from students
            element.second->eraseCourse(course.getName(), course.getMand());
            jsonModifyStud(*element.second,element.second->getIdCode());
        }
        for (auto& element : depProfessors){ // erase from professors
            element.second->eraseCourse(course.getName());
            jsonModifyProf(*element.second,element.second->getIdCode());
        }
        for (Semester* sem : semesters){ // erase from semesters
            sem->eraseCourse(course.getCode());
        }
        if (course.getMand()){
            numOfMandatory--;
        }
        delete it->second;
        depCourses.erase(it);
        return;
    }
    
}

// the following three delete functions operate as follows:
// finds Professor/ Student/ Course whose id code is input and deletes them from secretary
// utilizes Secretary::findProfessor/findStudent/findCourseByCode to get object pointer,
// if it is not null, the corresponding remove function gets called and also the jsonRemove function,
// in order to modify the json info file after deletion
void Secretary::deleteProfessor(){
    cout << "INPUT PROFESSOR ID: ";
    string id;
    cin >> id;
    Professor* prof = findProfessor(id);
    if (prof != nullptr) {  //same as above
        jsonRemoveProfessor(*prof);
        removePerson(*prof);
    } else{
        cout << "THE PERSON WITH ID " << id << " IS NO A PROFESSOR.\n";
    }
}

// ^^
void Secretary::deleteStudent(){
    cout << "INPUT STUDENT ID: ";
    string id;
    cin >> id;
    Student* stud = findStudent(id);
    if (stud != nullptr) {  //same as above
        jsonRemoveStudent(*stud);
        removePerson(*stud);
    } else{
        cout << "THE PERSON WITH ID " << id << " IS NOT A STUDENT.\n";
    }
}

// ^^
void Secretary::deleteCourse(){
    string name;
    cout << "INPUT COURSE CODE: ";
    cin >> name;     
    Course* course = findCourseByCode(name);
    if (course != nullptr){
        jsonRemoveCourse(*course);
        removeCourse(*course);
    } 
    else
        cout << "COURSE NOT FOUND\n";
}

// find function using a map is extremely simple
// find the Student which corresponds to the given key, in this case
// the id code, and if it exists return the object. else return nullptr
Student* Secretary::findStudent(const string& id){
    auto itStud = depStudents.find(id);
    if (itStud != depStudents.end()){
        return itStud->second;
    }
    return nullptr;
}

// same as above
Professor* Secretary::findProfessor(const string& id){
    auto itProf = depProfessors.find(id);
    if (itProf != depProfessors.end()){
        return itProf->second;
    }
    return nullptr;
}

// same as above
Course* Secretary::findCourseByCode(string code){
    for (auto& element : depCourses){
        if (element.second->getCode() == code){
            return element.second;
        }
    }
    return nullptr;
}

// find course by name, iterate the depCourses map and if 
// a course's name equals the given string return that course
// else return nullptr
Course* Secretary::findCourseByName(string name){
    for (auto& element : depCourses){
        if (element.second->getName() == name){
            return element.second;
        }
    }
    return nullptr;
}

// read a student, search for it in Secretary::depStudents and if found return a Pointer to it
// else return nullptr
Student* Secretary::readAndValidateStudent(){
    cout << "INPUT STUDENT ID: ";
    string id;
    cin >> id;
    Student* stud= findStudent(id);
    if (stud != nullptr){
        return stud;
    }
    else{
        cout << "THE PERSON WITH ID " << id << " IS NOT A STUDENT.\n";
        return nullptr;
    }
}

// same as above but  for Professor
Professor* Secretary::readAndValidateProfessor(){
    cout << "INPUT PROFESSOR ID: ";
    string id;
    cin >> id;
    Professor* prof = findProfessor(id);
    if (prof != nullptr){
        return prof;
    }
    else{
        cout << "THE PERSON WITH ID " << id << " IS NOT A PROFESSOR.\n";
        return nullptr;
    }
}

// ^^ but for Course
Course* Secretary::readAndValidateCourse(){
    cout << "INPUT COURSE CODE: ";
    string code;
    cin >> code;
    Course* course = findCourseByCode(code);
    if (course == nullptr){
        cout << "COURSE NOT FOUND\n";
    }
    return course;
}

// ^^ but for Semester. Because there is no findSemester function,
// all we iterate semesters vector and compare the semesters' year and season 
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

// read student information from json file, create the appropriate
// Student objects, and add them to Secretary::depStudents using Secretary::addPerson 
// for student, the file also contains information about the courses they've registered to and got graded in past semesters
void Secretary::readStudentsFromFile() {
    try{
        ifstream f("studentinfo.json"); // try to open file, if unable to open throw exception
        if (!f) throw runtime_error("COULD NOT OPEN FILE FOR READING");
        jStudents = json::parse(f); // parse json and put it in json array Secretary::jStudents
        for (auto& item : jStudents) { // for every object of array create a student from info and add it to secretary
            Student stud;              // the json object to Student conversion is done by the from_json function in student.cpp
            item.get_to(stud);    // tthe get_to function knows how "read" a Student object because of from_json
            addPerson(stud, false, false); 
        }

        f.close(); // close file
    } 
    catch(const exception &e) {
        cerr << "UNABLE TO OPEN FILE\n";
    }
}

// as above but for Professor
// for professor, the file also contains information about the courses they've teached in past semesters
void Secretary::readProfessorsFromFile(){
    try{
        ifstream f("profinfo.json");
        if (!f) throw runtime_error("COULD NOT OPEN FILE FOR READING");
        jProfessors = json::parse(f);
        Professor prof;
        for(auto& item: jProfessors){
            item.get_to(prof);
            addPerson(prof, false, false);
        }
        f.close();
    }
    catch(const exception &e){
        cerr << "UNABLE TO OPEN FILE\n";
    }   
}

// as above but for Course
void Secretary::readCourseFromFile(){
    try{
        ifstream f("courseinfo.json");
        if (!f) throw runtime_error("COULD NOT OPEN FILE FOR READING");
        jCourses = json::parse(f);
        Course course;
        for(auto& item: jCourses){
            item.get_to(course);
            addCourse(course, false);
        }
        f.close();
    }
    catch (const exception &e){
        cerr << "UNABLE TO OPEN FILE\n"; 
    }
}

// this function takes the data stored in student after reading the studentinfo.json file
// at the start of the program and creates the necessary objects and connections to store that
// info properly in secretary. eg. we have a student registered to a course in X semester who has also been graded
// and we want to create that semester and store in it that data. remember that the semester class is the one that connects
// course to student to grade for secretary
void Secretary::readCoursesAndGrades(Student* stud){
    unordered_map<string, SemesterGradeInstance*> map;
    map = stud->getCoursesWithGrades(); // get info from student 
    for (auto& element : map){
        Course* course;
        Semester* sem = nullptr;
        for(auto semptr : semesters){ // iterate map with course-semester-grade info. if a semester in that map does not exist in secretary, create it
            if(semptr->getYear() == element.second->year && semptr->getSeason() == element.second->isWinter){
                sem = semptr;
            }
        }
        if (sem == nullptr){
            Semester semTemp;
            semTemp.setYear(element.second->year);
            semTemp.setSeason(element.second->isWinter);
            sem = addSemester(semTemp);
        }
        course = findCourseByName(element.first);
        if (!course) return;
        sem->addStudToCourse(course,stud, false); // register student to course in semester
        StudentCourseInstance* sci = sem->isRegistered(course, stud);
        if (!sci) return;
        sci->grade = element.second->grade; // store the grade also in semester
    }
}

//this functions works similarly to the one above, but for professor and the courses they've teached in a semester
// takes the info from inside professor class, creates semester if needed and passes the info to it (in the Semester courseProfs map)
void Secretary::readProfessorCourses(Professor* prof){
    unordered_map<string,vector<pair<int,bool>>> map;
    map = prof->getProfCourses();
    for (auto& element : map){
        Semester* sem;
        for (auto& pair : element.second){
            sem = nullptr;
            for(auto& semptr : semesters){
                if(semptr->getYear() == pair.first && semptr->getSeason() == pair.second){
                    sem = semptr;
                }
            }
            if (sem == nullptr){
                Semester semTemp;
                semTemp.setYear(pair.first);
                semTemp.setSeason(pair.second);
                sem = addSemester(semTemp);
            }
            Course* course = findCourseByCode(element.first);
            sem->addProfToCourse(course, prof, false);
        }
    }
}

// converts given Student& to json object using function Student::to_json and inserts it to json array Secretary::jStudents
// then writes updated array version to file
void Secretary::printStudentToFile(Student& student){
    json studentJson; // create the json object
    student.to_json(studentJson, student); // Explicitly convert Student to json
    jStudents.push_back(studentJson); // insert to array
    printJson("studentinfo.json",jStudents); // write to file

}

// same as above, explicit conversion is not needed for Professor
void Secretary::printProfessorToFile(Professor& professor){
    jProfessors.push_back(professor);
    printJson("profinfo.json",jProfessors);

}

// same as above for Course
void Secretary::printCourseToFile(Course& course){
    jCourses.push_back(course);
    printJson("courseinfo.json",jCourses);

}

// prints a json array to the json file "fileName.json"
void Secretary::printJson(string fileName, json& array){
    try{
        ofstream f(fileName); // open file
        if (!f) throw runtime_error("COULD NOT OPEN FILE FOR WRITING"); // throw exception if unable to open file
        f << array.dump(4); // write to file
        f.close();
    }
    catch (const exception& e){
        cerr << e.what() << '\n';
    }
}

// search json array Secretary::jStudents for given id of Student, if found create a new json Student object from given Student& stud
// and replace old student with new in array
// then call printJson to update file
void Secretary::jsonModifyStud(Student& stud, string id){
    for (auto& element : jStudents){ // iterate through jStudents
        if (element["idCode"] == id){ // comparison is based on id code
            json studentJson; // create new json object and convert stud to json 
            stud.to_json(studentJson, stud);
            element = studentJson; // replace student
            break;
        }
    }
        
    printJson("studentinfo.json",jStudents); // write updated array to file

}

//same as above but for Professor
void Secretary::jsonModifyProf(Professor& prof, string id){
    for (auto& element : jProfessors){
        if (element["idCode"] == id){
            element = prof;
            break;
        }
    }
        
    printJson("profinfo.json",jProfessors);

}

// same but for Course
void Secretary::jsonModifyCourse(Course& course, string code){
    for (auto& element : jCourses){
        if (element["code"] == code){
            element = course;
            break;
        }
    }
        
    printJson("courseinfo.json",jCourses);
}

// search json array Secretary::jProfessors for given Professor(compare the professors' id codes). If found
// remove it from array and update file by writing the array again
void Secretary::jsonRemoveProfessor(Professor& prof){
    for (auto it = jProfessors.begin(); it != jProfessors.end(); ++it) { // iterate through jProfessors
        if ((*it)["idCode"] == prof.getIdCode()) {
            it = jProfessors.erase(it); //remove professor
            break;
        }
    }

    printJson("profinfo.json",jProfessors); // update file

}

//same as above, but for Student
void Secretary::jsonRemoveStudent(Student& stud){
    for (auto it = jStudents.begin(); it != jStudents.end(); ++it) {
        if ((*it)["idCode"] == stud.getIdCode()) {
            it = jStudents.erase(it);
            break;
        }
    }

    printJson("studentinfo.json",jStudents);
}

//same as above, but for Course
void Secretary::jsonRemoveCourse(Course& course){
    for (auto it = jCourses.begin(); it != jCourses.end(); ++it) { 
        if ((*it)["code"] == course.getCode()) {
            it = jCourses.erase(it); 
            break;
        }
    }

    printJson("courseinfo.json",jCourses); 
}

//read semester and season repeatedly until input is valid
void Secretary::readCurrentDate(){
    cout << "ENTER CURRENT YEAR: ";
    cin >> currYear;
    while (cin.fail()){
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "WRONG INPUT. PLEASE ENTER A VALID YEAR: ";
        cin >> currYear;
    }

    cout << "ENTER CURRENT SEMESTER SEASON ('W' OR 'S'): ";
    cin >> currSeason;
    while ((currSeason != 'w' && currSeason != 'W' && currSeason != 's' && currSeason != 'S') || cin.fail()){
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "WRONG INPUT. PLEASE ENTER 'W' OR 'S': ";
        cin >> currSeason;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

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
    os << "SECRETARY " << secretary.depName << ":" << endl;
    for (const auto& element : secretary.depStudents){
        os << "STUDENT:" << (*element.second); 
    }
    for (const auto& element : secretary.depProfessors){
        os << "PROFESSOR:" << (*element.second); 
    }
    os << endl;
    return os;
}

//overloaded operator >> that allows the user to change/input the name of the Secretary and add as many People to it as they want
istream& operator>>(istream& is, Secretary& sec){
    char type;
    cout << "ENTER DEPARTMENT NAME: ";
    is >> sec.depName;
    cout << "ENTER 'S' FOR STUDENT, 'F' FOR PROFESSOR, 0 TO STOP ADDING: ";
    is >> type;
    while(type !='0'){
        if(type == 's'){
            Student s;
            cout << "ENTER FIRST NAME, LAST NAME, AND ID CODE: ";
            is >> s;
            sec.addPerson(s);
        }
        if(type == 'f'){
            Professor f;
            cout << "ENTER FIRST NAME, LAST NAME, AND ID CODE: ";
            is >> f;
            sec.addPerson(f);
        }
        cout << "ENTER 'S' FOR STUDENT, 'F' FOR PROFESSOR, 0 TO STOP ADDING: ";
        is >> type;
    }
    return is;
}
