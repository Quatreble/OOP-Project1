CXX=g++
CXXFLAGS=-Wall

# The name of your executable
EXECUTABLE=myprogram

# List your source files here
SOURCES=main.cpp person.cpp secretary.cpp

# This will generate a list of object files
OBJECTS=$(SOURCES:.cpp=.o)

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJECTS)

# This rule tells make how to build .o files from .cpp files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# This rule tells make to clean up everything that can be regenerated
clean:
	rm -f $(EXECUTABLE) $(OBJECTS)
