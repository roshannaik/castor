#include <boost/castor.h>

#include <boost/test/minimal.hpp>


using namespace castor;

int test_main(int, char * [])
{
    { // multiple values in input
        lref<int> j;
        relation r = range(j, 1, 10) >>= reduce(j, std::multiplies<int>());

        BOOST_CHECK(r() && j.defined());
        BOOST_CHECK(*j == 3628800);
        BOOST_CHECK(!r());
        BOOST_CHECK(!j.defined());
    }

    { // single input
        lref<int> j;
        relation r = range(j, 1, 1) >>= reduce(j, std::multiplies<int>());

        BOOST_CHECK(r() && j.defined());
        BOOST_CHECK(*j == 1);
        BOOST_CHECK(!r());
        BOOST_CHECK(!j.defined());
    }

    { // no inputs
        lref<int> j;
        relation r = range(j, 2, 1) >>= reduce(j, std::plus<int>());

        BOOST_CHECK(!r() || !j.defined());
    }

    return 0;
}
