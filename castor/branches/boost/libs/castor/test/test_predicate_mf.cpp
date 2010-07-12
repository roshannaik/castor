#include <boost/castor.h>

#include <boost/test/minimal.hpp>

#include "test_predicate.h"


using namespace castor;

int test_main(int, char * [])
{
    { // member predicate function
        lref<Functor> f = Functor();
        lref<int> two = 2;
        relation r = predicate_mf(f, &Functor::isEven, two);
        int i = 0;
        for(; r(); ++i);

        BOOST_CHECK(i == 1);
    }

    { // failing member predicate function
        lref<Functor> f = Functor();
        relation r = predicate_mf(f, &Functor::isEven, 5);
        int i = 0;
        for(; r(); ++i);

        BOOST_CHECK(i == 0);
    }

#ifndef __BCPLUSPLUS__
    { // non-const obj, const member predicate
	lref<Functor> f = Functor();
	lref<int> one = 1;
	typedef bool(Functor::* mpred)(int, const int, int, int, lref<int>, const int) const;
	relation r = predicate_mf(f, &Functor::allEqual, 1, 1, 1, 1, 1, one);
	int i = 0;
	for(; r(); ++i);

	BOOST_CHECK(i == 1);
    }
#endif

#if !defined(__BCPLUSPLUS__)
    { // const obj, const member predicate
	lref<const Functor> f = Functor();
	relation r = predicate_mf(f, &Functor::allEqual, 1, 1, 1, 1, 1, 1);
	int i = 0;
	for(; r(); ++i);

	BOOST_CHECK(i == 1);
    }
#endif

#if !defined(__BCPLUSPLUS__) 
    { // const object, const member predicate
	lref<const Functor> f = Functor();
	relation r = predicate_mf(f, &Functor::ternaryConstPred, 2, 3, 6);

	int i = 0;
	for(; r(); ++i);

	BOOST_CHECK(i == 1);
    }
#endif

    {
	lref<MyPred> lp = MyPred();
#ifdef __BCPLUSPLUS__
	bool(MyPred::* meth)(void) = &MyPred::method;
	BOOST_CHECK(predicate_mf(lp, meth)());
#else
        BOOST_CHECK(predicate_mf(lp, &MyPred::method)());
#endif

#ifdef __BCPLUSPLUS__
	bool(MyPred::* meth1)(int) = &MyPred::method;
	BOOST_CHECK(predicate_mf(lp, meth1, 1)());
#else
        BOOST_CHECK(predicate_mf(lp, &MyPred::method, 1)());
#endif


#ifdef __BCPLUSPLUS__
	bool(MyPred::* meth2)(int, int) = &MyPred::method;
	BOOST_CHECK(predicate_mf(lp, meth2, 1, 2)());
#else
        BOOST_CHECK(predicate_mf(lp, &MyPred::method, 1, 2)());
#endif


#ifdef __BCPLUSPLUS__
	bool(MyPred::* meth3)(int, lref<int>, int) = &MyPred::method;
	BOOST_CHECK(predicate_mf(lp, meth3, 1, 2, 3)());
#else
        BOOST_CHECK(predicate_mf(lp, &MyPred::method, 1, 2, 3)());
#endif



#ifdef __BCPLUSPLUS__
	bool(MyPred::* meth4)(int, int, int, int) = &MyPred::method;
	BOOST_CHECK(predicate_mf(lp, meth4, 1, 2, 3, 4)());
#else
        BOOST_CHECK(predicate_mf(lp, &MyPred::method, 1, 2, 3, 4)());
#endif


#ifdef __BCPLUSPLUS__
	bool(MyPred::* meth5)(int, int, int, int, int) = &MyPred::method;
	BOOST_CHECK(predicate_mf(lp, meth5, 1, 2, 3, 4, 5)());
#else
        BOOST_CHECK(predicate_mf(lp, &MyPred::method, 1, 2, 3, 4, 5)());
#endif



#ifdef __BCPLUSPLUS__
	bool(MyPred::* meth6)(int, int, int, int, int, int) = &MyPred::method;
	BOOST_CHECK(predicate_mf(lp, meth6, 1, 2, 3, 4, 5, 6)());
#else
        BOOST_CHECK(predicate_mf(lp, &MyPred::method, 1, 2, 3, 4, 5, 6)());
#endif


#ifdef __BCPLUSPLUS__
	bool(MyPred::* meth2a)(int, int) = &MyPred::method;
	BOOST_CHECK(!predicate_mf(lp, meth2a, 3, 2)());
#else
        BOOST_CHECK(!predicate_mf(lp, &MyPred::method, 3, 2)());
#endif
    }

    return 0;
}
