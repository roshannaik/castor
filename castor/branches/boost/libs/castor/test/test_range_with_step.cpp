#include <boost/castor.h>

#include <boost/test/minimal.hpp>


using namespace castor;

int test_main(int, char * [])
{
    { // generate
        lref<int> li;
	int expected = 0, step = 2;
	relation r = range<int>(li, 0, 7, step);

	while (r()) {
            BOOST_CHECK(*li == expected);

            expected += step;
	}

	BOOST_CHECK(!li.defined());
    }

    { // test within range
	lref<int> i = 2;
	int step = 8;
	relation r = range<int>(i, 0, 7, step);

	BOOST_CHECK(r());
	BOOST_CHECK(i.defined());
	BOOST_CHECK(!r());
    }

    { // test out of range
	lref<int> i = 9, min = 0, max = 7, step = 3;
        relation r = range(i, min, max, step);

        BOOST_CHECK(!r());
        BOOST_CHECK(i.defined());
        BOOST_CHECK(!r());
    }

    { // empty range
        relation r = range<int>(7, 10, 2, -1);

        BOOST_CHECK(!r());
    }

    return 0;
}
