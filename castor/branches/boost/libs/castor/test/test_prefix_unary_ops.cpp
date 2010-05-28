#include <boost/castor.h>

#include <boost/test/minimal.hpp>

#include "test_ile.h"


using namespace castor;

int test_main(int, char * [])
{
    { // Simple Prefix increment/decrement
        lref<int> li = 1;

        BOOST_CHECK((++li)() == 2);
        BOOST_CHECK(*li == 2);

        BOOST_CHECK((--li)() == 1);
        BOOST_CHECK(*li == 1);
    }

    { // Compound Prefix increment expression
        lref<int> li = 2;

        BOOST_CHECK((++(++(++li)) * 2 + 5)() == 15);
        BOOST_CHECK(*li == 5);
    }

    { // Compound Prefix decrement expression
        lref<int> li = 5;

        BOOST_CHECK((--(--(--li)) * 2)() == 4);
        BOOST_CHECK(*li == 2);
    }

    return 0;
}
