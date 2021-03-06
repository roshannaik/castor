// Castor : Logic Programming Library for C++
// Copyright © 2007-2010 Roshan Naik (roshan@mpprogramming.com).
// This software is governed by the MIT license (http://www.opensource.org/licenses/mit-license.php).

#if !defined CASTOR_UTILS_H
#define CASTOR_UTILS_H 1

#include "lref.h"
#include "relation.h"
#include "eq.h"
#include "helpers.h"
#include "functional.h"

#include <list>
#include <deque>
#include <set>
#include <iterator>
#include <istream>
#include <iostream>
#include <algorithm>
#include <string>
#include <utility>


namespace castor {

struct IndexOutOfBounds {};

//-------------------------------------------------------------------------
//   Relation pause()
//-------------------------------------------------------------------------

template<class T>
struct Pause_r : Coroutine {
	lref<T> msg;
	Pause_r(const lref<T> &msg) : msg(msg)
	{ }
	bool operator()(void) {
		co_begin();
		if(msg.defined())
			std::cout << *msg;
		else
			std::cout << "undefined";
		std::cin.ignore();
		co_yield(true);
		co_end();
	}
};

template<class T>
struct Pause_r<T*> : Coroutine {
	T* msg;
	Pause_r(T* msg) : msg(msg)
	{ }
	bool operator()(void) {
		co_begin();
		std::cout << msg;
		std::cin.ignore();
		co_yield(true);
		co_end();
	}
};

template<typename T> inline
Pause_r<T> pause(lref<T>& s) {
	return Pause_r<T>(s);
}

template<typename T> inline
Pause_r<T> pause(const T& s) {
	return Pause_r<T>(lref<T>(s) );
}

// handles "string" and L"widestring" arguments
template<typename T> inline
Pause_r<T*> pause(T* s) {
	return Pause_r<T*>(s);
}


//-------------------------------------------------------------------------
//   Relation pause_f()
//-------------------------------------------------------------------------
template<class F>
struct PauseF_r : Coroutine {
	F func;
	PauseF_r(const F& func) : func(func)
	{ }
	bool operator()(void) {
		co_begin();
		std::cout << func();
		std::cin.ignore();
		co_yield(true);
		co_end();
	}
};

template<typename Func> inline
PauseF_r<Func> pause_f(Func f) {
	return PauseF_r<Func>(f);
}

//-------------------------------------------------------------------------
//   Relations defined() and undefined()
//      Equivalent to Prolog's metalogical predicates nonvar() and var()
//-------------------------------------------------------------------------

template<typename T>
struct Defined_r : public TestOnlyRelation<Defined_r<T> >{
    lref<T> r_;
    Defined_r(const lref<T>& r_) : r_(r_)
    { }

    bool apply(void) const {
        return r_.defined();
    }
};

template<typename T> inline  
Defined_r<T> defined(lref<T>& r_) {
    return Defined_r<T>(r_);
}

template<typename T>
struct UnDefined_r : public TestOnlyRelation<UnDefined_r<T> > {
    lref<T> r_;
    UnDefined_r(const lref<T>& r_) : r_(r_)
    { }

    bool apply(void) const {
        return !r_.defined();
    }
};

template<typename T> inline
UnDefined_r<T> undefined(lref<T>& r_) {
    return UnDefined_r<T>(r_);
}

//-------------------------------------------------------------------------
// empty/not_empty : To check if collection is empty or generate empty collection
//------------------------------------------------------------------------
// Tests for empty collection OR generates empty collection
template<typename Cont> inline
relation empty( lref<Cont>& c ) {
    return eq(c,Cont());
}

// this overload supports allows calls to empty without explicit template type arguments
// when c is not a lref.
template<typename Cont> inline 
relation empty(const Cont& c) {
    return Boolean( c.size()==0 );
}


template<typename Cont> inline
relation not_empty(lref<Cont>& c_) {
    lref<typename Cont::size_type> sz;
    return size(c_, sz) && predicate(sz!=(size_t)0);
}

// this overload allows calls to not_empty without explicit template type arguments
// when c is not a lref.
template<typename Cont> inline 
relation not_empty(const Cont& c) {
    return Boolean(c.size()!=0 );
}

//--------------------------------------------------------
//  Relation range
//--------------------------------------------------------
// Concepts : T<T, T==T 
//          : T is default constructable

template<typename T>
class Range_r : public Coroutine {
    lref<T> val, min_, max_;
public:
    Range_r(const lref<T>& val, const lref<T>& min_, const lref<T>& max_) : val(val), min_(min_), max_(max_)
    { }

    bool operator() () {
      co_begin();
      if(val.defined())
         co_return(    ( *min_<*val && *val<*max_ ) 
                     || ( *min_==*val ) 
                     || ( *max_==*val ) );
      for(val=min_; (*val<*max_) || (*val==*max_); ++val.get())
        co_yield(true);
      val.reset();
      co_end();
    }
};

// Concepts : T<T , T==T and prefix ++
template<typename T> inline
Range_r<T> range(lref<T> val, lref<T> min_, lref<T> max_) {
    return Range_r<T>(val, min_, max_);
}

template<typename T> inline
Range_r<T> range(lref<T> val, T min_, T max_) {
    return Range_r<T>(val, min_, max_);
}

//--------------------------------------------------------
//  Relation range (with step)
//--------------------------------------------------------
template<typename T>
class Range_Step_r : public Coroutine {
    lref<T> val, min_, max_, step_;
public:
    Range_Step_r(lref<T> val, const lref<T>& min_, const lref<T>& max_, const lref<T>& step_) : val(val), min_(min_), max_(max_), step_(step_)
    { }

    bool operator () (void) {
      co_begin();
      if(val.defined())
        co_return (    ( *min_<*val && *val<*max_ )
                     || ( *min_==*val )
                     || ( *max_==*val ) );

      for (val=min_; val.get()<*max_ || val.get()==*max_; val.get()+=*step_)
        co_yield(true);
      val.reset();
      co_end();
    }
};

// Concepts : T supports +=, < and ==
template<typename T> inline
Range_Step_r<T> range(lref<T> val, lref<T> min_, lref<T> max_, lref<T> step_) {
    return Range_Step_r<T>(val, min_, max_, step_);
}

template<typename T> inline
Range_Step_r<T> range(lref<T> val, T min_, T max_, T step_) {
    return Range_Step_r<T>(val, min_, max_, step_);
}

//--------------------------------------------------------
//  Relation range_dec
//--------------------------------------------------------
// Concepts : T>T, T==T 

template<typename T>
class RangeDec_r : public Coroutine {
    lref<T> val, min_, max_;
public:
    RangeDec_r(const lref<T>& val, const lref<T>& min_, const lref<T>& max_) : val(val), min_(min_), max_(max_)
    { }

    bool operator() () {
      co_begin();
      if(val.defined())
         co_return(    ( *min_<*val && *val<*max_ ) 
                     || ( *min_==*val ) 
                     || ( *max_==*val ) );
      for(val=max_; (*min_<val.get()) || (val.get()==*min_); --val.get())
        co_yield(true);
      val.reset();
      co_end();
    }
};

// Concepts : T<T , T==T and prefix --
template<typename T> inline
RangeDec_r<T> range_dec(lref<T> val, lref<T> max_, lref<T> min_) {
    return RangeDec_r<T>(val, min_, max_);
}

template<typename T> inline
RangeDec_r<T> range_dec(lref<T> val, T max_, T min_) {
    return RangeDec_r<T>(val, min_, max_);
}

//--------------------------------------------------------
//  Relation range_dec (with step)
//--------------------------------------------------------
template<typename T>
class RangeDec_Step_r : public Coroutine {
    lref<T> val, min_, max_, step_;
public:
    RangeDec_Step_r(lref<T> val, const lref<T>& min_, const lref<T>& max_, const lref<T>& step_) : val(val), min_(min_), max_(max_), step_(step_)
    { }

