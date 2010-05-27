#include <boost/castor.h>

#include <boost/test/minimal.hpp>


using namespace castor;

class myFastAndTestRel : public TestOnlyRelation<myFastAndTestRel> {
    lref<int> i;
public:
    myFastAndTestRel(lref<int> i) : i(i)
        {}

    myFastAndTestRel(const myFastAndTestRel& rhs) : i(rhs.i) {
#ifndef __BCPLUSPLUS__
	static int times = 0;
	BOOST_CHECK(times++ <= 1);
#endif
    }

    bool operator () (void) {
	co_begin();
	co_yield(*i < 20);
	co_end();
    }
};


int test_main(int, char * [])
{
    {
	lref<int> i;
	relation r = range(i, 18, 22) && myFastAndTestRel(i);
	while (r());
    }

    return 0;
}
