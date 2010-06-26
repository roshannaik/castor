#include <boost/castor.h>

#include <boost/test/minimal.hpp>


using namespace castor;

int test_main(int, char * [])
{
    { // 1 - using lref<Cont>
        lref<std::vector<int> > lvi = std::vector<int>();
        relation r = not_empty(lvi);

        BOOST_CHECK(!r());
    }

    { // 2 - not using lref<Cont>
        std::vector<int> vi = std::vector<int>();
        relation r = not_empty(vi);

        BOOST_CHECK(!r());
    }
    { //3 - const objects
        lref<const std::vector<int> > lvi = std::vector<int>();
        relation r = not_empty(lvi);

        BOOST_CHECK(!r());
    }
    return 0;
}
