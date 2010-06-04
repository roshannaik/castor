#include <boost/castor.h>

#include <boost/test/minimal.hpp>


using namespace castor;

int test_main(int, char * [])
{
    { // generate
        std::vector<int> items; items.push_back(1); items.push_back(2); items.push_back(3);
        lref<std::vector<int> > v = items;
        lref<std::vector<int> > h;
#if defined(__BCPLUSPLUS__) || defined(__GNUG__)
        relation r1 = head_n<std::vector<int>, std::vector<int> > (v, 2, h);
#else
        relation r1 = head_n(v, 2, h);
#endif
        int i = 0;
        for (;r1(); ++i) {
            std::vector<int> expected; expected.push_back(1); expected.push_back(2);

            BOOST_CHECK(*h == expected);
        }

        BOOST_CHECK(i == 1 && !h.defined());
    }

    { // compare
        std::vector<int> items; items.push_back(1); items.push_back(2); items.push_back(3);
        lref<std::vector<int> > v = items;
        lref<std::vector<int> > h = std::vector<int>(); (*h).push_back(1); (*h).push_back(2);
#if  defined(__BCPLUSPLUS__) || defined(__GNUG__)
        relation r = head_n<std::vector<int>, std::vector<int> >(v, 2, h);
#else
        relation r = head_n(v, 2, h);
#endif
        int i = 0;
        for (;r(); ++i);

        BOOST_CHECK(i == 1);
    }

    return 0;
}
