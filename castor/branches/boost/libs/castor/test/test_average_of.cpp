#include <boost/castor.h>

#include <boost/test/minimal.hpp>

#include "test_aggregate.h"


using namespace castor;

int test_main(int, char * [])
{
    int nums[] = { 1, 2, 3, 4, 5 };
    lref<std::vector<int> > vi = std::vector<int>(nums, nums + 5);

    { // gen 
        lref<int> a;
        relation r = average_of(vi, a);

        BOOST_CHECK(r());
        BOOST_CHECK(*a == 3);
        BOOST_CHECK(!r());
        BOOST_CHECK(!a.defined());
    }

    { // gen - with adder
        integer ints[] = {1, 2, 3, 4, 5};
        lref<std::vector<integer> > nums = std::vector<integer>(ints, ints + 5);
        lref<integer> a;
        relation r = average_of(nums, a, integer_add());

        BOOST_CHECK(r());
        BOOST_CHECK(a->i == 3);
    }

    { // test
        relation r = average_of(vi, 3);

        BOOST_CHECK(r());
 
        // failure test
        r = average_of(vi, 10);

        BOOST_CHECK(!r());
    }

    { // empty container
        lref<int> s;
        lref<std::vector<int> > v2 = std::vector<int>();
        relation r = average_of(v2, s);

        BOOST_CHECK(!r());
        BOOST_CHECK(!s.defined());
    }

    { // with group_by
        lref<int> i, a;
        lref<group<bool, int> > g;
        relation r = (range(i, 1, 10) >>= group_by(i, isEven, g)) && average_of(g, a);

        BOOST_CHECK(r());
        BOOST_CHECK(*a == 5);
        BOOST_CHECK(r());
        BOOST_CHECK(*a == 6);
        BOOST_CHECK(!r());
        BOOST_CHECK(!a.defined());
    }
    {// gen - with const container
        lref<const vector<int> > cv = vector<int>(nums,nums+5);
        lref<int> a;
        relation r = average_of(cv,a);
        BOOST_CHECK(r());
        BOOST_CHECK(*a==3);
        BOOST_CHECK(!r());
        BOOST_CHECK(!a.defined());
    }      

    return 0;
}