    bool operator () (void) {
      co_begin();
      if(val.defined())
        co_return (    ( *min_<*val && *val<*max_ )
                     || ( *min_==*val )
                     || ( *max_==*val ) );

      for (val=max_; *min_<val.get() || val.get()==*min_; val.get()-=*step_)
        co_yield(true);
      val.reset();
      co_end();
    }
};

// Concepts : T supports -=, < and ==
template<typename T> inline
RangeDec_Step_r<T> range_dec(lref<T> val, lref<T> max_, lref<T> min_, lref<T> step_) {
    return RangeDec_Step_r<T>(val, min_, max_, step_);
}

template<typename T> inline
RangeDec_Step_r<T> range_dec(lref<T> val, T max_, T min_, T step_) {
    return RangeDec_Step_r<T>(val, min_, max_, step_);
}


//--------------------------------------------------------
//  Relation item
//--------------------------------------------------------
template<class Itr>
class Item_r : public Coroutine {
    Itr itr, end_;
    typedef typename detail::Pointee<Itr>::result_type pointee_type;
    lref<pointee_type> obj;
public:
    Item_r(Itr beg_, Itr end_, const lref<pointee_type>& obj) : itr(beg_), end_(end_), obj(obj)
    { }

    bool operator () (void) {
      co_begin();
      if(obj.defined()) {
        for( ; effective_value(itr)!=effective_value(end_); ++effective_value(itr) ) {
          itr = std::find( effective_value(itr), effective_value(end_), obj.get() );
          co_yield(effective_value(itr)!=effective_value(end_));
        }
        co_return(false);
      }

      for ( ; effective_value(itr)!=effective_value(end_); ++effective_value(itr) ) {
        obj.set_ptr(&*effective_value(itr),false);
        co_yield(true);
      }
      obj.reset();
      co_end();
    }
};


template<typename Itr>
Item_r<Itr> item(lref<typename detail::Pointee<Itr>::result_type> obj, Itr begin_, Itr end_) {
    return Item_r<Itr>(begin_, end_, obj);
}


template<class Cont>
class ItemCont_r : public Coroutine {
    typedef typename Cont::value_type value_type;
    lref<value_type> obj;
    lref<Cont> cont;
    lref<typename Cont::iterator> i;
public:
    ItemCont_r(const lref<value_type>& obj, const lref<Cont>& cont_) : obj(obj), cont(cont_)
    { }

    bool operator () (void) {
      co_begin();
      if(obj.defined()) {
        for(i=cont->begin(); i.get()!=cont->end(); ++i.get()) {
          i = std::find( i.get(), cont->end(), obj.get() );
          co_yield(i.get()!=cont->end());
        }
        co_return(false);
      }
      for (i=cont->begin() ; i.get()!=cont->end(); ++i.get() ) {
        obj.set_ptr(&*i.get(),false);
        co_yield(true);
      }
      obj.reset();
      co_end();
    }
};


template<typename Cont>
ItemCont_r<Cont> item(lref<typename Cont::value_type> obj, lref<Cont>& cont_) {
    return ItemCont_r<Cont>(obj, cont_);
}

template<class Cont>
class ItemRCont_r : public Coroutine {
    typedef typename Cont::value_type value_type;
    lref<value_type> obj;
    lref<Cont> cont;
    lref<typename Cont::reverse_iterator> i;
public:
    ItemRCont_r(const lref<value_type>& obj, const lref<Cont>& cont_) : obj(obj), cont(cont_)
    { }

    bool operator () (void) {
      co_begin();
      if(obj.defined()) {
        for(i=cont->rbegin(); i.get()!=cont->rend(); ++i.get()) {
          i = std::find( i.get(), cont->rend(), obj.get() );
          co_yield(i.get()!=cont->rend());
        }
        co_return(false);
      }
      for (i=cont->rbegin() ; i.get()!=cont->rend(); ++i.get() ) {
        obj.set_ptr(&*i.get(),false);
        co_yield(true);
      }
      obj.reset();
      co_end();
    }
};

template<typename Cont> inline
ItemRCont_r<Cont> ritem(lref<typename Cont::value_type> obj, lref<Cont>& cont_) {
    return ItemRCont_r<Cont>(obj, cont_);
}

//-------------------------------------------------------------------------
// Relations unique, unique_f, unique_mem, unique_mf
//------------------------------------------------------------------------

template<typename T>
class Unique_r : public Coroutine {
    lref<T> item_;
	lref<std::set<T> > items;
public:
    Unique_r(const lref<T>& item_) : item_(item_), items(std::set<T>())
    { }

    bool operator () (void) {
      co_begin();
      co_return( items->insert(*item_).second );
      co_end();
    }
};

template<typename T> inline
Unique_r<T> unique(lref<T> item_) {
    return Unique_r<T>(item_);
}


// FuncObj = Nullary function object that has member typedef result_type
template<typename Func>
class Unique_f_r : public Coroutine {
    //typedef typename detail::return_type<Func>::result_type item_type;
    typedef typename Func::result_type item_type;
    lref<std::set<typename Func::result_type> > items;
    Func func;
public:
    Unique_f_r (const Func& func) : items(std::set<item_type>()), func(func)
    { }

    bool operator() (void) {
        co_begin();
        co_yield( items->insert(func()).second );
        co_end();
    }
};

// FuncObj = Nullary function object that has member typedef result_type
template<typename FuncObj> inline
Unique_f_r<FuncObj> unique_f(FuncObj f) {
    return Unique_f_r<FuncObj>(f);
}


template<typename Obj, typename MemberT>
class UniqueMem_r : public Coroutine {
	lref<Obj> obj_;
	MemberT Obj::* mem;
    lref<std::set<MemberT> > items;
public:
	UniqueMem_r(const lref<Obj>& obj_, MemberT Obj::* mem) : obj_(obj_), mem(mem), items(std::set<MemberT>())
	{ }

    bool operator() (void) {
		co_begin();
        co_yield( items->insert((*obj_).*mem).second );
		co_end();
	}
};

template<typename Obj, typename MemberT> inline
UniqueMem_r<Obj, MemberT> unique_mem(lref<Obj>& obj_, MemberT Obj::* mem) {
	return UniqueMem_r<Obj, MemberT>(obj_, mem);
}


template<typename R, typename MFunc, typename Obj>
class UniqueMf_r : Coroutine {
    lref<Obj> obj_;
    MFunc mf;
    lref<std::set<R> > items;
public:
    UniqueMf_r(lref<Obj>& obj_, MFunc mf) :obj_(obj_), mf(mf), items(std::set<R>())
    { }

    bool operator()(void) {
		co_begin();
        co_yield( items->insert( ((*obj_).*mf)() ).second );
		co_end();
    }
};

// Overloads for non-const member functions
template<typename R, typename Obj> inline
UniqueMf_r<R,R(Obj::*)(void), Obj>
unique_mf(lref<Obj>& obj_, R(Obj::*mf)(void) ) {
    return UniqueMf_r<R,R(Obj::*)(void), Obj>(obj_,mf);
}

template<typename R, typename Obj> inline
UniqueMf_r<R,R(Obj::*)(void) const, Obj>
unique_mf(lref<Obj>& obj_, R(Obj::*mf)(void) const) {
    return UniqueMf_r<R,R(Obj::*)(void) const, Obj>(obj_,mf);
}

//-------------------------------------------------------------------------
// Utility functions
//------------------------------------------------------------------------

namespace detail {
//    template<typename PointerToT, typename T> inline
//    T deref_lref(const lref<PointerToT>& ptr) {
//        return **ptr;
//    }

