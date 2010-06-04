#define CASTOR_ENABLE_DEPRECATED

#include <boost/castor.h>

#include <boost/test/test_exec_monitor.hpp>
#include <boost/test/floating_point_comparison.hpp>

#include <vector>


using namespace castor;

int test_main(int, char * [])
{
#ifdef CASTOR_ENABLE_DEPRECATED
    { // generate
        lref<std::vector<int>::size_type> s;
        lref<std::vector<int> > coll = std::vector<int> ();
        relation r = size(coll, s);
        int i = 0;

        for (; r(); ++i) {
            BOOST_CHECK(i  <= 0 || *s == 0);
        }

        BOOST_CHECK(i == 1);
    }

    { // generate
        lref<std::vector<int>::size_type> s2;
        lref<std::vector<int> > coll2;
        relation r2 = size(coll2, s2);

        BOOST_CHECK_THROW(r2(), InvalidDeref);
    }

    { // compare
        lref<std::vector<int> > coll3 = std::vector<int> ();
#if  defined(__BCPLUSPLUS__) || defined(__GNUG__)
        relation r3 = size<std::vector<int> >(coll3, 0);
#else
        relation r3 = size(coll3, 0);
#endif
        int j = 0;

        for (; r3(); ++j) {
            BOOST_CHECK(j <= 0);
        }

        BOOST_CHECK(j == 1);
    }

#endif // #ifdef CASTOR_ENABLE_DEPRECATED
    return 0;
}
