# OOP-Project1

Team members:
|  |  |
|------          |----      |
|Elpida Stergiou |sdi2200173|
|Ioannis Tampouris|sdi2200181|
|  |  |

The implementation was done in Visual Studio Code using g++. A makefile is included to build and link the file, to do so:
> make run //todo...


## Notes on requirements

We felt that there could have been more than one ways to meet some of the requirements, here is a list of the decisions we took:

- **A1.4**: concerning counting number of `Person`s: the `static int pCount` of class `Person` is incremented by one each time a new `Person` is instantiated and allocated on the stack, outside of the `Secretary` struct. 
- **A2.2**: concerning overloading the `+` operator for the `Secretary` object to add a `Person` with dynamic memory allocation: all `Person`s added to the `Secretary` are created/copied dynamically on the heap; the `+` operator just calls the existing `addPerson()` method which does exactly this.
- **A2.3**: concerning the overloading of io operators for `Secretary`: 
  - the `<<` operator is straight-forward since it just prints the fields of the object
  - the `>>` operator could be implemented in different ways, we chose to allow the user to:
    - first give the name of the `Secretary`
    - then add as many `Person`s as desired, for each one providing the `idCode`, `firstName`, and `lastName`.
- **A2.4**: concerning finding a `Person`: we chose to implement different methods based on the search *property*, we added methods for searching by `idCode`, `firstName`, `lastName`, `Person&`.

# Code implementation

There are comments in the code in the places where it is needed. The follwing sections provide general insights.


## class `Person`
Base class for people that are handled by the `Secretary` class. It is currently being extended by `Student` and `Faculty` for demonstration purposes.

The `Person` class is abstract, cannot be instantiated because of the pure virtual method `clone()`: we want the sub-classes to implement this method and dynamically clone themselves.

### class `Student` and `Faculty`
 These two sub-classes add no properties or methods at the moment. There are two important virtual methods that are overrided and are used from the `Secretary` class:
 * `equals` : provide object level equality 
 * `clone` : dynamically create a copy of itself

 ## class `Secretary`
 This class acts as the *repository* of `Person` instances. We use a vector to store instances of `Person` (an alternative would be to use a `map` but since we provide several `findXXX` methods it makes no sense to *index* with a single key; moreover we don't expect to have multiple instances since this is a demo, so linear search is quick for our purposes).

 Memory management of `Person` instances is performed by this class. 

Several operators have been overloaded according to requirements.

## `main()` method
The `main()` method calls all of the implemented methods and acts as a self-contained test. It showcases all required functionality.