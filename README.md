# OOP-Project1-Part2

Team members:
|  |  |
|------          |----      |
|Elpida Stergiou |sdi2200173|
|Ioannis Tampouris|sdi2200181|
|  |  |

The implementation was done in Visual Studio Code using g++. A makefile is included to build and link the file, to do so:
> make run 

# About Project

This Project implements a college department's secretary and its operations. All the main operations are handled through the `Secretary` class.
The other three main classes are: `Course`, `Student` and `Professor`, of which the last two are child classes of the abstract class `Person`.
Each object of those three classes has an ID code which is its unique identifier. The `Semester` class is the one which stores the majority of the connections between the objects. In this case "semester" refers to an actual semester in time, not to be confused with an academic semester. This semester is distinctive by
year (eg. 2023) and season (winter semester or summer semester) and it corresponds to an academic school semester. Thus, an association of course, student and/or professor is valid for that certain period of time, a semester. Obviously the secretary also stores past connections. 

All the data of the program is stored in JSON files. These are handled by the library __nlohmann/json__ which we chose to incorporate into our project. All information concerning the library is in __Include/json.hpp__. There are three main information files: `studentinfo.json`, `profinfo.json` and `courseinfo.json`.
There are also files named as such: CourseName + year which contain Students who passed a course in a certain semester, and their grades. All of these information files are loaded into the program at the start and while running the program new information can be added and old information can be accessed, modified or even deleted. After any of these changes the files are updated and new data is kept in them even after the program stops running. When the program is ran again, this new information will now be accessible.   

Specifically, the files contain information about student, professor and course data, and more importantly student registrations to courses, students' past grades,
courses taught by a professor, etc. Again, __all of this information is loaded into the program at the start and gets updated before the program finishes.__

# Code implementation

There are very specific comments in the code in the places where it is needed. The functionality of the code is described adequately.

## `main()` method

The `main()` method calls the constructor of a department's secretary. A;; functionality of the code is showcased by running it and checking the JSON files.
How to run: each time the program is run it first asks the user to input the semester(year and season) for which they want to run the program, as different semesters give different access to operations and info of the program. The program can be run repeatedly for different semesters, in order to handle any data.
__This program contains all the functionalities it was tasked to perform.__
