#include <boost/castor.h>

#include <boost/test/minimal.hpp>


using namespace castor;

int test_main(int, char * [])
{
    {
        lref<int> c;
        relation r = True(20) >>= count(c);

        BOOST_CHECK(r());
        BOOST_CHECK(*c == 20);
        BOOST_CHECK(!r() && !c.defined());
    }

    {
        lref<int> c;
        relation r = True() >>= count(c);

        BOOST_CHECK(r());
        BOOST_CHECK(*c == 1);
    }

    return 0;
}
