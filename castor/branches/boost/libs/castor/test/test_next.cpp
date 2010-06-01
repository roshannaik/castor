#include <boost/castor.h>

#include <boost/test/minimal.hpp>


using namespace castor;

int test_main(int, char * [])
{
    { // should unify successfully
        int ai[] = {1, 2, 3};
        lref<int*> n;
        relation r = next(ai + 0, n);
	int i = 0;

	for (; r(); ++i) {
            BOOST_CHECK(i  <= 0 && *(*n) == 2);
	}

	BOOST_CHECK(i == 1 && !n.defined());
    }

    {
	// should compare successfully
	int aj[] = {1, 2, 3};
	lref<int*> n = aj + 1;
	relation r2 = next(aj + 0, n);
	int j = 0;

	for (; r2(); ++j) {
            BOOST_CHECK(j  <= 0 && *(*n) == 2);
	}

	BOOST_CHECK(j == 1 && n.defined());
    }

    {
	// should not compare succesfully
	int ak[] = {1, 2, 3};
	lref<int*> n = ak + 1;
	relation r3 = next(n, n);
        int k = 0;
        for (; r3(); ++k);

        BOOST_CHECK(k == 0 && n.defined());
    }

    { // on simple values
        lref<int> n;
        next(1, n)();

        BOOST_CHECK(*n == 2);
    }

    { // on simple values 2
        BOOST_CHECK(next(1, 2)());
    }

    return 0;
}
