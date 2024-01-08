CXX=g++
CXXFLAGS=-Wall -IInclude

EXECUTABLE=myprogram

SOURCES=main.cpp Modules/person.cpp Modules/secretary.cpp Modules/course.cpp Modules/semester.cpp

OBJECTS=$(SOURCES:.cpp=.o)

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJECTS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

SourceFiles/%.o: SourceFiles/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

main.o: main.cpp
	$(CXX) $(CXXFLAGS) -c main.cpp -o main.o

clean:
	rm -f $(EXECUTABLE) $(OBJECTS)

run: $(EXECUTABLE)
	./$(EXECUTABLE)