    template<typename Itr>
    typename detail::Pointee<Itr>::result_type
    deref_ptr(lref<Itr> ptr) {
        return **ptr;
    }
} // namespace detail

template<typename Itr> inline
relation dereference(lref<Itr> pointer_, lref<typename detail::Pointee<Itr>::result_type> pointee) {
  typedef typename detail::Pointee<Itr>::result_type T;
  return eq_f(pointee, detail::bind<T>(detail::deref_ptr<Itr>, pointer_));
}


// Requires T to support prefix ++
template<typename T> inline
relation next(lref<T> curr_, lref<T> n) {
    lref<T> temp;
    return eq(curr_,temp) && eq_f(n,++temp);
}

template<typename T> inline
relation next(T curr_, lref<T> n) {
	return eq(n,++curr_);
}
template<typename T> inline
relation next(T curr_, const T& n) {
	if(n==++curr_)
		return True();
	return False();
}


// Requires T to support prefix --
template<typename T> inline
relation prev(lref<T> curr_, lref<T> p) {
    lref<T> temp;
    return eq(curr_,temp) && eq_f(p,--temp);
}
template<typename T> inline
relation prev(T curr_, lref<T> p) {
    return   eq(p,--curr_);
}
template<typename T> inline
relation prev(T curr_, const T& p) {
    if(p==--curr_)
        return True();
    return False();
}

//-------------------------------------------------------------
// head/tail/head_tail : For operating on Collections
//-------------------------------------------------------------
template<typename Seq>
class Head_r : public Coroutine {
    typedef typename Seq::value_type T;
    lref<Seq> seq_;
    lref<T> h;
public:
    Head_r(const lref<Seq>& seq_, const lref<T>& h) : seq_(seq_), h(h)
    { }

    bool operator () (void) {
      co_begin();
      if(seq_->empty())
        co_return(false);
      if(h.defined())
          co_return( *h==*(seq_->begin()) );
      h=*(seq_->begin());
      co_yield(true);
      h.reset();
      co_end();
    }
};

template<typename Seq> inline
Head_r<Seq> head(lref<Seq>& seq_, lref<typename Seq::value_type> h) {
    return Head_r<Seq>(seq_,h);
}

//------------------------------------------------------------------
//   Head : h consists of 1st n (n>0) items in the collection
//------------------------------------------------------------------
template<typename Seq, typename HeadSeq>
class Head_n_r : public Coroutine {
    lref<Seq> seq_;
    lref<typename HeadSeq::size_type> n_;
    lref<HeadSeq> h;
public:
    Head_n_r(const lref<Seq>& seq_, const lref<typename HeadSeq::size_type>& n_, const lref<HeadSeq>& h) : seq_(seq_), n_(n_), h(h)
    { }
    bool operator () (void) {
      typename HeadSeq::const_iterator start, end;
      co_begin();
      if( *n_ > seq_->size() )
        co_return(false);
      if(seq_->empty())
        co_return(false);
      if(h.defined()) {
          if( h->size() > seq_->size() )
              co_return(false);
          co_return( std::equal(h->begin(),h->end(),seq_->begin()) );
      }
      start = end = seq_->begin();
      std::advance(end, *n_);
      h=HeadSeq(start,end);
      co_yield(true);
      h.reset();
      co_end();
    }
};


template<typename Seq, typename HeadSeq> inline
relation head_n(lref<Seq>& seq_, lref<typename HeadSeq::size_type> n, lref<HeadSeq>& h) {
    lref<typename Seq::size_type> sz;
    return size(seq_,sz) && range<typename Seq::size_type>(n,0,sz) && Head_n_r<Seq, HeadSeq>(seq_, n, h);
}

//Concepts: Collection should have a constructor taking (T* begin, T* end)
//          Collection should comparable using operator==
template<typename Seq, typename TailSeq>
class Tail_r : public Coroutine {
    lref<Seq> seq_;
    lref<TailSeq> t;
public:
    Tail_r(const lref<Seq>& seq_, const lref<TailSeq>& t) : seq_(seq_), t(t)
    { }

    bool operator() (void) {
      typename Seq::iterator b;
      co_begin();
      if(seq_->empty())
        co_return(false);
		  if(t.defined()) {
			  b = seq_->begin();
        co_return( *t == Seq(++b, seq_->end()) );
		  }
		  b = seq_->begin();
      t = Seq(++b, seq_->end());
      co_yield(true);
      t.reset();
      co_end();
    }
};


template<typename Seq, typename TailSeq> inline
Tail_r<Seq,TailSeq> tail(lref<Seq>& seq_, lref<TailSeq>& t) {
    return Tail_r<Seq,TailSeq>(seq_,t);
}

//------------------------------------------------------------------
//   Tail : 
//------------------------------------------------------------------
template<typename Seq, typename TailSeq=Seq>
class Tail_N_r : public Coroutine {
    lref<Seq> seq_;
    lref<typename TailSeq::size_type> n_;
    lref<TailSeq> t;
public:
    Tail_N_r(const lref<Seq>& seq_, const lref<typename TailSeq::size_type>& n_, const lref<TailSeq>& t) : seq_(seq_), n_(n_), t(t)
    { }

