#include <boost/castor.h>

#include <boost/test/minimal.hpp>

#include <list>


using namespace castor;

int test_main(int, char * [])
{
    std::list<int> li; li.push_back(1); li.push_back(2); li.push_back(3);
    lref<std::list<int> > lrli;
    lref<std::list<int>::iterator> itrB, itrE;
    relation r = eq(lrli, li) && begin(lrli, itrB) && end(lrli, itrE);

    BOOST_CHECK(r());
    BOOST_CHECK(equal(*itrB, *itrE, li.begin()));

    return 0;
}
