// Castor : Logic Programming Library
// Copyright © 2007-2010 Roshan Naik (roshan@mpprogramming.com).
// This software is governed by the MIT license (http://www.opensource.org/licenses/mit-license.php).

#include <string>
#include <iostream>
#include <vector>
#include <stack>
#include <list>
#include <algorithm>
#include <functional>
#include <iomanip>
#include <ctime>
#include <utility>
#include <set>
#include <map>

#include <castor.h>
#include <includes/functional.h>

#include "test_core.h"
#include "test_utils.h"
#include "test_predicate.h"
#include "test_eq.h"
#include "test_io.h"
#include "test_ile.h"
#include "test_collection.h"
#include "test_aggregates.h"
#include "test_higher_order.h"

using namespace std;
using namespace castor;


void runtests();
void bar();

int main() 
{
  clock_t start = clock();
  //bar();
  runtests();
  cout << "\nTime taken " << (clock() - start) << "\n\n Testing Complete! ";
  cin.ignore();
  return 0;
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
    catch (const char* err) {
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
    run_test( test_eq_mem );

    // ILE
    run_test( test_ILE );
    run_test( test_eq_ILE );
    run_test( test_Create_with );
    run_test( test_create );
    run_test( test_get );
    run_test( test_call );
	run_test( test_mcall );
	run_test( test_at );

    // merge
     run_test( test_merge );
     // run_test( test_merge_withcuts );

    // prev & next
    run_test( test_next );
    run_test( test_prev );

    // head & tail
    run_test( testHead );
    run_test( test_head_list );
    run_test( test_tail );
    run_test( test_tail_list );
    run_test( test_head_tail );
    run_test( test_head_n_tail );
	
	// inc/dec
    run_test( test_inc_dec );

	// insert
    run_test( test_insert );
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
    run_test( test_predicate_mem );
	
    // Dynamic relations
	run_test( test_ExDisjunctions );
    run_test( test_Disjunctions );
    run_test( test_Conjunctions );

    // Utils
    run_test( test_cut );

    run_test( test_range );
    run_test( test_range_with_step );

    run_test( test_item );

    run_test( test_getValueCont );

    run_test( test_defined );
    run_test( test_undefined );

    run_test( test_unique );
    run_test( test_unique_f );
	run_test( test_unique_mf );
	run_test( test_unique_mem );

    run_test( test_empty );
    run_test( test_not_emtpy );

    run_test( test_dereference );

    run_test( test_eval );
    run_test( test_eval_mf );

    run_test( test_readFrom );
    run_test( test_writeTo );
    run_test( test_writeAllTo );
    run_test( test_writeTo_f );
    run_test( test_writeTo_mf );
    run_test( test_writeTo_mem );

    run_test( test_writeTo_readFrom );

    run_test( test_op_and_fast );
    run_test( test_op_exor );
    run_test( test_error );
	run_test( test_repeat );
	run_test( test_pause );
	run_test( test_zip );
	run_test( test_True );

	// collection 
	run_test( test_shuffle );
    run_test( test_permute );

	// aggregates
	run_test( test_order );
	run_test( test_order_mf );
	run_test( test_order_mem );
	run_test( test_reverse );
	run_test( test_count );
	run_test( test_sum );
	run_test( test_reduce );
	run_test( test_group_by );

	cout << "\nFailed: " << failCount << "\nPassed: " << passCount  << "\nTotal: " << testCount;
}

struct triplet {
	lref<int> p1,p2,p3;
	triplet(lref<int> p1, lref<int> p2, lref<int> p3) : p1(p1), p2(p2), p3(p3)
	{ }
	bool operator==(const triplet& rhs) {
		return *p1==*rhs.p1 && *p2==*rhs.p2 && *p3==*rhs.p3;
	}
};

template<typename A1,  typename A2>
struct Recall_2 : public TestOnlyRelation<Recall_2<A1,A2> > {
	lref<set<pair<A1,A2> > > s;
	lref<A1> a1; lref<A2> a2;
	pair<A1,A2> p;
	Recall_2 (lref<set<pair<A1,A2> > > s, const lref<A1>& a1, const lref<A2>& a2) : s(s), a1(a1), a2(a2)
	{ }

	bool operator()(void) {
		co_begin();
		while( true ) {
			if(!a1.defined() || !a2.defined()) {
				co_yield(true)
				continue;
			}
			p = make_pair(*a1,*a2);
			if( s->find(p)==s->end() ) {
				s->insert(p);
				co_yield( true );
			}
			else 
				co_yield(false);
		}
		co_end();
	}
};

template<typename A1,  typename A2>
struct Recall /*: public TestOnlyRelation<Recall<A1,A2> >*/ {
	lref<set<pair<A1,A2> > > s;
	lref<A1> a1; lref<A2> a2;
	pair<A1,A2> p;
	Recall (lref<set<pair<A1,A2> > > s, const lref<A1>& a1, const lref<A2>& a2) : s(s), a1(a1), a2(a2)
	{ }

	bool operator()(void) {
		if(!a1.defined()) {
			if(!a2.defined())
				p = make_pair(-1,-1);
			else
				p = make_pair(-1,*a2);
		} else if (!a2.defined()) {
			p = make_pair(*a1,-1);
		}
		else 
			p = make_pair(*a1,*a2);
		if( s->find(p)==s->end() ) {
			s->insert(p);
			return true ;
		}
		return false;
	}
};


typedef pair<int,int> pr;

template<typename A1, typename A2> inline
Recall<A1,A2> recall(lref<set<pr> > s, lref<A1>& a1, lref<A2>& a2) {
	return Recall<A1,A2>(s,a1,a2);
}

relation redge(lref<int> n1, lref<int> n2) {
    return eq(n1,1) && eq(n2,2)
        || eq(n1,2) && eq(n2,3)
        || eq(n1,3) && eq(n2,1)
        || eq(n1,3) && eq(n2,4)
        || eq(n1,4) && eq(n2,5) ;
}

relation rpath_body(lref<set<pr> > s, lref<int> n1, lref<int> n2) {
    lref<int> X;
    return  recall(s,n1,n2) && (   redge(n1, n2)
                                || redge(n1, X) && recurse(rpath_body, s, X, n2) );
}

relation rpath(lref<int> n1, lref<int> n2) {
    lref<set<pr> > s = set<pr>();
    return rpath_body(s, n1, n2) ;
}

template<typename Rel>
struct Memo_r : public Coroutine {
	relation rel;
	lref<unsigned long> p1, p2; 
	lref<map<unsigned long,unsigned long> > m;
	pr p;
	bool b, p2WasDefined;

	Memo_r(Rel r, lref<unsigned long> p1, lref<unsigned long> p2, lref<map<unsigned long,unsigned long> > m) : rel(recurse(r,p1,p2,m)), p1(p1), p2(p2), m(m), b(false), p2WasDefined(false)
	{ }
	
	bool operator() (void) {
		map<unsigned long,unsigned long>::iterator tmp;
		co_begin();
		p2WasDefined=p2.defined();
		if( p1.defined() ) {
			tmp = m->find(*p1);
			if( tmp==m->end() ) { // not computed previously
				if(rel()) {
					m->insert(make_pair(*p1,*p2));
					co_yield(true);
				}
				else
					co_return(false);
			}
			else { // computed previously
				if(p2.defined()) {
					co_yield(*p2==tmp->second);
				}
				else {
					p2=tmp->second;
					co_yield(true);
					p2.reset();
					co_return(false)
				}
			}
		} // if( p1.defined() )
		else { 
			// .. todo: check if p2 is defined
			if(rel()) {
				m->insert(make_pair(*p1,*p2));
				co_return(true);
			} 
			else
				co_return(false);
		}
		co_end();
	}
	void reset() {
		if(!p2WasDefined && p2.defined())
			throw "Reset may have to do some resetting";
	}
	typedef int UseFastAnd;
};

template<typename Rel>
Memo_r<Rel> memo(Rel r, lref<unsigned long> p1, lref<unsigned long> p2, lref<map<unsigned long,unsigned long> > m) {
	return Memo_r<Rel>(r,p1,p2,m);
}


relation fib_(lref<unsigned long> n, lref<unsigned long> f, lref<map<unsigned long,unsigned long> > m) {
	lref<unsigned long> n1,n2, f1,f2;
	return     eq(n,0) && eq(f,0)
			|| eq(n,1) && eq(f,1) 
			|| eq_f(n1,n-1) && eq_f(n2,n-2) && memo(fib_,n1,f1,m) && memo(fib_,n2,f2,m) && eq_f(f,f1+f2);
}

relation fib(lref<unsigned long> n, lref<unsigned long> f) {
	lref<map<unsigned long,unsigned long> > m(new map<unsigned long,unsigned long>(), true);
	return memo(fib_,n,f,m);
}



relation fib2(lref<int> n, lref<int> f) {
	lref<int> n1, n2, f1, f2;
	return     eq(n,0) && eq(f,0)
			|| eq(n,1) && eq(f,1) 
			|| prev(n,n1) && prev(n1,n2) && recurse(fib2,n1,f1) && recurse(fib2,n2,f2) && eq_f(f,f1+f2);
}

map<unsigned long,unsigned long> answers;

unsigned long
fibo(unsigned long n) {
	unsigned long fn1=0,fn2=0;
	if(n==0) {
		answers[n]=0;
		return 0;
	}
	if(n==1) {
		answers[n]=1;
		return 1;
	}
	map<unsigned long,unsigned long>::iterator n1 = answers.find(n-1), n2;
	if(n1!=answers.end())
		fn1 = n1->second;
	else
		fn1 = fibo(n-1);

	n2 = answers.find(n-2);
	if(n2!=answers.end())
		fn2 = n2->second;
	else
		fn2 = fibo(n-2);
	
	unsigned long result = fn1 + fn2;
	answers[n]=result;
	return result;
}


const bool has    = true;
const bool hasnot = false;

struct state {
    lref<string> _monkeyHoriz;
    lref<string> _monkeyVert;
    lref<string> _boxPos;
    lref<bool>   _hasBanana;

	state(lref<string> h, lref<string> v, lref<string> p, lref<bool> s)
       : _monkeyHoriz(h), _monkeyVert(v), _boxPos(p), _hasBanana(s) 
	{}
	
	bool alldefined() const {
		return _monkeyHoriz.defined() &&  _monkeyVert.defined() &&  _boxPos.defined() && _hasBanana.defined();

	}
	bool operator ==(const state& rhs) const {
		if(alldefined() && rhs.alldefined())  
			return *_monkeyHoriz==*(rhs._monkeyHoriz)  && *_monkeyVert==*(rhs._monkeyVert) && *_boxPos==*(rhs._boxPos) && *_hasBanana==*(rhs._hasBanana);
		else 
			return false;
   }
};


// Data member level unification support for state
template<class T>
struct Eq_m  : Coroutine {   // definition of class Coroutine and macros co_* are given at the bottom. 
  lref<state> s;
  lref<T> state::* mem;   // pointer to a lref data member of state
  lref<T> rhs;

  Eq_m (lref<state> s, lref<T> state::* mem, const lref<T>& rhs) : s(s), mem(mem), rhs(rhs)
  { }
 
  // Compare the two
  bool operator() (void)  {
       co_begin();  // default first line .. beginning of coroutine

	   if( rhs.defined() ) {
		   if( ((*s).*mem).defined() ) {
				co_return( *((*s).*mem) == *rhs  ); // returns result of comparison to caller. All future invocations of operator() will return false
		   }
		   else {
			   (*s).*mem = rhs;
				co_yield(true);    // returns true to caller. But next time operator() is invoked execution skips all above code and resumes from next statement 
				((*s).*mem).reset();  // undo side effects
		   }
	   }
       co_end();  // default last line .. end of coroutine
  }
};


template<class T>
Eq_m<T> eq_m( lref<state> s, lref<T> state::* mem, lref<T> rhs ) {
       return Eq_m<T>(s,mem,rhs);
}

struct Spin {
	string s;
	Spin(string& s) : s(s) {}
	string& operator()(void) {
		rotate(s.begin(), s.begin()+1, s.end());
		s.append("9");
		return s;
	}
};

struct CFunc {
	typedef int result_type;
	int operator()(void) {
		return 7;
	}
	int operator()(void) const{
		return 9;
	}
};


//template<class K, class V> inline
//relation write_grp(lref<group_item<K,V> >& item) {
//	return write_mem(item, &group_item<K,V>::key) && write(" : (") && writeAll(item, ",", ")\n");
//}
//
//template<class K1, class K2, class V> inline 
//relation write_grp(lref<group_item<K1, group_item<K2,V> > >& gitem) {
//	typedef group_item<K1, group_item<K2,V> > outerGrp;
//	lref<vector<group_item<K2,V> > > subgroups;
//	lref<group_item<K2,V> > subgroup;
//	return write_mem(gitem, &outerGrp::key) && write(" - ") 
//		&& eq_mem(subgroups, gitem, &outerGrp::values) 
//		&& item(subgroup,subgroups) 
//		&& write_grp(subgroup);
//}

struct mod10 {
	typedef int result_type;
	int operator()(int i) {
		return i%10;
	}
};

struct mod3 {
	typedef int result_type;
	int operator()(int i) {
		return i%3;
	}
};
void bar() {
	{
		lref<int> i,j;
		lref<group<int,group<int,int>>> g1;
		lref<group<int,int>> g2;
		relation r = range(i,1,30000) >> group_by(i, mod10(), g1).then(mod3()) ;
		//relation r = range(i,1,30000) >> group_by(i, mod10(), g2) ;
		clock_t start = clock();
		while(r()) {
			relation r2 = item(g2,g1);
			while(r2()) {
				relation r3 = item(j,g2);
				while(r());
			}
		}
		cout << (clock() - start) ;
		cout << " ms\n";
	}
}
