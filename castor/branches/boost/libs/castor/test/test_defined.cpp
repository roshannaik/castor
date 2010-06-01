#include <boost/castor.h>

#include <boost/test/minimal.hpp>


using namespace castor;

int test_main(int, char * [])
{
    {
        lref<int> li;

        BOOST_CHECK(!defined(li)());

        li = 2;

        BOOST_CHECK(defined(li)());
    }

    return 0;
}
