#include "test_eq.h"

#include <boost/castor.h>

#include <boost/test/minimal.hpp>

#include <vector>


using namespace castor;

int test_main(int, char * [])
{
    { // unify .. should pass
        lref<int> refi;
        relation r = eq_f( refi, detail::bind<int>(std::plus<int>(), 6, 3) );

	int i = 0;
	for (; r(); ++i) {
            BOOST_CHECK(i <= 0 && *refi == 6 + 3);
        }

	BOOST_CHECK(i == 1 && !refi.defined());
    }

    { // comparison .. should pass
        lref<int> refk = 9;
        relation r2 = eq_f( refk, detail::bind<int>(std::plus<int>(), 6, 3) );

	int k = 0;
	for (; r2(); ++k) {
            BOOST_CHECK(k <= 0 && *refk == 6 + 3);
        }

	BOOST_CHECK(k == 1 && refk.defined());
    }

    { // comparison .. should fail
        lref<int> refj = 1;
        relation r3 = eq_f( refj, detail::bind<int>(std::plus<int>(), 6, 3) );

	int j = 0;
	for (; r3(); ++j);

	BOOST_CHECK(j == 0 && refj.defined());
    }

    { // regular functions
        lref<int> li, lj, lk;
        relation r = eq(lj, 6) && eq(lk, 2) && eq_f(li, &compute, lj, lk);

        BOOST_CHECK(r() || *li == 2);

        BOOST_CHECK(eq_f<int>(0, getN)());
        BOOST_CHECK(eq_f<int>(1, getN, 1)());
        BOOST_CHECK(eq_f<int>(2, getN, 1, 1)());
        BOOST_CHECK(eq_f<int>(3, getN, 1, 1, 1)());
        BOOST_CHECK(eq_f<int>(4, getN, 1, 1, 1, 1)());
        BOOST_CHECK(eq_f<int>(5, getN, 1, 1, 1, 1, 1)());
        BOOST_CHECK(eq_f<int>(6, getN, 1, 1, 1, 1, 1, 1)());
    }

    { // regular functions - with overload ambiguity
        BOOST_CHECK((eq_f<int>(0, getN2)()));
        BOOST_CHECK((eq_f<int, int>(1, getN2, 1)()));
        BOOST_CHECK((eq_f<int, int, int>(2, getN2, 1, 1)()));
        BOOST_CHECK((eq_f<int, int, int, int>(3, getN2, 1, 1, 1)()));
        BOOST_CHECK((eq_f<int, int, int, int, int>(4, getN2, 1, 1, 1, 1)()));
        BOOST_CHECK((eq_f<int, int, int, int, int, int>(5, getN2, 1, 1, 1, 1, 1)()));
        BOOST_CHECK((eq_f<int, int, int, int, int, int, int>(6, getN2, 1, 1, 1, 1, 1, 1)()));
    }

    { // function objects -  compare mode
        lref<int> li, lj, lk;
        relation r = eq(lj, 6) && eq(lk, 2) && eq_f(li, Compute(), lj, lk);

        BOOST_CHECK(r() || *li == 2);

        BOOST_CHECK(eq_f<int>(0, Functor6())());

        Functor6 f;

        BOOST_CHECK(eq_f<int>(1, f, 1)());
        BOOST_CHECK(eq_f<int>(2, Functor6(), 1, 1)());
        BOOST_CHECK(eq_f<int>(3, f, 1, 1, 1)());
        BOOST_CHECK(eq_f<int>(4, Functor6(), 1, 1, 1, 1)());

        const Functor6 cf = Functor6(); lref<int> i = 1; lref<const int> ci = 1;

        BOOST_CHECK(eq_f<int>(5, cf, 1, 1, 1, 1, ci)());
        BOOST_CHECK(eq_f<int>(6, Functor6(), 1, 1, 1, i, 1, 1)());
        BOOST_CHECK(!eq_f<int>(7, Functor6(), 1, 1, 1, i, 1, ci)());
    }

    {
        // function objects - generate mode
        lref<int> i; const lref<int> ci;

        BOOST_CHECK(eq_f(i, Functor6())() && *i == 0);

        i.reset();
        Functor6 f;

        BOOST_CHECK(eq_f(i, f, 1)() && *i == 1);

        i.reset();

        BOOST_CHECK(eq_f(i, Functor6(), 1, 1)() && *i == 2);

        i.reset();

        BOOST_CHECK(eq_f(i, f, 1, 1, 1)() && *i == 3);
        BOOST_CHECK(eq_f(ci, Functor6(), 1, 1, 1, 1)() && *ci == 4);

        i.reset();

        const Functor6 cf = Functor6(); lref<int> i2 = 1; const lref<const int> ci2 = 1;

        BOOST_CHECK(eq_f(i, cf, 1, 1, 1, 1, ci2)() && *i == 5);

        i.reset();

        BOOST_CHECK(eq_f(i, Functor6(), 1, 1, 1, i2, 1, 1)() && *i == 6);

        i = 0;

        BOOST_CHECK(!eq_f(i, Functor6(), 1, 1, 1, i2, 1, ci2)());
    }

    { // ILE
        lref<int> li, lj, lk;
        relation r = eq(lj, 6) && eq(lk, 2) && eq_f(li, lj / lk - 1);

        BOOST_CHECK(r() || *li == 2);
    }

    return 0;
}
