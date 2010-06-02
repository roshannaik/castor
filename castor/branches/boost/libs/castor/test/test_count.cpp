#include <boost/castor.h>

#include <boost/test/minimal.hpp>


using namespace castor;

int test_main(int, char * [])
{
    { // generate
	int ai[] = { 10, 9, 8, 7, 6, 5, 4, 3, 2, 1 };
	lref<int> j, c;
	relation r = item(j, ai, ai + 10) >>= count(c);

	BOOST_CHECK(r());
	BOOST_CHECK(*c == 10);
	BOOST_CHECK(!r());
	BOOST_CHECK(!c.defined());
    }

    { // generate
        lref<int> j, c;
        relation r = range(j, 1, -1) >>= count(c);

        BOOST_CHECK(r());
        BOOST_CHECK(*c == 0);
        BOOST_CHECK(!r());
        BOOST_CHECK(!c.defined());
    }

    return 0;
}
