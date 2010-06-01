#include <boost/castor.h>

#include <boost/test/minimal.hpp>


using namespace castor;

int test_main(int, char * [])
{
    { // 1 - success
        lref<int> j;
        int s = 0, times = 3;
        relation r = repeat(1, times, j);
        while (r() && *j == 1)
            ++s;

        BOOST_CHECK(s == times);
    }

    { // 2 - failure
        lref<int> j;
        int s = 0, times = 3;
        for (relation r = repeat(1, times, j); r() && *j == 2; ++s)
            ;

        BOOST_CHECK(s == 0);
    }

    { // 3 - (lref) - success
        lref<int> j, one = 1;
        int s = 0, times = 3;
        for (relation r = repeat(one, times, j); r() && *j == 2; ++s)
            ;

        BOOST_CHECK(s == 0);
    }

    return 0;
}
