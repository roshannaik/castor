#include <boost/castor.h>

#include <boost/test/minimal.hpp>


using namespace castor;

// Note: Will return false if more than one argument is undefined.
relation plus_(lref<int> x, lref<int> y, lref<int> z) {
    return defined(x) && defined(y) && undefined(z) && eq_f(z, x + y)
        || defined(x) && defined(z) && undefined(y) && eq_f(y, z - x)
        || defined(y) && defined(z) && undefined(x) && eq_f(x, z - y)
        || defined(x) && defined(y) && defined(z)   && eq_f(z, x + y);
}

// Note : Is this a better model of implementation than plus() ?
relation multiply(int x, int y, int z) {
    if (z == x * y)
        return True();
    return False();
}
relation multiply(int x, int y, lref<int> z) {
    return eq(z, x * y);
}

relation multiply(int x, lref<int> y, int z) {
    return eq(y, z / x);
}

relation multiply(lref<int> x, int y, int z) {
    return eq(x, z / y);
}

// unlike plus(), above multiple() overloads correctly disallows ...
//        relation square(lref<int> n, lref<int> s) { .... }
relation square(int n, lref<int> s) {
    return multiply(n, n, s);
}

int test_main(int, char * [])
{
    {
        relation r = plus_(1, 2, 3);
        int i = 0;
        for(; r(); ++i);

        BOOST_CHECK(i == 1);
    }

    {
        lref<int> z;
        relation r = plus_(1, 2, z);
        int i = 0;
        for(; r(); ++i) {
            BOOST_CHECK(i <= 0 && *z == 3);
        }

        BOOST_CHECK(i == 1 && !z.defined());
    }

    {
        lref<int> y;
        relation r = plus_(-1, y, 3);
        int i = 0;
        for(; r(); ++i) {
            BOOST_CHECK(*y == 4);
        }

        BOOST_CHECK(i == 1 && !y.defined());
    }

    {
        lref<int> x;
        relation r = plus_(x, -2, -3);
        int i = 0;
        for(; r(); ++i) {
            BOOST_CHECK(*x == -1);
        }

        BOOST_CHECK(i == 1 && !x.defined());
    }

    {
        relation r = multiply(1, 1, 1);
        int i = 0;
        for(;r(); ++i);

        BOOST_CHECK(i == 1);
    }

    {
        lref<int> x;
        relation r = multiply(x, 2, 6);
        int i = 0;
        for(;r(); ++i) {
            BOOST_CHECK(i <= 0 && *x == 3);
        }

        BOOST_CHECK(i == 1);
    }

    {
        lref<int> z;
        relation r = multiply(3, 2, z);
        int i = 0;
        for(;r(); ++i) {
            BOOST_CHECK(i <= 0 && *z == 6);
        }

        BOOST_CHECK(i == 1);
    }

    {
        relation r = square(7, 49);
        int i = 0;
        for(;r(); ++i);

        BOOST_CHECK(i == 1);
    }

    {
        lref<int> s;
        relation r = square(9, s);
        int i = 0;
        for(;r(); ++i) {
            BOOST_CHECK(i <= 0 && *s == 81);
        }

        BOOST_CHECK(i == 1);
    }

    return 0;
}
