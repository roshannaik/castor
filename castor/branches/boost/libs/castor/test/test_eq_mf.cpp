#include "test_eq.h"

#include <boost/castor.h>

#include <boost/test/minimal.hpp>

#include <vector>


using namespace castor;

namespace {
struct Base {
    int b;
    Base():b(0) {}
    int foo(){ return 1; }
    int foo_base(){ return 2; }
};
struct Derived : Base {
    int d;
    Derived() : d(10), Base() {}
    int foo() { return 3; }
    int foo_derived(){ return 4; }
};
} // namespace 

int test_main(int, char * [])
{
    {
        lref<int> refi;
        lref<Compute2> comp2 = Compute2(6);
        lref<int> two = 2;
        relation r = eq_mf(refi, comp2, &Compute2::apply, two);

	int i = 0;
	for (; r(); ++i) {
            BOOST_CHECK(i <= 0 && *refi == 2);
        }

	BOOST_CHECK(i == 1 && !refi.defined());
    }

    {
        lref<int> refi;
        lref<Compute2> comp2 = Compute2(6);
        lref<int> li = 2;
        relation r = eq_mf(refi, comp2, &Compute2::apply, li);

	int i = 0;
	for (; r(); ++i) {
            BOOST_CHECK(i <= 0 && *refi == 2);
        }

	BOOST_CHECK(i == 1 && !refi.defined());
    }

    {
        // generate mode
        lref<int> i; const lref<int> ci;
        lref<Functor6> f = Functor6();

        BOOST_CHECK(eq_mf(i, f, &Functor6::zero)() && *i == 0);
        BOOST_CHECK(eq_mf(i, f, &Functor6::method)() && *i == 0);

        i.reset();

        BOOST_CHECK(eq_mf(i, f, &Functor6::method, 1)() && *i == 1);

        i.reset();

        BOOST_CHECK(eq_mf(i, f, &Functor6::method, 1, 1)() && *i == 2);

        i.reset();

        BOOST_CHECK(eq_mf(i, f, &Functor6::method, 1, 1, 1)() && *i == 3);
        BOOST_CHECK(eq_mf(ci, f, &Functor6::method, 1, 1, 1, 1)() && *ci == 4);

        i.reset();

        lref<int> i2 = 1; const lref<const int> ci2 = 1;

        BOOST_CHECK(eq_mf(i, f, &Functor6::method, 1, 1, 1, 1, ci2)() && *i == 5);

        i.reset();

        BOOST_CHECK(eq_mf(i, f, &Functor6::method, 1, 1, 1, i2, 1, 1)() && *i == 6);

        i.reset();

        // const method
        BOOST_CHECK((eq_mf<int, Functor6>(i, f, &Functor6::cmethod, 1, 1)() && *i == 2));

        // const Obj, const method
#ifndef __GNUG__
        lref<const Functor6> fc = Functor6();
        BOOST_CHECK(eq_mf<int, const Functor6>(i, fc, &Functor6::cmethod, 1, 1)() && *i == 2);
#endif

	// const lref, const method -- turn the const lref into plain lref and then use it
        const lref<Functor6> cf = Functor6();
        f = cf;

        BOOST_CHECK((eq_mf<int, Functor6>(i, f, &Functor6::cmethod, 1, 1)() && *i == 2));

        i = 0;

        BOOST_CHECK(!eq_mf(i, f, &Functor6::method, 1, 1, 1, i2, 1, ci2)());
    }
    { // member function of base type
        lref<int> one=1, two=2, three=3, four=4;
        lref<Derived> d = Derived();

        BOOST_CHECK(eq_mf(one,d,&Base::foo)());

        BOOST_CHECK(eq_mf(three,d,&Derived::foo)());


        BOOST_CHECK(eq_mf(two,d,&Base::foo_base)());

        BOOST_CHECK(eq_mf(two,d,&Derived::foo_base)());

        BOOST_CHECK(eq_mf(four,d,&Derived::foo_derived)());
    }

    return 0;
}
