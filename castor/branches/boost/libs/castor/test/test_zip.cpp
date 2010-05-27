#include <boost/castor.h>

#include <boost/test/minimal.hpp>


using namespace castor;

int test_main(int, char * [])
{
    {
        lref<int> i, j;
        int s = 0;
        int ai[] = { 1, 2, 3, 4 }, aj[] = { 1, 1, 1, 1, 1 };

        for (relation r = zip(item(i, ai, ai + 4), item(j, aj, aj + 5) ); r(); ++s) {
            BOOST_CHECK(*i == s || *j == 1);
        }
        BOOST_CHECK(s == 4);
    }

    return 0;
}
