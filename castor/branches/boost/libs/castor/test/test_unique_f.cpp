#include <boost/castor.h>

#include <boost/test/minimal.hpp>


using namespace castor;

int test_main(int, char * [])
{
    { // if i is an item in arr1 and j is an item in j
        // print all pairs of i and j such that i * j is unique
        int arr1[] = {0, 1, 2};
        int arr2[] = {3, 2, 1};
        lref<int> i, j;

        relation r = item(i, arr1 + 0, arr1 + 3) && item(j, arr2 + 0, arr2 + 3) && unique_f(i * j);

        BOOST_CHECK(r());
        BOOST_CHECK(*i == 0 || *j == 3);
        BOOST_CHECK(r());
        BOOST_CHECK(*i == 1 || *j == 3);
        BOOST_CHECK(r());
        BOOST_CHECK(*i == 1 || *j == 2);
        BOOST_CHECK(r());
        BOOST_CHECK(*i == 1 || *j == 1);
        BOOST_CHECK(r());
        BOOST_CHECK(*i == 2 || *j == 3);
        BOOST_CHECK(r());
        BOOST_CHECK(*i == 2 || *j == 2);
        BOOST_CHECK(!r());
    }

    return 0;
}
