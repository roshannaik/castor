#include <boost/castor.h>

#include <boost/test/minimal.hpp>

#include <list>


using namespace castor;

int test_main(int, char * [])
{
    {
        std::list<int> li; li.push_back(1); li.push_back(2); li.push_back(3);
        lref<std::list<int> > lrli;
        lref<std::list<int>::iterator> itrB, itrE;
        relation r = eq(lrli, li) && begin(lrli, itrB) && end(lrli, itrE);

        BOOST_CHECK(r());
        BOOST_CHECK(equal(*itrB, *itrE, li.begin()));
    }
    { // const container with const_iterator
        lref<const std::string> s = "castor";
        std::string::const_iterator b = s->begin(), e = s->end();
        lref<std::string::const_iterator> lb, le;
        relation r = begin(s,lb) && end(s,le) && eq(b,lb) && eq(e,le);
        BOOST_CHECK(r());
    }
    { // non const container with const_iterator
        lref<std::string> s = "castor";
        std::string::const_iterator b = s->begin(), e = s->end();
        lref<std::string::const_iterator> lb, le;
        relation r = begin(s,lb) && end(s,le) && eq(b,lb) && eq(e,le);
        BOOST_CHECK(r());
    }
    return 0;
}
