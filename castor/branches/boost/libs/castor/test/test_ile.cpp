#include <boost/castor.h>

#include <boost/test/minimal.hpp>

#include <sstream>

#include "test_ile.h"


using namespace castor;

int test_main(int, char * [])
{
    { // simple ILE comparison as relation
        lref<int> li = 2, lj = 3;
        relation r = predicate(li < lj);
        int i = 0;
        for (; r(); ++i);

        BOOST_CHECK(i == 1);
    }

    { // more complex comparisons as relation
        lref<int> li = 2, lj = 3;
        relation r = predicate(lj + 2 - li > li + 3 - lj);
        int i = 0;
        for (; r(); ++i);

        BOOST_CHECK(i == 1);
    }

    {
        lref<int> i = 3;
        int global = 0;

        BOOST_CHECK((id(global) + i == 3)());
    }

    {
        std::stringstream sout;
        (id<std::ostream>(sout) << "hello" << 3)();
        std::string s;
        (id<std::istream>(sout) >> id(s))();

        BOOST_CHECK(s == "hello3");
    }

    {
        std::stringstream sout;
        lref<int> one = 1;
        lref<std::ostream> lsout(&sout, false);
        (lsout << "hello" << one)();
        std::string s;
        lref<std::string> ls(&s, false);
        (id<std::istream>(sout) >> ls)();

        BOOST_CHECK(s == "hello1");
    }

    {
        lref<std::string> ls("hello");

        BOOST_CHECK((ls == "hello")());
    }

    {
        lref<std::string> ls("hell");

        BOOST_CHECK((ls + "o" == "hello")());
    }

    return 0;
}