    bool operator() (void) {
      typename TailSeq::const_iterator start = seq_->begin(), end = seq_->end();
      co_begin();
      if(*n_ > seq_->size())
        co_return(false);
      if(t.defined()) {
          if( t->size() > seq_->size() )
            co_return(false);
          start = seq_->begin();
          std::advance(start, seq_->size() - *n_);
          co_return( std::equal(t->begin(),t->end(),start) );
      }
      std::advance(start, seq_->size() - *n_);
      t=TailSeq(start,end);
      co_yield(true);
      t.reset();
      co_end();
    }
};


template<typename Seq, typename TailSeq> inline
relation tail_n(lref<Seq>& seq_, lref<typename TailSeq::size_type> n, lref<TailSeq>& t) {
    lref<typename Seq::size_type> sz;
    return size(seq_,sz) && range<typename Seq::size_type>(n,0,sz) && Tail_N_r<Seq,TailSeq>(seq_, n, t);
}


template<typename Seq, typename TailSeq> inline
relation head_tail(lref<Seq>& seq_, lref<typename TailSeq::value_type> h, lref<TailSeq>& t) {
    return head(seq_,h) && tail(seq_, t);
}

template<typename Seq, typename HeadSeq> inline
relation head_n_tail(lref<Seq>& seq_, lref<typename HeadSeq::size_type> n, lref<HeadSeq>& h, lref<HeadSeq>& t) {
    lref<typename Seq::size_type> sz, tailSize;
    return head_n(seq_, n, h) && size(seq_,sz) && eq_f(tailSize,sz-n) && tail_n(seq_, tailSize, t);
}

// inserts a value into a collection
template<typename Seq>
relation insert(lref<typename Seq::value_type> value_, lref<typename Seq::iterator> b_, lref<typename Seq::iterator> e_, lref<Seq>& insertedSeq) {
	lref<typename Seq::value_type> v;
	lref<typename Seq::iterator> n;
	lref<Seq> tmp;
#ifdef __BCPLUSPLUS__
	relation (*self)(lref<typename Seq::value_type> , lref<typename Seq::iterator> , lref<typename Seq::iterator> , lref<Seq>& ) = &insert<Seq>;
	return   sequence(insertedSeq)(value_)(b_,e_)
		  || predicate(b_!=e_) && next(b_,n) && recurse(self,value_,n,e_,tmp) && dereference(b_,v) && sequence(insertedSeq)(v)(tmp);
#else
	return   sequence(insertedSeq)(value_)(b_,e_)
		  || predicate(b_!=e_) && next(b_,n) && recurse(&insert<Seq>,value_,n,e_,tmp) && dereference(b_,v) && sequence(insertedSeq)(v)(tmp);
#endif
}

// inserts a sequence into a sequence
template<typename Seq>
relation insert_seq(lref<typename Seq::iterator> valuesB_, lref<typename Seq::iterator> valuesE_, lref<typename Seq::iterator> b_, lref<typename Seq::iterator> e_, lref<Seq>& insertedSeq) {
	lref<typename Seq::value_type> v;
	lref<typename Seq::iterator> n;
	lref<Seq> tmp;
#ifdef __BCPLUSPLUS__
	relation (*self)(lref<typename Seq::iterator> , lref<typename Seq::iterator> , lref<typename Seq::iterator> , lref<typename Seq::iterator> , lref<Seq>& ) = &insert_seq<Seq>;
	return   sequence(insertedSeq)(valuesB_,valuesE_)(b_,e_)
		  || predicate(b_!=e_) && next(b_,n) && recurse(self,valuesB_,valuesE_,n,e_,tmp) && dereference(b_,v) && sequence(insertedSeq)(v)(tmp);
#else
	return   sequence(insertedSeq)(valuesB_,valuesE_)(b_,e_)
		  || predicate(b_!=e_) && next(b_,n) && recurse(&insert_seq<Seq>,valuesB_,valuesE_,n,e_,tmp) && dereference(b_,v) && sequence(insertedSeq)(v)(tmp);
#endif
}

//----------------------------------------------------------------------
//  increment/decrement relations
//----------------------------------------------------------------------

template<typename T>
class Inc_r : public Coroutine {
    lref<T> obj;
    lref<T> oldValue;
public:
    explicit Inc_r(const lref<T>& obj) : obj(obj)
    { }

    bool operator ()(void) {
      co_begin();
      oldValue=obj; // copy the original value
      ++(*obj);
      co_yield(true);
      obj=*oldValue;
      co_end();
    }
};

template<typename T> inline
Inc_r<T> inc(lref<T>& value_) {
    return Inc_r<T>(value_);
}


template<typename T>
class Dec_r : public Coroutine {
    lref<T> obj;
    lref<T> oldValue;
public:
    explicit Dec_r(const lref<T>& obj) : obj(obj)
    { }

    bool operator()(void) {
      co_begin();
      oldValue=obj; // copy the original value
      --(*obj);
      co_yield(true);
      obj=*oldValue;
      co_end();
    }
};

template<typename T>  inline
Dec_r<T> dec(lref<T>& value_) {
    return Dec_r<T>(value_);
}

//----------------------------------------------------------------------
// Convert a CollectionA<lref<T> > to CollectionB<T>
//     : Expects all items in collOfRef to be defined()... will throw is any one is undefined()
//----------------------------------------------------------------------
template<typename ContOfT, typename ContOfLrefT>
ContOfT getValueCont(const ContOfLrefT& seq) {
    ContOfT result;
    for(typename ContOfLrefT::const_iterator i = seq.begin(); i!=seq.end(); ++i)
        result.insert(result.end(), **i);
    return result;
}

// Concepts :
//     Seq : Seq<lref<T> > is default constructible
//                   : Seq::end()
//                   : Seq::insert(iter, value)
//                   : Seq::value_type provides the type of the values in the collection
//                   : Seq::size()
//                   : Seq::iterator
//
//     Seq::value_type : CopyConstructible, Assignable, Comparable
//
template<typename Seq>
class Sequence_r {
    typedef typename Seq::value_type T;
    typedef lref<typename Seq::iterator> LrefIter;
    lref<Seq> l;
    std::deque<T> r_Values;
    std::deque<lref<T> > r_Refs;
    std::deque<std::pair<LrefIter, LrefIter> >r_LrefPairs;
    std::deque<Seq> r_Lists;
    std::deque<lref<Seq> > r_refLists;
    enum elemKind {REF, VALUE, /*COLLECTION,*/ REF_COLLECTION, REF_PAIR };
	std::list<elemKind> history; // stores the order in which elements were added to r_Values, r_Refs and r_Lists
    enum {START, LIST_ASSIGNED, ELEMS_ASSSIGNED, END} state;
    bool ignore_;
    
public:
    Sequence_r(const lref<Seq>& l) : l(l), state(START), ignore_(false)
    { }

    Sequence_r& operator() (lref<T> item) { // appending new lref item into the collection
        r_Refs.push_back(item);
        history.push_back(REF);
        return *this;
    }

    //Sequence_r& operator() (const T& item) { // appending new item into the collection
    //    r_Values.push_back(item);
    //    history.push_back(VALUE);
    //    return *this;
    //}

    template<typename ConvertibleToT>
    Sequence_r& operator() (const ConvertibleToT& item) { // appending new item into the collection
        r_Values.push_back(item);
        history.push_back(VALUE);
        return *this;
    }

    Sequence_r& operator() (lref<Seq>& items) { // appending new items into the collection
        r_refLists.push_back(items);
        history.push_back(REF_COLLECTION);
        return *this;
    }

    //
    //template<typename Iter>
    //Sequence_r& operator() (Iter start, Iter end) { // appending new items from an arbitrary iterator delimited sequence into the collection
    //    detail::IfElse<is_lref<Iter>::result, push_lref_pair, push_values<Iter> >::type::apply(start, end, *this);
    //    return *this;
    //} 

    //template<typename Seq>
    //Sequence_r& operator() (const lref<typename Seq::iterator>& start
    //                       , const lref<typename Seq::iterator>& end) { // appending new items from an arbitrary iterator delimited sequence
    //    for(typename Seq::iterator i = *start ; i!=*end; ++i) {
    //        r_Values.push_back(*i);
    //        history.push_back(VALUE);
    //    }
    //    return *this;
    //}

    Sequence_r& operator() (LrefIter start, LrefIter end) {
        r_LrefPairs.push_back( make_pair(start,end) );
        history.push_back(REF_PAIR);
        return *this;
    }
    
    template<typename Iter>
    Sequence_r& operator() (Iter start, Iter end) {
        for(;start!=end; ++start) {
            r_Values.push_back(*start);
            history.push_back(VALUE);
        }
        return *this;
    }



    Sequence_r& ignore() {
        ignore_=true;
        return *this;
    }

    // Requires all items in rhsRefs and r_Lists to be defined.. else will throw 
    bool operator() (void) {
        switch(state) {
        case START:
            if(l.defined()) {
                state=END;
                return compareItems();
            }
            l=constructCollection();
            state=LIST_ASSIGNED;
            return true;
        case LIST_ASSIGNED:
            l.reset();
            state=END;
            return false;
        default: // case END:
            return false;
        }
    }
private:
    Seq constructCollection() {
        Seq result;
        for(typename std::list<elemKind>::iterator i=history.begin(); i!= history.end(); ++i) {
            switch(*i) {
            case VALUE:
                result.insert(result.end(), r_Values.front());
                r_Values.pop_front();
                break;
            case REF:
                result.insert(result.end(), *(r_Refs.front()));
                r_Refs.pop_front();
                break;
            case REF_PAIR: {
                LrefIter b = r_LrefPairs.front().first, e = r_LrefPairs.front().second;
                result.insert(result.end(), *b, *e); //std::copy(*b, *e, inserter(result,result.end()) );
                r_LrefPairs.pop_front();
                break;
            }
            default: { // ==REF_COLLECTION
                lref<Seq> items = r_refLists.front();
                r_refLists.pop_front();
                result.insert(result.end(), (*items).begin(), (*items).end() ); // std::copy((*items).begin(), (*items).end(), inserter(result,result.end()) );
                break;
            }
            }// switch
        }// for
        return result;
    }
    
