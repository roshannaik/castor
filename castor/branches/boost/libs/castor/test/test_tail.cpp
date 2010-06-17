#include <boost/castor.h>

#include <boost/test/test_exec_monitor.hpp>
#include <boost/test/floating_point_comparison.hpp>


using namespace castor;

int test_main(int, char * [])
{
    { // generate
        int arr[] = { 3, 4, 5 };
        lref<std::vector<int> > rv = std::vector<int>(arr + 0, arr + 3);
        lref<std::vector<int> > tl;
        relation r = tail(rv, tl);
        int i = 0;
        for (; r(); ++i) {
            std::vector<int> expected (arr + 1, arr + 3);
    
            BOOST_CHECK(*tl == expected);
        }

        BOOST_CHECK(i == 1 && !tl.defined());
    }

    { // test
        int arrj[] = { 1, 2, 3 };
        lref<std::vector<int> > rvj = std::vector<int>(arrj + 0, arrj + 3);
        lref<std::vector<int> > tlj = std::vector<int>(arrj + 1, arrj + 3);
        relation r2 = tail(rvj, tlj);
        int j = 0;
        for (; r2(); ++j);

        BOOST_CHECK(j == 1 && tlj.defined());
    }

    { // tail of empty list
        lref<std::vector<int> > tk;
        lref<const std::vector<int> >vk = std::vector<int>();
        relation r3 =  tail(vk, tk);

        BOOST_CHECK(!r3());
    }

    { // tail of undefined list
        lref<std::vector<int> > tm;
        lref<std::vector<int> >vm;
        relation rm =  tail(vm, tm);

        BOOST_CHECK_THROW(rm(), InvalidDeref);
    }

    return 0;
}
