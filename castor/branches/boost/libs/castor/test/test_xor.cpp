#include <boost/castor.h>

#include <boost/test/minimal.hpp>


using namespace castor;

int test_main(int, char * [])
{
    {
        lref<int> i;
        relation r = (range(i, 1, 3) ^ range(i, 5, 7) ^ range(i, 9, 10));

        BOOST_CHECK(r());
        BOOST_CHECK(*i == 1);

        BOOST_CHECK(r());
        BOOST_CHECK(*i == 2);

        BOOST_CHECK(r());
        BOOST_CHECK(*i == 3);

        BOOST_CHECK(!r());
        BOOST_CHECK(!i.defined());
    }

    {
        lref<int> i;
        relation r = (range(i, 1, 0) ^ range(i, 5, 7) ^ range(i, 9, 10));

        BOOST_CHECK(r());
        BOOST_CHECK(*i == 5);

        BOOST_CHECK(r());
        BOOST_CHECK(*i == 6);

        BOOST_CHECK(r());
        BOOST_CHECK(*i == 7);

        BOOST_CHECK(!r());
        BOOST_CHECK(!i.defined());
    }

    return 0;
}
