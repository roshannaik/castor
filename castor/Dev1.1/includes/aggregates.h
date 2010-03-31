// Castor : Logic Programming Library
// Copyright © 2007-2010 Roshan Naik (roshan@mpprogramming.com).
// This software is governed by the MIT license (http://www.opensource.org/licenses/mit-license.php).

#if !defined CASTOR_AGGREGATES_H
#define CASTOR_AGGREGATES_H 1

#include "coroutine.h"
#include "relation.h"
#include "lref.h"
#include "helpers.h"
#include <vector>
#include <utility>
#include <iterator>
#include <memory>
#include <algorithm>
#include <functional>

namespace castor {


class agg_relation {
    struct impl {
        virtual ~impl(){}
        virtual impl* clone() const=0;
        virtual bool operator()(relation&)=0;
    };

    template<class F>
    struct wrapper : public impl {
        explicit wrapper(const F& f_) : f(f_)
        { }
        virtual impl* clone() const {
            return new wrapper<F>(this->f);
        }
        virtual bool operator()(relation& r) {
            return this->f(r);
        }
    private:
        F f;
    };

    std::auto_ptr<impl> pimpl;

    template<class Expr>  // disable instantiating agg_relation from ILE
    agg_relation(const Ile<Expr>& rhs);
public:
    typedef bool result_type;

    // Concept : F supports the method... bool F::operator()(relation&)
    template<class F>
    agg_relation(F f) : pimpl(new wrapper<F>(f)) { 
        typedef bool (F::* boolMethod)(relation&);  // Compiler Error : f must support method bool F::operator()(relation)
    }

    agg_relation(const agg_relation& rhs) : pimpl(rhs.pimpl->clone())
    { }

    agg_relation& operator=(const agg_relation& rhs) {
        this->pimpl.reset(rhs.pimpl->clone());
        return *this;
    }

    bool operator()(relation & r) /*const*/ {
      return (*this->pimpl.get())(r); // we use get() instead of * in pimpl since constructors guarantee pimpl will be initialized
	}
};


class Aggregate_r {
	relation r;
	agg_relation ar;
public:
	Aggregate_r(const relation& r, const agg_relation& ar) : r(r), ar(ar)
	{ }

	bool operator()(void) {
		return ar(r);
	}
};

inline
Aggregate_r operator >> (const relation& r, const agg_relation& ar) {
	return Aggregate_r(r,ar);
}


template<typename T, typename Pred>
struct Order_ar : public Coroutine {
	lref<T> obj, initialValue;
	std::vector<T> v;
	typename std::vector<T>::iterator itr;
	Pred pred;

	Order_ar(const lref<T>& obj, Pred pred) : obj(obj), pred(pred)
	{}

	bool operator() (relation& r) {
		co_begin();
		initialValue = obj;
		if(!r())
			co_return(false);
		do {
			v.push_back(*obj);
		} while(r());

		std::sort(v.begin(), v.end(),pred);
		for(itr=v.begin(); itr!=v.end(); ++itr) {
			obj = *itr;
			co_yield(true);
		}
		obj = initialValue;
		co_end();
	}
};


//order
template<typename T>  inline
Order_ar<T,std::less<T> > 
order(lref<T>& obj) {
	return Order_ar<T,std::less<T> >(obj,std::less<T>());
}

template<typename T, typename Pred>  inline
Order_ar<T,Pred> 
order(lref<T>& obj, Pred p) {
	return Order_ar<T,Pred>(obj,p);
}

namespace detail {
template<typename T, typename BinPred>
struct RevCmp {
	BinPred p;
	explicit RevCmp(BinPred p) : p(p)
	{}

	bool operator()(const T& l, const T& r)  {
		return p(r,l);
	}
	bool operator()(const T& l, const T& r)  const {
		return p(r,l);
	}
};
} // namespace detail



namespace detail {
template<typename T, typename MemFunc, typename BinPred>
struct MfPred {
	BinPred p;
	MemFunc mf;
	MfPred(MemFunc mf, BinPred p) : mf(mf), p(p)
	{}
	bool operator()(T& l, T& r) {
		return p( (l.*mf)(), (r.*mf)() );
	}
	bool operator()(const T& l, const T& r)  const {
		return p( (l.*mf)(), (r.*mf)() );
	}
};

// reverses order arguments to BinPred
template<typename T, typename MemFunc, typename BinPred>
struct MfPred2 {
	BinPred p;
	MemFunc mf;
	MfPred2(MemFunc mf, BinPred p) : mf(mf), p(p)
	{}
	bool operator()(T& l, T& r) {
		return p( (r.*mf)(), (l.*mf)() );
	}
	bool operator()(const T& l, const T& r)  const {
		return p( (r.*mf)(), (l.*mf)() );
	}
};

} // namespace detail

template<typename T, typename Pred>
struct OrderByBase : public Coroutine {
	lref<T> obj, initialValue;
	std::vector<T> v;
	typename std::vector<T>::iterator itr;
	Pred pred;

