#include <boost/castor.h>

#include <boost/test/minimal.hpp>

#include "test_predicate.h"


using namespace castor;

int test_main(int, char * [])
{
    { // global predicate : success
        relation r = predicate(isEven, 2);
        int i = 0;
        for(; r(); ++i);

        BOOST_CHECK(i == 1);
    }

    { // global predicate : failure
        relation r = predicate(isEven, 3);
        int i = 0;
        for(; r(); ++i);

        BOOST_CHECK(i == 0);
    }

    { // global predicate taking lref : success
        lref<int> j = 2;
        relation r = predicate(isEven_lref, j);
        int i = 0;
        for(; r(); ++i);

        BOOST_CHECK(i == 1);
    }

    { // global predicate taking lref : failure
        relation r = predicate(isEven_lref, 3);
        int i = 0;
        for(; r(); ++i);

        BOOST_CHECK(i == 0);
    }

    { // static member predicate
        relation r = predicate(&Functor::staticMembPred, 2);
        int i = 0;
        for(; r(); ++i);

        BOOST_CHECK(i == 1);
    }

    { //  function pointers
        BOOST_CHECK(predicate(myPred)());
        BOOST_CHECK(predicate(myPred, 1)());
        BOOST_CHECK(predicate(myPred, 1, 2)());
        BOOST_CHECK(predicate(myPred, 1, 2, 3)());
        BOOST_CHECK(predicate(myPred, 1, 2, 3, 4)());
        BOOST_CHECK(predicate(myPred, 1, 2, 3, 4, 5)());
        BOOST_CHECK(predicate(myPred, 1, 2, 3, 4, 5, 6)());
        BOOST_CHECK(!predicate(myPred, 3, 2)());
    }

    { //  function objects
        BOOST_CHECK(predicate(MyPred())());
        BOOST_CHECK(predicate(MyPred(), 1)());
        BOOST_CHECK(predicate(MyPred(), 1, 2)());
        BOOST_CHECK(predicate(MyPred(), 1, 2, 3)());
        BOOST_CHECK(predicate(MyPred(), 1, 2, 3, 4)());
        BOOST_CHECK(predicate(MyPred(), 1, 2, 3, 4, 5)());
        BOOST_CHECK(predicate(MyPred(), 1, 2, 3, 4, 5, 6)());
        BOOST_CHECK(!predicate(MyPred(), 3, 2)());
    }

    return 0;
}
