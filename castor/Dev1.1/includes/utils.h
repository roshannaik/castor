// Castor : Logic Programming Library for C++
// Copyright © 2007 Roshan Naik (naikrosh@gmail.com)

#if !defined CASTOR_UTILS_H
#define CASTOR_UTILS_H 1

#include <list>
#include <deque>
#include <set>
#include <iterator>
#include <istream>
#include <algorithm>
#include <string>
#include <utility>

#include "lref.h"
#include "relation.h"
#include "eq.h"
#include "helpers.h"

namespace castor {

struct IndexOutOfBounds {};


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
    return size(c_, sz) && predicate(sz!=0);
}

// this overload allows calls to not_empty without explicit template type arguments
// when c is not a lref.
template<typename Cont> inline 
relation not_empty(const Cont& c) {
    return Boolean(c.size()!=0 );
}

//--------------------
//  Relation range
//--------------------
// Concepts : T<T, T==T 
//          : T is default constructable

template<typename T>
class Range_r : public custom_relation {
    lref<T> val, min_, max_;
public:
    Range_r(const lref<T>& val, const lref<T>& min_, const lref<T>& max_) : min_(min_), max_(max_), val(val)
    { }

    bool operator() () {
      rel_begin();
      if(val.defined())
         rel_return(    ( *min_<*val && *val<*max_ ) 
                     || ( *min_==*val ) 
                     || ( *max_==*val ) );
      for(val=min_; (*val<*max_) || (*val==*max_); ++val.get())
        rel_yield(true);
      val.reset();
      rel_end();
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

//--------------------
//  Relation range (with step)
//--------------------
template<typename T>
class Range_Step_r : public custom_relation {
    lref<T> val, min_, max_, step_;
public:
    Range_Step_r(lref<T> val, const lref<T>& min_, const lref<T>& max_, const lref<T>& step_) : min_(min_), max_(max_), step_(step_), val(val)
    { }

    bool operator () (void) {
      rel_begin();
      if(val.defined())
        rel_return (    ( *min_<*val && *val<*max_ )
                     || ( *min_==*val )
                     || ( *max_==*val ) );

      for (val=min_; *val<*max_ || *val==*max_; *val+=*step_)
        rel_yield(true);
      val.reset();
      rel_end();
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


//--------------------
//  Relation item
//--------------------
template<typename Itr>
class Item_r : public custom_relation {
    Itr itr, end_;
    typedef typename ::castor::detail::Pointee<Itr>::result_type pointee_type;
    lref<pointee_type> obj;
public:
    Item_r(Itr beg_, Itr end_, const lref<pointee_type>& obj) : itr(beg_), end_(end_), obj(obj)
    { }

    bool operator () (void) {
      rel_begin();
      if(obj.defined()) 
        rel_return ( std::count( effective_value(itr), effective_value(end_), *obj)!=0 );

      for ( ; effective_value(itr)!=effective_value(end_); ++effective_value(itr) ) {
        obj=*effective_value(itr);
        rel_yield(true)
      }
      obj.reset();
      rel_end();
    }
};

template<typename Itr>
Item_r<Itr> item(lref<typename ::castor::detail::Pointee<Itr>::result_type> obj, Itr begin_, Itr end_) {
    return Item_r<Itr>(begin_, end_, obj);
}

template<typename Cont>
relation item(lref<typename Cont::value_type> obj, lref<Cont>& cont_) {
    lref<typename Cont::iterator> b, e;
    return begin(cont_,b) && end(cont_, e) && item(obj, b, e);
}

//-------------------------------------------------------------------------
// Relations unique and unique_f
//------------------------------------------------------------------------

template<typename T>
class Unique_r : public custom_relation {
    lref<std::set<T> > items;
    lref<T> item_;
public:
    Unique_r(const lref<T>& item_) : item_(item_), items(std::set<T>())
    { }

    bool operator () (void) {
      rel_begin();
      if( items->find(*item_) != items->end() )
        rel_return(false);
      items->insert(*item_);
      rel_return(true);
      rel_end();
    }
};

template<typename T> inline
Unique_r<T> unique(lref<T> item_) {
    return Unique_r<T>(item_);
}


// FuncObj = Nullary function object that has member typedef result_type
template<typename FuncObj>
class Unique_f_r : public custom_relation {
    typedef typename FuncObj::result_type item_type;
    lref<std::set<typename FuncObj::result_type> > items;
    FuncObj func;
public:
    Unique_f_r (const FuncObj& func) : items(std::set<item_type>()), func(func)
    { }

    bool operator() (void) {
        rel_begin();
        item_type value = func();
        if( items->find(value) != items->end() )
          rel_return(false);
        items->insert(value);
        rel_return(true);
        rel_end();
    }
};

// FuncObj = Nullary function object that has member typedef result_type
template<typename FuncObj> inline
Unique_f_r<FuncObj> unique_f(FuncObj f) {
    return Unique_f_r<FuncObj>(f);
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
    typename ::castor::detail::Pointee<Itr>::result_type
    deref_ptr(lref<Itr> ptr) {
        return **ptr;
    }
} // namespace detail

template<typename Itr> inline
relation deref(lref<Itr> pointer_, lref<typename ::castor::detail::Pointee<Itr>::result_type> pointee) {
  typedef typename ::castor::detail::Pointee<Itr>::result_type T;
  return eq_f(pointee, ::castor::detail::bind<T>(::castor::detail::deref_ptr<Itr>, pointer_));
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
class Head_r : public custom_relation {
    typedef typename Seq::value_type T;
    lref<Seq> seq_;
    lref<T> h;
public:
    Head_r(const lref<Seq>& seq_, const lref<T>& h) : seq_(seq_), h(h)
    { }

    bool operator () (void) {
      rel_begin();
      if(seq_->empty())
        rel_return(false);
      if(h.defined())
          rel_return( *h==*(seq_->begin()) );
      h=*(seq_->begin());
      rel_yield(true);
      h.reset();
      rel_end();
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
class Head_n_r : public custom_relation {
    lref<HeadSeq> h;
    lref<typename HeadSeq::size_type> n_;
    lref<Seq> seq_;
public:
    Head_n_r(const lref<Seq>& seq_, const lref<typename HeadSeq::size_type>& n_, const lref<HeadSeq>& h) : seq_(seq_), n_(n_), h(h)
    { }
    bool operator () (void) {
      typename HeadSeq::const_iterator start, end;
      rel_begin();
      if( *n_ > seq_->size() )
        rel_return(false);
      if(seq_->empty())
        rel_return(false);
      if(h.defined()) {
          if( h->size() > seq_->size() )
              rel_return(false);
          rel_return( std::equal(h->begin(),h->end(),seq_->begin()) );
      }
      start = end = seq_->begin();
      std::advance(end, *n_);
      h=HeadSeq(start,end);
      rel_yield(true);
      h.reset();
      rel_end();
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
class Tail_r : public custom_relation {
    lref<Seq> seq_;
    lref<TailSeq> t;
public:
    Tail_r(const lref<Seq>& seq_, const lref<TailSeq>& t) : seq_(seq_), t(t)
    { }

    bool operator() (void) {
      typename Seq::iterator b;
      rel_begin();
      if(seq_->empty())
        rel_return(false);
		  if(t.defined()) {
			  b = seq_->begin();
        rel_return( *t == Seq(++b, seq_->end()) );
		  }
		  b = seq_->begin();
      t = Seq(++b, seq_->end());
      rel_yield(true);
      t.reset();
      rel_end();
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
class Tail_N_r : public custom_relation {
    lref<Seq> seq_;
    lref<typename TailSeq::size_type> n_;
    lref<TailSeq> t;
public:
    Tail_N_r(const lref<Seq>& seq_, const lref<typename TailSeq::size_type>& n_, const lref<TailSeq>& t) : seq_(seq_), n_(n_), t(t)
    { }

    bool operator() (void) {
      typename TailSeq::const_iterator start = seq_->begin(), end = seq_->end();
      rel_begin();
      if(*n_ > seq_->size())
        rel_return(false);
      if(t.defined()) {
          if( t->size() > seq_->size() )
            rel_return(false);
          start = seq_->begin();
          std::advance(start, seq_->size() - *n_);
          rel_return( std::equal(t->begin(),t->end(),start) );
      }
      std::advance(start, seq_->size() - *n_);
      t=TailSeq(start,end);
      rel_yield(true);
      t.reset();
      rel_end();
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
		  || predicate(b_!=e_) && next(b_,n) && recurse(self,value_,n,e_,tmp) && deref(b_,v) && sequence(insertedSeq)(v)(tmp);
#else
	return   sequence(insertedSeq)(value_)(b_,e_)
		  || predicate(b_!=e_) && next(b_,n) && recurse(&insert<Seq>,value_,n,e_,tmp) && deref(b_,v) && sequence(insertedSeq)(v)(tmp);
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
		  || predicate(b_!=e_) && next(b_,n) && recurse(self,valuesB_,valuesE_,n,e_,tmp) && deref(b_,v) && sequence(insertedSeq)(v)(tmp);
#else
	return   sequence(insertedSeq)(valuesB_,valuesE_)(b_,e_)
		  || predicate(b_!=e_) && next(b_,n) && recurse(&insert_seq<Seq>,valuesB_,valuesE_,n,e_,tmp) && deref(b_,v) && sequence(insertedSeq)(v)(tmp);
#endif
}

//----------------------------------------------------------------------
//  increment/decrement relations
//----------------------------------------------------------------------

template<typename T>
class Inc_r : public custom_relation {
    lref<T> obj;
    lref<T> oldValue;
public:
    explicit Inc_r(const lref<T>& obj) : obj(obj)
    { }

    bool operator ()(void) {
      rel_begin();
      oldValue=obj; // copy the original value
      ++(*obj);
      rel_yield(true);
      obj=*oldValue;
      rel_end();
    }
};

template<typename T> inline
Inc_r<T> inc(lref<T>& value_) {
    return Inc_r<T>(value_);
}


template<typename T>
class Dec_r : public custom_relation {
    lref<T> obj;
    lref<T> oldValue;
public:
    explicit Dec_r(const lref<T>& obj) : obj(obj)
    { }

    bool operator()(void) {
      rel_begin();
      oldValue=obj; // copy the original value
      --(*obj);
      rel_yield(true);
      obj=*oldValue;
      rel_end();
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
    //    ::castor::detail::IfElse<is_lref<Iter>::result, push_lref_pair, push_values<Iter> >::type::apply(start, end, *this);
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
                size_t numItems = ::castor::detail::countItems(*b, *e);
                if ( ((unsigned)::castor::detail::countItems(li,l_end)<numItems) || !std::equal(*b, *e, li) )
                    return false;
                std::advance(li, numItems);
                r_LrefPairs.pop_front();
                break;
            }
            default: { //==REF_COLLECTION
                typename Seq::iterator b = (*r_refLists.front()).begin(),
                                        e = (*r_refLists.front()).end();
                if ( (unsigned)::castor::detail::countItems(li,l_end)<(*r_refLists.front()).size()
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
class Size_r : public custom_relation {
    lref<typename Cont::size_type> sz;
    lref<Cont> cont_;
public:
    Size_r(const lref<Cont>& cont_, const lref<typename Cont::size_type>& sz) : sz(sz), cont_(cont_)
    { }

    bool operator() (void) {
      rel_begin();
      if(sz.defined())
        rel_return( *sz==cont_->size() );
      sz=cont_->size();
      rel_yield(true);
      sz.reset();
      rel_end();
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
class Begin_r : public custom_relation {
    typedef typename Cont::iterator Iter;
    lref<Cont> cont_;
    lref<Iter> iter;
public:
    Begin_r(const lref<Cont>& cont_, const lref<Iter>& iter) : cont_(cont_), iter(iter)
    { }

    bool operator()(void) {
      rel_begin();
      if(iter.defined())
        rel_return( *iter==cont_->begin() );
      iter=cont_->begin();
      rel_yield(true);
      iter.reset();
      rel_end();
    }
};

// 1st argument disallows a raw vector to be passed as argument. This ensures
// Itr is not an iterator to a copy of the argument... since passing a raw
// vector to a lref causes the lref to make a copy of the vector.
template<typename Cont> inline
Begin_r<Cont> begin(lref<Cont>& cont_, lref<typename Cont::iterator> iter) {
    return Begin_r<Cont>(cont_, iter);
}

//--------------------------------------------------------
//  end relation : For working with iterators 
//--------------------------------------------------------
template<typename Cont>
class End_r : public custom_relation {
    typedef typename Cont::iterator IterT;
    lref<Cont> cont_;
    lref<IterT> iter;
public:
    End_r(const lref<Cont>& cont_, const lref<IterT>& iter) : cont_(cont_), iter(iter)
    { }
    
    bool operator()(void) {
      rel_begin();
      if(iter.defined())
        rel_return( *iter==cont_->end() );
      iter=cont_->end();
      rel_yield(true);
      iter.reset();
      rel_end();
    }
};

template<typename Cont> inline
End_r<Cont> end(lref<Cont>& cont_, lref<typename Cont::iterator> iter) {
    return End_r<Cont>(cont_, iter);
}


//---------------------------------------------------------------
//    Error Relation : Always throws an exception
//---------------------------------------------------------------
template<typename ExceptionType=char*>
struct Error : TestOnlyRelation<Error<ExceptionType> > {
    ExceptionType e;
    Error(const ExceptionType & e) :e(e)
    { }

    bool apply(void) const {
        throw e;
    }
};


template<typename ExceptionType>
Error<ExceptionType> error( const ExceptionType& err ) {
    return Error<ExceptionType>(err);
}

Error<const char*> error( const char* err ) {
    return Error<const char*>(err);
}

//--------------------------------------------------------
//  negate relation : invert true/false result from another relation
//--------------------------------------------------------
template<typename Rel>
class Negate_r : public TestOnlyRelation<Negate_r<Rel> > {
    Rel r;
public:
    Negate_r(const Rel& r) : r(r)
    { }

    bool apply() {
        return !r();
    }
};

template<typename Rel>
Negate_r<Rel> negate(const Rel& rel) {
    return Negate_r<Rel>(rel);
}

} // namespace castor
#endif
