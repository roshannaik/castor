#include <boost/castor.h>

#include <boost/test/minimal.hpp>

#include "test_aggregate.h"


using namespace castor;

int test_main(int, char * [])
{
    std::string arr[] = { "Prolog", "Leda", "castor", "C#" };
    lref<std::vector<std::string> > v = std::vector<std::string>(arr, arr + 4);

    { // basic - gen
        lref<std::string> m;
        relation r = max_of(v, m);

        BOOST_CHECK(r());
        BOOST_CHECK(*m == "castor");
        BOOST_CHECK(!r());
        BOOST_CHECK(!m.defined());
    }

    { // basic - test
        lref<std::string> m = "castor";
        relation r = max_of(v, m);

        BOOST_CHECK(r());
        BOOST_CHECK(*m == "castor");
        BOOST_CHECK(!r());
        BOOST_CHECK(m.defined());
 
        // failure test
        // TODO: this should accept the string literal "Prolog" as its second
        // argument, but doesn't for GCC 4.4.3
        r = max_of(v, lref<std::string>("Prolog"));

        BOOST_CHECK(!r());
    }

    { // basic - empty container
        lref<std::string> m;
        lref<std::vector<std::string> > v2 = std::vector<std::string>();
        relation r = max_of(v2, m);

        BOOST_CHECK(!r());
        BOOST_CHECK(!m.defined());
    }

    { // with group_by
        lref<int> i, m;
        lref<group<bool, int> > g;
        relation r = (range(i, 1, 100) >>= group_by(i, isEven, g)) && max_of(g, m);

        BOOST_CHECK(r());
        BOOST_CHECK(*m == 99);
        BOOST_CHECK(r());
        BOOST_CHECK(*m == 100);
        BOOST_CHECK(!r());
        BOOST_CHECK(!m.defined());
    }

    { // test - with cmp
        // TODO: this should accept the string literal "C#" as its second
        // argument, but doesn't for GCC 4.4.3
        relation r = max_of(v, lref<std::string>("C#"), std::greater<std::string>());

        BOOST_CHECK(r());
    }

    return 0;
}
