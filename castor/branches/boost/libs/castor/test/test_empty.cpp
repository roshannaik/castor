#include <boost/castor.h>

#include <boost/test/minimal.hpp>

#include <vector>


using namespace castor;

int test_main(int, char * [])
{
    { // 1 - lref to std::vector
        lref<std::vector<int> > lvi;
        relation r = empty(lvi);

        BOOST_CHECK(r());
        BOOST_CHECK(lvi.defined() && lvi->empty());
        BOOST_CHECK(!r());
        BOOST_CHECK(!lvi.defined());
    }

    { // 2 - not using lref
        std::vector<int> vi;
        relation r = empty(vi);

        BOOST_CHECK(r());
    }
    { // 3- const Cont (not lref<Cont>)
        const vector<int> vi;
        relation r = empty(vi);
        BOOST_CHECK(r());
    }
    return 0;
}
