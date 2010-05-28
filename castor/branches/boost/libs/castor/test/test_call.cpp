#include <boost/castor.h>

#include <boost/test/minimal.hpp>

#include <string>

#include "test_ile.h"


using namespace castor;


int test_main(int, char * [])
{
    { // regular functions
        call(blah)();

	BOOST_CHECK(call(foo)() == 0);

#ifdef __BCPLUSPLUS__
	int(*p)(int) = &foo;
	BOOST_CHECK(call(p, 1)() == 1);
#else
        BOOST_CHECK(call(static_cast<int(*)(int)> (foo), 1)() == 1);
#endif

#ifdef __BCPLUSPLUS__
	int(*p2)(std::string) = &foo;
	BOOST_CHECK(call(p2, std::string("castor"))() == -1);
#else
        BOOST_CHECK(call(static_cast<int(*)(std::string)> (foo), std::string("castor"))() == -1);
#endif

	BOOST_CHECK(call(foo, 1, 2)() == 2);
        BOOST_CHECK(call(foo, 1, 2, 3)() == 3);
        BOOST_CHECK(call(foo, 1, 2, 3, 4)() == 4);
        BOOST_CHECK(call(foo, 1, 2, 3, 4, 5)() == 5);
        BOOST_CHECK(call(foo, 1, 2, 3, 4, 5, 6)() == 6);
    }

    { // function objects
        BOOST_CHECK(call(My())() == 0);
        BOOST_CHECK(call(My(), 1)() == 1);
        BOOST_CHECK(call(My(), std::string(""))() == -1);
        BOOST_CHECK(call(My(), 1, 2)() == 2);
        BOOST_CHECK(call(My(), 1, 2, 3)() == 3);
        BOOST_CHECK(call(My(), 1, 2, 3, 4)() == 4);
        BOOST_CHECK(call(My(), 1, 2, 3, 4, 5)() == 5);
        BOOST_CHECK(eq(6, call(My(), 1, 2, 3, 4, 5, 6)())());
    }

    { // invoking const and non const operator() 
	const Func cf = Func();

	BOOST_CHECK(call<const Func>(cf, 4)() == 4);
	BOOST_CHECK(call<Func>(cf, 4)() == 5);
    }

    {
	lref<int> i = 0;

	BOOST_CHECK(call(bar, i)() == 2);
    }

    return 0;
}
