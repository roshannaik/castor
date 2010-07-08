// Castor : Logic Programming Library
// Copyright © 2007-2010 Roshan Naik (roshan@mpprogramming.com).
// This software is governed by the MIT license (http://www.opensource.org/licenses/mit-license.php).

#include <castor.h>
using namespace castor;

void test_zip() {
		{
		lref<int> i, j;
		int s=0;
		int ai[] = { 1,2,3,4 }, aj[] = { 1,1,1,1,1 };

		for(relation r = zip( item(i,ai,ai+4), item(j,aj,aj+5) ); r(); ++s) {
			if(*i!=s  && *j!=1)
				throw "failed test_zip 1";
		}
		if(s!=4)
			throw "failed test_zip 1";
		}
}

void test_skip() {
	{ // int argument
	lref<int> i;
	int ai[] = { 1,2,3,4 };
    relation r = skip( 2, item(i,ai,ai+4) );
    int j=3;
	while( r() ) {
		if(*i!=j++)
			throw "failed test_skip 1";
	}
	}
	{ // lref<int> argument
	    lref<int> i;
        lref<unsigned long> s=2;
        relation r = skip( s, range(i,1,4) );
        int j=3;
	    while( r() ) {
		    if(*i!=j++)
			    throw "failed test_skip 2";
	    }
    }
    { // skip first two evens in the array
    int ai[] = { 1,2,3,4,5,6,7,8 };
    lref<int> i;
    relation r = skip(2, item(i,ai,ai+8) && predicate(i%2==0) ) ;
    if(! r() )
        throw "failed test_skip 3";
    if( *i!=6 )
        throw "failed test_skip 3";
    if(! r() )
        throw "failed test_skip 3";
    if( *i!=8 )
        throw "failed test_skip 3";
    if( r() )
        throw "failed test_skip 3";
    if( i.defined() )
        throw "failed test_skip 3";
    }
}
