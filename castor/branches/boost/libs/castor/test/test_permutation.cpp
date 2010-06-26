#include <boost/castor.h>

#include <boost/test/minimal.hpp>


using namespace castor;

int test_main(int, char * [])
{
    { // test mode
        lref<std::string> s = "hello", ps = "olleh";
        relation r = permutation(s, ps);

        BOOST_CHECK(r());
        BOOST_CHECK(!r());
    }

    { // generate mode
        lref<int> c = 0;
        lref<std::string> s = "hello", ps, ss = "helol";
        relation r = permutation(s, ps) && eq(ps, ss) && eval(++c);

        BOOST_CHECK(r() || *c == 1);

        ss = "heoll";

        BOOST_CHECK(r() || *c == 2);

        BOOST_CHECK(!r());
    }

    return 0;
}
