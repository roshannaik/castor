#include <boost/castor.h>

#include <boost/test/minimal.hpp>


using namespace castor;

int test_main(int, char * [])
{
    { // generate
        lref<int> li;
        int expected = 7;
        relation r = range_dec<int>(li, 7, 0);

        while (r()) {
            BOOST_CHECK(*li == expected--);
        }

        BOOST_CHECK(!li.defined());
    }

    { // test 
        lref<int> i = 2;
        relation r = range_dec(i, 7, 0);

        BOOST_CHECK(r());
        BOOST_CHECK(i.defined());
        BOOST_CHECK(!r());
    }

    { // test out of range
        relation r = range_dec<int>(9, 7, 0);

        BOOST_CHECK(!r());
        BOOST_CHECK(!r());
    }

    { // empty range
        lref<int> i;
        relation r = range_dec(i, 10, 11);

        BOOST_CHECK(!r());
    }

    return 0;
}