	OrderByBase(const lref<T>& obj, Pred p) : obj(obj), pred(p)
	{}

	bool operator() (relation& r) {
		co_begin();
		initialValue = obj;
		if(!r())
			co_return(false);
		do {
			v.push_back(*obj);
		} while(r());
		
		std::sort(v.begin(), v.end(), pred);
		for(itr=v.begin(); itr!=v.end(); ++itr) {
			obj = *itr;
			co_yield(true);
		}
		obj = initialValue;
		co_end();
	}
};

template<typename T, typename MemFunc, typename Pred>
struct OrderMf_ar : public OrderByBase<T, detail::MfPred<T,MemFunc,Pred> > {
	typedef OrderByBase<T, detail::MfPred<T,MemFunc,Pred> > BaseClass;
	OrderMf_ar(const lref<T>& obj, MemFunc mf, Pred p) : BaseClass(obj, detail::MfPred<T,MemFunc,Pred>(mf,p))
	{}
};

//order_mf
template<typename T, typename MemFunc>  inline
OrderMf_ar<T,MemFunc,std::less<typename detail::return_type<MemFunc>::result_type> > 
order_mf(lref<T>& obj, MemFunc f) {
	typedef typename detail::return_type<MemFunc>::result_type R;
	return OrderMf_ar<T,MemFunc,std::less<R> >(obj,f,std::less<R>());
}

template<typename T, typename MemFunc, typename Pred>  inline
OrderMf_ar<T,MemFunc,Pred> 
order_mf(lref<T>& obj, MemFunc f, Pred p) {
	return OrderMf_ar<T,MemFunc,Pred>(obj,f,p);
}



//-------------------------------------------------
// order_mem(obj)
//-------------------------------------------------

namespace detail {
template<typename T, typename Mem, typename BinaryPred>
struct CompareMember {
	Mem T::* mem;
	BinaryPred p;
	CompareMember(Mem T::* mem, BinaryPred pred) : mem(mem), p(pred)
	{}
	bool operator()(T& l, T& r) {
		return p( l.*mem, r.*mem );
	}
	bool operator()(const T& l, const T& r)  const {
		return p( l.*mem, r.*mem );
	}
};
} // namespace detail

template<typename T, typename Mem, typename Pred>
struct OrderMem_ar : public OrderByBase<T,detail::CompareMember<T,Mem,Pred> > {
	typedef OrderByBase<T,detail::CompareMember<T,Mem,Pred> > BaseClass;
	OrderMem_ar(const lref<T>& obj, Mem T::* mem, Pred p) : BaseClass(obj, detail::CompareMember<T,Mem,Pred>(mem,p))
	{}
};

//order_mem
template<typename T, typename MemberT>  inline
OrderMem_ar<T,MemberT,std::less<MemberT> > 
order_mem(lref<T>& obj, MemberT T::* mem) {
	return OrderMem_ar<T,MemberT,std::less<MemberT> >(obj,mem,std::less<MemberT>());
}

template<typename T, typename MemberT, typename Pred>  inline
OrderMem_ar<T,MemberT,Pred> 
order_mem(lref<T>& obj, MemberT T::* mem, Pred p) {
	return OrderMem_ar<T,MemberT,Pred>(obj,mem,p);
}

//-------------------------------------------------
// reverse(obj)
//-------------------------------------------------
template<typename T>
struct Reverse : public Coroutine {
	lref<T> obj, initialValue;
	std::vector<T> v;
	typename std::vector<T>::reverse_iterator itr;

	explicit Reverse(const lref<T>& obj) : obj(obj)
	{}

