#include <boost/castor.h>

#include <boost/test/minimal.hpp>

#include "test_utils.h"


using namespace castor;

int test_main(int, char * [])
{
    {
        lref<int> i1 = 1;
        Functor6 f;

        eval(f)();

        BOOST_CHECK(f.result == 0);

        eval(f, 1)();

        BOOST_CHECK(f.result == 1);

	eval(f, i1, 2)();

	BOOST_CHECK(f.result == 3);

	eval(f, 1, 2, 3)();

	BOOST_CHECK(f.result == 6);

	eval(f, 1, 2, 3, 4)();

	BOOST_CHECK(f.result == 10);

	eval(f, i1, 2, 3, 4, 5)();

	BOOST_CHECK(f.result == 15);

	eval(f, 1, 2, 3, 4, 5, 6)();

	BOOST_CHECK(f.result == 21);
    }

    { // 2
	lref<int> i1 = 1;
	eval(getN)();

	BOOST_CHECK(getNIntResult == 0);

#ifdef __BCPLUSPLUS__
	eval<int, int, int>(getN, 1)();
#else
	eval<int, int>(getN, 1)();
#endif

	BOOST_CHECK(getNIntResult == 1);


#ifdef __BCPLUSPLUS__
	eval<int, int, int, lref<int>, int>(getN, i1, 2)();
#else
	eval<int, int, int>(getN, i1, 2)();
#endif

	BOOST_CHECK(getNIntResult == 2);

#ifdef __BCPLUSPLUS__
	eval<int, int, int, int, int, int, int>(getN, 1, 2, 3)();
#else
	eval<int, int, int, int>(getN, 1, 2, 3)();
#endif

	BOOST_CHECK(getNIntResult == 3);

#ifdef __BCPLUSPLUS__
	eval<int, int, int, int, int, int, int, int, int>(getN, 1, 2, 3, 4)();
#else
	eval<int, int, int, int>(getN, 1, 2, 3, 4)();
#endif

	BOOST_CHECK(getNIntResult == 4);

#ifdef __BCPLUSPLUS__
	eval<int, int, int, int, int, int, lref<int>, int, int, int, int>(getN, i1, 2, 3, 4, 5)();
#else
	eval<int, int, int, int, int, int>(getN, i1, 2, 3, 4, 5)();
#endif

	BOOST_CHECK(getNIntResult == 5);

#ifdef __BCPLUSPLUS__
	eval<int, int, int, int, int, int, int, int, int, int, int, int, int>(getN, 1, 2, 3, 4, 5, 6)();
#else
	eval<int, int, int, int, int, int, int>(getN, 1, 2, 3, 4, 5, 6)();
#endif

	BOOST_CHECK(getNIntResult == 6);
    }

    { // 3
	lref<int> i1 = 1;
#ifdef __BCPLUSPLUS__
	eval<double, double, int>(getN, 1)();
#else
	eval<double, double>(getN, 1)();
#endif

	BOOST_CHECK(getNDblResult == -1);

#ifdef __BCPLUSPLUS__
	eval<double, double, double, lref<int>, int>(getN, i1, 2)();
#else
	eval<double, double, double>(getN, i1, 2)();
#endif

	BOOST_CHECK(getNDblResult == -2);

#ifdef __BCPLUSPLUS__
	eval<double, double, double, double, int, int, int>(getN, 1, 2, 3)();
#else
	eval<double, double, double, double>(getN, 1, 2, 3)();
#endif

	BOOST_CHECK(getNDblResult == -3);

#ifdef __BCPLUSPLUS__
	eval<double, double, double, double, double, int, int, int, int>(getN, 1, 2, 3, 4)();
#else
	eval<double, double, double, double, double>(getN, 1, 2, 3, 4)();
#endif

	BOOST_CHECK(getNDblResult == -4);

#ifdef __BCPLUSPLUS__
	eval<double, double, double, double, double, double, lref<int>, int, int, int, int>(getN, i1, 2, 3, 4, 5)();
#else
	eval<double, double, double, double, double, double>(getN, i1, 2, 3, 4, 5)();
#endif

	BOOST_CHECK(getNDblResult == -5);

#ifdef __BCPLUSPLUS__
	eval<double, double, double, double, double, double, double, int, int, int, int, int, int>(getN, 1, 2, 3, 4, 5, 6)();
#else
	eval<double, double, double, double, double, double, double>(getN, 1, 2, 3, 4, 5, 6)();
#endif

	BOOST_CHECK(getNDblResult == -6);
    }

    {
        lref<int> i1 = 1;
        eval(getN2, 1)();

        BOOST_CHECK(getN2Result == -1);

        eval(getN2, i1, 2)();

        BOOST_CHECK(getN2Result == -2);

        eval(getN2, 1, 2, 3)();

        BOOST_CHECK(getN2Result == -3);

        eval(getN2, 1, 2, 3, 4)();

        BOOST_CHECK(getN2Result == -4);

        eval(getN2, i1, 2, 3, 4, 5)();

        BOOST_CHECK(getN2Result == -5);

        eval(getN2, 1, 2, 3, 4, 5, 6)();

        BOOST_CHECK(getN2Result == -6);
    }

    return 0;
}
