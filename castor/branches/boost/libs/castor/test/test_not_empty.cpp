#define CASTOR_ENABLE_DEPRECATED // TODO: Remove the need for this (it's due to reliance on deprecated size<>()).

#include <boost/castor.h>

#include <boost/test/minimal.hpp>


using namespace castor;

int test_main(int, char * [])
{
    { // 1 
        lref<std::vector<int> > lvi = std::vector<int>();
        relation r = not_empty(lvi);

        BOOST_CHECK(!r());
    }

    { // 2
        std::vector<int> vi = std::vector<int>();
        relation r = not_empty(vi);

        BOOST_CHECK(!r());
    }

    return 0;
}
