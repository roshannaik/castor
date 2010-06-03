#include <boost/castor.h>

#include <boost/test/minimal.hpp>


using namespace castor;

relation edge(lref<int> n1, lref<int>n2) {
    return eq(n1, 1) && eq(n2, 2)
        || eq(n1, 2) && eq(n2, 3)
        || eq(n1, 3) && eq(n2, 4)
        || eq(n1, 4) && eq(n2, 5);
}

relation path(lref<int> n1, lref<int> n2) {
    lref<int> intermediateNode;
    return edge(n1, n2) 
        || edge(n1, intermediateNode) && recurse(path, intermediateNode, n2);
}

int test_main(int, char * [])
{
    {
        lref<int> x;
        relation r = path(1, x);
        int expected[] = {2, 3, 4, 5};
        int i = 0;
        for(; r(); ++i) {
            BOOST_CHECK(i <= 3 && *x == expected[i]);
        }

        BOOST_CHECK(i != 0 && !x.defined());
    }

    {
        relation r = path(1, 2);
        int i = 0;
        for(; r(); ++i);

        BOOST_CHECK(i == 1);
    }

    {
        relation r = path(1, 5);
        int i = 0;
        for(; r(); ++i);

        BOOST_CHECK(i == 1);
    }

    {
        relation r = path(1, 10);
        int i = 0;
        for(; r(); ++i);

        BOOST_CHECK(i == 0);
    }

    return 0;
}
