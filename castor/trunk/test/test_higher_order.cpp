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

