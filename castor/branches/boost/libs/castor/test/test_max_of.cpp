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
#ifdef __GNUG__
        r =  max_of<std::vector<std::string> >(v,"Prolog") ;
#else
        r =  max_of(v,"Prolog") ;
#endif

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
#ifdef __GNUG__
        relation r =  max_of<std::vector<std::string> >(v,"C#", std::greater<std::string>() ) ;
#else
        relation r =  max_of(v,"C#", std::greater<std::string>() ) ;
#endif

        BOOST_CHECK(r());
    }
    {// basic - gen - with const container
        lref<std::string> m;
        lref<const std::vector<std::string> > cv = std::vector<std::string>(arr, arr+4);

        relation r =  max_of(cv,m);
        BOOST_CHECK(r());
        BOOST_CHECK(*m=="castor");
        BOOST_CHECK(!r());
        BOOST_CHECK(!m.defined());
    }
    return 0;
}
