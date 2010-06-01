#include <boost/castor.h>

#include <boost/test/minimal.hpp>


using namespace castor;

int test_main(int, char * [])
{
    { // simple inc()
        lref<int> li = 2;
        relation r = inc(li);

        BOOST_CHECK(r());
        BOOST_CHECK(*li == 3);
        BOOST_CHECK(!r());
        BOOST_CHECK(*li == 2);
    }

    { // simple dec()
        lref<int> li = 2;
        relation r = dec(li);

        BOOST_CHECK(r());
        BOOST_CHECK(*li == 1);
        BOOST_CHECK(!r());
        BOOST_CHECK(*li == 2);
    }    

    { // inc(lre<ptr>)
        int arr[] = {1, 2, 3, 4};
        lref<int*> ptr = arr;
        relation r = inc(ptr);

        BOOST_CHECK(r());
        BOOST_CHECK(**ptr == 2);
        BOOST_CHECK(!r());
    }

    { // dec(lre<ptr>)
        int arr[] = {1, 2, 3, 4};
        lref<int*> ptr = arr + 2;
        relation r = dec(ptr);

        BOOST_CHECK(r());
        BOOST_CHECK(**ptr == 2);
        BOOST_CHECK(!r());
    }

    return 0;
}
