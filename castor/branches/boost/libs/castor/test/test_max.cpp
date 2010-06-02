#include <boost/castor.h>

#include <boost/test/minimal.hpp>


using namespace castor;

int test_main(int, char * [])
{
    { // basic
        lref<int> i;
        relation r = range(i, 1, 100) >>= max(i);

        BOOST_CHECK(r());
        BOOST_CHECK(*i == 100);
        BOOST_CHECK(!r());
        BOOST_CHECK(!i.defined());
    }

    { // with cmp
        lref<int> i;
        relation r = range(i, 1, 100) >>= max(i, std::greater<int>()); // max as min

        BOOST_CHECK(r());
        BOOST_CHECK(*i == 1);
        BOOST_CHECK(!r());
        BOOST_CHECK(!i.defined());
    }

    return 0;
}
