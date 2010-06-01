#include <boost/castor.h>

#include <boost/test/minimal.hpp>


using namespace castor;

int test_main(int, char * [])
{
    // print items appearing only once in arr
    int arr[] = {0, 1, 2, 3, 3, 2, 4};
    lref<int> i;
    int expected = 0;
    relation r = item(i, arr + 0, arr + 7) && unique(i);
    int count = 0;

    while (r()) {
        count++;

        BOOST_CHECK(*i == expected++);
    }

    BOOST_CHECK(count == 5);

    return 0;
}