    // invariant : l.defined()==true
    bool compareItems() { 
        typename Seq::const_iterator li = (*l).begin(), l_end = (*l).end();
        for(typename std::list<elemKind>::iterator hi=history.begin(); hi!= history.end(); ++hi) {
            if(li==(*l).end())
                return ignore_;
            switch(*hi) {
            case VALUE:
                if (*li != r_Values.front())
                    return false;
                r_Values.pop_front();
                ++li;
                break;
            case REF:
                if (*li != *r_Refs.front())
                    return false;
                r_Refs.pop_front();
                ++li;
                break;
            case REF_PAIR: {
                LrefIter b = r_LrefPairs.front().first, e = r_LrefPairs.front().second;
                size_t numItems = detail::countItems(*b, *e);
                if ( ((unsigned)detail::countItems(li,l_end)<numItems) || !std::equal(*b, *e, li) )
                    return false;
                std::advance(li, numItems);
                r_LrefPairs.pop_front();
                break;
            }
            default: { //==REF_COLLECTION
                typename Seq::iterator b = (*r_refLists.front()).begin(),
                                        e = (*r_refLists.front()).end();
                if ( (unsigned)detail::countItems(li,l_end)<(*r_refLists.front()).size()
                     || !std::equal(b, e, li) )
                    return false;
                std::advance(li, (*r_refLists.front()).size());
                r_refLists.pop_front();
                break;
            }
            }// switch
        }// for
        return li == (*l).end();
    }

};

template<typename Seq> inline
Sequence_r<Seq> sequence(lref<Seq>& seq) {
    return Sequence_r<Seq>(seq);
}

//--------------------------------------------------------
//  Size: s is the size of the collection coll
//--------------------------------------------------------

template<typename Cont>
class Size_r : public Coroutine {
    lref<typename Cont::size_type> sz;
    lref<Cont> cont_;
public:
    Size_r(const lref<Cont>& cont_, const lref<typename Cont::size_type>& sz) : sz(sz), cont_(cont_)
    { }

    bool operator() (void) {
      co_begin();
      if(sz.defined())
        co_return( *sz==cont_->size() );
      sz=cont_->size();
      co_yield(true);
      sz.reset();
      co_end();
    }
};
// Concept: Cont provides member function size and member typedef size_type
template<typename Cont> inline
Size_r<Cont> size(lref<Cont>& cont_, lref<typename Cont::size_type> sz) {
    return Size_r<Cont>(cont_, sz);
}

//--------------------------------------------------------
//  Merge : m is the sorted merge of the two sorted sequences l, r
//--------------------------------------------------------

template<typename Seq>
relation merge(lref<Seq>& l_, lref<Seq>& r_, lref<Seq>& m) {
	lref<typename Seq::value_type> h1, h2;
	lref<Seq> t1, t2, tmp;
#ifdef __BCPLUSPLUS__
	relation (*self)(lref<Seq>&, lref<Seq>&, lref<Seq>&) = &merge<Seq>;
	return     ( empty(l_) && eq(m,r_) )
			^  ( empty(r_) && eq(m,l_) )
			^  ( head_tail(l_,h1,t1) && head_tail(r_,h2,t2)
				   && (   predicate(h1==h2) && recurse(self,t1,t2,tmp) && sequence(m)(h1)(h2)(tmp)
					   || predicate(h1<h2)  && recurse(self,t1,r_,tmp)  && sequence(m)(h1)(tmp)
					   || predicate(h1>h2)  && recurse(self,l_,t2,tmp)  && sequence(m)(h2)(tmp)
					   )
				);
#else
	return     ( empty(l_) && eq(m,r_) )
			^  ( empty(r_) && eq(m,l_) )
			^  ( head_tail(l_,h1,t1) && head_tail(r_,h2,t2)
				   && (   predicate(h1==h2) && recurse(merge<Seq>,t1,t2,tmp) && sequence(m)(h1)(h2)(tmp)
					   || predicate(h1<h2)  && recurse(merge<Seq>,t1,r_,tmp)  && sequence(m)(h1)(tmp)
					   || predicate(h1>h2)  && recurse(merge<Seq>,l_,t2,tmp)  && sequence(m)(h2)(tmp)
					   )
				);
#endif
}

//--------------------------------------------------------
//  begin relation : For working with iterators
//--------------------------------------------------------
template<typename Cont>
class Begin_r : public Coroutine {
    typedef typename Cont::iterator Iter;
    lref<Cont> cont_;
    lref<Iter> iter;
public:
    Begin_r(const lref<Cont>& cont_, const lref<Iter>& iter) : cont_(cont_), iter(iter)
    { }

    bool operator()(void) {
      co_begin();
      if(iter.defined())
        co_return( *iter==cont_->begin() );
      iter=cont_->begin();
      co_yield(true);
      iter.reset();
      co_end();
    }
};

// 1st argument disallows a raw vector to be passed as argument... since passing 
// a raw vector to a lref causes the lref to make a copy of the vector.
template<typename Cont> inline
Begin_r<Cont> begin(lref<Cont>& cont_, const lref<typename Cont::iterator>& iter) {
    return Begin_r<Cont>(cont_, iter);
}

//--------------------------------------------------------
//  end relation : For working with iterators 
//--------------------------------------------------------
template<typename Cont>
class End_r : public Coroutine {
    typedef typename Cont::iterator IterT;
    lref<Cont> cont_;
    lref<IterT> iter;
public:
    End_r(const lref<Cont>& cont_, const lref<IterT>& iter) : cont_(cont_), iter(iter)
    { }

    bool operator()(void) {
      co_begin();
      if(iter.defined())
        co_return( *iter==cont_->end() );
      iter=cont_->end();
      co_yield(true);
      iter.reset();
      co_end();
    }
};

template<typename Cont> inline
End_r<Cont> end(lref<Cont>& cont_, const lref<typename Cont::iterator>& iter) {
    return End_r<Cont>(cont_, iter);
}


//--------------------------------------------------------
//  begin relation : For working with reverse iterators
//--------------------------------------------------------
template<typename Cont>
class RBegin_r : public Coroutine {
    typedef typename Cont::reverse_iterator Iter;
    lref<Cont> cont_;
    lref<Iter> iter;
public:
    RBegin_r(const lref<Cont>& cont_, const lref<Iter>& iter) : cont_(cont_), iter(iter)
    { }

    bool operator()(void) {
      co_begin();
      if(iter.defined())
        co_return( *iter==cont_->rbegin() );
      iter=cont_->rbegin();
      co_yield(true);
      iter.reset();
      co_end();
    }
};

// 1st argument disallows a raw vector to be passed as argument... since passing 
// a raw vector to a lref causes the lref to make a copy of the vector.
template<typename Cont> inline
RBegin_r<Cont> rbegin(lref<Cont>& cont_, const lref<typename Cont::reverse_iterator>& iter) {
    return RBegin_r<Cont>(cont_, iter);
}

//--------------------------------------------------------
//  rend relation : For working with reverse iterators 
//--------------------------------------------------------
template<typename Cont>
class REnd_r : public Coroutine {
    typedef typename Cont::reverse_iterator IterT;
    lref<Cont> cont_;
    lref<IterT> iter;
public:
    REnd_r(const lref<Cont>& cont_, const lref<IterT>& iter) : cont_(cont_), iter(iter)
    { }

