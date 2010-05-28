#include <boost/castor.h>

#include <boost/test/minimal.hpp>

#include "test_ile.h"


using namespace castor;

int test_main(int, char * [])
{
    {
        lref<My> lm = My(1);

        BOOST_CHECK(get(lm, &My::i)() == 1);
    }

    {
        lref<My> lm = My(1);

	BOOST_CHECK((get(lm, &My::i) + get(lm, &My::i))() == 2);
    }

    return 0;
}

