// Castor : Logic Programming Library
// Copyright © 2008 Roshan Naik (roshan@mpprogramming.com).
// This software is goverened by the MIT license (http://www.opensource.org/licenses/mit-license.php).

#include <string>
#include <iostream>
#include <vector>
#include <stack>
#include <list>
#include <algorithm>
#include <functional>
#include <iomanip>
#include <ctime>
#include <sstream>
#include <utility>

#include <castor.h>

using namespace std;
using namespace castor;

void runtests();


template<typename Func>
vector<int> getMatchingNumbers(int min, int max, Func pred) {
    vector<int> result;
    for(int i=min; i<=max; ++i)
        if(pred(i))
            result.push_back(i);
    return result;
}

bool isEven2(int i) {
		return i % 2 == 0;
}

void foo()
{
  lref<int> i;
  clock_t start = clock();
  relation r = range(i,1,1000000) && predicate(isEven2,i);
  while(r());
  cout << "\nTime taken " << (clock() - start) << "\n\n Testing Complete! ";
}

int main()
{
  //foo();
  clock_t start = clock();
  runtests();
  cout << "\nTime taken " << (clock() - start) << "\n\n Testing Complete! ";
  cin.ignore();
  return 0;
}

//--------------------------------------------------------
//  Tests  : basic lref test for compilation errors
//--------------------------------------------------------

void test_lref() {
    lref<string> r2 = "123";
    // these only check for compiler errors
   lref<string> rs; lref<const char*> rc = "world";
   rs = "hello";
   if(*rs!="hello")
       throw "failed test_lref 1";
   rs = rc;
   if(*rs!="world")
       throw "failed test_lref 1";
}

//--------------------------------------------------------
//  Tests  : begin and end relations to obtain iterators to lref<CollectionT>
//--------------------------------------------------------
void test_begin_end() {
    list<int> li; li.push_back(1); li.push_back(2); li.push_back(3);
    lref<list<int> > lrli;
    lref<list<int>::iterator> itrB, itrE;
    relation r = eq(lrli, li) && begin(lrli,itrB) && end(lrli,itrE);
    if(!r())
        throw "failed test_begin_end 1";
    if(!equal(*itrB, *itrE, li.begin()))
        throw "failed test_begin_end 1";
}

//--------------------------------------------------------
//  Tests  : for relational || and && operators
//--------------------------------------------------------
void test_or_and() {
    list<pair<int,int> > expected , actual; 
    expected.push_back(make_pair(1,1));
    expected.push_back(make_pair(1,2));
    expected.push_back(make_pair(2,1));
    expected.push_back(make_pair(2,2));

    lref<int> a, b;
    relation r =  (eq(a,1) || eq(a,2))
               && (eq(b,1) || eq(b,2));
    while(r())
        actual.push_back( make_pair(*a,*b));
    if(actual!=expected)
        throw "failed test_or_and 1";
}

//--------------------------------------------------------
//   Family relationships
//--------------------------------------------------------

//  (mother1, father1) =>(son1 and daughter1); 
//  (mother2, father2) =>(son2 and daughter2) 
//  (gmother2, gfather2) =>(father2)
relation child( lref<string> progeny, lref<string> mother, lref<string> father ) {
    return     eq(progeny,"son1")      &&  eq(mother,"mother1")  && eq(father,"father1")
            || eq(progeny,"daughter1") &&  eq(mother,"mother1")  && eq(father,"father1")
            || eq(progeny,"son2")      &&  eq(mother,"mother2")  && eq(father,"father2")
            || eq(progeny,"daughter2") &&  eq(mother,"mother2")  && eq(father,"father2")
			|| eq(progeny,"father2")   &&  eq(mother,"gmother2") && eq(father,"gfather2")

			//|| eq(progeny,"gfather3")   &&  eq(mother,"ggmother2") && eq(father,"ggfather2")
			//|| eq(progeny,"gfather4")   &&  eq(mother,"ggmother2") && eq(father,"ggfather2")
			//|| eq(progeny,"gfather4")   &&  eq(mother,"ggmother2") && eq(father,"ggfather2")
	;
}


relation mother(lref<string> ch, lref<string> mom) {
    lref<string> x;
    return child(ch,mom,x);
}

relation father(lref<string> ch, lref<string> pop) {
    lref<string> x;
    return child(ch,x,pop);
}

relation parent(lref<string> par, lref<string> kid) {
    return father(kid,par) || mother(kid,par);
}

relation ancestor(lref<string> ans, lref<string> des) {
	lref<string> X;
	return parent(ans,des)
		|| parent(X,des) && recurse(ancestor,ans,X)
	;
}

relation spouse(lref<string> husband, lref<string> wife) {
	lref<string> child;
	return father(child,husband) && mother(child,wife);
}


//--------------------------------------------------------
//   Tests : Family relationship
//--------------------------------------------------------

void test_all_husband_wives() {
    lref<string> h,w;
    relation r=spouse(h,w);
    string expectedH[] = {"father1", "father1", "father2", "father2", "gfather2"};
    string expectedW[] = {"mother1", "mother1", "mother2", "mother2", "gmother2"};
    int i=0;
    for(; r(); ++i) {
        //cout << *h << " " << *w <<"\n";
        if(i>4 || *h!=expectedH[i] || *w!=expectedW[i])
            throw "failed test_all_husband_wives";
    }
    if(i==0 || h.defined() || w.defined())
        throw "failed test_all_husband_wives";
}

void test_all_mothers() {
    lref<string> w, ch;
    relation r=mother(ch,w);
    string expected[] = {"mother1", "mother1", "mother2", "mother2", "gmother2"};
    int i=0;
    for(; r(); ++i) {
        //cout << *w <<"\n";
        if(i>4 || expected[i]!=*w)
          throw "failed test_all_mothers";
    }
    if(i==0 || w.defined() || ch.defined())
        throw "failed test_all_mothers";
}

void test_wifeof_father1() {
    lref<string> w;
    relation r=spouse("father1",w);
    string expected[] = {"mother1", "mother1"};
    int i=0;
    for(; r(); ++i) {
        // cout << *w <<"\n";
        if(i>1 || expected[i]!=*w)
          throw "failed test_wifeof_father1";
    }
    if(i==0 || w.defined())
        throw "failed test_wifeof_father1";
}

void test_husbandof_wife2() {
    lref<string> h;
    relation r=spouse(h,"mother2");
    string expected[] = {"father2", "father2"};
    int i=0;
    for(; r(); ++i) {
        //cout << *h <<"\n";
        if(i>1 || expected[i]!=*h)
          throw "failed test_husbandof_wife2";
    }
    if(i==0 || h.defined())
        throw "failed test_husbandof_wife2";
}

void test_childrenof_f1_m1() {
    lref<string> ch;
    relation r=child(ch,"mother1","father1");

    string expected[] = { "son1", "daughter1"};
    int i=0;
    for(; r(); ++i) {
       //cout << *ch <<"\n";
      if(i>1 || expected[i]!=*ch)
          throw "failed test_childrenof_f1_m1";
    }
    if(i==0 || ch.defined())
        throw "failed test_childrenof_f1_m1";
}

void test_ischildof_f1_m1() {
    relation r=child("daughter1","mother1","father1");
    if(!r())
        throw "failed test_ischildof_f1_m1";
}

void test_parent_s1() {
    lref<string> p;
    relation r=parent(p,"son1");
    string expected[] = { "father1", "mother1"};
    int i=0;
    for(; r(); ++i) {
       //cout << *p << " is the parent of son1\n";
      if(i>1 || expected[i]!=*p)
          throw "failed test_parent_s1";
    }
    if(i==0 || p.defined())
        throw "failed test_parent_s1";
}

void test_ancestors_s2() {
    lref<string> a;
    relation r = ancestor(a,"son2");
    string expected[] = { "father2", "mother2", "gfather2", "gmother2" };
    int i=0;
    for(; r(); ++i) {
        // cout << *a << " is an ancestor of son2!\n";
        if(i>3 || *a!=expected[i])
            throw "failed test_ancestors_s2";
    }
    if(i==0 || a.defined() )
        throw "failed test_ancestors_s2";
}

void test_gf2_ancestor_s2() {
    lref<string> a;
    relation r = ancestor("gfather2","son2");
    if(!r())
        throw "failed test_gf2_ancestor_s2";
}

//--------------------------------------------------------
//   Relational looping using && False()
//--------------------------------------------------------

bool push(std::stack<std::string>& coll, std::string val) {
    coll.push(val);
    return true;
}



// some_relation && False()  ... for looping through all solutions in some_relation
void test_relational_loop() {
    lref<string> ch, f;
    lref<stack<string> > lst = stack<string>();
    relation r = child(ch,"mother1", f) && /*write_f(ch+string(" ")) && */predicate(&push, lst, ch) && False();
    if( r() )
        throw "failed test_relational_loop";

    if( lst->size()!=2 )
        throw "failed test_relational_loop";
    if(lst->top()!="daughter1")
        throw "failed test_relational_loop";
    lst->pop();
    if(lst->top()!="son1")
        throw "failed test_relational_loop";
}

//--------------------------------------------------------
//   Graph Relations
//--------------------------------------------------------

relation edge(lref<int> n1, lref<int>n2) {
    return 
        eq(n1,1) && eq(n2,2)
        || eq(n1,2) && eq(n2,3)
        || eq(n1,3) && eq(n2,4)
        || eq(n1,4) && eq(n2,5)
    ;
}

relation path(lref<int> n1, lref<int> n2) {
    lref<int> intermediateNode;
    return  edge(n1, n2) 
         || edge(n1, intermediateNode) && recurse(path, intermediateNode, n2)
        ;
}

//--------------------------------------------------------
//   Test: Graph 
//--------------------------------------------------------

void test_allNodes_connectedTo_1() {
    lref<int> x;
    relation r=path(1,x);
	int expected[] = {2,3,4,5};
	int i=0;
	for(; r(); ++i) {
		// cout << "connected to " << *x << "\n";
		if(i>3 || *x!=expected[i])
			throw "failed test_allNodes_connectedTo_1";
	}
	if(i==0 || x.defined())
		throw "failed test_allNodes_connectedTo_1";

}

void test_path_1_2() {
	relation r=path(1,2);
	int i=0;
	for(; r(); ++i);
	if(i!=1)
		throw "failed test_path_1_2";
}

