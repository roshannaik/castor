#include <castor.h>
#include <iostream>
#include <vector>

using namespace castor;
using namespace std;


void test_shuffle() {
	{ // generate mode, lref<Itr>
	  lref<int> c;
	  lref<string> s = "hello", ss;
	  lref<string::iterator> sb = s->begin(), se = s->end();
	  relation r = zip(True(20), shuffle(sb,se,ss)) >> count(c);
	  if(!r())
		  throw "failed test_shuffle 1";
	  if(*c!=20)
		  throw "failed test_shuffle 1";
	  if(r() || c.defined() || !ss.defined())
		  throw  "failed test_shuffle 1";
	}
	{ // generate mode, Itr
	  lref<int> c;
	  lref<string> s = "hello", ss;
	  string::iterator sb = s->begin(), se = s->end();
	  relation r = zip(True(20), shuffle(sb,se,ss)) >> count(c);
	  if(!r())
		  throw "failed test_shuffle 2";
	  if(*c!=20)
		  throw "failed test_shuffle 2";
	  if(r() || c.defined() || !ss.defined())
		  throw  "failed test_shuffle 2";
	}	
	{ // test mode
	  lref<int> c;
	  lref<string> s = "hello", ss;
	  lref<string::iterator> sb = s->begin(), se = s->end();
	  relation r = (permute(s,ss) && shuffle(ss,s)) >> count(c);
	  if(!r())
		  throw "failed test_shuffle 3";
	  if(*c!=60)
		  throw  "failed test_shuffle 3";
	  if(r() || c.defined() || ss.defined())
		  throw  "failed test_shuffle 3";
	}
}


void test_permute() {
	{ // test mode
	  lref<string> s = "hello", ps="olleh";
	  relation r = permute(s,ps) ;
	  if(!r())
		  throw "failed test_permute 1";
	  if(r())
		  throw  "failed test_permute 1";
	}
	{ // generate mode
	  lref<int> c=0;
	  lref<string> s = "hello", ps, ss="helol";
	  relation r = permute(s,ps) && eq(ps,ss) && eval(++c);
	  if(!r() && *c!=1)
		  throw "failed test_permute 2";
	  ss="heoll";
	  if(!r() && *c!=2)
		  throw "failed test_permute 2";
	  if(r())
		  throw "failed test_permute 2";
	}
}