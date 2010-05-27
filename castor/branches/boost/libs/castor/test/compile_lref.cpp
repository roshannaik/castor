#include <boost/castor.h>

#include <string>

#include <boost/test/minimal.hpp>


using namespace castor;

struct Base {
    virtual ~Base() {}
};

struct Derived : public Base {
    virtual ~Derived() {}
};

struct A {
    static int i;

    A() { ++i; }
    A(const A&) { ++i; }
    ~A() { --i; }
};

int A::i = 0;

int test_main(int, char * [])
{
    {
	lref<std::string> r2 = "123";
	lref<std::string> rs; lref<const char*> rc = "world";
	rs = "hello";
	rs = rc;
    }

    { // Derived type assignment
        lref<Base> b(new Base, true);
        b.set_ptr(new Derived, true);
    }

    return 0;
}