	bool operator() (relation& r) {
		co_begin();
		initialValue = obj;
		if(!r())
			co_return(false);
		do {
			v.push_back(*obj);
		} while(r());

		for(itr=v.rbegin(); itr!=v.rend(); ++itr) {
			obj = *itr;
			co_yield(true);
		}
		obj = initialValue;
		co_end();
	}
};


template<typename T>  inline
Reverse<T> reverse(lref<T>& obj) {
	return Reverse<T>(obj);
}

//-------------------------------------------------
// sum(i)
//-------------------------------------------------

template<typename T>
struct Sum_ar : public Coroutine {
	lref<T> i, total;

	explicit Sum_ar(const lref<T>& i) : i(i)
	{}

	bool operator() (relation& r) {
		co_begin();
		if(!r())
			co_return(false);
		for(total=i; r(); total.get()+=*i);
		i=total;
		co_yield(true);
		i.reset();
		co_end();
	}
};

template<typename T> inline
Sum_ar<T> sum(lref<T>& i) {
	return Sum_ar<T>(i);
}

//-------------------------------------------------
// reduce(i,binFunc)
//-------------------------------------------------

//Concept: BinFunc:  Sum BinFunc(Sum&, T&)
template<typename T, typename BinFunc>
struct Reduce_ar : public Coroutine {
	lref<T> i, total;
	BinFunc acc;

	Reduce_ar(const lref<T>& i, const BinFunc& acc) : i(i), acc(acc)
	{}

	bool operator() (relation& r) {
		co_begin();
		if(!r())
			co_return(false);
		for(total=i; r(); total.get()=acc(total.get(),*i) );
		i=total;
		co_yield(true);
		i.reset();
		co_end();
	}
};

template<typename T, typename BinFunc> inline
Reduce_ar<T,BinFunc> reduce(lref<T>& i, BinFunc acc) {
	return Reduce_ar<T,BinFunc>(i,acc);
}

//-------------------------------------------------
// count(n) - n is number of times argument relation succeeded
//-------------------------------------------------

// obj is bidirectional
template<typename T>
struct Count_ar : public Coroutine {
	lref<T> obj, tmp;

	Count_ar(const lref<T>& obj) : obj(obj)
	{}

	bool operator() (relation& r) {
		co_begin();
		tmp=0;
		while(r())
			++tmp.get();
		if(obj.defined()) {
			co_return(*tmp == *obj);
		}
		else {
			obj = tmp;
			co_yield(true);
			obj.reset();
		}
		co_end();
	}
};


// Cocnept : T is a numeric type
template<typename T> inline
Count_ar<T> count(const lref<T>& obj) {
	return Count_ar<T>(obj);
}


//-------------------------------------------------
// group_by(i, selector).then(..).then(..) - group seq using cond into (key,grp)
//-------------------------------------------------

namespace detail {

template<int N>
struct nth {
	template<class R, class T1, class T2>
	static R& key(std::pair<T1,T2>& p) {
		return nth<N-1>::key<R>(p.second);
	}
};

template<>
struct nth<0> {
	template<class T>
	static T& key(T & p) {
		return p;
	}
	template<class T1, class T2>
	static T1& key(std::pair<T1,T2>& p) {
		return p.first;
	}
};

// Functor returns the v[index].first, where v is a vector<pair> v
template<class T>
class nthValue {
	lref<std::vector<T> > v;
public:
	typedef typename T::first_type& result_type;

	nthValue(lref<std::vector<T> >& v) : v(v)
	{ }

	result_type operator()(typename std::vector<T>::size_type index) {
		return ((*v)[index]).first;
	}
};

template<class T>
class group_iterator {
	Func1<T&,size_t> getV;
	size_t curr;
public:
	typedef std::random_access_iterator_tag iterator_category;
	typedef T value_type;
	typedef T* pointer;
	typedef T& reference;
	typedef size_t difference_type;
	typedef size_t distance_type;

	group_iterator(const Func1<T&,size_t>& getV, size_t curr) : getV(getV), curr(curr)
	{ }
	
	group_iterator(const group_iterator& rhs) : getV(rhs.getV), curr(rhs.curr)
	{ }

	group_iterator& operator =(const group_iterator& rhs) {
		curr=rhs.curr;
		getV=rhs.getV;
		return *this;
	}

	T& operator *() {
		return getV(curr);
	}

	T* operator ->() {
		return (&**this);
	}

