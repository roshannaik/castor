#include <boost/castor.h>

#include <boost/test/minimal.hpp>

#include "test_utils.h"


using namespace castor;

namespace {
struct Base {
    int b;
    Base():b(0) {}
    void foo(int){ }
    void foo_base(int,int){ }
};
struct Derived : Base {
    int d;
    Derived() : d(10), Base() {}
    void foo(int) { }
    void foo_derived(int,int,int){ }
};
} // namespace 

int test_main(int, char * [])
{
    {
	lref<int> i1 = 1;
	lref<Functor6> f = Functor6();

#ifdef __BCPLUSPLUS__
	int(Functor6::* m0)(void) = &Functor6::method;
	eval_mf(f, m0)();
#else
	eval_mf(f, &Functor6::method)();
#endif

	BOOST_CHECK(f->result == 0);

#ifdef __BCPLUSPLUS__
	int(Functor6::* m1)(int) = &Functor6::method;
	eval_mf(f, m1, 1)();
#else
	eval_mf(f, &Functor6::method, 1)();
#endif

	BOOST_CHECK(f->result == 1);

#ifdef __BCPLUSPLUS__
	int(Functor6::* m2)(int, int) = &Functor6::method;
	eval_mf(f, m2, i1, 2)();
#else
	eval_mf(f, &Functor6::method, i1, 2)();
#endif

	BOOST_CHECK(f->result == 3);

#ifdef __BCPLUSPLUS__
	int(Functor6::* m3)(int, int, int) = &Functor6::method;
	eval_mf(f, m3, 1, 2, 3)();
#else
	eval_mf(f, &Functor6::method, 1, 2, 3)();
#endif

	BOOST_CHECK(f->result == 6);

#ifdef __BCPLUSPLUS__
	int(Functor6::* m4)(int, int, int, int) = &Functor6::method;
	eval_mf(f, m4, 1, 2, 3, 4)();
#else
	eval_mf(f, &Functor6::method, 1, 2, 3, 4)();
#endif

	BOOST_CHECK(f->result == 10);

#ifdef __BCPLUSPLUS__
	int(Functor6::* m5)(int, int, int, int, int) = &Functor6::method;
	eval_mf(f, m5, i1, 2, 3, 4, 5)();
#else
	eval_mf(f, &Functor6::method, i1, 2, 3, 4, 5)();
#endif

	BOOST_CHECK(f->result == 15);

#ifdef __BCPLUSPLUS__
	int(Functor6::* m6)(int, int, int, int, int, int) = &Functor6::method;
	eval_mf(f, m6, 1, 2, 3, 4, 5, 6)();
#else
	eval_mf(f, &Functor6::method, 1, 2, 3, 4, 5, 6)();
#endif

	BOOST_CHECK(f->result == 21);
    }

    {
	lref<Functor6> f = Functor6();

#ifdef __BCPLUSPLUS__
	int(Functor6::* fm)(int, int) = &Functor6::max;
	eval_mf(f, fm, 1, 2)();
#else
	eval_mf<int>(f, &Functor6::max, 1, 2)();
#endif

#ifdef __BCPLUSPLUS__
	double(Functor6::* fmd)(double, double) = &Functor6::max;
	eval_mf<double>(f, fmd, 2.0, 3.0)();
#else
	eval_mf<double>(f, &Functor6::max, 2.0, 3.0)();
#endif
        eval_mf<int>(f, &Functor6::cmax, 1, 2)();    	
    }

    { // on member functions from base & derived type
        lref<Derived> d = Derived();
        BOOST_CHECK(eval_mf(d,&Base::foo,1)());

        BOOST_CHECK(eval_mf(d,&Derived::foo,1)());

        BOOST_CHECK(eval_mf(d,&Derived::foo_base,1,2)());

        BOOST_CHECK(eval_mf(d,&Derived::foo_derived,1,2,3)());
    }

    return 0;
}
