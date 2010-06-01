#include <boost/castor.h>

#include <boost/test/minimal.hpp>


using namespace castor;

int test_main(int, char * [])
{
    { // generate
        lref<int> li;
	int expected = 7, step = 2;
	relation r = range_dec(li, 7, 0, step);

	while (r()) {
            BOOST_CHECK(*li == expected);

            expected -= step;
	}

	BOOST_CHECK(!li.defined());
    }

    { // test  step>max
	relation r = range_dec<int>(2, 7, 0, 8);

	BOOST_CHECK(r());
    }

    { // gen  step>max
        lref<int> i;
	relation r = range_dec<int>(i, 7, 0, 8);

	BOOST_CHECK(r());
	BOOST_CHECK(!r());
    }

    { // test out of range
	lref<int> i = 9, min = 0, max = 7, step = 3;
        relation r = range(i, max, min, step);

        BOOST_CHECK(!r());
        BOOST_CHECK(i.defined());
        BOOST_CHECK(!r());
    }

    { // empty range
        relation r = range<int>(7, 10, 12);

        BOOST_CHECK(!r());
    }

    return 0;
}
