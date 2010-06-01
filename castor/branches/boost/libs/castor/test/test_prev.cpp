#include <boost/castor.h>

#include <boost/test/minimal.hpp>


using namespace castor;

int test_main(int, char * [])
{
    { // generate
        int ai[] = {1, 2, 3};
        lref<int*> p;
        relation r = prev(ai + 1, p);
        int i = 0;
        for (; r(); ++i) {
            BOOST_CHECK(i  <= 0 && *(*p) == 1);
        }

        BOOST_CHECK(i == 1 && !p.defined());
    }

    { // test  : aj + 2 is next of aj + 1
        int aj[] = {1, 2, 3};
        lref<int*> pj = aj + 1;
        relation r2 = prev(aj + 2, pj);
        int j = 0;

        for (; r2(); ++j) {
            BOOST_CHECK(j  <= 0 && *(*pj) == 2);
        }

        BOOST_CHECK(j == 1 && pj.defined());
    }

    { // test - failing
        int ak[] = {1, 2, 3};
        lref<int*> pk = ak + 1;
        relation r3 = prev(ak + 1, pk);
        int k = 0;
        for (; r3(); ++k);

        BOOST_CHECK(k == 0 && pk.defined());
    }

    return 0;
}
