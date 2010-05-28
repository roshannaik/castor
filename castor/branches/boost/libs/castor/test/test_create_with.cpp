#define CASTOR_ENABLE_DEPRECATED

#include <boost/castor.h>

#include <boost/test/minimal.hpp>

#include <string>

#include "test_ile.h"


using namespace castor;

int test_main(int, char * [])
{
    {
        BOOST_CHECK((Create<std::pair<int, int> >::with(1, 2)().first == 1));
        BOOST_CHECK(Create<My>::with()().i == 0);
        BOOST_CHECK(Create<My>::with(1)().i == 1);
        BOOST_CHECK(Create<My>::with(1, 2)().i == 2);
        BOOST_CHECK(Create<My>::with(1, 2, 3)().i == 3);
        BOOST_CHECK(Create<My>::with(1, 2, 3, 4)().i == 4);
        BOOST_CHECK(Create<My>::with(1, 2, 3, 4, 5)().i == 5);
        BOOST_CHECK(Create<My>::with(1, 2, 3, 4, 5, 6)().i == 6);

        int i = 1, j = 2, k = 3; lref<int> l = 4, m = 5;

        BOOST_CHECK(Create<My>::with(i, j, k, l, m, 6)().i == 6);
    }

    {
        lref<std::string> hello = "hello", world = "world";

        BOOST_CHECK((Create<std::string>::with(hello) + world)() == (hello + world)());

        lref<std::string> dear = "dear";

        BOOST_CHECK((Create<std::string>::with(hello) + world)() != (hello + dear)());
    }

    return 0;
}
