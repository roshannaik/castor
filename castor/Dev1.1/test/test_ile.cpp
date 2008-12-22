#include <castor.h>
#include <string>
#include <utility>

using namespace castor;
using namespace std;

void test_Prefix_UnaryOps_ILE() {
    {// Simple Prefix increment/decrement
    lref<int> li = 1;
    if((++li)() != 2)
        throw "failed test_Prefix_UnaryOps_ILE 1";
    if(*li != 2)
        throw "failed test_Prefix_UnaryOps_ILE 1";

    if((--li)() != 1)
        throw "failed test_Prefix_UnaryOps_ILE 2";
    if(*li != 1)
        throw "failed test_Prefix_UnaryOps_ILE 2";
    }

    {// Compound Prefix increment expression
    lref<int> li = 2;
    if( (++(++(++li))*2+5)() != 15)
        throw "failed test_Prefix_UnaryOps_ILE 3";
    if(*li != 5)
        throw "failed test_Prefix_UnaryOps_ILE 3";
    }
    {// Compound Prefix decrement expression
    lref<int> li = 5;
    if( (--(--(--li))*2)() != 4)
        throw "failed test_Prefix_UnaryOps_ILE 4";
    if(*li != 2)
        throw "failed test_Prefix_UnaryOps_ILE 4";
    }
}

void test_Postfix_UnaryOps_ILE() {
    {// Simple Postfix increment/decrement
    lref<int> li = 1;
    if((li++)() != 1)
        throw "failed test_Postfix_UnaryOps_ILE 1";
    if(*li != 2)
        throw "failed test_Postfix_UnaryOps_ILE 1";

    if((li--)() != 2)
        throw "failed test_Postfix_UnaryOps_ILE 2";
    if(*li != 1)
        throw "failed test_Postfix_UnaryOps_ILE 2";
    }

    {// Compound Postfix increment expression
    lref<int> li = 2;
    if( ( (li++)*2+5)() != 9 )
        throw "failed test_Postfix_UnaryOps_ILE 3";
    if(*li != 3)
        throw "failed test_Postfix_UnaryOps_ILE 3";
    }
    {// Compound Postfix decrement expression
    lref<int> li = 5;
    if( ((li--)*2)() != 10)
        throw "failed test_Postfix_UnaryOps_ILE 4";
    if(*li != 4)
        throw "failed test_Postfix_UnaryOps_ILE 4";
    }
    {// Mix of Postfix and Prefix inc/decrement expressions
    lref<int> li = 2;
    ( (--li)*(li++) )(); /*result of this evaluation is an undefined value */
    if(*li != 2) // li's value is however defined
        throw "failed test_Postfix_UnaryOps_ILE 5";
    }
}



//--------------------------------------------------------
//  Tests  : ILE
//--------------------------------------------------------

void test_ILE() {
    { // simple ILE comparison as relation
    lref<int> li = 2 , lj=3;
    relation r = predicate(li < lj);
    int i=0;
    for(; r(); ++i);
    if(i!=1) throw "failed test_ILE 1";
    }
    { // more complex comparisons as relation
    lref<int> li = 2 , lj=3;
    relation r = predicate(lj+2-li > li+3-lj);
    int i=0;
    for(; r(); ++i);
    if(i!=1) throw "failed test_ILE 2";
    }
}

void test_eq_ILE() {
    {// simple ILE
    lref<int> li1, li2 = 2;
    relation r = eq_f(li1, li2 * 4);
    if(! r() )
        throw "failed test_eq_ILE 1";
    if(*li1 != 8)
        throw "failed test_eq_ILE 1";
    if(r())
        throw "failed test_eq_ILE 1";
    if(li1.defined())
        throw "failed test_eq_ILE 1";
    }
    {// compund ILE using *,+
    lref<int> ri1=27, ri2=2, ri3=3;
    relation r = eq_f(ri1, 1*ri2*ri3*2*ri2+3);
    int i=0;
    for(; r(); ++i) {
        if(i>0)
            throw "failed test_eq_ILE 2";
    }
    if(i!=1 || !ri1.defined())
        throw "failed test_eq_ILE 2";
    }
    {// compund ILE using +,-,*,/
    lref<int> rj1, rj2=10, rj3=3;
    relation r = eq_f(rj1, (rj2*rj3+10)/(rj2-5) );
    int j=0;
    for(; r(); ++j) {
        if(j>0 || *rj1!=8)
            throw "failed test_eq_ILE 3";
    }
    if(j!=1 || rj1.defined())
        throw "failed test_eq_ILE 3";
    }
    {// compund ILE using prefix ops +,- 
    lref<int> rj1, rj2=10;
    relation r = eq_f(rj1, -(-rj2));
    int j=0;
    for(; r(); ++j) {
        if(j>0 || *rj1!=10)
            throw "failed test_eq_ILE 4";
    }
    if(j!=1 || rj1.defined())
        throw "failed test_eq_ILE 4";
    }
}


struct My {
    int i;
    My() : i(0) { }
    My(int) : i(1) { }
    My(int,int) : i(2) { }
    My(int,int,int) : i(3) { }
    My(int,int,int,int) : i(4) { }
    My(int,int,int,int,int) : i(5) { }
    My(int,int,int,int,int,int) : i(6) { }
    operator int()   { return i; }

