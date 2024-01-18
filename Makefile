CXX=g++
CXXFLAGS=-Wall -IInclude -g 

EXECUTABLE=myprogram

SOURCES=main.cpp Modules/person.cpp Modules/secretary.cpp Modules/course.cpp Modules/semester.cpp

OBJECTS=$(SOURCES:.cpp=.o)

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJECTS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(EXECUTABLE) $(OBJECTS)

run: $(EXECUTABLE)
	./$(EXECUTABLE)

valgrind: $(EXECUTABLE)
	valgrind --leak-check=full ./$(EXECUTABLE)
