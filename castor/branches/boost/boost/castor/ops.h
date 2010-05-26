// Castor : Logic Programming Library
// Copyright © 2008 Roshan Naik (roshan@mpprogramming.com).
// This software is goverened by the MIT license (http://www.opensource.org/licenses/mit-license.php).

#if !defined CASTOR_OPS_H
#define CASTOR_OPS_H 1

#include <boost/castor/relation.h>
#include <boost/castor/lref.h>
#include <boost/castor/workaround.h>

namespace castor {
#ifdef CASTOR_ALL_REGULAR_OPS
	#undef CASTOR_USE_GENERIC_OR
	#define CASTOR_USE_REGULAR_OR
	#undef CASTOR_USE_GENERIC_AND
	#define CASTOR_USE_REGULAR_AND
	#undef CASTOR_USE_GENERIC_EXOR
	#define CASTOR_USE_REGULAR_EXOR
#endif

#ifdef CASTOR_ALL_GENERIC_OPS
	#define CASTOR_USE_GENERIC_OR
	#undef  CASTOR_USE_REGULAR_OR
	#define CASTOR_USE_GENERIC_AND
	#undef  CASTOR_USE_REGULAR_AND
	#define CASTOR_USE_GENERIC_EXOR
	#undef  CASTOR_USE_REGULAR_EXOR
#endif


#if !defined(CASTOR_USE_GENERIC_OR) && !defined(CASTOR_USE_REGULAR_OR)
 #define CASTOR_USE_GENERIC_OR
#endif

#if !defined(CASTOR_USE_GENERIC_AND) && !defined(CASTOR_USE_REGULAR_AND)
 #define CASTOR_USE_REGULAR_AND
#endif

#if !defined(CASTOR_USE_GENERIC_EXOR) && !defined(CASTOR_USE_REGULAR_EXOR)
  #define CASTOR_USE_GENERIC_EXOR
#endif

//---------------------------------------------------------------
//    Relational OR operator : provides backtracking
//---------------------------------------------------------------
//Concepts: L supports  bool operator()
//        : R supports  bool operator()

template<typename L, typename R>
class Or_r : public Coroutine {
	L l;
	R r;
public:
	Or_r(const L & l, const R & r) : l(l), r(r)
	{ }

	bool operator() (void) {
	  co_begin();
	  while(l())
		co_yield(true);
	  while(r())
		co_yield(true);
	  co_end();
	}
};

#ifdef CASTOR_USE_GENERIC_OR
template<typename L, typename R> inline
Or_r<L,R> operator || (const L& l, const R & r) {
	return Or_r<L,R>(l, r);
}
#else
inline
Or_r<relation,relation> operator || (const relation& l, const relation& r) {
	return Or_r<relation,relation>(l, r);
}
#endif
//---------------------------------------------------------------
//    Relational AND operator
//---------------------------------------------------------------

template<typename L, typename R, bool fast=false>
class And_r : public Coroutine {
  L l;
  relation r; R rbegin;
public:
	And_r(const L& l, const R& r) : l(l), r(r), rbegin(r)
	{ }

  bool operator () (void) { // SLOW
    co_begin();
    while(l()) {
	  while(r())
        co_yield(true);
      r=rbegin;
    }
    co_end();
  }
};

template<typename L, typename R> 
class And_r <L,R,true> : public Coroutine {
  L l;
  R r;
public:
	And_r(const L& l, const R& r) : l(l), r(r)
	{ }

  bool operator () (void) {
    co_begin();
	while(l()) {
	  if(r())
		co_yield(true);
	  r.reset();
	}
	co_end();
  }
};

namespace detail {
struct twoBytes { char a[2]; };
template<typename T> char     fastAndCheck(typename T::UseFastAnd *);
template<typename T> twoBytes fastAndCheck(...);

} // namespace detail

template<typename T>
struct IsTestOnlyRelation {
  enum {result=( sizeof(::castor::detail::fastAndCheck<T>(0))==1 )};
};

template<typename Rel>
struct IsRelation_Constraint {
  static void constraints(Rel a) {
	relation r = a;
  }
};

#ifdef CASTOR_USE_GENERIC_AND
template<typename L, typename R> inline
And_r<L,R,IsTestOnlyRelation<R>::result> operator && (const L & l, const R & r) {
	return And_r<L,R,IsTestOnlyRelation<R>::result>(l, r);
}
#else

#if defined(CASTOR_DISABLE_AND_OPTIMIZATION)
inline
And_r<relation,relation, false> operator && (const relation & l, const relation & r) {
  return And_r<relation, relation, false>(l, r);
}
#else
template<typename R> inline
And_r<relation,R,IsTestOnlyRelation<R>::result> operator && (const relation & l, const R & r) {
  return And_r<relation, R, IsTestOnlyRelation<R>::result>(l, r);
}
#endif

#endif


template<typename L, typename R>
class ExOr_r : public Coroutine {
    L l; 
    R r;
	bool lSucceeded;
public:
    ExOr_r(const L& l, const R& r) : l(l), r(r), lSucceeded(false)
    { }

    bool operator () (void) {
      co_begin();
      while(l()) {
        lSucceeded=true;
        co_yield(true);
      }
      if(lSucceeded)
        co_return(false);
	  while(r()) {
        co_yield(true);
	  }
      co_end();
    }
};


#ifdef CASTOR_USE_GENERIC_EXOR
template<typename L, typename R> inline 
ExOr_r<L,R> operator ^ (const L & l, const R & r) {
	return ExOr_r<L,R>(l, r);
}
#else
inline
ExOr_r<relation,relation> operator ^ (const relation & l, const relation & r) {
	return ExOr_r<relation,relation>(l, r);
}
#endif

} // namespace castor
#endif
