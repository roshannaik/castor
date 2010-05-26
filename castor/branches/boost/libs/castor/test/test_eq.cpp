#include "test_eq.h"

#include <boost/castor.h>

#include <boost/test/minimal.hpp>

#include <vector>


using namespace castor;

int test_main(int, char * [])
{
    {
        lref<std::string> ll, lr;
        std::string l, r;

        BOOST_CHECK(!eq("l", r)());
        BOOST_CHECK(!eq("l", r, &myStrCmp)());
        BOOST_CHECK(!eq(l, "r")());
        BOOST_CHECK(!eq(l, "r", &myStrCmp)());
        BOOST_CHECK(eq(l, r)());
        BOOST_CHECK(eq(l, r, &myStrCmp)());
        BOOST_CHECK(eq(ll, r)());
        BOOST_CHECK(eq(ll, r, &myStrCmp)());
        BOOST_CHECK(!eq(ll, "r")());
        BOOST_CHECK(!eq(ll, "r", &myStrCmp)());
        BOOST_CHECK(eq("l", lr)());
        BOOST_CHECK(eq("l", lr, &myStrCmp)());
        BOOST_CHECK(!eq(l, lr)());
        BOOST_CHECK(!eq(l, lr, &myStrCmp)());
        BOOST_CHECK(!eq(ll, lr)());
        BOOST_CHECK(!eq(ll, lr, &myStrCmp)());

        lref<A> la = A(); lref<B> lb; A a; B b;

        BOOST_CHECK(eq(la, lb)());
        BOOST_CHECK(eq(a, b)());
        BOOST_CHECK(eq(a, b, &cmpAB)());
        BOOST_CHECK(eq(la, b)());
        BOOST_CHECK(eq(la, b, &cmpAB)());
        BOOST_CHECK(eq(a, lb)());
        BOOST_CHECK(eq(a, lb, &cmpAB)());
        BOOST_CHECK(eq(la, la)());
        BOOST_CHECK(eq(la, la, &cmpAB)());
    }

    {
        relation r = eq<int>( 2, 2 );
        int i = 0;

        for (; r(); ++i);

        BOOST_CHECK(i == 1);

        r = eq<int>( 2, 3 );
        int j = 0;

        for (; r(); ++j);

        BOOST_CHECK(j == 0);
    }

    {
        int a[] = { 1, 2, 3, 4 };
        std::vector<int> v (a + 0, a + 4);
        lref<std::vector<int> > rv;
        relation r = eq(rv, v);
        int i = 0;

        for (; r(); ++i) {
            BOOST_CHECK(v == *rv);
        }

        BOOST_CHECK(i == 1 && !rv.defined());
    }

    {
        int b[] = { 1, 2, 3, 4 };
        std::vector<int> v2 (b + 0, b + 4);
        lref<std::vector<int> > rv2;

        BOOST_CHECK(eq(rv2, v2)());
        BOOST_CHECK(v2 == *rv2);
    }

    return 0;
}
