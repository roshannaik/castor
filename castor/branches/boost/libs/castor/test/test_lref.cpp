#include <boost/castor.h>

#include <string>

#include <boost/test/minimal.hpp>


using namespace castor;

//--------------------------------------------------------
//  Tests  : basic lref test for compilation errors
//--------------------------------------------------------

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
        // these only check for compiler errors
	lref<std::string> rs; lref<const char*> rc = "world";
	rs = "hello";

	BOOST_CHECK(*rs == "hello");

	rs = rc;

	BOOST_CHECK(*rs == "world");
    }

    {
        lref<Base> b(new Base, true);
        b.set_ptr(new Derived, true);
    }

    //  -- pointer stuff --
    { // basic initialization
        {
            lref<A> r(new A, true);

            BOOST_CHECK(A::i == 1);
        }

        BOOST_CHECK(A::i == 0);
		
        A* pa = new A;
        {
            lref<A> r(pa, false);

            BOOST_CHECK(A::i == 1);
        }

        BOOST_CHECK(A::i == 1);

        delete pa;
    }

    { // basic assignment
        A::i = 0;

        {
            lref<A> r; r.set_ptr(new A, true);

            BOOST_CHECK(A::i == 1);
        }

        BOOST_CHECK(A::i == 0);

        {
            lref<A> r; r.set_ptr(new A, false);

            BOOST_CHECK(A::i == 1);
        }

        BOOST_CHECK(A::i == 1);
    }

    { // init and assignment
	A::i = 0;

	lref<A> r(new A, true);
	r.set_ptr(new A, true);

	BOOST_CHECK(A::i == 1);
    }

    { // unmanaged to managed transition
		A::i=0;
        A a;
        lref<A> r(&a, false);
        r=a;
		BOOST_CHECK(A::i==2);
    }
    { // managed to unmanaged transition
		A::i=0;
        lref<A> r(new A, true);
        A a;
        r.set_ptr(&a,false);
		BOOST_CHECK(A::i==1);
    }
    return 0;
}
