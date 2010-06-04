#include <boost/castor.h>

#include <boost/test/minimal.hpp>


using namespace castor;

int test_main(int, char * [])
{
    { // generate
        std::vector<int> items; items.push_back(1); items.push_back(9); items.push_back(2); items.push_back(3);
        lref<std::vector<int> > v = items;
        lref<std::vector<int> > t;
#if  defined(__BCPLUSPLUS__) || defined(__GNUG__)
        relation r1 = tail_n<std::vector<int>, std::vector<int> >(v, 2, t);
#else
        relation r1 = tail_n(v, 2, t);
#endif

        int i = 0;
        for (;r1(); ++i) {
            std::vector<int> expected; expected.push_back(2); expected.push_back(3);
    
            BOOST_CHECK(*t == expected);
        }

        BOOST_CHECK(i == 1 && !t.defined());
    }

    { // compare
        std::vector<int> items; items.push_back(1); items.push_back(2); items.push_back(3);
        lref<std::vector<int> > v = items;
        lref<std::vector<int> > t = std::vector<int>(); (*t).push_back(2); (*t).push_back(3);
#if  defined(__BCPLUSPLUS__) || defined(__GNUG__)
	relation r = tail_n<std::vector<int>, std::vector<int> >(v, 2, t);
#else
	relation r = tail_n(v, 2, t);
#endif
	int i = 0;
	for (;r(); ++i);

	BOOST_CHECK(i == 1);
    }

    { // tail of length 0 of a list is an empty list
        lref<std::vector<int> > t, v = std::vector<int>();
#if  defined(__BCPLUSPLUS__) || defined(__GNUG__)
        relation r = tail_n<std::vector<int>, std::vector<int> >(v, 0, t);
#else
        relation r = tail_n(v, 0, t);
#endif

        BOOST_CHECK(r());
        BOOST_CHECK(t.defined());
    }

    return 0;
}
