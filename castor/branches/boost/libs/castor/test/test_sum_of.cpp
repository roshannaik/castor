#include <boost/castor.h>

#include <boost/test/minimal.hpp>

#include "test_aggregate.h"


using namespace castor;

int test_main(int, char * [])
{
    std::string arr[] = { "Prolog", "Leda", "castor", "C#" };
    lref<std::vector<std::string> > vs = std::vector<std::string>(arr, arr + 4);
    int nums[] = { 1, 2, 3, 4, 5 };
    lref<std::vector<int> > vi = std::vector<int>(nums, nums + 5);

    { // basic - gen - with strings
        lref<std::string> s;
        relation r = sum_of(vs, s);

        BOOST_CHECK(r());
        BOOST_CHECK(*s == "PrologLedacastorC#");
        BOOST_CHECK(!r());
        BOOST_CHECK(!s.defined());
    }

    { // basic - gen - with ints
        lref<int> s;
        relation r = sum_of(vi, s);

        BOOST_CHECK(r());
        BOOST_CHECK(*s == 15);
        BOOST_CHECK(!r());
        BOOST_CHECK(!s.defined());
    }

    { // basic - test
        // TODO: this should accept the int literal 15 as its second argument,
        // but doesn't for GCC 4.4.3
        relation r = sum_of(vi, lref<int>(15));

        BOOST_CHECK(r());
 
        // failure test
        // TODO: this should accept the int literal 10 as its second argument,
        // but doesn't for GCC 4.4.3
        r = sum_of(vi, lref<int>(10));

        BOOST_CHECK(!r());
    }

    { // basic - empty container
        lref<int> s;
        lref<std::vector<int> > v2 = std::vector<int>();
        relation r = min_of(v2, s);

        BOOST_CHECK(!r());
        BOOST_CHECK(!s.defined());
    }

    { // with group_by
        lref<int> i, s;
        lref<group<bool, int> > g;
        relation r = (range(i, 1, 10) >>= group_by(i, isEven, g)) && sum_of(g, s);

        BOOST_CHECK(r());
        BOOST_CHECK(*s == 25);
        BOOST_CHECK(r());
        BOOST_CHECK(*s == 30);
        BOOST_CHECK(!r());
        BOOST_CHECK(!s.defined());
    }

    return 0;
}
