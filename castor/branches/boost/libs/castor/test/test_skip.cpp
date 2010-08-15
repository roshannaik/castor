#include <boost/castor.h>

#include <boost/test/minimal.hpp>


using namespace castor;

int test_main(int, char * [])
{
	{ // int argument
	    lref<int> i;
	    int ai[] = { 1,2,3,4 };
        relation r = item(i,ai,ai+4) >>= skip(2);
        int j=3;
	    while( r() ) {
		    BOOST_CHECK(*i==j++);
	    }
	}
	{ // lref<int> argument
	    lref<int> i;
        lref<unsigned long> s=2;
        relation r = range(i,1,4) >>= skip(s);
        int j=3;
	    while( r() ) {
		    BOOST_CHECK(*i==j++);
	    }
    }
    { // skip first two evens in the array
        int ai[] = { 1,2,3,4,5,6,7,8 };
        lref<int> i;
        relation r = item(i,ai,ai+8) && predicate(i%2==0) >>= skip(2) ;
        BOOST_CHECK( r() );
        BOOST_CHECK( *i==6 );
        BOOST_CHECK( r() );
        BOOST_CHECK( *i==8 );
        BOOST_CHECK( !r() );
        BOOST_CHECK(! i.defined() );
    }

    return 0;
}
