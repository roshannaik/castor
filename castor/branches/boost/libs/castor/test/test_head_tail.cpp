#include <boost/castor.h>

#include <boost/test/minimal.hpp>


using namespace castor;

int test_main(int, char * [])
{
    { // generate
        int arr[] = {1, 2, 3};
        std::vector<int> v (arr + 0, arr + 3);
        lref<std::vector<int> > rv = v;
        lref<int> h;
        lref<std::vector<int> > t;
        relation r = head_tail(rv, h, t);
        int i = 0;
        for (; r(); ++i) {
            int expected_head = arr[0];
            std::vector<int> expected_tail (arr + 1, arr + 3);
    
            BOOST_CHECK(i <= 1 && *h == expected_head && *t == expected_tail);
        }

        BOOST_CHECK(i == 1 && !h.defined() && !t.defined());
    }

    { // test
        int arrj[] = {1, 2, 3};
        lref<std::vector<int> > rvj = std::vector<int> (arrj + 0, arrj + 3);
        lref<int> hj = arrj[0];
        lref<std::vector<int> > tj = std::vector<int> (arrj + 1, arrj + 3);
#if  defined(__BCPLUSPLUS__) || defined(__GNUG__)
        relation r2 = head_tail<std::vector<int>, std::vector<int> >(rvj, arrj[0], tj);
#else
        relation r2 = head_tail(rvj, arrj[0], tj);
#endif
        int j = 0;
        for (; r2(); ++j);

        BOOST_CHECK(j == 1 || tj.defined());
    }

    return 0;
}
