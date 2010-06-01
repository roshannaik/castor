#include <boost/castor.h>

#include <boost/test/minimal.hpp>


using namespace castor;

int test_main(int, char * [])
{
    { // generate
        lref<int> li;
        int expected = 0;
        relation r = range<int>(li, 0, 7);

        while (r()) {
            BOOST_CHECK(*li == expected++);
        }

        BOOST_CHECK(!li.defined());
    }

    { // test within range
        lref<int> i = 2;
        relation r = range<int>(i, 0, 7);

        BOOST_CHECK(r());
        BOOST_CHECK(i.defined());
        BOOST_CHECK(!r());
    }

    { // test out of range
        lref<int> i = 9, min = 0, max = 7;
        relation r = range(i, min, max);

        BOOST_CHECK(!r());
        BOOST_CHECK(i.defined());
        BOOST_CHECK(!r());
    }

    { // empty range
        lref<int> i;
        relation r = range(i, 10, 2, -1);

        BOOST_CHECK(!r());
    }

    return 0;
}
