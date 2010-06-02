#include <boost/castor.h>

#include <boost/test/minimal.hpp>

#include "test_aggregate.h"


using namespace castor;

int test_main(int, char * [])
{
    int nums[] = { 1, 2, 3, 4, 5 };
    lref<std::vector<int> > vi = std::vector<int>(nums, nums + 5);

    { // gen 
        lref<int> i;
        relation r = item(i, vi) >>= average(i);

        BOOST_CHECK(r());
        BOOST_CHECK(*i == 3);
        BOOST_CHECK(!r());
        BOOST_CHECK(!i.defined());
    }

    { // gen - with adder
        integer ints[] = {1, 2, 3, 4, 5};
        lref<std::vector<integer> > integers = std::vector<integer>(ints, ints + 5);
        lref<integer> a;
        relation r = item(a, integers) >>= average(a, integer_add());

        BOOST_CHECK(r());
        BOOST_CHECK(a->i == 3);
    }

    { // gen - with adder & divider
        integer ints[] = {1, 2, 3, 4, 5};
        lref<std::vector<integer> > integers = std::vector<integer>(ints, ints + 5);
        lref<integer> a;
        relation r = item(a, integers) >>= average(a, integer_add());

        BOOST_CHECK(r());
        BOOST_CHECK(a->i == 3);
    }

    { // empty container
        lref<int> s;
        lref<std::vector<int> > v2 = std::vector<int>();
        relation r = item(s, v2) >>= average(s);

        BOOST_CHECK(!r());
        BOOST_CHECK(!s.defined());
    }

    return 0;
}