    bool operator()(void) {
      co_begin();
      if(iter.defined())
        co_return( *iter==cont_->rend() );
      iter=cont_->rend();
      co_yield(true);
      iter.reset();
      co_end();
    }
};

template<typename Cont> inline
REnd_r<Cont> rend(lref<Cont>& cont_, const lref<typename Cont::reverse_iterator>& iter) {
    return REnd_r<Cont>(cont_, iter);
}



//-------------------------------------------------
// eval() - Invoke the function/function object: Succeeds only once
//-------------------------------------------------
template<typename Func>
class Eval_r0 : Coroutine {
    Func f;
public:
    typedef typename detail::return_type<Func>::result_type result_type;
    Eval_r0(Func f) :f(f)
    { }

    bool operator()(void) {
        co_begin();
        f();
        co_return(true);
        co_end();
    }
};

template<typename Func, typename A1>
class Eval_r1 : Coroutine {
    Func f;
    A1 a1;
public:
    typedef typename detail::return_type<Func>::result_type result_type;
    Eval_r1(Func f, const A1& a1) :f(f), a1(a1)
    { }

    bool operator()(void) {
        co_begin();
        f(effective_value(a1));
        co_return(true);
        co_end();
    }
};

template<typename Func, typename A1, typename A2>
class Eval_r2 : Coroutine {
    Func f;
    A1 a1; A2 a2;
public:
    typedef typename detail::return_type<Func>::result_type result_type;
    Eval_r2(Func f, const A1& a1, const A2& a2) :f(f), a1(a1), a2(a2)
    { }

    bool operator()(void) {
        co_begin();
        f(effective_value(a1), effective_value(a2));
        co_return(true);
        co_end();
    }
};

template<typename Func, typename A1, typename A2, typename A3>
class Eval_r3 : Coroutine {
    Func f;
    A1 a1; A2 a2; A3 a3;
public:
    typedef typename detail::return_type<Func>::result_type result_type;
    Eval_r3(Func f, const A1& a1, const A2& a2, const A3& a3) :f(f), a1(a1), a2(a2), a3(a3)
    { }

    bool operator()(void) {
        co_begin();
        f(effective_value(a1), effective_value(a2), effective_value(a3));
        co_return(true);
        co_end();
    }
};

template<typename Func, typename A1, typename A2, typename A3, typename A4>
class Eval_r4 : Coroutine {
    Func f;
    A1 a1; A2 a2; A3 a3; A4 a4;
public:
    typedef typename detail::return_type<Func>::result_type result_type;
    Eval_r4(Func f, const A1& a1, const A2& a2, const A3& a3, const A4& a4) :f(f), a1(a1), a2(a2), a3(a3), a4(a4)
    { }

    bool operator()(void) {
        co_begin();
        f(effective_value(a1), effective_value(a2), effective_value(a3), effective_value(a4));
        co_return(true);
        co_end();
    }
};

template<typename Func, typename A1, typename A2, typename A3, typename A4, typename A5>
class Eval_r5 : Coroutine {
    Func f;
    A1 a1; A2 a2; A3 a3; A4 a4; A5 a5;
public:
    typedef typename detail::return_type<Func>::result_type result_type;
    Eval_r5(Func f, const A1& a1, const A2& a2, const A3& a3, const A4& a4, const A5& a5) :f(f), a1(a1), a2(a2), a3(a3), a4(a4), a5(a5)
    { }

    bool operator()(void) {
        co_begin();
        f(effective_value(a1), effective_value(a2), effective_value(a3), effective_value(a4), effective_value(a5));
        co_return(true);
        co_end();
    }
};

template<typename Func, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6>
class Eval_r6 : Coroutine {
    Func f;
    A1 a1; A2 a2; A3 a3; A4 a4; A5 a5; A6 a6;
public:
    typedef typename detail::return_type<Func>::result_type result_type;
    Eval_r6(Func f, const A1& a1, const A2& a2, const A3& a3, const A4& a4, const A5& a5, const A6& a6) :f(f), a1(a1), a2(a2), a3(a3), a4(a4), a5(a5), a6(a6)
    { }

    bool operator()(void) {
        co_begin();
        f(effective_value(a1), effective_value(a2), effective_value(a3), effective_value(a4), effective_value(a5), effective_value(a6));
        co_return(true);
        co_end();
    }
};

// nullary
template<typename Func> inline
Eval_r0<Func> eval(Func f) {
    return Eval_r0<Func>(f);
}

template<typename R> inline
Eval_r0<R(*)(void)> eval(R(* f)(void)) {
    return Eval_r0<R(*)(void)>(f);
}

// unary
template<typename Func, typename A1> inline
Eval_r1<Func,A1> eval(Func f, const A1& a1_) {
    return Eval_r1<Func,A1>(f,a1_);
}

template<typename R, typename P1, typename A1> inline
Eval_r1<R(*)(P1),A1> eval(R(* f)(P1), const A1& a1_) {
    return Eval_r1<R(*)(P1),A1>(f,a1_);
}

// binary
template<typename Func, typename A1, typename A2> inline
Eval_r2<Func,A1,A2> eval(Func f, const A1& a1_, const A2& a2_) {
    return Eval_r2<Func,A1,A2>(f,a1_,a2_);
}

template<typename R, typename P1, typename P2, typename A1, typename A2> inline
Eval_r2<R(*)(P1,P2),A1,A2> eval(R(* f)(P1,P2), const A1& a1_, const A2& a2_) {
    return Eval_r2<R(*)(P1,P2),A1,A2>(f,a1_,a2_);
}

// ternary
template<typename Func, typename A1, typename A2, typename A3> inline
Eval_r3<Func,A1,A2,A3> eval(Func f, const A1& a1_, const A2& a2_, const A3& a3_) {
    return Eval_r3<Func,A1,A2,A3>(f,a1_,a2_,a3_);
}

template<typename R, typename P1, typename P2, typename P3, typename A1, typename A2, typename A3> inline
Eval_r3<R(*)(P1,P2,P3),A1,A2,A3> eval(R(* f)(P1,P2,P3), const A1& a1_, const A2& a2_, const A3& a3_) {
    return Eval_r3<R(*)(P1,P2,P3),A1,A2,A3>(f,a1_,a2_,a3_);
}

// quaternary
template<typename Func, typename A1, typename A2, typename A3, typename A4> inline
Eval_r4<Func,A1,A2,A3,A4> eval(Func f, const A1& a1_, const A2& a2_, const A3& a3_, const A4& a4_) {
    return Eval_r4<Func,A1,A2,A3,A4>(f,a1_,a2_,a3_,a4_);
}

template<typename R, typename P1, typename P2, typename P3, typename P4, typename A1, typename A2, typename A3, typename A4> inline
Eval_r4<R(*)(P1,P2,P3,P4),A1,A2,A3,A4> eval(R(* f)(P1,P2,P3,P4), const A1& a1_, const A2& a2_, const A3& a3_, const A4& a4_) {
    return Eval_r4<R(*)(P1,P2,P3,P4),A1,A2,A3,A4>(f,a1_,a2_,a3_,a4_);
}

// quinary
template<typename Func, typename A1, typename A2, typename A3, typename A4, typename A5> inline
Eval_r5<Func,A1,A2,A3,A4,A5> eval(Func f, const A1& a1_, const A2& a2_, const A3& a3_, const A4& a4_, const A5& a5_) {
    return Eval_r5<Func,A1,A2,A3,A4,A5>(f,a1_,a2_,a3_,a4_,a5_);
}

template<typename R, typename P1, typename P2, typename P3, typename P4, typename P5, typename A1, typename A2, typename A3, typename A4, typename A5> inline
Eval_r5<R(*)(P1,P2,P3,P4,P5),A1,A2,A3,A4,A5> eval(R(* f)(P1,P2,P3,P4,P5), const A1& a1_, const A2& a2_, const A3& a3_, const A4& a4_, const A5& a5_) {
    return Eval_r5<R(*)(P1,P2,P3,P4,P5),A1,A2,A3,A4,A5>(f,a1_,a2_,a3_,a4_,a5_);
}

// sestary
template<typename Func, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6> inline
Eval_r6<Func,A1,A2,A3,A4,A5,A6> eval(Func f, const A1& a1_, const A2& a2_, const A3& a3_, const A4& a4_, const A5& a5_, const A6& a6_) {
    return Eval_r6<Func,A1,A2,A3,A4,A5,A6>(f,a1_,a2_,a3_,a4_,a5_,a6_);
}

template<typename R, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6> inline
Eval_r6<R(*)(P1,P2,P3,P4,P5,P6),A1,A2,A3,A4,A5,A6> eval(R(* f)(P1,P2,P3,P4,P5,P6), const A1& a1_, const A2& a2_, const A3& a3_, const A4& a4_, const A5& a5_, const A6& a6_) {
    return Eval_r6<R(*)(P1,P2,P3,P4,P5,P6),A1,A2,A3,A4,A5,A6>(f,a1_,a2_,a3_,a4_,a5_,a6_);
}

//-------------------------------------------------
// eval_mf() - Invoke the function/function object: Always succeeds once only
//-------------------------------------------------
template<typename Obj, typename MemFunc>
class Eval_mf_r0 : Coroutine {
    lref<Obj> obj_;
	MemFunc mf;
public:
    Eval_mf_r0(lref<Obj> obj_, MemFunc mf) :obj_(obj_), mf(mf)
    { }

