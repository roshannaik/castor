#include <boost/castor.h>

#include <boost/test/minimal.hpp>

#include "test_ile.h"


using namespace castor;

int test_main(int, char * [])
{
    { // Simple Postfix increment/decrement
        lref<int> li = 1;

        BOOST_CHECK((li++)() == 1);
        BOOST_CHECK(*li == 2);

        BOOST_CHECK((li--)() == 2);
        BOOST_CHECK(*li == 1);
    }


    { // Compound Postfix increment expression
        lref<int> li = 2;

        BOOST_CHECK(((li++) * 2 + 5)() == 9);
        BOOST_CHECK(*li == 3);
    }

    { // Compound Postfix decrement expression
        lref<int> li = 5;

        BOOST_CHECK(((li--) * 2)() == 10);
        BOOST_CHECK(*li == 4);
    }

    { // Mix of Postfix and Prefix inc/decrement expressions
        lref<int> li = 2;
        ((--li) * (li++))(); // result of this evaluation is an undefined value

        BOOST_CHECK(*li == 2); // li's value is however defined
    }

    return 0;
}
