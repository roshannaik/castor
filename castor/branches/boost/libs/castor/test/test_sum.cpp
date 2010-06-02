#include <boost/castor.h>

#include <boost/test/minimal.hpp>


using namespace castor;

int test_main(int, char * [])
{
    {
        lref<int> j;
        relation r = range(j, 1, 10) >>= sum(j);

        BOOST_CHECK(r() && j.defined());
        BOOST_CHECK(*j == 55);
        BOOST_CHECK(!r());
        BOOST_CHECK(!j.defined());
    }

    {
        lref<int> j;
        relation r = range(j, 1, 1) >>= sum(j);

        BOOST_CHECK(r() && j.defined());
        BOOST_CHECK(*j == 1);
        BOOST_CHECK(!r());
        BOOST_CHECK(!j.defined());
    }

    {
        lref<int> j;
        relation r = range(j, 2, 1) >>= sum(j);

        BOOST_CHECK(!r() || !j.defined());
    }

    return 0;
}
