#define CASTOR_ENABLE_DEPRECATED // TODO: Remove the need for this (it's due to reliance on deprecated size<>()).

#include <boost/castor.h>

#include <boost/test/minimal.hpp>


using namespace castor;

int test_main(int, char * [])
{
    { // test
        int arr[] = {1, 2, 3};
        lref<std::vector<int> > lv = std::vector<int>(arr + 0, arr + 3);
        lref<std::vector<int> > h, t;
#if  defined(__BCPLUSPLUS__) || defined(__GNUG__)
        relation r = head_n_tail<std::vector<int>, std::vector<int> >(lv, 1, h, t);
#else
        relation r = head_n_tail(lv, 1, h, t);
#endif
        int i = 0;
        for (; r(); ++i) {
            BOOST_CHECK(h->size() == 1 && (*h)[0] == 1 && t->size() == 2 && (*t)[0] == 2 && (*t)[1] == 3);
        }

        BOOST_CHECK(i == 1);
    }

    { // generate heads and tails
        int arr[] = {1, 2, 3};
        lref<std::vector<int> > lv = std::vector<int>(arr + 0, arr + 3);
        lref<std::vector<int> > h, t;
        lref<std::vector<int>::size_type> n;
        relation r = head_n_tail(lv, n, h, t);
        int i = 0;
        for (; r(); ++i);

        BOOST_CHECK(i == 4);
    }

    return 0;
}
