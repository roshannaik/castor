#include <boost/castor.h>

#include <boost/test/minimal.hpp>


using namespace castor;

int test_main(int, char * [])
{
    {
        int ai[] = { 10, 9, 8, 7, 6, 5, 4, 3, 2, 1 };
        std::vector<int> vi(ai, ai + 10); 
        sort(vi.begin(), vi.end());

        lref<int> j;
        relation r = item(j, ai, ai + 10) >>= order(j);
        for (int i = 0; r(); ++i) {
            BOOST_CHECK(vi.at(i) == *j);
        }

        BOOST_CHECK(!j.defined());
    }

    { // with comparator
        int ai[] = { 10, 9, 8, 7, 6, 5, 4, 3, 2, 1 };
        std::vector<int> vi(ai, ai + 10); 
        sort(vi.begin(), vi.end());

        lref<int> j;
        relation r = item(j, ai, ai + 10) >>= order(j, std::greater<int>());
        for (int i = 9; r(); --i) {
            BOOST_CHECK(vi.at(i) == *j);
        }

        BOOST_CHECK(!j.defined());
    }

    return 0;
}