	group_iterator& operator ++() {
		++curr;
		return *this;
	}

	group_iterator operator ++(int) {
		group_iterator tmp(*this);
		++curr;
		return tmp;
	}

	group_iterator& operator --() {
		--curr;
		return *this;
	}

	group_iterator operator --(int) {
		group_iterator tmp(*this);
		--curr;
		return tmp;
	}

	group_iterator& operator +=(difference_type amt) {
		curr+=amt;
		return *this;
	}

	group_iterator& operator -=(difference_type amt) {
		curr-=amt;
		return *this;
	}

	group_iterator operator +(difference_type amt) const {
		group_iterator tmp(*this);
		return tmp+=amt;
	}

	group_iterator operator -(difference_type amt) const {
		group_iterator tmp(*this);
		return tmp-=amt;
	}

	const value_type& operator[](difference_type offset) const {
		return getV(curr+offset);
	}

	//todo: consider deep comparison (i.e. ensure referring to same vector)
	bool operator ==(const group_iterator& rhs) const {
		return curr==rhs.curr;
	}

	bool operator !=(const group_iterator& rhs) const {
		return !(*this==rhs);
	}

	bool operator >(const group_iterator& rhs) const {
		return curr>rhs.curr;
	}

	bool operator >=(const group_iterator& rhs) const {
		return curr>=rhs.curr;
	}

	bool operator <(const group_iterator& rhs) const {
		return curr<rhs.curr;
	}

	bool operator <=(const group_iterator& rhs) const {
		return curr<=rhs.curr;
	}
};

} // namespace detail

//------------------------------------------

template<class K, class V>
struct group {
	typedef K key_type;
	typedef V value_type;
	K key;
	typedef detail::group_iterator<V> iterator;
private:
	size_t first;
	size_t last;
	detail::Func1<V&,size_t> getV;

	template<class, class, class , class , class, class> friend class GroupBy;
	template<class, class, class , class , class> friend class GroupBy2;

	template<class, class> friend struct group;

	group(const K& key, size_t first, size_t last, detail::Func1<V&,size_t> getV) : key(key), first(first), last(last), getV(getV)
	{ }
public:
	
	size_t size() const {
		return last-first;
	}

	bool empty() const {
		return size()==0;
	}

	bool operator==(const group& rhs) const {
		return key==rhs.key && first==first && last==last;
	}

	iterator begin() const {
		return iterator(getV,first);
	}

	iterator end() const {
		return iterator(getV,last);
	}

	template<int N, class ElementT>
	static group get_group(lref<std::vector<ElementT> >& v, size_t first, size_t last) {
		using namespace detail;
		if( first>=last || v->size()<last)
			throw IndexOutOfBounds();

		size_t end1=0;
		std::vector<ElementT>& rvec = v.get();
		for(end1=first+1; end1<last; ++end1) {
			if( nth<N>::key<K>(rvec[end1]) != nth<N>::key<K>(rvec[first]) ) {
				break;
			}
		}
		return group( nth<N>::key<K>((*v)[first]), first, end1, nthValue<ElementT>(v) );
	}
};

template<class K, class V, class T>
struct group<group<K,V>, T >; // first template argument cannot be group<>


template<class K, class K2, class V2>
struct group<K, group<K2,V2> > {
	typedef K key_type;
	typedef group<K2,V2> value_type;
	typedef typename std::vector<value_type>::iterator iterator;

	K key;
private:
	lref<std::vector<value_type> > subgroups;
	size_t first;
	size_t last;

	template<class, class, class, class, class, class> friend class GroupBy;
	template<class, class, class , class , class> friend class GroupBy2;
	template<class, class> friend struct group;
public:
	group(const K& key, lref<std::vector<value_type> >& subgroups, size_t first, size_t last/*, detail::Func1<value_type&,size_t> getV*/) : key(key), subgroups(subgroups), first(first), last(last)
	{ }

	size_t size() const {
		return last-first;
	}

	bool empty() const {
		return first==last;
	}

	bool operator==(const group& rhs) const {
		return key==rhs.key && first==first && last==last && *subgroups==*rhs.subgroups;
	}

	iterator begin() const {
		return subgroups->begin();
	}

	iterator end() const {
		return subgroups->end();
	}

