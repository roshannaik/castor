#include <boost/castor.h>

#include <boost/test/minimal.hpp>

#include "test_ile.h"


using namespace castor;

int test_main(int, char * [])
{
    { // simple ILE
        lref<int> li1, li2 = 2;
        relation r = eq_f(li1, li2 * 4);

        BOOST_CHECK(r());
        BOOST_CHECK(*li1 == 8);
        BOOST_CHECK(!r());
        BOOST_CHECK(!li1.defined());
    }

    { // compund ILE using *, +
        lref<int> ri1 = 27, ri2 = 2, ri3 = 3;
        relation r = eq_f(ri1, 1 * ri2 * ri3 * 2 * ri2 + 3);
        int i = 0;

        for (; r(); ++i) {
            BOOST_CHECK(i <= 0);
        }

        BOOST_CHECK(i == 1 && ri1.defined());
    }

    { // compund ILE using +, -, *, /
        lref<int> rj1, rj2 = 10, rj3 = 3;
        relation r = eq_f(rj1, (rj2 * rj3 + 10) / (rj2 - 5));
        int j = 0;

        for (; r(); ++j) {
            BOOST_CHECK(j <= 0 && *rj1 == 8);
        }

        BOOST_CHECK(j == 1 && !rj1.defined());
    }

    { // compund ILE using prefix ops +, - 
        lref<int> rj1, rj2 = 10;
        relation r = eq_f(rj1, -(-rj2));
        int j = 0;

        for (; r(); ++j) {
            BOOST_CHECK(j <= 0 || *rj1 == 10);
        }

        BOOST_CHECK(j == 1 && !rj1.defined());
    }

    return 0;
}
