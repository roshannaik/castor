#include <boost/castor.h>


using namespace castor;

struct MyBase {
    int j;
    MyBase() : j(0)
    {}
};

struct My : MyBase {
    int i;
    My() : i(0) { }
    My(int) : i(1) { }
    My(int, int) : i(2) { }
    My(int, int, int) : i(3) { }
    My(int, int, int, int) : i(4) { }
    My(int, int, int, int, int) : i(5) { }
    My(int, int, int, int, int, int) : i(6) { }

    operator int()   { return i; }

    typedef int result_type;

    int operator() (void)                          { return 0; }
    int operator() (int)                           { return 1; }
    int operator() (std::string)                   { return -1; }
    int operator() (int, int)                      { return 2; }
    int operator() (int, int, int)                 { return 3; }
    int operator() (int, int, int, int)            { return 4; }
    int operator() (int, int, int, int, int)       { return 5; }
    int operator() (int, int, int, int, int, int)  { return 6; }
};

struct Name {
    std::string firstName, lastName;
    Name(std::string firstName, std::string lastName) : firstName(firstName), lastName(lastName) {}
    bool operator==(const Name& rhs) {
        return firstName == rhs.firstName && lastName == rhs.lastName;
    }
};

void blah(void)                          { }

int foo(void)                            { return 0; }
int foo(int)                             { return 1; }
int foo(int, int)                        { return 2; }
int foo(int, int, int)                   { return 3; }
int foo(int, int, int, int)              { return 4; }
int foo(int, int, int, int, int)         { return 5; }
int foo(int, int, int, int, int, int)    { return 6; }
int foo(std::string)                     { return -1; }

struct Func {
    typedef int result_type ;
    int operator()(int i) const {
        return i;
    }
    int operator()(int i) {
        return i + 1;
    }
};

int bar(lref<int> i) {
    i = 4;
    return 2;
}


struct BaseObj {
    int bmethod(int i) { return i; }
};

struct Obj : BaseObj  {
    typedef int result_type;

    int method0 (void)                         { return  0; }

    int method1 (int)                          { return  1; }

    int method (std::string)                   { return -1; }
    int method (int, int)                      { return  2; }
    int method (int, int, int)                 { return  3; }
    int method (int, int, int)    const        { return -3; }
    int method (int, int, int, int)            { return  4; }
    int method (int, int, int, int, int)       { return  5; }
    int method (int, int, int, int, int, int)  { return  6; }

    int mmethod (lref<int> i)                  { i = 9; return  2; }
};
