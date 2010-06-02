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

    { // test - with adder
        integer ints[] = {1, 2, 3, 4};
        lref<std::vector<integer> > nums = std::vector<integer>(ints, ints + 4);
        lref<integer> s;
        relation r = reduce_of(nums, s, integer_add());

        BOOST_CHECK(r());
        BOOST_CHECK(s->i == 10);
    }

    { // gen - with multiplier
        integer ints[] = {1, 2, 3, 4};
        lref<std::vector<integer> > nums = std::vector<integer>(ints, ints + 4);
        lref<integer> s = integer(24);
        relation r = reduce_of(nums, s, integer_mult());

        BOOST_CHECK(r());
        BOOST_CHECK(s->i == 24);
    }

    return 0;
}