    bool operator()(void) {
        co_begin();
        ((*obj_).*mf)();
        co_return(true);
        co_end();
    }
};

template<typename Obj, typename MemFunc, typename A1>
class Eval_mf_r1 : Coroutine {
    lref<Obj> obj_;
	MemFunc mf;
    A1 a1;
public:
    Eval_mf_r1(lref<Obj> obj_, MemFunc mf, const A1& a1) :obj_(obj_), mf(mf), a1(a1)
    { }

    bool operator()(void) {
        co_begin();
        ((*obj_).*mf)(effective_value(a1));
        co_return(true);
        co_end();
    }
};

template<typename Obj, typename MemFunc, typename A1, typename A2>
class Eval_mf_r2 : Coroutine {
    lref<Obj> obj_;
	MemFunc mf;
    A1 a1; A2 a2;
public:
    Eval_mf_r2(lref<Obj> obj_, MemFunc mf, const A1& a1, const A2& a2) :obj_(obj_), mf(mf), a1(a1), a2(a2)
    { }

    bool operator()(void) {
        co_begin();
        ((*obj_).*mf)(effective_value(a1), effective_value(a2));
        co_return(true);
        co_end();
    }
};

template<typename Obj, typename MemFunc, typename A1, typename A2, typename A3>
class Eval_mf_r3 : Coroutine {
    lref<Obj> obj_;
	MemFunc mf;
    A1 a1; A2 a2; A3 a3;
public:
    Eval_mf_r3(lref<Obj> obj_, MemFunc mf, const A1& a1, const A2& a2, const A3& a3) :obj_(obj_), mf(mf), a1(a1), a2(a2), a3(a3)
    { }

    bool operator()(void) {
        co_begin();
        ((*obj_).*mf)(effective_value(a1), effective_value(a2), effective_value(a3));
        co_return(true);
        co_end();
    }
};

template<typename Obj, typename MemFunc, typename A1, typename A2, typename A3, typename A4>
class Eval_mf_r4 : Coroutine {
    lref<Obj> obj_;
	MemFunc mf;
    A1 a1; A2 a2; A3 a3; A4 a4;
public:
    Eval_mf_r4(lref<Obj> obj_, MemFunc mf, const A1& a1, const A2& a2, const A3& a3, const A4& a4) :obj_(obj_), mf(mf), a1(a1), a2(a2), a3(a3), a4(a4)
    { }

    bool operator()(void) {
        co_begin();
        ((*obj_).*mf)(effective_value(a1), effective_value(a2), effective_value(a3), effective_value(a4));
        co_return(true);
        co_end();
    }
};

template<typename Obj, typename MemFunc, typename A1, typename A2, typename A3, typename A4, typename A5>
class Eval_mf_r5 : Coroutine {
    lref<Obj> obj_;
	MemFunc mf;
    A1 a1; A2 a2; A3 a3; A4 a4; A5 a5;
public:
    Eval_mf_r5(lref<Obj> obj_, MemFunc mf, const A1& a1, const A2& a2, const A3& a3, const A4& a4, const A5& a5) :obj_(obj_), mf(mf), a1(a1), a2(a2), a3(a3), a4(a4), a5(a5)
    { }

    bool operator()(void) {
        co_begin();
        ((*obj_).*mf)(effective_value(a1), effective_value(a2), effective_value(a3), effective_value(a4), effective_value(a5));
        co_return(true);
        co_end();
    }
};

template<typename Obj, typename MemFunc, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6>
class Eval_mf_r6 : Coroutine {
    lref<Obj> obj_;
	MemFunc mf;
    A1 a1; A2 a2; A3 a3; A4 a4; A5 a5; A6 a6;
public:
    Eval_mf_r6(lref<Obj> obj_, MemFunc mf, const A1& a1, const A2& a2, const A3& a3, const A4& a4, const A5& a5, const A6& a6) :obj_(obj_), mf(mf), a1(a1), a2(a2), a3(a3), a4(a4), a5(a5), a6(a6)
    { }