	template<int N, class T>
	static group get_group(lref<std::vector<T> >& v, size_t first, size_t last) {
		using namespace detail;
		if( first>=last || v->size()<last)
			throw IndexOutOfBounds();
		std::vector<T>& rvec = v.get();
		size_t end1=0, first2=first;
		for(end1=first+1; end1!=last; ++end1) {
			if( nth<N>::key<K>(rvec[end1]) != nth<N>::key<K>(rvec[first]) )
				break;
		}
		lref<std::vector<value_type> > subgroups(new std::vector<value_type>(), true);
		for(size_t first2=first; first2<end1; first2=subgroups->rbegin()->last )
			subgroups->push_back( value_type::get_group<N+1,T>(v, first2, end1) );

		return group( nth<N>::key<K>(rvec[first]), subgroups, first, end1);
	}
};

//------------------------------------------
namespace detail {

//FuncList: FuncList<selector1, FuncList<selector2, FuncList<selector3, FuncList<selector4, None> > > >
template<class Func, class Inner>
struct FuncList;

template<class Func>
struct FuncList<Func,None> {
	typedef typename return_type<Func>::result_type result_type;
	Func selector;

	FuncList(Func f) : selector(f)
	{ }

	template<class Obj>
	result_type
	operator()(Obj& obj) {
		return selector(obj);
	}
};

template<class Func, class Func2>
struct FuncList<Func, FuncList<Func2,None> > {
	typedef FuncList<Func2,None> Inner;
	typedef std::pair<typename return_type<Func>::result_type, typename Inner::result_type> result_type;
	Func selector;
	Inner inner;

	FuncList(FuncList<Func,None> flist, Func2 lastF) : selector(flist.selector), inner(lastF)
	{ }

	template<class Obj>
	result_type operator()(Obj& obj) {
		return std::make_pair(selector(obj),inner(obj));
	}
};

template<class Func, class Func2, class Inner2>
struct FuncList <Func, FuncList<Func2,Inner2> > {
	typedef FuncList<Func2,Inner2> Inner;
	typedef std::pair<typename return_type<Func>::result_type, typename Inner::result_type> result_type;
	Func selector;
	Inner inner;

	template<class NewFunc, class OldInner>
	FuncList(FuncList<Func,OldInner> flist, NewFunc lastF) : selector(flist.selector), inner(flist.inner, lastF)
	{ }

	template<class Obj>
	result_type operator()(Obj& obj) {
		return std::make_pair(selector(obj),inner(obj));
	}
};

//--------------------------------------

template<class FList, class NewFunc>
struct AddFunc;

template<class Func, class Func2>
struct AddFunc<FuncList<Func,None>, Func2> {
	typedef FuncList<Func, FuncList<Func2,None> > result_type;
};

template<class Func, class Inner, class Func2>
struct AddFunc<FuncList<Func,Inner>, Func2> {
	typedef FuncList<Func, typename  AddFunc<Inner,Func2>::result_type> result_type;
};


//------------------------------------------

//FuncList: NestedCmp<cmp1, NestedCmp<cmp2, NestedCmp<cmp3,cmp4> > >
template<class Cmp, class Inner>
struct NestedCmp;

template<class Cmp1, class Cmp2, class Inner>
struct NestedCmp<NestedCmp<Cmp1,Cmp2>, Inner>;

template<class Cmp1, class Cmp2>
struct NestedCmp {
	typedef bool result_type;
	Cmp1 cmp;
	Cmp2 inner;

	NestedCmp(Cmp1 cmp, Cmp2 inner) : cmp(cmp), inner(inner)
	{ }

	template<class T>
	bool operator()(const T& lhs, const T& rhs) {
		if(cmp(lhs.first,rhs.first))
			return true;
		if(cmp(rhs.first,lhs.first))
			return false;

		return inner(lhs.second,rhs.second);
	}
};


template<class Cmp, class Cmp2, class Cmp3>
struct NestedCmp<Cmp, NestedCmp<Cmp2,Cmp3> > {
	typedef bool result_type;
	typedef NestedCmp<Cmp2,Cmp3> Inner;
	Cmp cmp;
	Inner inner;

	template<class NewCmp, class OldInner>
	NestedCmp(NestedCmp<Cmp,OldInner> flist, NewCmp lastCmp) : cmp(flist.cmp), inner(flist.inner, lastCmp)
	{ }

