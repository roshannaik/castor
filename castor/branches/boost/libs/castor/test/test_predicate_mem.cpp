#include <boost/castor.h>

#include <boost/test/minimal.hpp>

#include "test_predicate.h"


using namespace castor;

int test_main(int, char * [])
{
    {
        lref<Functor> lf = Functor();

        BOOST_CHECK(predicate_mem(lf, &Functor::t)());
        BOOST_CHECK(!predicate_mem(lf, &Functor::f)());
    }

    return 0;
}
