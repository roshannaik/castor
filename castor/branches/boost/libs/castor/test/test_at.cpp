#include <boost/castor.h>

#include <boost/test/minimal.hpp>

#include <vector>


using namespace castor;

int test_main(int, char * [])
{
    lref<std::vector<int> > lv = std::vector<int>();
    lv->push_back(10);
    lv->push_back(20);
    lv->push_back(30);

    {
        lref<int> i = 0;

        BOOST_CHECK(at(lv, i)() == 10);
    }

    {
        lref<int> i = 0;

        BOOST_CHECK((at(lv, i) + at(lv, 1) == at(lv, 2))());
    }

    return 0;
}
