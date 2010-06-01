#include <boost/castor.h>

#include <boost/test/minimal.hpp>


using namespace castor;

int test_main(int, char * [])
{
    lref<int> x;
    relation r = undefined(x);
    int i = 0;
    for (; r(); ++i);

    BOOST_CHECK(i == 1);

    lref<int> y = 2;
    relation r2 = undefined(y);
    for (i = 0; r2(); ++i);

    BOOST_CHECK(i == 0);

    return 0;
}