	template<typename T>
	bool operator()(const T& lhs, const T& rhs) {
		if(cmp(lhs.first,rhs.first))
			return true;
		if(cmp(rhs.first,lhs.first))
			return false;
		return inner(lhs.second,rhs.second);
	}
};


//--------------------------------------
template<class CmpList, class NewCmp>
struct AddCmp;

template<class T, class Cmp1, class Cmp2>
struct AddCmp<T, NestedCmp<Cmp1,Cmp2> >;

template<class Cmp1, class Cmp2>
struct AddCmp {
	typedef NestedCmp<Cmp1, Cmp2> result_type;
};

template<class Cmp1, class Cmp2, class Cmp3>
struct AddCmp<NestedCmp<Cmp1,Cmp2>, Cmp3> {
	typedef NestedCmp<Cmp1, typename AddCmp<Cmp2,Cmp3>::result_type> result_type;
};


//------------------------------------------
template<class KeyCmp, class ValCmp>
struct GroupElemCmp {
	KeyCmp keyCmp;
	ValCmp valCmp;
	GroupElemCmp(KeyCmp keyCmp, ValCmp valCmp) : keyCmp(keyCmp), valCmp(valCmp)
	{ }

	template<class T>
	bool operator ()(T& lhs, const T& rhs) {
		if(keyCmp(lhs.second,rhs.second))
			return true;
		if(keyCmp(rhs.second,lhs.second))
			return false;
		return valCmp(lhs.first,rhs.first);
	}
};

template<class KeyCmp>
struct GroupElemCmp<KeyCmp,None> {
	KeyCmp keyCmp;
	GroupElemCmp(KeyCmp keyCmp, None) : keyCmp(keyCmp)
	{ }

	template<class T>
	bool operator ()(T& lhs, const T& rhs) {
		return keyCmp(lhs.second,rhs.second);
	}
};

//------------------------------------------
//ElementType:  pair<Value, pair<Key1, pair<Key2, pair<Key3,Key4> > > >

template<class Group>
struct nested_keys;

template<class K, class V>
struct nested_keys<group<K,V> > {
	typedef K result_type;
};

template<class K1, class K2, class V2>
struct nested_keys<group<K1, group<K2,V2> > > {
	typedef std::pair<K1, typename nested_keys<group<K2,V2> >::result_type > result_type;
};


} //namespace detail

//------------------------------------------

template<class Item, class Grp, class K, class V, class Sel, class KCmp>
class GroupBy {
	lref<Item> i;
	Sel selectors;
	lref<Grp> g;
	KCmp keyCmps;
	
	typedef std::pair<Item, typename detail::nested_keys<Grp>::result_type> ElementType;
	lref<std::vector<ElementType> > elements;
public:	
	GroupBy(lref<Item> i, Sel selectors, lref<Grp> g, KCmp keyCmps) : i(i), selectors(selectors), g(g), keyCmps(keyCmps)
	{ }

	template<class Sel2>
	GroupBy<Item,Grp,K,typename V::value_type,typename detail::AddFunc<Sel,Sel2>::result_type, typename detail::AddCmp<KCmp,std::less<typename V::key_type> >::result_type>
    then(Sel2 keyselector) {
		typedef typename detail::AddFunc<Sel,Sel2>::result_type NewSelector;
		typedef typename detail::AddCmp<KCmp,std::less<typename V::key_type> >::result_type NewComparator;
		return GroupBy<Item,Grp,K,typename V::value_type,NewSelector,NewComparator>(i, NewSelector(selectors,keyselector), g, NewComparator(keyCmps, std::less<typename V::key_type>()));
	}

	template<class Sel2, class KCmp2>
	GroupBy<Item,Grp,K,typename V::value_type,typename detail::AddFunc<Sel,Sel2>::result_type, typename detail::AddCmp<KCmp,KCmp2>::result_type>
    then(Sel2 keySelector, KCmp2 keyCmp) {
		typedef typename detail::AddFunc<Sel,Sel2>::result_type NewSelector;
		typedef typename detail::AddCmp<KCmp,KCmp2>::result_type NewComparator;
		return GroupBy<Item,Grp,K,typename V::value_type,NewSelector,NewComparator>(i, NewSelector(selectors,keySelector), g, NewComparator(keyCmps, keyCmp));
	}
};

template<class Item, class Grp, class Sel, class KCmp, class VCmp>
class GroupBy2;

template<class Item, class Grp, class K, class Sel, class KCmp>
class GroupBy<Item, Grp, K, Item, Sel, KCmp> : public Coroutine {
	lref<Item> i;
	Sel selectors;
	lref<Grp> g;
	KCmp keyCmps;
	typedef std::pair<Item, typename detail::nested_keys<Grp>::result_type> ElementType;

