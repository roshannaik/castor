// Castor : Logic Programming Library
// Copyright © 2008 Roshan Naik (roshan@mpprogramming.com).
// This software is goverened by the MIT license (http://www.opensource.org/licenses/mit-license.php).

#if !defined CASTOR_OPS_H
#define CASTOR_OPS_H 1

#include "relation.h"
#include "lref.h"
#include "workaround.h"

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
// default to undefined CASTOR_USE_GENERIC_AND
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
class Or_r {
    int state;
    L l;
    R r;
public:
    Or_r(const L & l, const R & r) : l(l), r(r), state(0)
    { }

    bool operator()(void) {
        switch(state) {
        case 0: 
            if(l())
                return true;
        case 1: 
            state=1;
            if(r())
                return true;
        default: 
            state=2;
            return false;
        } // switch
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

template<typename L, typename R>
class And_r {
	int state;
	L l;
    R r, rbegin;
public:
	And_r(const L& l, const R& r) : l(l), r(r), rbegin(r), state(0)
	{ }

	bool operator()(void) {
		switch (state) {
		case 0:
			while( l() ) {
		case 1:
			state=1;
				while( r() )
					return true;
				r = rbegin;
				state=0;
			}// while
		default:
			state=2;
			return false;
		} // switch
	}
};

#ifdef CASTOR_USE_GENERIC_AND
template<typename L, typename R> inline 
And_r<L,R> operator && (const L & l, const R & r) {
	return And_r<L,R>(l, r);
}
#else
inline 
And_r<relation,relation> operator && (const relation & l, const relation & r) {
	return And_r<relation,relation>(l, r);
}
#endif

template<typename L, typename R>
class ExOr_r {
    int state; 
    bool lSucceeded;
    L l; 
    R r;
public:
    ExOr_r(const L& l, const R& r) : l(l), r(r), state(0), lSucceeded(false)
    { }

    bool operator()(void) {
        switch(state) {
        case 0:
            if(l()) {
                lSucceeded=true;
                return true;
            }
        case 1:
            state=1;
            if(lSucceeded)
                return false;
        case 2:
            state=2;
            if(r())
               return true;
        default:
            state=3;
            return false;
        }
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