void test_path_1_5() {
	relation r=path(1,5);
	int i=0;
	for(; r(); ++i);
	if(i!=1)
		throw "failed test_path_1_5";
}

void test_nopath_1_5() {
	relation r=path(1,10);
	int i=0;
	for(; r(); ++i);
	if(i!=0)
		throw "failed test_path_1_5";
}


//--------------------------------------------------------
//   Integer Arithmetic  : Plus
//--------------------------------------------------------
// Note: Will return false if more than one argument is undefined.
relation plus_(lref<int> x, lref<int> y, lref<int> z) {
	return defined(x) && defined(y) && undefined(z) && eq_f(z,x+y)
	   ||  defined(x) && defined(z) && undefined(y) && eq_f(y,z-x)
	   ||  defined(y) && defined(z) && undefined(x) && eq_f(x,z-y)
	   ||  defined(x) && defined(y) && defined(z)   && eq_f(z,x+y)
	;
}

//--------------------------------------------------------
//   Tests : Integer Arithmetic  : Plus
//--------------------------------------------------------
void test_plus() {
	relation r = plus_(1,2,3);
	int i=0;
	for(; r(); ++i);
	if(i!=1)
		throw "failed test_plus";
}

void test_plus_z() {
	lref<int> z;
	relation r = plus_(1,2,z);
	int i=0;
	for(; r(); ++i) {
		if(i>0 || *z!=3)
			throw "failed test_plus_z";
	}
	if(i!=1 || z.defined())
		throw "failed test_plus_z";
}

void test_plus_y() {
	lref<int> y;
	relation r = plus_(-1,y,3);
	int i=0;
	for(; r(); ++i) {
		if(*y!=4)
			throw "failed test_plus_y";
	}
	if(i!=1 || y.defined())
		throw "failed test_plus_y";
}

void test_plus_x() {
	lref<int> x;
	relation r = plus_(x,-2,-3);
	int i=0;
	for(; r(); ++i) {
		if(*x!=-1)
			throw "failed test_plus_x";
	}
	if(i!=1 || x.defined())
		throw "failed test_plus_x";
}

//--------------------------------------------------------
//   Integer Arithmetic  : Multiply, Square
//--------------------------------------------------------

// Note : Is this a better model of implementation than plus() ?
relation multiply(int x, int y, int z) {
	if(z==x*y)
        return True();
	return False();
}
relation multiply(int x, int y, lref<int> z) {
	return eq(z,x*y);
}

relation multiply(int x, lref<int> y, int z) {
	return eq(y,z/x);
}

relation multiply(lref<int> x, int y, int z) {
	return eq(x,z/y);
}

// unlike plus(), above multiple() overloads correctly disallows ...
//        relation square(lref<int> n, lref<int> s) { .... }
relation square(int n, lref<int> s) {
	return multiply(n,n,s);
}

//--------------------------------------------------------
//   Tests : Integer Arithmetic  : Multiply, Square
//--------------------------------------------------------

void test_multiply() {
	relation r = multiply(1,1,1);
	int i=0;
	for(;r(); ++i);
	if(i!=1)
		throw "failed test_multiply";
}

void test_multiply_x() {
	lref<int> x;
	relation r = multiply(x,2,6);
	int i=0;
	for(;r(); ++i) {
		if(i>0 || *x!=3)
			throw "failed test_multiply_x";
	}
	if(i!=1)
		throw "failed test_multiply_x";
}

void test_multiply_z() {
	lref<int> z;
	relation r = multiply(3,2,z);
	int i=0;
	for(;r(); ++i) {
		if(i>0 || *z!=6)
			throw "failed test_multiply_z";
	}
	if(i!=1)
		throw "failed test_multiply_z";
}

void test_square() {
	relation r = square(7,49);
	int i=0;
	for(;r(); ++i);
	if(i!=1)
		throw "failed test_square";
}

void test_square_s() {
	lref<int> s;
	relation r = square(9,s);
	int i=0;
	for(;r(); ++i) {
		if(i>0 || *s!=81)
			throw "failed test_square_s";
	}
	if(i!=1)
		throw "failed test_square_s";
}

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
// Test : EQ
//--------------------------------------------------------
// structs A and B are inter convertible
struct A; 
struct B;

struct A {
    A() { }

    A(const B&)
    { }

    void operator=(const B&)
    { }

    bool operator==(const B&) const{
        return true;
    }
};

struct B {
    B() 
    { }

    B(const A&){}

    void operator =(const A&)
    { }

    bool operator==(const A&) const{
        return true;
    }
};

bool myStrCmp(const string&l , const string& r) {
    return l==r;
}

bool cmpAB(const A&, const B&) {
    return true;
}
void test_eq() {
	lref<string> ll , lr;
    string l, r;
    eq("l",r)();
    eq("l",r,&myStrCmp)();
    eq(l,"r")();
    eq(l,"r",&myStrCmp)();
    eq(l,r)();
    eq(l,r,&myStrCmp)();
    eq(ll,r)();
    eq(ll,r,&myStrCmp)();
    eq(ll,"r")();
    eq(ll,"r",&myStrCmp)();
    eq("l",lr)();
    eq("l",lr,&myStrCmp)();
    eq(l,lr)();
    eq(l,lr,&myStrCmp)();
    eq(ll,lr)();
    eq(ll,lr,&myStrCmp)();

    lref<A> la = A(); lref<B> lb; A a; B b;
    eq(la,lb)();
    eq(a,b)();
    eq(a,b, &cmpAB)();
    eq(la,b)();
    eq(la,b,&cmpAB)();
    eq(a,lb)();
    eq(a,lb,&cmpAB)();
    eq(la,la)();
    eq(la,la,&cmpAB)();
}

void test_eq_values() {
    relation r = eq<int>( 2, 2 );
	int i=0;
	for(; r(); ++i);
	if(i!=1)
		throw "failed test_eq_values 1";

    r = eq<int>( 2, 3 );
    int j=0;
	for(; r(); ++j);
	if(j!=0)
		throw "failed test_eq_values 2";
}

void test_eq_with_coll() {
    int a[] = { 1,2,3,4 };
    vector<int> v (a+0, a+4);
    lref<vector<int> > rv;
    relation r = eq(rv,v);
    int i=0;
    for(; r(); ++i) {
        if(v!=(*rv))
            throw "failed test_eq_with_coll 1";
    }
    if(i!=1 || rv.defined())
        throw "failed test_eq_with_coll 1";

    int b[] = { 1,2,3,4 };
    vector<int> v2 (b+0, b+4);
    lref<vector<int> > rv2;
    if(eq(rv2,v2)()) {
        if(v2!=(*rv2))
            throw "failed test_eq_with_coll 2";
    }
    else
        throw "failed test_eq_with_coll 2";
}

inline bool cmpi(int i, int j) {
    return i==j;
}

void test_eq_seq() {
    { // generate container with elements
        int ai[] = {1,2,3};
        lref<vector<int> > vi;
        relation r = eq_seq(vi, ai, ai+3);
	    int i=0;
	    for(; r(); ++i) {
            if(i>0 || (*vi).at(0)!=1 || (*vi).at(1)!=2 || (*vi).at(2)!=3 )
                throw "failed test_eq_coll 1";
        }
	    if(i!=1 || vi.defined())
		    throw "failed test_eq_coll 1";
    }
    {// compare using custom comparator
    const int ai[] = {1,2,3};
    lref<vector<int> > vi = vector<int>(ai+0, ai+3);

    relation r = eq_seq(vi, ai, ai+3, &cmpi);
	if(!r())
        throw "failed test_eq_coll 2";
    }
    {
    const int bi[] = {1,2,3};
    lref<vector<int> > vj = vector<int>(bi, bi+3);
    int ci[] = {1,2,3};
    relation r2 = eq_seq(vj, ci, ci+3);
	int j=0;
	for(; r2(); ++j);
	if(j!=1 || !vj.defined())
		throw "failed test_eq_coll 3";
    }
    {
    const int di[] = {1,2,3};
    lref<vector<int> > vk = vector<int>(di, di+3);
    int ei[] = {3,2,3};
    relation r3 = eq_seq(vk, ei, ei+3);
	int k=0;
	for(; r3(); ++k);
	if(k!=0 || !vk.defined())
		throw "failed test_eq_coll 4";
    }
}

int compute(int j, int k) {
    return j/k-1;
}

struct Compute {
    int operator()(int j, int k) {
        return j/k-1;
    }
};

void test_eq_f() {
    { // unify .. should pass
    lref<int> refi;
    relation r = eq_f( refi, ::castor::detail::bind<int>(plus<int>(), 6, 3) );
	int i=0;
	for(; r(); ++i) {
        if(i>0 || *refi!=6+3)
            throw "failed test_eq_f 1";
    }
	if(i!=1 || refi.defined())
		throw "failed test_eq_f 1";
    }
    { // comparison .. should pass
    lref<int> refk = 9;
    relation r2 = eq_f( refk, ::castor::detail::bind<int>(plus<int>(), 6, 3) );
	int k=0;
	for(; r2(); ++k) {
        if(k>0 || *refk!=6+3)
            throw "failed test_eq_f 2";
    }
	if(k!=1 || !refk.defined())
		throw "failed test_eq_f 2";
    }
    { // comparison .. should fail
    lref<int> refj = 1;
    relation r3 = eq_f( refj, ::castor::detail::bind<int>(plus<int>(), 6, 3) );
	int j=0;
	for(; r3(); ++j);
	if(j!=0 || !refj.defined())
		throw "failed test_eq_f 3";
    }
    { // regular function
    lref<int> li, lj, lk;
    relation r = eq(lj,6) && eq(lk,2) && eq_f(li,&compute, lj, lk);
    if(!r() && *li!=2)
       throw "failed test_eq_f 4";
    }
    { // function objects
    lref<int> li, lj, lk;
    relation r = eq(lj,6) && eq(lk,2) && eq_f(li, Compute(),lj,lk);
    if(!r() && *li!=2)
       throw "failed test_eq_f 5";
    }
    { // ILE
    lref<int> li, lj, lk;
    relation r = eq(lj,6) && eq(lk,2) && eq_f(li,lj/lk-1);
    if(!r() && *li!=2)
       throw "failed test_eq_f 6";
    }
}

struct Compute2 {
    int j; 
    Compute2(int j) : j(j) 
    {}
    int apply(int k) const {
        return j/k-1;
    }
};