	lref<std::vector<ElementType> > elements;
	typename std::vector<ElementType>::size_type first;
public:
	GroupBy(lref<Item> i, Sel selectors, lref<Grp> g, KCmp keyCmps) : i(i), selectors(selectors), g(g), keyCmps(keyCmps), first(0)
	{ }

	template<class VCmp>
	GroupBy2<Item,Grp,Sel,KCmp,VCmp>
    values_by(VCmp valueCmp) {
		return GroupBy2<Item,Grp,Sel,KCmp,VCmp>(i, selectors, g, keyCmps, valueCmp);
	}

	bool operator()(relation& rhs) {
		co_begin();
		elements.set_ptr(new std::vector<ElementType>(), true);
		if(g.defined())
			co_return(false);
		while(rhs())
			elements->push_back(std::make_pair(*i,selectors(*i)));
		if(elements->empty())
			co_return(false);
		
		std::sort(elements->begin(), elements->end(), detail::GroupElemCmp<KCmp,detail::None>(keyCmps,detail::None()));
 
		for(; first<elements->size(); first = g->last) {
			g = Grp::get_group<1>(elements, first, elements->size());
			co_yield(true);
		}
		g.reset();
		co_end();
	}
};

template<class Item, class Grp, class Sel, class KCmp, class VCmp>
class GroupBy2 : public Coroutine {
	lref<Item> i;
	Sel selectors;
	lref<Grp> g;
	KCmp keyCmps;
	VCmp valueCmp;
	typedef std::pair<Item, typename detail::nested_keys<Grp>::result_type> ElementType;

	lref<std::vector<ElementType> > elements;
	typename std::vector<ElementType>::size_type first;
public:
	GroupBy2(lref<Item> i, Sel selectors, lref<Grp> g, KCmp keyCmps, VCmp valueCmp) : i(i), selectors(selectors), g(g), keyCmps(keyCmps), valueCmp(valueCmp), first(0)
	{ }

	bool operator()(relation& rhs) {
		co_begin();
		elements.set_ptr(new std::vector<ElementType>(), true);
		if(g.defined())
			co_return(false);
		while(rhs())
			elements->push_back(std::make_pair(*i,selectors(*i)));
		if(elements->empty())
			co_return(false);
		
		std::sort(elements->begin(), elements->end(), detail::GroupElemCmp<KCmp,VCmp>(keyCmps,valueCmp));
 
		for(; first<elements->size(); first = g->last) {
			g = Grp::get_group<1>(elements, first, elements->size());
			co_yield(true);
		}
		g.reset();
		co_end();
	}
};


template<class Item, class Sel, class K, class V>
GroupBy<Item,group<K,V>,K,V,detail::FuncList<Sel,detail::None>, std::less<K> > //inline
group_by(lref<Item>& i, Sel keyselector, lref<group<K,V> >& g) {
	using namespace detail;
	typedef typename return_type<Sel>::result_type ret_type;
	ASSERT_SAME_TYPE(ret_type,K,"Group's key type does not match Selector's return type");
	return GroupBy<Item,group<K,V>,K,V,FuncList<Sel,None>,std::less<K> >(i,FuncList<Sel,None>(keyselector),g, std::less<K>());
}

template<class Item, class Sel, class K, class V, class KCmp>
GroupBy<Item,group<K,V>, K,V,detail::FuncList<Sel,detail::None>,KCmp> //inline
group_by(lref<Item>& i, Sel keyselector, lref<group<K,V> >& g, KCmp keyCmp) {
	using namespace detail;
	typedef typename return_type<Sel>::result_type ret_type;
	ASSERT_SAME_TYPE(ret_type,K,"Group's key type does not match Selector's return type");
	return GroupBy<Item,group<K,V>,K,V,FuncList<Sel,None>,KCmp>(i,FuncList<Sel,None>(keyselector),g,keyCmp);
}


} // namespace castor

#endif