    bool operator()(void) {
        co_begin();
        ((*obj_).*mf)(effective_value(a1), effective_value(a2), effective_value(a3), effective_value(a4), effective_value(a5), effective_value(a6));
        co_return(true);
        co_end();
    }
};

// Overloads for non-const member functions
template<typename R, typename Obj> inline
Eval_mf_r0<Obj,R(Obj::*)(void)> 
eval_mf(lref<Obj>& obj_, R(Obj::*mf)(void) ) {
    return Eval_mf_r0<Obj,R(Obj::*)(void)>(obj_,mf);
}

template<typename R, typename P1, typename Obj, typename A1> inline
Eval_mf_r1<Obj,R(Obj::*)(P1),A1> 
eval_mf(lref<Obj>& obj_, R(Obj::* mf)(P1), const A1& a1_) {
    return Eval_mf_r1<Obj,R(Obj::*)(P1),A1>(obj_,mf,a1_);
}

template<typename R, typename P1, typename P2, typename Obj, typename A1, typename A2> inline
Eval_mf_r2<Obj,R(Obj::*)(P1,P2),A1,A2> 
eval_mf(lref<Obj>& obj_, R(Obj::* mf)(P1,P2), const A1& a1_, const A2& a2_) {
    return Eval_mf_r2<Obj,R(Obj::*)(P1,P2),A1,A2>(obj_,mf,a1_,a2_);
}

template<typename R, typename P1, typename P2, typename P3, typename Obj, typename A1, typename A2, typename A3> inline
Eval_mf_r3<Obj,R(Obj::*)(P1,P2,P3),A1,A2,A3> 
eval_mf(lref<Obj>& obj_, R(Obj::* mf)(P1,P2,P3), const A1& a1_, const A2& a2_, const A3& a3_) {
    return Eval_mf_r3<Obj,R(Obj::*)(P1,P2,P3),A1,A2,A3>(obj_,mf,a1_,a2_,a3_);
}

template<typename R, typename P1, typename P2, typename P3, typename P4, typename Obj, typename A1, typename A2, typename A3, typename A4> inline
Eval_mf_r4<Obj,R(Obj::*)(P1,P2,P3,P4),A1,A2,A3,A4> 
eval_mf(lref<Obj>& obj_, R(Obj::* mf)(P1,P2,P3,P4), const A1& a1_, const A2& a2_, const A3& a3_, const A4& a4_) {
    return Eval_mf_r4<Obj,R(Obj::*)(P1,P2,P3,P4),A1,A2,A3,A4>(obj_,mf,a1_,a2_,a3_,a4_);
}

template<typename R, typename P1, typename P2, typename P3, typename P4, typename P5, typename Obj, typename A1, typename A2, typename A3, typename A4, typename A5> inline
Eval_mf_r5<Obj,R(Obj::*)(P1,P2,P3,P4,P5),A1,A2,A3,A4,A5> 
eval_mf(lref<Obj>& obj_, R(Obj::* mf)(P1,P2,P3,P4,P5), const A1& a1_, const A2& a2_, const A3& a3_, const A4& a4_, const A5& a5_) {
    return Eval_mf_r5<Obj,R(Obj::*)(P1,P2,P3,P4,P5),A1,A2,A3,A4,A5>(obj_,mf,a1_,a2_,a3_,a4_,a5_);
}

template<typename R, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename Obj, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6> inline
Eval_mf_r6<Obj,R(Obj::*)(P1,P2,P3,P4,P5,P6),A1,A2,A3,A4,A5,A6> 
eval_mf(lref<Obj>& obj_, R(Obj::* mf)(P1,P2,P3,P4,P5,P6), const A1& a1_, const A2& a2_, const A3& a3_, const A4& a4_, const A5& a5_, const A6& a6_) {
    return Eval_mf_r6<Obj,R(Obj::*)(P1,P2,P3,P4,P5,P6),A1,A2,A3,A4,A5,A6>(obj_,mf,a1_,a2_,a3_,a4_,a5_,a6_);
}

// Overloads for const member functions
template<typename R, typename Obj> inline
Eval_mf_r0<Obj,R(Obj::*)(void) const> 
eval_mf(lref<Obj>& obj_, R(Obj::*mf)(void) const) {
    return Eval_mf_r0<Obj,R(Obj::*)(void) const>(obj_,mf);
}

template<typename R, typename P1, typename Obj, typename A1> inline
Eval_mf_r1<Obj,R(Obj::*)(P1) const,A1> 
eval_mf(lref<Obj>& obj_, R(Obj::* mf)(P1) const, const A1& a1_) {
    return Eval_mf_r1<Obj,R(Obj::*)(P1) const,A1>(obj_,mf,a1_);
}

template<typename R, typename P1, typename P2, typename Obj, typename A1, typename A2> inline
Eval_mf_r2<Obj,R(Obj::*)(P1,P2) const,A1,A2> 
eval_mf(lref<Obj>& obj_, R(Obj::* mf)(P1,P2) const, const A1& a1_, const A2& a2_) {
    return Eval_mf_r2<Obj,R(Obj::*)(P1,P2) const,A1,A2>(obj_,mf,a1_,a2_);
}

template<typename R, typename P1, typename P2, typename P3, typename Obj, typename A1, typename A2, typename A3> inline
Eval_mf_r3<Obj,R(Obj::*)(P1,P2,P3) const,A1,A2,A3> 
eval_mf(lref<Obj>& obj_, R(Obj::* mf)(P1,P2,P3) const, const A1& a1_, const A2& a2_, const A3& a3_) {
    return Eval_mf_r3<Obj,R(Obj::*)(P1,P2,P3) const,A1,A2,A3>(obj_,mf,a1_,a2_,a3_);
}

template<typename R, typename P1, typename P2, typename P3, typename P4, typename Obj, typename A1, typename A2, typename A3, typename A4> inline
Eval_mf_r4<Obj,R(Obj::*)(P1,P2,P3,P4) const,A1,A2,A3,A4> 
eval_mf(lref<Obj>& obj_, R(Obj::* mf)(P1,P2,P3,P4) const, const A1& a1_, const A2& a2_, const A3& a3_, const A4& a4_) {
    return Eval_mf_r4<Obj,R(Obj::*)(P1,P2,P3,P4) const,A1,A2,A3,A4>(obj_,mf,a1_,a2_,a3_,a4_);
}

template<typename R, typename P1, typename P2, typename P3, typename P4, typename P5, typename Obj, typename A1, typename A2, typename A3, typename A4, typename A5> inline
Eval_mf_r5<Obj,R(Obj::*)(P1,P2,P3,P4,P5) const,A1,A2,A3,A4,A5> 
eval_mf(lref<Obj>& obj_, R(Obj::* mf)(P1,P2,P3,P4,P5) const, const A1& a1_, const A2& a2_, const A3& a3_, const A4& a4_, const A5& a5_) {
    return Eval_mf_r5<Obj,R(Obj::*)(P1,P2,P3,P4,P5) const,A1,A2,A3,A4,A5>(obj_,mf,a1_,a2_,a3_,a4_,a5_);
}

template<typename R, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename Obj, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6> inline
Eval_mf_r6<Obj,R(Obj::*)(P1,P2,P3,P4,P5,P6) const,A1,A2,A3,A4,A5,A6> 
eval_mf(lref<Obj>& obj_, R(Obj::* mf)(P1,P2,P3,P4,P5,P6) const, const A1& a1_, const A2& a2_, const A3& a3_, const A4& a4_, const A5& a5_, const A6& a6_) {
    return Eval_mf_r6<Obj,R(Obj::*)(P1,P2,P3,P4,P5,P6) const,A1,A2,A3,A4,A5,A6>(obj_,mf,a1_,a2_,a3_,a4_,a5_,a6_);
}


//-------------------------------------------------
// repeat(val_i,count_i,r) - repeat val_i, count_i times, into r
//-------------------------------------------------

template<typename T>
struct Repeat_r : public Coroutine {
	lref<T> val, val_i, r;
	unsigned int count_i;
	unsigned int i;
	Repeat_r(const lref<T>& val_i, const unsigned int count_i, const lref<T>& r) : val(), val_i(val_i), r(r), count_i(count_i) { 
	}

	bool operator()(void) {
		co_begin();
		val = r;  // save
		r = val_i;
		for(i=count_i; i!=0; --i)
			co_yield(true);
		r = val;  // restore
		co_end();
	}
};


template<typename T>
Repeat_r<T> repeat(lref<T>& val_i,unsigned int count_i, lref<T>& r) {
	return Repeat_r<T>(val_i, count_i, r);
}

template<typename T>
Repeat_r<T> repeat(T val_i, unsigned int count_i, lref<T>& r) {
	return Repeat_r<T>(lref<T>(val_i), count_i, r);
}


} // namespace castor
#endif