    typedef int result_type;
    int operator() (void)                     { return 0; }
    int operator() (int)                      { return 1; }
    int operator() (string)                   { return -1; }
    int operator() (int,int)                  { return 2; }
    int operator() (int,int,int)              { return 3; }
    int operator() (int,int,int,int)          { return 4; }
    int operator() (int,int,int,int,int)      { return 5; }
    int operator() (int,int,int,int,int,int)  { return 6; }
};

void test_Create_with() {
#ifdef CASTOR_ENABLE_DEPRECATED
    {
    if(Create<pair<int,int> >::with(1,2)().first!=1)
        throw "failed test_Create_with 1";
    if(Create<My>::with()().i != 0)
        throw "failed test_Create_with 1";
    if(Create<My>::with(1)().i != 1)
        throw "failed test_Create_with 1";
    if(Create<My>::with(1,2)().i != 2)
        throw "failed test_Create_with 1";
    if(Create<My>::with(1,2,3)().i != 3)
        throw "failed test_Create_with 1";
    if(Create<My>::with(1,2,3,4)().i != 4)
        throw "failed test_Create_with 1";
    if(Create<My>::with(1,2,3,4,5)().i != 5)
        throw "failed test_Create_with 1";
    if(Create<My>::with(1,2,3,4,5,6)().i != 6)
        throw "failed test_Create_with 1";
    int i=1,j=2,k=3; lref<int> l = 4, m =5;
    if(Create<My>::with(i,j,k,l,m,6)().i != 6)
        throw "failed test_Create_with 1";
    }
    {
    lref<string> hello = "hello", world="world";
    if((Create<string>::with(hello)+world)() != (hello+world)())
        throw "failed test_Create_with 2";

    lref<string> dear="dear";
    if((Create<string>::with(hello)+world)() == (hello+dear)())
        throw "failed test_Create_with 2";
    }
#endif
}


void test_create() {
    {
    if(create<pair<int,int> >(1,2)().first!=1)
        throw "failed test_create 1";
    if(create<My>()().i != 0)
        throw "failed test_create 1";
    if(create<My>(1)().i != 1)
        throw "failed test_create 1";
    if(create<My>(1,2)().i != 2)
        throw "failed test_create 1";
    if(create<My>(1,2,3)().i != 3)
        throw "failed test_create 1";
    if(create<My>(1,2,3,4)().i != 4)
        throw "failed test_create 1";
    if(create<My>(1,2,3,4,5)().i != 5)
        throw "failed test_create 1";
    if(create<My>(1,2,3,4,5,6)().i != 6)
        throw "failed test_create 1";
    int i=1,j=2,k=3; lref<int> l = 4, m =5;
    if(create<My>(i,j,k,l,m,6)().i != 6)
        throw "failed test_create 1";
    }
    {
    lref<string> hello = "hello", world="world";
    if((create<string>(hello)+world)() != (hello+world)())
        throw "failed test_create 2";

    lref<string> dear="dear";
    if((create<string>(hello)+world)() == (hello+dear)())
        throw "failed test_create 2";
    }
}

struct Name {
    string firstName, lastName;
    Name(string firstName, string lastName) : firstName(firstName), lastName(lastName)
    { }
    bool operator==(const Name& rhs) {
        return firstName==rhs.firstName && lastName==rhs.lastName;
    }
};

void test_get() {
    {
    lref<My> lm = My(1);
    if(get(lm,&My::i)()!=1)
        throw "failed test_get 1";
    }
}

void blah(void)                     { }
int foo(void)                       { return 0; }
int foo(int)                        { return 1; }
int foo(int,int)                    { return 2; }
int foo(int,int,int)                { return 3; }
int foo(int,int,int,int)            { return 4; }
int foo(int,int,int,int,int)        { return 5; }
int foo(int,int,int,int,int,int)    { return 6; }
int foo(string)                     { return -1; }


void test_call() {
    { // regular functions
    call(blah)();
	if(call(foo)()!=0)
		throw "failed test_call 1";
#ifdef __BCPLUSPLUS__
	int(*p)(int) = &foo;
	if(call(p,1)()!=1)
#else
	if(call(static_cast<int(*)(int)> (foo),1)()!=1)
#endif
		throw "failed test_call 1";

#ifdef __BCPLUSPLUS__
	int(*p2)(string) = &foo;
	if(call(p2,string("castor"))()!=-1)
#else
	if(call(static_cast<int(*)(string)> (foo),string("castor"))()!=-1)
#endif
		throw "failed test_call 1";
	if(call(foo,1,2)()!=2)
        throw "failed test_call 1";
    if(call(foo,1,2,3)()!=3)
        throw "failed test_call 1";
    if(call(foo,1,2,3,4)()!=4)
        throw "failed test_call 1";
    if(call(foo,1,2,3,4,5)()!=5)
        throw "failed test_call 1";
    if(call(foo,1,2,3,4,5,6)()!=6)
        throw "failed test_call 1";
    }
    { // function objects
    if(call(My())()!=0)
        throw "failed test_call 2";
    if(call(My(),1)()!=1)
        throw "failed test_call 2";
    if(call(My(),string(""))()!=-1)
        throw "failed test_call 2";
    if(call(My(),1,2)()!=2)
        throw "failed test_call 2";
    if(call(My(),1,2,3)()!=3)
        throw "failed test_call 2";
    if(call(My(),1,2,3,4)()!=4)
        throw "failed test_call 2";
    if(call(My(),1,2,3,4,5)()!=5)
        throw "failed test_call 2";
    if( ! eq(6,call(My(),1,2,3,4,5,6)())() )
        throw "failed test_call 2";
    }
}