void test_eq_mf() {
    {
    lref<int> refi;
    lref<Compute2> comp2 = Compute2(6);
    lref<int> two=2;
    relation r = eq_mf(refi, comp2, &Compute2::apply, two);
	int i=0;
	for(; r(); ++i) {
        if(i>0 || *refi!=2)
            throw "failed test_eq_mf 1";
    }
	if(i!=1 || refi.defined())
		throw "failed test_eq_mf 1";
    }
    {
    lref<int> refi;
    lref<Compute2> comp2 = Compute2(6);
    lref<int> li=2;
    relation r = eq_mf(refi, comp2, &Compute2::apply, li);
	int i=0;
	for(; r(); ++i) {
        if(i>0 || *refi!=2)
            throw "failed test_eq_mf 1";
    }
	if(i!=1 || refi.defined())
		throw "failed test_eq_mf 1";
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
    operator int() {
        return i;
    }
};

void test_Create_with() {
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
}


void test_Create() {
    {
    if(create<pair<int,int> >(1,2)().first!=1)
        throw "failed test_create_with 1";
    if(create<My>()().i != 0)
        throw "failed test_create_with 1";
    if(create<My>(1)().i != 1)
        throw "failed test_create_with 1";
    if(create<My>(1,2)().i != 2)
        throw "failed test_create_with 1";
    if(create<My>(1,2,3)().i != 3)
        throw "failed test_create_with 1";
    if(create<My>(1,2,3,4)().i != 4)
        throw "failed test_create_with 1";
    if(create<My>(1,2,3,4,5)().i != 5)
        throw "failed test_create_with 1";
    if(create<My>(1,2,3,4,5,6)().i != 6)
        throw "failed test_create_with 1";
    int i=1,j=2,k=3; lref<int> l = 4, m =5;
    if(create<My>(i,j,k,l,m,6)().i != 6)
        throw "failed test_create_with 1";
    }
    {
    lref<string> hello = "hello", world="world";
    if((create<string>(hello)+world)() != (hello+world)())
        throw "failed test_create_with 2";

    lref<string> dear="dear";
    if((create<string>(hello)+world)() == (hello+dear)())
        throw "failed test_create_with 2";
    }
}

//--------------------------------------------------------
//  Tests  : Merge
//--------------------------------------------------------
void test_merge() {
    { // generate : left is empty vector
    int ai[] = {1,2,3};
    lref<vector<int> >  vi, vj = vector<int>(ai, ai+0);
    lref<vector<int> > m;
    relation r = castor::merge<vector<int> >(vi,vj,m);
    int i=0;
    for(; r(); ++i) {
        if((*m)!=(*vj))
            throw "failed test_merge 1";
    }
    if(i!=1)
        throw "failed test_merge 1";
    }

    { // generate : left&right are non-empty vector
    int aj[] ={1,3};
    int bj[]={2,4};
    lref</*const*/ vector<int> > rlj= vector<int>(aj+0, aj+2) , rrj= vector<int>(bj+0,bj+2); 
    lref<vector<int> > m2; 
    relation rj =  merge<vector<int> >(rlj,rrj,m2);
    int j=0;
    for(; rj(); ++j) {
        int expect[] = {1,2,3,4};
        vector<int> expected(expect+0, expect+4);
        if(*m2!=expected)
            throw "failed test_merge 2";
    }
    if(j!=1)
        throw "failed test_merge 2";
    }

    { // compare
    int ak[] ={1,3}, bk[]={2,4};
    int expect[] = {1,2,3,4};
    vector<int> expected3(expect+0, expect+4);

    lref</*const*/ vector<int> > rlk= vector<int>(ak+0, ak+2) , rrk= vector<int>(bk+0,bk+2);
    lref<vector<int> > m3; 
    relation rk =  merge<vector<int> >(rlk,rrk,m3);
    int k=0;
    for(; rk(); ++k) {
        if(*m3!=expected3)
            throw "failed test_merge 3";
    }
    if(k!=1)
        throw "failed test_merge 3";
    }

    { // generate first seq given a seq and a
    int ak[] ={1,3}, bk[]={2,4};
    int expect[] = {1,2,3,4};
    vector<int> expected3(expect+0, expect+4);

    lref</*const*/ vector<int> > rlk= vector<int>(ak+0, ak+2) , rrk= vector<int>(bk+0,bk+2);
    lref<vector<int> > m3; 
    relation rk =  merge<vector<int> >(rlk,rrk,m3);
    int k=0;
    for(; rk(); ++k) {
        if(*m3!=expected3)
            throw "failed test_merge 3";
    }
    if(k!=1)
        throw "failed test_merge 3";
    }
}


//--------------------------------------------------------
// Tests : next()  : 
//--------------------------------------------------------
void test_next() {
    {
    // should unify successfully
    int ai[] = {1,2,3};
    lref<int*> n;
    relation r = next(ai+0, n);
	int i=0;
	for(; r(); ++i) {
		if(i>0 || *(*n)!=2)
			throw "failed test_next 1";
	}
	if(i!=1 || n.defined())
		throw "failed test_next 1";
	}
	{
	// should compare successfully
	int aj[] = {1,2,3};
	lref<int*> n = aj+1;
	relation r2 = next(aj+0,n);
	int j=0;
	for(; r2(); ++j) {
		if(j>0 || *(*n)!=2)
			throw "failed test_next 2";
	}
	if(j!=1 || !n.defined())
		throw "failed test_next 2";
	}
	{
	// should not compare succesfully
	int ak[] = {1,2,3};
	lref<int*> n = ak+1;
	relation r3 = next(n, n);  // element cannot be next of itself
    int k=0;
    for(; r3(); ++k);
    if(k!=0 || !n.defined())
        throw "failed test_next 3";
    }
    { //on simple values
    lref<int> n;
    next(1,n)();
    if(*n!=2)
        throw "failed test_next 4";
    }
    { //on simple values 2
    if(! next(1,2)())
        throw "failed test_next 4";
    }
}

void test_prev() {
    {// generate
    int ai[] = {1,2,3};
    lref<int*> p;
    relation r = prev(ai+1,p);
    int i=0;
    for(; r(); ++i) {
        if(i>0 || *(*p)!=1)
            throw "failed test_prev 1";
    }
    if(i!=1 || p.defined())
        throw "failed test_prev 1";
    }
    {// test  : aj+2 is next of aj+1
   int aj[] = {1,2,3};
    lref<int*> pj = aj+1;
    relation r2 = prev(aj+2, pj);
    int j=0;
    for(; r2(); ++j) {
        if(j>0 || *(*pj)!=2)
            throw "failed test_prev 2";
    }
    if(j!=1 || !pj.defined())
        throw "failed test_prev 2";
    }
    {// test - failing
    int ak[] = {1,2,3};
    lref<int*> pk = ak+1;
    relation r3 = prev(ak+1,pk);  // element cannot be prev of itself
    int k=0;
    for(; r3(); ++k);
    if(k!=0 || !pk.defined())
        throw "failed test_prev 3";
    }
}

//--------------------------------------------------------
// Tests : head(), tail(), head_tail()
//--------------------------------------------------------
void testHead() {
    {// generate
    std::vector<int> v;
    v.push_back(2);
    lref</*const*/ vector<int> > rvi =v; //= std::vector<int>(1);
    
    lref<int> h;
    relation r = head(rvi,h);
    int i=0;
    for(; r(); ++i) {
        if(*h!=2)
            throw "failed testHead 1";
    }
    if(i!=1 || h.defined())
        throw "failed testHead 1";
    }
    {//compare
    vector<int> vj;
    vj.push_back(2);
    lref</*const*/ vector<int> > rvj = vj;
    lref<int> h2=2;
    relation r2 = head(rvj,h2);
    int j=0;
    for(; r2(); ++j) {
        if(*h2!=2)
            throw "failed testHead 2";
    }
    if(j!=1 || !h2.defined())
        throw "failed testHead 2";
    }
    {// compare - failing
    std::vector<int> vk;
    vk.push_back(9);
    lref</*const*/ vector<int> > rvk = vk;
    lref<int> h3=2;
    relation r3 = head(rvk,h3);
    int k=0;
    for(; r3(); ++k);
    if(k!=0 || !h3.defined())
        throw "failed testHead 3";
    }
    {// head of empty list
    lref<int> hl;
    lref</*const*/ vector<int> >vl = vector<int>();
    relation rl =  head(vl,hl);
    if(rl())
        throw "failed testHead 4";
    }
    {// head of undefiend list
    lref<int> hm;
    lref</*const*/ vector<int> >vm;
    relation rm =  head(vm,hm);
    int passed=false;
    try { rm(); }
    catch (...) { passed=true; }
    if(!passed)
        throw "failed testHead 5";
    }
}

void test_head_list() {
    {// generate
    vector<int> items; items.push_back(1); items.push_back(2); items.push_back(3);
    lref</*const*/ vector<int> > v = items;
    lref<vector<int> > h;
#if defined(__BCPLUSPLUS__) || defined(__GNUG__)
    relation r1 = head_n<vector<int>, vector<int> > (v,2,h);
#else
    relation r1 = head_n(v,2,h);
#endif
    int i=0;
    for(;r1(); ++i) {
        vector<int> expected; expected.push_back(1); expected.push_back(2);
        if(*h!=expected)
            throw "failed test_head_list";
    }
    if(i!=1 || h.defined())
        throw "failed test_head_list";
    }

    {// compare
    vector<int> items; items.push_back(1); items.push_back(2); items.push_back(3);
    lref</*const*/ vector<int> > v = items;
    lref<vector<int> > h = vector<int>(); (*h).push_back(1); (*h).push_back(2);
#if  defined(__BCPLUSPLUS__) || defined(__GNUG__)
    relation r = head_n<vector<int>, vector<int> >(v,2,h);
#else
    relation r = head_n(v,2,h);
#endif
    int i=0;
    for(;r(); ++i);
    if(i!=1)
        throw "failed test_head_list";
    }
}

void test_tail_list() {
    {// generate
    vector<int> items; items.push_back(1); items.push_back(9); items.push_back(2); items.push_back(3);
    lref</*const*/ vector<int> > v = items;
    lref<vector<int> > t;
#if  defined(__BCPLUSPLUS__) || defined(__GNUG__)
    relation r1 = tail_n<vector<int>, vector<int> >(v,2,t);
#else
    relation r1 = tail_n(v,2,t);
#endif

    int i=0;
    for(;r1(); ++i) {
        vector<int> expected; expected.push_back(2); expected.push_back(3);
        if(*t!=expected)
            throw "failed test_tail_list 1";
    }
    if(i!=1 || t.defined())
        throw "failed test_tail_list 1";
    }

    {// compare
    vector<int> items; items.push_back(1); items.push_back(2); items.push_back(3);
    lref</*const*/ vector<int> > v = items;
    lref<vector<int> > t = vector<int>(); (*t).push_back(2); (*t).push_back(3);
#if  defined(__BCPLUSPLUS__) || defined(__GNUG__)
	relation r = tail_n<vector<int>, vector<int> >(v,2,t);
#else
	relation r = tail_n(v,2,t);
#endif
	int i=0;
	for(;r(); ++i);
	if(i!=1)
		throw "failed test_tail_list 2";
	}
	{ // tail of length 0 of a list is an empty list
	  lref<vector<int> > t, v=vector<int>();
#if  defined(__BCPLUSPLUS__) || defined(__GNUG__)
	  relation r = tail_n<vector<int>, vector<int> >(v,0,t);
#else
	  relation r = tail_n(v,0,t);
#endif
      if(!r())
        throw "failed test_tail_list 3";
      if(!t.defined())
        throw "failed test_tail_list 3";
    }
}

void test_tail() {
    {
    // generate
    int arr[] = { 3, 4, 5 };
    lref</*const*/ vector<int> > rv = vector<int>(arr+0, arr+3);
    lref<vector<int> > tl;
    relation r = tail(rv,tl);
    int i=0;
    for(; r(); ++i) {
        vector<int> expected (arr+1, arr+3);
        if(*tl!=expected)
            throw "failed test_tail 1";
    }
    if(i!=1 || tl.defined())
       throw "failed test_tail 1";
    }
    {
    // test
    int arrj[] = { 1, 2, 3 };
    lref</*const*/ vector<int> > rvj = vector<int>(arrj+0, arrj+3);
    lref<vector<int> > tlj = vector<int>(arrj+1, arrj+3);
    relation r2 = tail(rvj,tlj);
    int j=0;
    for(; r2(); ++j);
    if(j!=1 || !tlj.defined())
        throw "failed test_tail 2";
    }
    {
    // tail of empty list
    lref<vector<int> > tk;
    lref</*const*/ vector<int> >vk = vector<int>();
    relation r3 =  tail(vk,tk);
    if(r3())
        throw "failed test_tail 3";
    }
    {
    // tail of undefined list
    lref<vector<int> > tm;
    lref</*const*/ vector<int> >vm;
    relation rm =  tail(vm,tm);
    int passed=false;
    try { rm(); }
    catch (...) { passed=true; }
    if(!passed)
        throw "failed test_tail 4";
    }
}


void test_head_tail() {
    {
    // generate
    int arr[] = {1,2,3};
    vector<int> v (arr+0, arr+3);
    lref</*const*/ vector<int> > rv = v;
    lref<int> h;
    lref<vector<int> > t;
    relation r = head_tail(rv,h,t);
    int i=0;
    for(; r(); ++i) {
        int expected_head = arr[0];
        vector<int> expected_tail (arr+1, arr+3);
        if(i>1 || *h!=expected_head || *t!=expected_tail)
            throw "failed test_head_tail 1";
    }
    if(i!=1 || h.defined() || t.defined() )
        throw "failed test_head_tail 1";
    }
    {
    //test
    int arrj[] = {1,2,3};
    lref</*const*/ vector<int> > rvj = vector<int> (arrj+0, arrj+3);
    lref<int> hj = arrj[0];
    lref<vector<int> > tj = vector<int> (arrj+1, arrj+3);
#if  defined(__BCPLUSPLUS__) || defined(__GNUG__)
    relation r2 = head_tail<vector<int>,vector<int> >(rvj, arrj[0], tj);
#else
    relation r2 = head_tail(rvj, arrj[0], tj);
#endif
    int j=0;
    for(; r2(); ++j);
    if(j!=1 || !tj.defined() )
        throw "failed test_head_tail 2";
    }
}

void test_head_n_tail() {
    {//test
    int arr[] = {1,2,3};
    lref<vector<int> > lv = vector<int>(arr+0, arr+3);
    lref<vector<int> > h, t;
#if  defined(__BCPLUSPLUS__) || defined(__GNUG__)
    relation r = head_n_tail<vector<int>, vector<int> >(lv, 1, h, t);
#else
    relation r = head_n_tail(lv, 1, h, t);
#endif
    int i=0;
    for(; r(); ++i) {
        if( h->size()!=1  || (*h)[0]!=1 || t->size()!=2 || (*t)[0]!=2 || (*t)[1]!=3 )
            throw "failed test_head_n_tail 1";
    }
    if(i!=1)
        throw "failed test_head_n_tail 1";
    }
    {//generate heads and tails
    int arr[] = {1,2,3};
    lref<vector<int> > lv = vector<int>(arr+0, arr+3);
    lref<vector<int> > h, t;
    lref<vector<int>::size_type> n;
    relation r = head_n_tail(lv, n, h, t);
    int i=0;
    for(; r(); ++i);
    if(i!=4)
        throw "failed test_head_n_tail 2";
    }
}

void test_insert() {
    { // 1 .. input list is simple list<int>
    list<int> li; li.push_back(1); li.push_back(2);
    lref<list<int> > insertedList;
#if  defined(__BCPLUSPLUS__) || defined(__GNUG__)
    relation r = insert<list<int> >(9, li.begin(), li.end(), insertedList);
#else
    relation r = insert(9, li.begin(), li.end(), insertedList);
#endif    
    int expected[][3] = { {9,1,2}, {1,9,2}, {1,2,9} };
    int i=0;
    for(; r(); ++i)
        if( !equal(expected[i], expected[i]+3,insertedList->begin()) )
            throw "failed test_insert 1";
    if(i!=3)
        throw "failed test_insert 1";
    }
    { // 2 .. input list is simple lref<list<int> >
    lref<list<int> > li = list<int>(); li->push_back(1); li->push_back(2);
    lref<list<int> > insertedList;
#if  defined(__BCPLUSPLUS__) || defined(__GNUG__)
    relation r = insert<list<int> >(9, li->begin(), li->end(), insertedList);
#else
    relation r = insert(9, li->begin(), li->end(), insertedList);
#endif
    int expected[][3] = { {9,1,2}, {1,9,2}, {1,2,9} };
    int i=0;
    for(; r(); ++i)
        if( !equal(expected[i], expected[i]+3,insertedList->begin()) )
            throw "failed test_insert 2";
    if(i!=3)
        throw "failed test_insert 2";
    }
    { // 3 .. test ...success
    lref<list<int> > li = list<int>(); li->push_back(1); li->push_back(2);
    int values[] = {1,9,2};
    lref<list<int> >insertedList = list<int>(values+0, values+3);
    relation r = insert<list<int> >(9, li->begin(), li->end(), insertedList);
    int i=0;
    for(; r(); ++i);
    if(i!=1)
        throw "failed test_insert 3";
    }
    { // 4 .. test.. failure
    lref<list<int> > li = list<int>(); li->push_back(1); li->push_back(2);
    int values[] = {1,2,3};
    lref<list<int> > insertedList = list<int>(values+0, values+3);
#if  defined(__BCPLUSPLUS__) || defined(__GNUG__)
    relation r = insert<list<int> >(9, li->begin(), li->end(), insertedList);
#else
    relation r = insert(9, li->begin(), li->end(), insertedList);
#endif
    int i=0;
    for(; r(); ++i);
    if(i!=0)
        throw "failed test_insert 4";
    }
}

void test_insert_seq() {
    { // 1 .. input list is simple list<int>
    list<int> li; li.push_back(1); li.push_back(2);
    list<int> values; values.push_back(7); values.push_back(8);
    lref<list<int> > insertedList;
#if  defined(__BCPLUSPLUS__) || defined(__GNUG__)
    relation r = insert_seq<list<int> >(values.begin(), values.end(), li.begin(), li.end(), insertedList);
#else
    relation r = insert_seq(values.begin(), values.end(), li.begin(), li.end(), insertedList);
#endif
    int expected[][4] = { {7,8,1,2}, {1,7,8,2}, {1,2,7,8} };
    int i=0;
    for(; r(); ++i)
        if( !equal(expected[i], expected[i]+3,insertedList->begin()) )
            throw "failed test_insert_seq 1";
    if(i!=3)
        throw "failed test_insert_seq 1";
    }
    { // 2 .. input list is simple lref<list<int> >
    lref<list<int> > li = list<int>(); li->push_back(1); li->push_back(2);
    list<int> values; values.push_back(7); values.push_back(8);
    lref<list<int> > insertedList;
#if  defined(__BCPLUSPLUS__) || defined(__GNUG__)
    relation r = insert_seq<list<int> >(values.begin(), values.end(), li->begin(), li->end(), insertedList);
#else
    relation r = insert_seq(values.begin(), values.end(), li->begin(), li->end(), insertedList);
#endif
    int expected[][4] = { {7,8,1,2}, {1,7,8,2}, {1,2,7,8} };
    int i=0;
    for(; r(); ++i)
        if( !equal(expected[i], expected[i]+3,insertedList->begin()) )
            throw "failed test_insert _seq 2";
    if(i!=3)
        throw "failed test_insert_seq 2";
    }
    { // 3 .. test ...success
    lref<list<int> > li = list<int>(); li->push_back(1); li->push_back(2);
    list<int> values; values.push_back(7); values.push_back(8);
    int insValues[] = {1,7,8,2};
    lref<list<int> > insertedList = list<int>(insValues+0, insValues+4);
    relation r = insert_seq<list<int> >(values.begin(), values.end(), li->begin(), li->end(), insertedList);
    int i=0;
    for(; r(); ++i);
    if(i!=1)
        throw "failed test_insert_seq 3";
    }
    { // 4 .. test.. failure
    lref<list<int> > li = list<int>(); li->push_back(1); li->push_back(2);
    list<int> values; values.push_back(7); values.push_back(8);
    int insValues[] = {1,2,3,4};
    lref<list<int> > insertedList = list<int>(insValues+0, insValues+4);
    relation r = insert_seq<list<int> >(values.begin(), values.end(), li->begin(), li->end(), insertedList);
    int i=0;
    for(; r(); ++i);
    if(i!=0)
        throw "failed test_insert_seq 4";
    }
}

void test_inc_dec() {
    {// simple inc()
    lref<int> li = 2;
    relation r = inc(li);
    if( !r() )
        throw "failed test_inc_dec";
    if(*li!=3)  throw "failed test_inc_dec";
    if(r())     throw "failed test_inc_dec";
    if(*li!=2)   throw "failed test_inc_dec";
    }
    {// simple dec()
    lref<int> li = 2;
    relation r = dec(li);
    if( !r() )  throw "failed test_inc_dec";
    if(*li!=1)  throw "failed test_inc_dec";
    if(r())     throw "failed test_inc_dec";
    if(*li!=2)   throw "failed test_inc_dec";
    }    
    {// inc(lre<ptr>)
    int arr[] = {1,2,3,4};
    lref<int*> ptr = arr;
    relation r = inc(ptr);
    if( !r() )
        throw "failed test_inc_dec";
    if(**ptr!=2)
        throw "failed test_inc_dec";
    if(r())
        throw "failed test_inc_dec";
    }
    {// dec(lre<ptr>)
    int arr[] = {1,2,3,4};
    lref<int*> ptr = arr+2;
    relation r = dec(ptr);
    if( !r() )
        throw "failed test_inc_dec";
    if(**ptr!=2)
        throw "failed test_inc_dec";
    if(r())
        throw "failed test_inc_dec";
    }
}
//--------------------------------------------------------
//  Tests  : sequence
//--------------------------------------------------------
void test_sequence() { 
    { // copmare ... unequal number of elements .. L < R
    int a[] = {1,2,3};
    lref<list<int> > rli = list<int>(a+0,a+3);
    lref<int> i1 =1;
    list<int> il2; il2.push_back(2);
    relation r = sequence(rli)(i1)(il2.begin(), il2.end())(3)(4);
    if(r())
        throw "failed test_sequence 1";
    }
    { // compare ... unequal number of elements .. L > R
    int b[] = {1,2,3,4};
    lref<list<int> > rly = list<int>(b+0, b+4);
    lref<int> y1 =1;
    list<int> yl2; yl2.push_back(2);
    relation rm = sequence(rly)(y1)(yl2.begin(), yl2.end())(3);
    if(rm())
        throw "failed test_sequence 2";
    }
    { // compare ...  equal number of identical elements
    int c[] = {1,2,3,4};
    lref<list<int> > rlj = list<int>(c+0,c+4);
    lref<int> j2=2;
    list<int> jl3; jl3.push_back(3); jl3.push_back(4);
    relation rj = sequence(rlj)(1)(j2)(jl3.begin(), jl3.end());
    if(!rj())
        throw "failed test_sequence 3";
    }
    { // compare ...  differring elements
    int n[] = {1,2,3};
    lref<list<int> > rln = list<int>(n+0,n+3);
    lref<int> n2=2;
    relation rn = sequence(rln)(1)(n2)(4);
    if(rn())
        throw "failed test_sequence 4";

    }
    { // compare partial collection (using ignore())
    int d[] = {1,2,3,4};
    lref<list<int> > rlk = list<int>(d+0,d+4);
    lref<int> k2 = 2; 
    list<int> l3; l3.push_back(d[2]);
    lref<list<int> > k3 = l3;
    relation rk = sequence(rlk)(1)(k2)(k3).ignore();
    if(rk())
       throw "failed test_sequence 5";

    }
    { // generate list
    int e[] = {1,2,3};
    lref<list<int> > rlx ;
    lref<int> x1=1;
    list<int> lx3; lx3.push_back(e[2]);
    relation rx = sequence(rlx)(x1)(2)(lx3.begin(), lx3.end());
    int x=0;
    for(; rx(); ++x) {
        if(x>1 ||  *rlx != list<int>(e+0, e+3))
            throw "failed test_sequence 6";
    }
    if(x!=1 || rlx.defined())
       throw "failed test_sequence 6";
    }
    { // generate list : using mix of values, lref, lists,  lref<vector>
    lref<vector<string> > ln;
    string s = "One";
    lref<string> lrs = "Two";
    list<string> ls;
    ls.push_back("Three"); ls.push_back("Four");
    vector<string> lsTemp; 
    lsTemp.push_back("Five"); lsTemp.push_back("Six");
    lref<vector<string> > lrls=lsTemp;
    // create the sequence
    relation numbers = sequence(ln)("Zero")(s)(lrs)(ls.begin(), ls.end())(lrls);
    // see what it produces
    if(!numbers())
        throw "failed test_sequence 7";
    string expected[] ={ "Zero", "One", "Two", "Three", "Four", "Five", "Six"};
    if(!equal(expected, expected+7, ln->begin() ))
        throw "failed test_sequence 7";
    }
    { // generate using lref<iterator> 
    list<int> li; li.push_back(1); li.push_back(2); li.push_back(3);
    lref<list<int> > lrli1, lrli2=li;
    lref<list<int>::iterator> itrB, itrE;
    if(! (begin(lrli2,itrB) && end(lrli2, itrE) && sequence(lrli1)(itrB, itrE))() )
        throw "failed test_sequence 8";
    if(!equal(li.begin(), li.end(), lrli1->begin()))
        throw "failed test_sequence 8";
    }
    { // compare using lref<iterator>  
    list<int> li; li.push_back(1); li.push_back(2); li.push_back(3);
    lref<list<int> > lrli=li;
    lref<list<int>::iterator> itrB, itrE;
    if(! (begin(lrli,itrB) && end(lrli, itrE) && sequence(lrli)(itrB, itrE))() )
        throw "failed test_sequence 9";
    if(!equal(li.begin(), li.end(), lrli->begin()))
        throw "failed test_sequence 9";
    }
    { // generate using iterators
    list<int> li; li.push_back(1); li.push_back(2); li.push_back(3);
    lref<list<int>::iterator> itrB, itrE;
    lref<list<int> > lrli1, lrli2=li;
    if(! (begin(lrli2, itrB) && end(lrli2, itrE) && sequence(lrli1)(li.begin(), li.end()))() )
        throw "failed test_sequence 10";
    if(!equal(li.begin(), li.end(), lrli1->begin()))
        throw "failed test_sequence 10";
    }
    { // test for empty sequence
    lref<list<int> > li = list<int>();
    if(!sequence<list<int> >(li)())
        throw "failed test_sequence 11";
    }
    { // generate for empty sequence
    lref<list<int> > lrli;
    if(!sequence(lrli)())
        throw "failed test_sequence 12";
    if(!lrli->empty())
        throw "failed test_sequence 12";
    }

}



// Test: Size of collections
void test_size() {
    {
    // generate
    lref<vector<int>::size_type> s;
    lref</*const*/ vector<int> > coll = vector<int> ();
    relation r=size(coll,s);
    int i=0;
    for(; r(); ++i) {
        if(i>0 || *s!=0)
            throw "failed test_size 1";
    }
    if(i!=1)
        throw "failed test_size 1";
    }
    {
    // generate
    lref<vector<int>::size_type> s2;
    lref</*const*/ vector<int> > coll2;
    relation r2=size(coll2,s2);
    int passed2=false;
    try { r2(); }
    catch (...) { passed2=true; }
    if(!passed2)
        throw "failed test_size 2";
    }
    {
    // compare
    lref</*const*/ vector<int> > coll3 = vector<int> ();
#if  defined(__BCPLUSPLUS__) || defined(__GNUG__)
    relation r3=size<vector<int> >(coll3,0);
#else
    relation r3=size(coll3,0);
#endif
    int j=0;
    for(; r3(); ++j) {
        if(j>0)
            throw "failed test_size 3";
    }
    if(j!=1)
        throw "failed test_size 3";
    }
}


//--------------------------------------------------------
//  Tests  : Finite Automata
//--------------------------------------------------------

struct Nfa {
// all tranistions in the NFA => ((ab)*ba)|b
static relation transition(lref<int> state1,  lref<char> input, lref<int> state2) {
return eq(state1, 0) && eq(input, 'a') && eq(state2, 1)
   ||  eq(state1, 0) && eq(input, 'b') && eq(state2, 2)
   ||  eq(state1, 0) && eq(input, 'b') && eq(state2, 4)
   ||  eq(state1, 1) && eq(input, 'b') && eq(state2, 0)
   ||  eq(state1, 2) && eq(input, 'a') && eq(state2, 3) ;
}
// all final states of the NFA
static relation final(lref<int> state) {
    return eq(state, 3) || eq(state, 4);
}
};
   
// rule determining successful exuecution of a FA
template<typename FA>
relation runFA(lref<string> input,  lref<int> startState=0)
{
  lref<char> firstChar;
  lref<string> rest;
  lref<int> nextState;
#ifdef __BCPLUSPLUS__
  relation (*self)(lref<string>,  lref<int>) = &runFA<FA>;
  return eq(input, "") && FA::final(startState)
	  || head(input,firstChar) && FA::transition(startState, firstChar, nextState) && tail(input, rest) && recurse(self, rest, nextState)
	  ;
#else
  return eq(input, "") && FA::final(startState)
	  || head(input,firstChar) && FA::transition(startState, firstChar, nextState) && tail(input, rest) && recurse(runFA<FA>, rest, nextState)
	  ;
#endif
}

void test_FA() {
    if( ! runFA<Nfa>("abba")() )
        throw "failed test_FA";
    if( runFA<Nfa>("aabba")() )
        throw "failed test_FA";

}

//--------------------------------------------------------
//  Tests  : recurse relation for member relations
//--------------------------------------------------------
struct MyRel {
    relation decrementToUnity(lref<int> j) {
        return  eq(j,1)
            || predicate(j!=1) && dec(j) && recurse_mf(this, &MyRel::decrementToUnity, j);
    }
    
    relation decrementToUnity_c(lref<int> j) const {
        return eq(j,1)
            || predicate(j!=1) && dec(j) && recurse_mf(this, &MyRel::decrementToUnity_c, j);
    }

    relation decrementToUnity_c2(lref<int> j, lref<int>& blah)  const {
        return eq(j,1)
            || predicate(j!=1) && dec(j) && recurse_mf(this, &MyRel::decrementToUnity_c2, j, blah);
    }

};

void test_recurse_mf() {
    {//non const obj, non const member relation
    MyRel mr;
    lref<int> value=4;
    mr.decrementToUnity(value)();
    if(*value!=1)
        throw "failed test_recurse_mf 1";
    }
    { //non const obj, const member relation
    MyRel mr;
    lref<int> value=4;
    mr.decrementToUnity_c(value)();
    if(*value!=1)
        throw "failed test_recurse_mf 2";
    }
    { //const obj, const member relation
#if defined(__GNUG__)
    const MyRel mr = MyRel();
#else
    const MyRel mr;
#endif
    lref<int> value=4;
    mr.decrementToUnity_c(value)();
    if(*value!=1)
        throw "failed test_recurse_mf 3";
    }
}

//--------------------------------------------------------
//  Tests  : predicate relation for converting predicates to relations
//--------------------------------------------------------
bool isEven(int num) {
    return num%2 == 0;
}

bool isEven_lref(lref<int> num) {
    return (*num)%2 == 0;
}

struct Functor {
    int j;
    Functor() : j(4)
    { }

    bool isEven(int i) {
        return (i+j)%2 == 0;
    }

    bool isEven_c(int i) const {
        return i%2 == 0;
    }

    bool allEqual(int i, const int j, int k, int l, lref<int> m, const int n) const {
        return i==j==k==l==*m==n;
    }

    bool ternaryConstPred(int i, const int j, int k) const {
        return true;
    }
    static bool staticMembPred(int i) {
        return true;
    }
};

void test_predicate() {
    {// global predicate : success
    relation r = predicate(isEven,2);
    int i=0;
    for(; r(); ++i);
    if(i!=1)
        throw "failed test_predicate 1";
    }
    { // global predicate : failure
    relation r = predicate(isEven,3);
    int i=0;
    for(; r(); ++i);
    if(i!=0)
        throw "failed test_predicate 2";
    }
    {// global predicate taking lref : success
    lref<int> j=2;
    relation r = predicate(isEven_lref,j);
    int i=0;
    for(; r(); ++i);
    if(i!=1)
        throw "failed test_predicate 3";
    }
    {// global predicate taking lref : failure
    relation r = predicate(isEven_lref,3);
    int i=0;
    for(; r(); ++i);
    if(i!=0)
        throw "failed test_predicate 4";
    }
    { // static member predicate
    relation r = predicate(&Functor::staticMembPred, 2);
    int i=0;
    for(; r(); ++i);
    if(i!=1)
        throw "failed test_predicate 5";
    }
}
void test_predicate_mf() {
    {// member predicate function
    lref<Functor> f = Functor();
    lref<int> two = 2;
    relation r = predicate_mf(f, &Functor::isEven, two);
    int i=0;
    for(; r(); ++i);
    if(i!=1)
        throw "failed test_predicate 6";
    }
    {// failing member predicate function
    lref<Functor> f = Functor();
    relation r = predicate_mf(f, &Functor::isEven, 5);
    int i=0;
    for(; r(); ++i);
    if(i!=0)
        throw "failed test_predicate 7";
    }
    {// non-const obj, const member predicate 
    lref<Functor> f = Functor();
    lref<int> one = 1;
    relation r = predicate_mf(f, &Functor::allEqual, 1,1, 1,1, 1,one);
    int i=0;
    for(; r(); ++i);
    if(i!=1)
        throw "failed test_predicate 8";
    }
    {// const obj, const member predicate 
    lref<const Functor> f = Functor();
    relation r = predicate_mf(f, &Functor::allEqual, 1,1, 1,1, 1,1);
    int i=0;
    for(; r(); ++i);
    if(i!=1)
        throw "failed test_predicate 9";
    }
    { // const object, const member predicate
    lref<const Functor> f = Functor();
    relation r = predicate_mf(f, &Functor::ternaryConstPred, 2, 3, 6);
    int i=0;
    for(; r(); ++i);
    if(i!=1)
        throw "failed test_predicate 10";
    }
}

void test_Disjunctions() {
    {// push_back
    Disjunctions males, males2;
    lref<string> p;
    males.push_back(eq(p, "roshan"));
    males.push_back(eq(p, "shivaram"));
    if(!males()) 
        throw "failed test_Disjunctions 1";
    if(*p!="roshan") 
        throw "failed test_Disjunctions 1";
	males2=males;
    if(!males()) 
        throw "failed test_Disjunctions 1";
	if(*p!="shivaram") 
        throw "failed test_Disjunctions 1";
	if(males()) 
        throw "failed test_Disjunctions 1"; // all done
    if(p.defined()) 
        throw "failed test_Disjunctions 1";

    if(!males2()) 
        throw "failed test_Disjunctions 2";
	if(*p!="shivaram") 
        throw "failed test_Disjunctions 2";
	if(males2()) 
        throw "failed test_Disjunctions 2"; // all done
    }
    {// push_front
    Disjunctions males, males2;
    lref<string> p;
    males.push_front(eq(p, "shivaram"));
    males.push_front(eq(p, "roshan"));
    if(!males()) 
        throw "failed test_Disjunctions 3";
    if(*p!="roshan") 
        throw "failed test_Disjunctions 3";
	males2=males;
    if(!males()) 
        throw "failed test_Disjunctions 3";
	if(*p!="shivaram") 
        throw "failed test_Disjunctions 3";
	if(males()) 
        throw "failed test_Disjunctions 3"; // all done
    if(p.defined()) 
        throw "failed test_Disjunctions 3";

    if(!males2()) 
        throw "failed test_Disjunctions 4";
	if(*p!="shivaram") 
        throw "failed test_Disjunctions 4";
	if(males2()) 
        throw "failed test_Disjunctions 4"; // all done
    }
    {
    Disjunctions books;
    lref<string> t, a;
    books.push_back(eq(t,"Tuesdays with Morrie") && eq(a,"Mitch Albom"));
    books.push_back(eq(t,"Bertrand Russell in 90 minutes") && eq(a,"Paul Strathern"));
    if(!books())
        throw "failed test_Disjunctions 5";
    if(*a != "Mitch Albom")
        throw "failed test_Disjunctions 5";
    if(!books())
        throw "failed test_Disjunctions 5";
    if(*a != "Paul Strathern")
        throw "failed test_Disjunctions 5";
    if(books())
        throw "failed test_Disjunctions 5"; // all done
    }
}

void test_ExDisjunctions() {
    {// push_back
    ExDisjunctions males, males2;
    lref<string> p;
    males.push_back(eq(p, "roshan"));
    males.push_back(eq(p, "shivaram"));
    if(!males()) 
        throw "failed test_ExDisjunctions 1";
    if(*p!="roshan") 
        throw "failed test_ExDisjunctions 1";
	males2=males;
	if(males()) 
        throw "failed test_ExDisjunctions 1"; // all done
    if(p.defined()) 
        throw "failed test_ExDisjunctions 1";
    
	if(males2()) 
        throw "failed test_ExDisjunctions 2"; // all done
    }
    {// push_front
    ExDisjunctions males, males2;
    lref<string> p;
    males.push_back(eq(p, "shivaram"));
    males.push_front(eq(p, "roshan"));
    if(!males()) 
        throw "failed test_ExDisjunctions 2";
    if(*p!="roshan") 
        throw "failed test_ExDisjunctions 2";
	males2=males;
	if(males()) 
        throw "failed test_ExDisjunctions 2"; // all done
    if(p.defined()) 
        throw "failed test_ExDisjunctions 2";
    
	if(males2()) 
        throw "failed test_ExDisjunctions 3"; // all done
    }
    {
    ExDisjunctions books;
    lref<string> t, a;
    books.push_back(eq(t,"Tuesdays with Morrie") && eq(a,"Mitch Albom"));
    books.push_back(eq(t,"Bertrand Russell in 90 minutes") && eq(a,"Paul Strathern"));
    if(!books())
        throw "failed test_ExDisjunctions 4";
    if(*a != "Mitch Albom")
        throw "failed test_ExDisjunctions 4";
    if(books())
        throw "failed test_ExDisjunctions 4"; // all done
    }
}

void test_Conjunctions() {
    { // push_back
    Conjunctions gender, gender2;
    lref<string> p, m;
    gender.push_back(eq(p, "roshan"));
    gender.push_back(eq(m, "male"));
    if(!gender()) 
        throw "failed test_Conjunctions 1";
    if(*p!="roshan") 
        throw "failed test_Conjunctions 1";
    gender2 = gender;
	if(gender()) 
        throw "failed test_Conjunctions 1"; // all done

    if(gender2())
        throw "failed test_Conjunctions 2"; // all done
    }
    { // push_front
    Conjunctions gender, gender2;
    lref<string> p, m;
    gender.push_front(eq(m, "male"));
    gender.push_front(eq(p, "roshan"));
    if(!gender()) 
        throw "failed test_Conjunctions 3";
    if(*p!="roshan") 
        throw "failed test_Conjunctions 3";
    gender2 = gender;
	if(gender()) 
        throw "failed test_Conjunctions 3"; // all done

    if(gender2())
        throw "failed test_Conjunctions 4"; // all done
    }
    {
    Conjunctions books;
    lref<string> t, a;
    books.push_back(eq(t,"Tuesdays with Morrie") && eq(a,"Mitch Albom"));
    if(!books())
        throw "failed test_Conjunctions 5";
    if(*a != "Mitch Albom")
        throw "failed test_Conjunctions 5";
    if(books())
        throw "failed test_Conjunctions 5"; // all done
    }
}
void test_cut() {
#ifndef __BCPLUSPLUS__   // cut() && cut() not supported in BCB
	{ // cut() && cut()
	relation r = cutexpr( cut() && cut() );
	if( r() )
		throw "failed test_cut";
	}
#endif // __BCPLUSPLUS__
	{ // cut() && ...       TODO : test for all solns here
	lref<stack<string> > lss = stack<string>(); lref<string> ls = "blah";
	relation r = cutexpr(cut() && predicate(&push,lss,ls));
	if( !r() )
		throw "failed test_cut 1";
	if(lss->empty())
		throw "failed test_cut 2";
	}
	{ // ... && cut()
	lref<stack<string> > lss = stack<string>(); lref<string> s = "blah";
	relation r = cutexpr( cut() && predicate(&push,lss,s) );
	if( !r() )
		throw "failed test_cut 3";
	if( lss->empty())
		throw "failed test_cut 4";
	if( r() )
		throw "failed test_cut 5";
	}
#ifndef __BCPLUSPLUS__   // cut() && cut() not supported in BCB
	{ // cut() || cut()
	relation r = cutexpr( cut() || cut() );
	if( r() )
		throw "failed test_cut 6";
	}
#endif // __BCPLUSPLUS__
	{ // cut() || ...
    lref<stack<string> > lss = stack<string>(); lref<string> s = "blah";
    relation r = cutexpr( cut() || predicate(&push,lss,s) );
    if( r() )
        throw "failed test_cut 7";
    if(!lss->empty())
        throw "failed test_cut 8";
    }
    { // .. || cut()
    lref<stack<string> > lss = stack<string>(); lref<string> s = "blah";
    relation r = cutexpr( predicate(&push,lss,s) || cut() );
    if(! r() )
        throw "failed test_cut 9";
    if(lss->size()!=1)
       throw "failed test_cut 10";
    }
    {// full blown cut expression
    lref<stack<string> > lss = stack<string>(); lref<string> s1="one", s2="two", s3="three";
    lref<int> i =2, j=3;
    relation r = cutexpr(
            predicate(&push,lss,s1) &&  predicate(i<j)  &&  cut() //&& write("i<j")
         || predicate(&push,lss,s2) &&  predicate(i>j)  &&  cut() && True() //&& write("i>j")
         || predicate(&push,lss,s3) &&  predicate(i==j)  &&  cut() //&& write("i==j")
      );
    relation temp = r;
    while(r());
    if(lss->size()!=1)
        throw "failed test_cut 11";

    r = temp;
    i=3; j=2; lss = stack<string>();
    while(r());
    if(lss->size()!=2)
        throw "failed test_cut 12";

    r = temp; lss = stack<string>();
    i=2; j=2;
    while(r());
    if(lss->size()!=3)
        throw "failed test_cut 13";
    }
//#endif // __BCPLUSPLUS__
}


void test_range() {
    { // generate
    lref<int> li;
    int expected=0;
    relation r = range<int>(li, 0, 7);
    while(r()) {
        if(*li!=expected++)
            throw "failed test_range 1";
    }
    if(li.defined())
        throw "failed test_range 1";
    }
    { // test within range
    lref<int> i =2;
    relation r = range<int>(i, 0, 7);
    if(!r())
        throw "failed test_range 2";
    if(!i.defined())
        throw "failed test_range 2";
    if(r())
        throw "failed test_range 2";
    }
    { // test out of range
    lref<int> i =9, min =0 , max=7;
    relation r = range(i, min, max);
    if(r())
        throw "failed test_range 2";
    if(!i.defined())
        throw "failed test_range 2";
    if(r())
        throw "failed test_range 2";
    }
    { // empty range
        lref<int> i;
        relation r = range(i,10,2,-1);
        if(r())
            throw "failed test_range 3";
    }
}

void test_range_with_step() {
    { // generate
    lref<int> li;
	int expected=0, step=2;
	relation r = range<int>(li, 0, 7, step);
	while(r()) {
		if(*li!=expected)
			throw "failed test_range_with_step 1";
		expected+=step;
	}
	if(li.defined())
		throw "failed test_range_with_step 1";
	}
	{ // test within range
	lref<int> i =2;
	int step =8;
	relation r = range<int>(i, 0, 7, step);
	if(!r())
		throw "failed test_range_with_step 2";
	if(!i.defined())
		throw "failed test_range_with_step 2";
	if(r())
		throw "failed test_range_with_step 2";
	}
	{ // test out of range
	lref<int> i =9, min =0 , max=7, step=3;
    relation r = range(i, min, max, step);
    if(r())
        throw "failed test_range_with_step 2";
    if(!i.defined())
        throw "failed test_range_with_step 2";
    if(r())
        throw "failed test_range_with_step 2";
    }
    { // empty range
        relation r = range<int>(7,10,2,-1);
        if(r())
            throw "failed test_range_with_step 3";
    }
}
void test_item() {
    {//1 - iterate over std::collection
    list<int> li;
	  li.push_back(1); li.push_back(2); li.push_back(3); li.push_back(4); li.push_back(5);
	  lref<int> obj;
	  relation r = item(obj, li.begin(), li.end());
    int j=1;
    while(r())
        if(*obj!= j++)
            throw "failed test_item 1";
    if(obj.defined())
        throw "failed test_item 1";
    }
    {//2 - lookup over std::collection (using lref)
    list<int> li;
    li.push_back(1); li.push_back(2); li.push_back(3); li.push_back(4); li.push_back(5);
    lref<int> obj=2;
    relation r = item(obj, li.begin(), li.end());
    int j=2;
    while(r())
        if(*obj!= j++)
            throw "failed test_item 2";
    if(j!=3 && *obj!=2)
        throw "failed test_item 2";
    }
    {//3 - iterator with array type
    int li[] = {1,2,3,4,5};
    lref<int> obj;
    relation r = item(obj, li+0, li+5);
    int j=1;
    while(r())
        if(*obj!= j++)
            throw "failed test_item 3";
    if(obj.defined())
        throw "failed test_item 3";
    }
    {//4 - lref<int*> over array type
    int ai[] = {1,2,3,4,5};
    lref<int> obj;
    lref<int*> b = ai+0, e = ai+5;
    relation r = item(obj, b, e);
    int j=1;
    while(r())
        if(*obj!= j++)
            throw "failed test_item 4";
    if(obj.defined())
        throw "failed test_item 4";
    }
    {//5 - using lref<vector<int>::iterator>
    int ai[] = {1,2,3,4,5};
    vector<int> vi(ai+0, ai+5);
    lref<int> obj;
    lref<vector<int>::iterator> b = vi.begin(), e = vi.end();
    relation r = item(obj, b, e);
    int j=1;
    while(r())
        if(*obj!= j++)
            throw "failed test_item 5";
    if(obj.defined())
        throw "failed test_item 5";
    }

    {//6 - find an value item that exist
    int ai[]  = { 1, 2, 3, 4 };
    vector<int> vi(ai+0, ai+4);
#if  defined(__BCPLUSPLUS__) || defined(__GNUG__)
    if( ! item<vector<int>::iterator>(4, vi.begin(), vi.end())() )
#else
    if( ! item(4, vi.begin(), vi.end())() )
#endif
        throw "failed test_item 6";
    }
    {//7- find a value item that doesnt exist
    int ai[]  = { 1, 2, 3, 4 };
    vector<int> vi(ai+0, ai+4);
#if  defined(__BCPLUSPLUS__) || defined(__GNUG__)
    if( item<vector<int>::iterator>(9, vi.begin(), vi.end())() )
#else
    if( item(9, vi.begin(), vi.end())() )
#endif
        throw "failed test_item 7";
    }
    {
    // 8- intersection of array1 and array2
    int arr1[] = {1,2,3,4};
    int arr2[] = {6,3,7,1};
    lref<int> obj;
    relation r = item(obj, arr1+0, arr1+4) && item(obj, arr2+0, arr2+4);
    if(!r() && *obj!=1)
        throw "failed test_item 8";
    if(!r() && *obj!=3)
        throw "failed test_item 8";
    if(r())
        throw "failed test_item 8";
    if(obj.defined())
        throw "failed test_item 8";
    }
}

void test_getValueCont() {
    {// going from from list<lref<int> > to vector<int>
    list<lref<int> > lri;
    lri.push_back(1); lri.push_back(2); lri.push_back(3);
    vector<int> vi = getValueCont<vector<int> >(lri);
    if(vi[0]!=1  || vi[1]!=2 || vi[2] !=3)
        throw "failed test_getValues 1";
    }
    {// going from from list<int*> to vector<int>
    int i[] = {1,2,3};
    list<int*>lri;
    lri.push_back(i+0);
    lri.push_back(i+1);
    lri.push_back(i+2);
    vector<int> vi = getValueCont<vector<int> >(lri);
    if(vi[0]!=1  || vi[1]!=2 || vi[2]!=3)
        throw "failed test_getValues 2";
    }
}

//--------------------------------------------------------
// Test : defined
//--------------------------------------------------------
void test_defined() {
    {
        lref<int> li;
        if(defined(li)())
            throw "failed test_defined 1";
        li=2;
        if(!defined(li)())
            throw "failed test_defined 1";
    }
}

//--------------------------------------------------------
// Test : undefined
//--------------------------------------------------------
void test_undefined() {
	lref<int> x;
	relation r= undefined(x);
	int i=0;
	for(; r(); ++i);
	if(i!=1)
		throw "failed test_undefined 1";

	lref<int> y = 2;
    relation r2= undefined(y);
	for(i=0; r2(); ++i);
	if(i!=0)
		throw "failed test_undefined 2";
}

void test_unique() {
    // print items appearing only once in arr
    int arr[] = {0, 1, 2, 3, 3, 2};
    lref<int> i;
    int expected=0;
    relation r = item(i, arr+0, arr+5) && unique(i);
    while(r())
        if(*i!= expected++)
		   throw "failed test_unique 1";
}


void test_unique_f() {
    // if i is an item in arr1 and j is an item in j
    // print all pairs of i and j such that i*j is unique
    int arr1[] = {0, 1, 2};
    int arr2[] = {3, 2, 1};
    lref<int> i, j;

    relation r = item(i, arr1+0, arr1+3) && item(j, arr2+0, arr2+3) && unique_f(i*j);
    if(!r())
        throw "failed test_unique_f 1";
    if(*i!=0 || *j!=3)
        throw "failed test_unique_f 1";
    if(!r())
        throw "failed test_unique_f 1";
    if(*i!=1 || *j!=3)
        throw "failed test_unique_f 1";
    if(!r())
        throw "failed test_unique_f 1";
    if(*i!=1 || *j!=2)
        throw "failed test_unique_f 1";
    if(!r())
        throw "failed test_unique_f 1";
    if(*i!=1 || *j!=1)
        throw "failed test_unique_f 1";
    if(!r())
        throw "failed test_unique_f 1";
    if(*i!=2 || *j!=3)
        throw "failed test_unique_f 1";
    if(!r())
        throw "failed test_unique_f 1";
    if(*i!=2 || *j!=2)
        throw "failed test_unique_f 1";
    if(r())
        throw "failed test_unique_f 1";
}

void test_empty() {
    { // 1- lref to vector
    lref<vector<int> > lvi;
    relation r = empty(lvi);
    if(!r())
        throw "failed test_empty 1";
    if(!lvi.defined() && !lvi->empty() )
        throw "failed test_empty 1";
    if(r())
        throw "failed test_empty 1";
    if(lvi.defined())
        throw "failed test_empty 1";
    }
    { // 2- not using lref
    vector<int> vi;
    relation r = empty(vi);
    if(!r())
        throw "failed test_empty 2";
    }
}

void test_not_emtpy() {
    { //1 
    lref<vector<int> > lvi = vector<int>();
    relation r = not_empty(lvi);
    if(r())
        throw "failed test_not_empty 1";
    }
    { //2
    vector<int> vi = vector<int>();
    relation r = not_empty(vi);
    if(r())
        throw "failed test_not_empty 2";
    }
}

void test_deref() {
    {
    // dereferencing raw pointers... gen
    int two=2;
    int* pi= &two;
    lref<int> li;
    relation r = deref<int*>(pi, li);
    if(!r() || *li!=2)
        throw "failed test_deref 1";
    }
    {
     //dereferencing logic references to iterators... test
    vector<int> vi; vi.push_back(4); vi.push_back(5);
    lref<vector<int>::iterator > lItr = vi.begin();
    lref<int> li=4;
    //relation r = deref<vector<int>::iterator>(lItr, vi.begin());
    relation r = deref(lItr, li);
    if(!r())
        throw "failed test_deref 2"; 
    }
    {
     //dereferencing iterators... test
    vector<int> vi; vi.push_back(4); vi.push_back(5);
    relation r = deref<vector<int>::iterator>(vi.begin(), 9);
    if(r())
        throw "failed test_deref 3"; 
    }

}

void test_read() {
    { // read value into undefined lref
	stringstream sstrm;
	sstrm << "Hello";
	lref<string> s;
	if(!readFrom(sstrm,s)())
		throw "failed test_read 1";
	}
	{ // read value into defined lref
	stringstream sstrm;
	sstrm << "Hello";
	lref<string> s = "Hello";
	if(!readFrom(sstrm,s)())
		throw "failed test_read 2";
	}
	{ // read value into defined lref
	stringstream sstrm;
	sstrm << "Hello";
	if(readFrom(sstrm,"World")())
		throw "failed test_read 3";
	}
	{ // read all values from stream
	stringstream sstrm;
	sstrm << "Hello World";
	lref<string> s;
	relation r = readFrom(sstrm,s);
	if(!r() || *s!="Hello")
		throw "failed test_read 4";
	if(!r() || *s!="World")
		throw "failed test_read 4";
	}
}

void test_write() {
	{ // write value to stream
	stringstream sstrm;
	if(!writeTo(sstrm, "Hello")())
        throw "failed test_write 1"; 
    string s;
    sstrm >> s;
    if(s!="Hello")
        throw "failed test_write 1"; 
    }
}

namespace test_details {
struct num {
    int i;

    num(int i) : i(i)
    { }

    num(const num& n) : i(n.i)
    { }

    num operator + (const num& r) {
        return num(i + r.i);
    }
    static num static_compute(num i, num j, num k) {
        return num(i.i + j.i / k.i);
    }

    num nonstatic_compute(num i, num j, num k) {
        return num(i.i + j.i / k.i);
    }
};

num operator *(num l, num r) {
    return num(l.i * r.i);
}

ostream& operator << (ostream &o, const num& n) {
    return o << n.i;
}

}//namespace test_details


void test_write_f() {
	typedef test_details::num num;
	{ // write value of computing an ILE to stream
	stringstream sstrm;
	lref<num> i=3;
	writeTo_f(sstrm, i+i*i)();
	if(sstrm.str()!="12")
		throw "failed test_write_f 1";
	}
	{ // write value of computing a func
	stringstream sstrm;
	lref<num> i=3;
	writeTo_f(sstrm, &num::static_compute, i,i,i)();
	if(sstrm.str()!="4")
		throw "failed test_write_f 2";
	}
}

void test_write_mf() {
	{
	typedef test_details::num num;
	stringstream sstrm;
	lref<num> i=3;
	lref<num> n(5);
	writeTo_mf(sstrm, n,&num::nonstatic_compute, i,i,i)();
	if(sstrm.str()!="4")
		throw "failed test_write_mf 1";
	}
}

void test_write_read() {
  { // write value to stream and then read it
    stringstream sstrm;
    relation r = writeTo(sstrm, "Hello") && readFrom(sstrm,"Hello");
    if(!r())
      throw "failed test_write_read 1"; 
  }
  {// copy words from one stream to another
    stringstream inputData, outputData;
    inputData << "Hello World.";
    lref<string> ls;
    relation copyWords = readFrom(inputData, ls) && writeTo(outputData, ls);
    int count=0;
    while(copyWords()) { ++count; }
    if(count!=2 ||  ls.defined())
      throw "failed test_write_read 2";
  }
}

class myTestOnlyRel : public TestOnlyRelation<myTestOnlyRel> {
  lref<int> i;
public:
  myTestOnlyRel(lref<int> i) : i(i) 
  {}
  myTestOnlyRel(const myTestOnlyRel& rhs) : i(rhs.i){
    static int times=0;
    if(++times>2)
      throw "Copy construction not allowed for this relation";
  }

  bool operator () (void) {
    rel_begin();   
    rel_return(*i<20);
    rel_end();
  }
};
    

void test_op_and_fast() {
  {
    lref<int> i;
    relation r = range(i,18,22) && myTestOnlyRel(i);
    while(r());
  }
}
void test_op_exor() {
    {
    lref<int> i;
    relation r = (range(i,1,3) ^ range(i,5,7) ^ range(i,9,10) );
    if(!r()) throw "failed test_op_exor 1";
    if(*i!=1) throw "failed test_op_exor 1";

    if(!r()) throw "failed test_op_exor 1";
    if(*i!=2) throw "failed test_op_exor 1";

    if(!r()) throw "failed test_op_exor 1";
    if(*i!=3) throw "failed test_op_exor 1";

    if(r()) throw "failed test_op_exor 1";
    if(i.defined()) throw "failed test_op_exor 1";
    }
    {
    lref<int> i;
    relation r = (range(i,1,0) ^ range(i,5,7) ^ range(i,9,10) );
    if(!r()) throw "failed test_op_exor 2";
    if(*i!=5) throw "failed test_op_exor 2";

    if(!r()) throw "failed test_op_exor 2";
    if(*i!=6) throw "failed test_op_exor 2";

    if(!r()) throw "failed test_op_exor 2";
    if(*i!=7) throw "failed test_op_exor 2";

    if(r()) throw "failed test_op_exor 2";
    if(i.defined()) throw "failed test_op_exor 2";
    }
        
}

void test_error() {
    { // 1
    std::exception e;
    relation r =  error(e);
    try { 
        r();
        throw "failed test_error 1";
    } catch (std::exception&)
    { }
    }
    { // 2
    relation r =  error("unexpected error");
    try { 
        r();
        throw "failed test_error 2";
    } catch (const char*)
    { }    
    }
}

namespace {
int testCount=0;
int passCount=0;
int failCount=0;
}

void run_test( void(*testFunc)(void) ) {
	try {
        ++testCount;
        testFunc();
        ++passCount;
    }
    catch (char* err){
        ++failCount;
        cout << err <<"\n";
    }
}

void runtests() {
    // lref tests
    run_test( test_lref );
    run_test( test_begin_end ); // begin and end for lref<CollectionT>

    // test for || and && operators
    run_test( test_or_and );
	
    // family
	run_test( test_all_husband_wives );
	run_test( test_all_mothers );
	run_test( test_wifeof_father1 );
	run_test( test_husbandof_wife2 );
	run_test( test_childrenof_f1_m1 );
	run_test( test_ischildof_f1_m1 );
	run_test( test_parent_s1 );
	run_test( test_ancestors_s2 );
	run_test( test_gf2_ancestor_s2 );
    run_test( test_relational_loop );

	//graph
	run_test( test_allNodes_connectedTo_1 );
	run_test( test_path_1_2 );
	run_test( test_path_1_5 );
	run_test( test_nopath_1_5 );

	//// artithmetic  : plus
	run_test( test_plus );
	run_test( test_plus_z );
	run_test( test_plus_y );
	run_test( test_plus_x );

	// artithmetic  : multiply, square
	run_test( test_multiply );
	run_test( test_multiply_x );
	run_test( test_multiply_z );
	run_test( test_square );
	run_test( test_square_s );
    run_test( test_Prefix_UnaryOps_ILE );
    run_test( test_Postfix_UnaryOps_ILE );

    // eq
    run_test( test_eq );
    run_test( test_eq_values );
    run_test( test_eq_with_coll );
    run_test( test_eq_seq );
    run_test( test_eq_f );
    run_test( test_eq_mf );

    // ILE
    run_test( test_ILE );
    run_test( test_eq_ILE );
    run_test( test_Create_with );

    // merge
     run_test( test_merge );
     // run_test( test_merge_withcuts );

    // next
    run_test( test_next );
    run_test( test_prev );

    // head & tail
    run_test( testHead );
    run_test( test_head_list );
    run_test( test_tail );
    run_test( test_tail_list );
    run_test( test_head_tail );
    run_test( test_head_n_tail );

    run_test( test_insert );
    
    run_test( test_inc_dec );
    run_test( test_insert_seq );

    //sequence()
    run_test( test_sequence );

    // size()
    run_test( test_size );
    
    // runFA<Nfa>()
    run_test( test_FA );

    // recurse
    run_test( test_recurse_mf );
    
    // predicate
    run_test( test_predicate );
    run_test( test_predicate_mf );
	
	  run_test( test_ExDisjunctions );
    run_test( test_Disjunctions );
    run_test( test_Conjunctions );

    // Utils
    //run_test( test_cut );

    run_test( test_range );
    run_test( test_range_with_step );

    run_test( test_item );

    run_test( test_getValueCont );

    run_test( test_defined );
    run_test( test_undefined );

    run_test( test_unique );
    run_test( test_unique_f );

    run_test( test_empty );
    run_test( test_not_emtpy );

    run_test( test_deref );
    
    run_test( test_read );
    run_test( test_write );
    run_test( test_write_f );
    run_test( test_write_mf );

    run_test( test_write_read );

    run_test( test_op_and_fast );
    run_test( test_op_exor );
    run_test( test_error );
    
    cout << "\nFailed: " << failCount<< "\nPassed: " << passCount  << "\nTotal: " << testCount;
}
