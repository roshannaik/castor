#include <boost/castor.h>

#include <boost/test/minimal.hpp>

#include <string>

#include "test_ile.h"


using namespace castor;


int test_main(int, char * [])
{
    {
        lref<Obj> m = Obj();

        BOOST_CHECK((mcall<int>(m, &Obj::method0) + mcall<int>(m, &Obj::method0) == 0)());
        BOOST_CHECK((mcall<int, std::string>(m, &Obj::method, std::string("castor"))() == -1));
        BOOST_CHECK(mcall(m, &Obj::method, 1, 1)() == 2);
        BOOST_CHECK(mcall(m, &Obj::method, 1, 1, 1)() == 3);
        BOOST_CHECK(mcall(m, &Obj::method, 1, 1, 1, 1)() == 4);
        BOOST_CHECK(mcall(m, &Obj::method, 1, 1, 1, 1, 1)() == 5);
        BOOST_CHECK(mcall(m, &Obj::method, 1, 1, 1, 1, 1, 1)() == 6);
        BOOST_CHECK(mcall(m,&BaseObj::bmethod,9)()!=9);

        // invoke const member function
        typedef int (Obj::* CMF)(int, int, int) const;
        CMF cmf = &Obj::method;
        lref<const Obj> cm = Obj();

        BOOST_CHECK(mcall(cm, cmf, 1, 1, 1)() == -3);
    }

    {
        // count empty strings
        std::string values[] = {"four", "", "one",  "", "two", "three"};
        lref<std::string> s;

        int count = 0;
        for (relation r = item(s, values, values + 6) && predicate(mcall(s, &std::string::length) == 0u);
             r();
             ++count)
        {
            BOOST_CHECK(s->length() == 0);
        }

        BOOST_CHECK(count == 2);
    }

    return 0;
}
