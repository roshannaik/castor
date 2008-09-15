// Castor : Logic Programming Library
// Copyright © 2008 Roshan Naik (roshan@mpprogramming.com).
// This software is goverened by the MIT license (http://www.opensource.org/licenses/mit-license.php).

#if !defined CASTOR_EQ_H
#define CASTOR_EQ_H 1

#include "lref.h"
#include "ile.h"
#include "relation.h"
#include "helpers.h"
#include <cstring>
namespace castor {

template<typename Derived, typename L, typename R, typename Cmp=::castor::detail::None>
class UnifyBase : public OneSolutionRelation<Derived> {
    Cmp cmp;
public:
    UnifyBase(const Cmp& cmp) : cmp(cmp)
    { }

    bool compare(const L& l, const R& r) {
        return cmp(l,r);
    }
};

template<typename Derived, typename L, typename R>
struct UnifyBase<Derived, L, R, ::castor::detail::None> : public OneSolutionRelation<Derived> {
    UnifyBase(const ::castor::detail::None&)
    { }

    bool compare(const L& l, const R& r) {
        return l==r; 
    }
};

template<typename L, typename R, typename Cmp=::castor::detail::None>
struct UnifyL : public UnifyBase<UnifyL<L,R,Cmp>, L, R, Cmp> {
    lref<L> l;
    R r;
    typedef UnifyBase<UnifyL<L,R,Cmp>, L, R, Cmp> Base;
    bool changed;

    UnifyL(const lref<L>& l, const R& r, const Cmp& cmp) : Base(cmp), l(l), r(r), changed(false)
    { }

    bool apply(void) {
        if(l.defined())
            return Base::compare(*l,r);
        l=r;
        changed=true;
        return true;
    }
    void revert() {
        if(changed) {
            l.reset();
            changed=false;
        }
    }
};

template<typename L, typename R, typename Cmp=::castor::detail::None>
class UnifyLR: public UnifyBase<UnifyLR<L,R,Cmp>, L, R, Cmp> {
    lref<L> l;
    lref<R> r;
	enum {nochange, lchanged, rchanged} state;
    typedef UnifyBase<UnifyLR<L,R,Cmp>, L, R, Cmp> Base;
public:
    UnifyLR(const lref<L>& l, const lref<R>& r, const Cmp& cmp) : Base(cmp), l(l), r(r), state(nochange)
    { }

    bool apply (void) { // throws if  both l and r are not defined
		if(l.defined()) {
			if(r.defined())
                return Base::compare(*l,*r);
			r=*l;
			state=rchanged;
			return true;
		}
        l=*r; // throws if r is not defined
        state=lchanged;
        return true;
    }
    
    void revert(void) {
        if(state==lchanged)
            l.reset();
        if(state==rchanged)
            r.reset();
		state=nochange;
    }
};

template<typename L, typename R> inline
UnifyLR<L,R> eq(lref<L>& l, lref<R>& r) {
    return UnifyLR<L,R>(l,r, ::castor::detail::None());
}

template<typename L, typename R, typename Cmp> inline
UnifyLR<L,R,Cmp> eq(lref<L>& l, lref<R>& r, Cmp cmp) {
    return UnifyLR<L,R,Cmp>(l,r, cmp);
}

template<typename L, typename R> inline
Boolean eq(const L& l, const R& r) {
    return Boolean(l==r);
}
// eq : treat char* as a string
inline
Boolean eq(const char* l, const char* r) {
    return Boolean( std::strcmp(l,r)==0 );
}

template<typename Cmp> inline
Boolean eq(const char* l, const char* r, Cmp cmp) {
    return Boolean( cmp(l,r) );
}

template<typename L, typename R, typename Cmp> inline
Boolean eq(const L& l, const R& r, Cmp cmp) {
    return Boolean( cmp(l,r) );
}

template<typename L, typename R> inline
UnifyL<L,R> eq(const lref<L>& l, const R& r) {
    return UnifyL<L,R>(l,r, ::castor::detail::None());
}

template<typename L, typename R, typename Cmp> inline
UnifyL<L,R,Cmp> eq(const lref<L>& l, const R& r, Cmp cmp) {
    return UnifyL<L,R,Cmp>(l,r, cmp);
}

template<typename L, typename R> inline
UnifyL<R,L> eq(const L& l, const lref<R>& r) {
    return UnifyL<R,L>(r,l, ::castor::detail::None());
}

template<typename L, typename R, typename Cmp> inline
UnifyL<R,L,Cmp> eq(const L& l, const lref<R>& r, Cmp cmp) {
    return UnifyL<R,L,Cmp>(r,l,cmp);
}

template<typename T> inline
UnifyL<T,T> eq(const lref<T>& l, const char* r) {
	return UnifyL<T,T>(l, r, ::castor::detail::None());
}
template<typename T, typename Cmp> inline
UnifyL<T,T,Cmp> eq(const lref<T>& l, const char* r, Cmp cmp) {
	return UnifyL<T,T,Cmp>(l,r,cmp);
}

template<typename T> inline
UnifyL<T,T> eq(const char* l, const lref<T>& r) {
	return UnifyL<T,T>(r,l, ::castor::detail::None());
}

template<typename T, typename Cmp> inline
UnifyL<T,T,Cmp> eq(const char* l, const lref<T>& r, Cmp cmp) {
	return UnifyL<T,T,Cmp>(r,l,cmp);
}



namespace detail {

    template<typename T1, typename T2> inline
    bool compare(const T1& t1, const T2& t2, const ::castor::detail::None&) {
        return t1==t2;
    }
#ifdef __BCPLUSPLUS__
    template<typename T1, typename T2, typename Cmp> inline
    bool compare(const T1& t1, const T2& t2, const Cmp& cmp) {
        return cmp(t1,t2);
    }
#else
    template<typename T1, typename T2, typename Cmp> inline
    bool compare(const T1& t1, const T2& t2, Cmp cmp) {
        return cmp(t1,t2);
    }
#endif

    template<typename Iter1, typename Iter2, typename Cmp>
    bool compare_seq(Iter1 first1, Iter1 last1, Iter2 first2, Iter2 last2, Cmp cmp=::castor::detail::None()) {
        do{
            if(first1==last1) 
                return first2==last2;
            else {
                if(first2==last2)
                    return false;
                if(!::castor::detail::compare(*first1,*first2, cmp))
                    return false;
            }
            ++first1; ++first2;
        }while(true);
    }
}

//----------------------------------------------------------------
// eq_seq : Unify Collection with sequence
//----------------------------------------------------------------
template<typename Cmp, typename Collection, typename Iter, typename Derived>
struct Eq_Seq_r : public OneSolutionRelation<Derived> {
private:
    Cmp cmp;
public:
    Eq_Seq_r(const Cmp& cmp) : cmp(cmp)
    { }

    bool compare(const lref<Collection>& coll, const Iter& beg, const Iter& end) {
        return ::castor::detail::compare_seq(coll->begin(), coll->end(), beg, end, cmp);
    }
};

template<typename Collection, typename Iter, typename Derived>
#ifdef __BCPLUSPLUS__
struct Eq_Seq_r<detail::None, Collection, Iter, Derived> : public OneSolutionRelation<Derived> {
#else
struct Eq_Seq_r< ::castor::detail::None, Collection, Iter, Derived> : public OneSolutionRelation<Derived> {
#endif
    Eq_Seq_r(const ::castor::detail::None&)
    { }

    bool compare(const lref<Collection>& coll, const Iter& beg, const Iter& end) {
        return ::castor::detail::compare_seq(coll->begin(), coll->end(), beg, end, ::castor::detail::None());
    }
};

template<typename Collection, typename Iter, typename Cmp=::castor::detail::None>
class EqSeq_r : public Eq_Seq_r<Cmp, Collection, Iter, EqSeq_r<Collection, Iter, Cmp> > {
    lref<Collection> c;
    Iter begin_, end_;
    bool changed;
    typedef Eq_Seq_r<Cmp, Collection, Iter, EqSeq_r<Collection, Iter, Cmp> > Base;
public:
    typedef Collection collection_type;
    typedef Iter iterator_type;

    EqSeq_r(const lref<Collection>& c, const Iter& begin_, const Iter& end_, const Cmp& cmp) : Base(cmp) ,c(c), begin_(begin_), end_(end_), changed(false)
    { }

    bool apply (void) {
        if(c.defined())
            return Base::compare(c, begin_, end_);
        c=Collection(begin_, end_);
        changed=true;
        return true;
    }

    void revert(void) {
        if(changed) c.reset();
    }
};

template<typename Cont, typename Iter> inline 
EqSeq_r<Cont, Iter> eq_seq(const lref<Cont>& c, Iter begin_, Iter end_) {
    return EqSeq_r<Cont, Iter>(c, begin_, end_, ::castor::detail::None());
}

template<typename Cont, typename Iter, typename Cmp> inline 
EqSeq_r<Cont, Iter, Cmp> eq_seq(const lref<Cont>& c, Iter begin, Iter end, Cmp cmp) {
    return EqSeq_r<Cont, Iter, Cmp>(c, begin, end, cmp);
}


//----------------------------------------------------------------
// eq_f : Unify with a value obtained by calling a function
//----------------------------------------------------------------
template<typename L, typename Func>
class Eq_f_r : public OneSolutionRelation<Eq_f_r<L, Func> > {
    lref<L> l;
    Func func;
    bool changed;
public:
    Eq_f_r (const lref<L>& l, const Func& func) : l(l), func(func), changed(false)
    { }

    bool apply (void) {
        if(l.defined())
            return *l==func();
        changed=true;
        l=func();
        return true;
    }

    void revert(void) {
        if(changed)
            l.reset();
    }
};

template<typename L, typename Func1, typename A1>
class Eq_f_r1 : public OneSolutionRelation<Eq_f_r1<L, Func1, A1> > {
    lref<L> l;
    A1 arg1;
    Func1 func;
    bool changed;
public:
    Eq_f_r1 (const lref<L>& l, const Func1& func, const A1& arg1) : l(l), func(func), arg1(arg1), changed(false)
    { }

    bool apply (void) {
        if(l.defined())
            return *l==func(effective_value(arg1));
        changed=true;
        l=func(effective_value(arg1));
        return true;
    }

    void revert(void) {
        if(changed)
            l.reset();
    }
};

template<typename L, typename Func2, typename A1, typename A2>
class Eq_f_r2 : public OneSolutionRelation<Eq_f_r2<L, Func2, A1, A2> > {
    lref<L> l;
    A1 arg1; A2 arg2; 
    Func2 func;
    bool changed;
public:
    Eq_f_r2 (const lref<L>& l, const Func2& func, const A1& arg1, const A2& arg2) : l(l), func(func), arg1(arg1), arg2(arg2), changed(false)
    { }

    bool apply (void) {
        if(l.defined())
            return *l==func(effective_value(arg1), effective_value(arg2));
        changed=true;
        l=func(effective_value(arg1), effective_value(arg2));
        return true;
    }

    void revert(void) {
        if(changed)
            l.reset();
    }
};


template<typename L, typename Func3, typename A1, typename A2, typename A3>
class Eq_f_r3 : public OneSolutionRelation<Eq_f_r3<L, Func3, A1, A2, A3> > {
    lref<L> l;
    A1 arg1; A2 arg2; A3 arg3;
    Func3 func;
    bool changed;
public:
    Eq_f_r3 (const lref<L>& l, const Func3& func, const A1& arg1, const A2& arg2, const A3& arg3) : l(l), func(func), arg1(arg1), arg2(arg2), arg3(arg3), changed(false)
    { }

    bool apply (void) {
        if(l.defined())
            return *l==func(effective_value(arg1), effective_value(arg2), effective_value(arg3));
        changed=true;
        l=func(effective_value(arg1), effective_value(arg2), effective_value(arg3));
        return true;
    }

    void revert(void) {
        if(changed)
            l.reset();
    }
};

template<typename L, typename Func4, typename A1, typename A2, typename A3, typename A4>
class Eq_f_r4 : public OneSolutionRelation<Eq_f_r4<L, Func4, A1, A2, A3, A4> > {
    lref<L> l;
    A1 arg1; A2 arg2; A3 arg3; A4 arg4;
    Func4 func;
    bool changed;
public:
    Eq_f_r4 (const lref<L>& l, const Func4& func, const A1& arg1, const A2& arg2, const A3& arg3, const A4& arg4) : l(l), func(func), arg1(arg1), arg2(arg2), arg3(arg3), arg4(arg4), changed(false)
    { }

    bool apply (void) {
        if(l.defined())
            return *l==func(effective_value(arg1), effective_value(arg2), effective_value(arg3), effective_value(arg4));
        changed=true;
        l=func(effective_value(arg1), effective_value(arg2), effective_value(arg3), effective_value(arg4));
        return true;
    }

    void revert(void) {
        if(changed)
            l.reset();
    }
};


template<typename L, typename Func5, typename A1, typename A2, typename A3, typename A4, typename A5>
class Eq_f_r5 : public OneSolutionRelation<Eq_f_r5<L, Func5, A1, A2, A3, A4, A5> > {
    lref<L> l;
    A1 arg1; A2 arg2; A3 arg3; A4 arg4; A5 arg5;
    Func5 func;
    bool changed;
public:
    Eq_f_r5 (const lref<L>& l, const Func5& func, const A1& arg1, const A2& arg2, const A3& arg3, const A4& arg4, const A5& arg5) : l(l), func(func), arg1(arg1), arg2(arg2), arg3(arg3), arg4(arg4), arg5(arg5), changed(false)
    { }

    bool apply (void) {
        if(l.defined())
            return *l==func(effective_value(arg1), effective_value(arg2), effective_value(arg3), effective_value(arg4), effective_value(arg5));
        changed=true;
        l=func(effective_value(arg1), effective_value(arg2), effective_value(arg3), effective_value(arg4), effective_value(arg5));
        return true;
    }

    void revert(void) {
        if(changed)
            l.reset();
    }
};

template<typename L, typename Func6, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6>
class Eq_f_r6 : public OneSolutionRelation<Eq_f_r6<L, Func6, A1, A2, A3, A4, A5, A6> > {
    lref<L> l;
    A1 arg1; A2 arg2; A3 arg3; A4 arg4; A5 arg5; A6 arg6;
    Func6 func;
    bool changed;
public:
    Eq_f_r6 (const lref<L>& l, const Func6& func, const A1& arg1, const A2& arg2, const A3& arg3, const A4& arg4, const A5& arg5, const A6& arg6) : l(l), func(func), arg1(arg1), arg2(arg2), arg3(arg3), arg4(arg4), arg5(arg5), arg6(arg6), changed(false)
    { }

    bool apply (void) {
        if(l.defined())
            return *l==func(effective_value(arg1), effective_value(arg2), effective_value(arg3), effective_value(arg4), effective_value(arg5), effective_value(arg6));
        changed=true;
        l=func(effective_value(arg1), effective_value(arg2), effective_value(arg3), effective_value(arg4), effective_value(arg5), effective_value(arg6));
        return true;
    }

    void revert(void) {
        if(changed)
            l.reset();
    }
};

// Func = function or function object of type T(void)
template<typename T, typename Func> inline
Eq_f_r<T, Func> eq_f(lref<T> l, Func f) {
    return Eq_f_r<T, Func>(l,f);
}

template<typename T, typename Func1, typename A1> inline
Eq_f_r1<T, Func1, A1> eq_f(lref<T> l, Func1 f, const A1& a1_) {
    return Eq_f_r1<T, Func1, A1>(l,f, a1_);
}

template<typename T, typename Func2, typename A1, typename A2> inline
Eq_f_r2<T, Func2, A1, A2> eq_f(lref<T> l, Func2 f, const A1& a1_, const A2& a2_) {
    return Eq_f_r2<T, Func2, A1, A2>(l,f, a1_, a2_);
}

template<typename T, typename Func3, typename A1, typename A2, typename A3> inline
Eq_f_r3<T, Func3, A1, A2, A3> eq_f(lref<T> l, Func3 f, const A1& a1_, const A2& a2_, const A3& a3_) {
    return Eq_f_r3<T, Func3, A1, A2, A3>(l,f, a1_, a2_, a3_);
}

template<typename T, typename Func4, typename A1, typename A2, typename A3, typename A4> inline
Eq_f_r4<T, Func4, A1, A2, A3, A4> eq_f(lref<T> l, Func4 f, const A1& a1_, const A2& a2_, const A3& a3_, const A4& a4_) {
    return Eq_f_r4<T, Func4, A1, A2, A3, A4>(l,f, a1_, a2_, a3_, a4_);
}

template<typename T, typename Func5, typename A1, typename A2, typename A3, typename A4, typename A5> inline
Eq_f_r5<T, Func5, A1, A2, A3, A4, A5> eq_f(lref<T> l, Func5 f, const A1& a1_, const A2& a2_, const A3& a3_, const A4& a4_, const A5& a5_) {
    return Eq_f_r5<T, Func5, A1, A2, A3, A4, A5>(l,f, a1_, a2_, a3_, a4_, a5_);
}

template<typename T, typename Func6, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6> inline
Eq_f_r6<T, Func6, A1, A2, A3, A4, A5, A6> eq_f(lref<T> l, Func6 f, const A1& a1_, const A2& a2_, const A3& a3_, const A4& a4_, const A5& a5_, const A6& a6_) {
    return Eq_f_r6<T, Func6, A1, A2, A3, A4, A5, A6>(l,f, a1_, a2_, a3_, a4_, a5_, a6_);
}


//----------------------------------------------------------------
// eq_mf : Unify with a value obtained by calling a member function
//----------------------------------------------------------------

template<typename L, typename Obj, typename MemFunT>
class Eq_mf_r0 : public OneSolutionRelation<Eq_mf_r0<L,Obj,MemFunT> > {
    lref<L> l;
	lref<Obj> obj_;
    MemFunT mf;
    bool changed;
public:
    Eq_mf_r0(const lref<L>& l, const lref<Obj>& obj_, MemFunT mf) : l(l), obj_(obj_), mf(mf), changed(false)
	{ }

    bool apply (void) {
        if(l.defined())
            return *l==( ((*obj_).*mf)() );
        changed=true;
        l=( ((*obj_).*mf)() );
        return true;
	}

    void revert(void) {
        if(changed)
            l.reset();
    }
};


template<typename L, typename Obj, typename MemFunT, typename A1>
class Eq_mf_r1 : public OneSolutionRelation<Eq_mf_r1<L,Obj,MemFunT,A1> > {
    lref<L> l;
	lref<Obj> obj_;
    MemFunT mf;
	A1 a1_;
    bool changed;
public:
    Eq_mf_r1(const lref<L>& l, const lref<Obj>& obj_, MemFunT mf, const A1& a1_) : l(l), obj_(obj_), mf(mf), a1_(a1_), changed(false)
	{ }

    bool apply (void) {
        if(l.defined())
            return *l==( ((*obj_).*mf)(effective_value(a1_)) );
        changed=true;
        l=( ((*obj_).*mf)(effective_value(a1_)) );
        return true;
	}

    void revert(void) {
        if(changed)
            l.reset();
    }
};


template<typename L, typename Obj, typename MemFunT, typename A1, typename A2>
class Eq_mf_r2 : public OneSolutionRelation<Eq_mf_r2<L,Obj,MemFunT,A1,A2> > {
    lref<L> l;
	lref<Obj> obj_;
    MemFunT mf;
	A1 a1_; A2 a2_;
    bool changed;
public:
    Eq_mf_r2(const lref<L>& l, const lref<Obj>& obj_, MemFunT mf, const A1& a1_, const A2& a2_) : l(l), obj_(obj_), mf(mf), a1_(a1_), a2_(a2_), changed(false)
	{ }

    bool apply (void) {
        if(l.defined())
            return *l==( ((*obj_).*mf)(effective_value(a1_),effective_value(a2_)) );
        changed=true;
        l=( ((*obj_).*mf)(effective_value(a1_),effective_value(a2_)) );
        return true;
	}

    void revert(void) {
        if(changed)
            l.reset();
    }
};


template<typename L, typename Obj, typename MemFunT, typename A1, typename A2, typename A3>
class Eq_mf_r3 : public OneSolutionRelation<Eq_mf_r3<L,Obj,MemFunT,A1,A2,A3> > {
    lref<L> l;
	lref<Obj> obj_;
    MemFunT mf;
	A1 a1_; A2 a2_; A3 a3_;
    bool changed;
public:
    Eq_mf_r3(const lref<L>& l, const lref<Obj>& obj_, MemFunT mf, const A1& a1_, const A2& a2_, const A3& a3_) : l(l), obj_(obj_), mf(mf), a1_(a1_), a2_(a2_), a3_(a3_), changed(false)
	{ }

    bool apply (void) {
        if(l.defined())
            return *l==( ((*obj_).*mf)(effective_value(a1_),effective_value(a2_),effective_value(a3_)) );
        changed=true;
        l=( ((*obj_).*mf)(effective_value(a1_),effective_value(a2_),effective_value(a3_)) );
        return true;
	}

    void revert(void) {
        if(changed)
            l.reset();
    }
};


template<typename L, typename Obj, typename MemFunT, typename A1, typename A2, typename A3, typename A4>
class Eq_mf_r4 : public OneSolutionRelation<Eq_mf_r4<L,Obj,MemFunT,A1,A2,A3,A4> > {
    lref<L> l;
	lref<Obj> obj_;
    MemFunT mf;
	A1 a1_; A2 a2_; A3 a3_; A4 a4_;
    bool changed;
public:
    Eq_mf_r4(const lref<L>& l, const lref<Obj>& obj_, MemFunT mf, const A1& a1_, const A2& a2_, const A3& a3_, const A4& a4_) : l(l), obj_(obj_), mf(mf), a1_(a1_), a2_(a2_), a3_(a3_), a4_(a4_), changed(false)
	{ }

    bool apply (void) {
        if(l.defined())
            return *l==( ((*obj_).*mf)(effective_value(a1_),effective_value(a2_),effective_value(a3_),effective_value(a4_)) );
        changed=true;
        l=( ((*obj_).*mf)(effective_value(a1_),effective_value(a2_),effective_value(a3_),effective_value(a4_)) );
        return true;
	}

    void revert(void) {
        if(changed)
            l.reset();
    }
};


template<typename L, typename Obj, typename MemFunT, typename A1, typename A2, typename A3, typename A4, typename A5>
class Eq_mf_r5 : public OneSolutionRelation<Eq_mf_r5<L,Obj,MemFunT,A1,A2,A3,A4,A5> > {
    lref<L> l;
	lref<Obj> obj_;
    MemFunT mf;
	A1 a1_; A2 a2_; A3 a3_; A4 a4_; A5 a5_;
    bool changed;
public:
    Eq_mf_r5(const lref<L>& l, const lref<Obj>& obj_, MemFunT mf, const A1& a1_, const A2& a2_, const A3& a3_, const A4& a4_, const A5& a5_) : l(l), obj_(obj_), mf(mf), a1_(a1_), a2_(a2_), a3_(a3_), a4_(a4_), a5_(a5_), changed(false)
	{ }

    bool apply (void) {
        if(l.defined())
            return *l==( ((*obj_).*mf)(effective_value(a1_),effective_value(a2_),effective_value(a3_),effective_value(a4_),effective_value(a5_)) );
        changed=true;
        l=( ((*obj_).*mf)(effective_value(a1_),effective_value(a2_),effective_value(a3_),effective_value(a4_),effective_value(a5_)) );
        return true;
	}

    void revert(void) {
        if(changed)
            l.reset();
    }
};


template<typename L, typename Obj, typename MemFunT, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6>
class Eq_mf_r6 : public OneSolutionRelation<Eq_mf_r6<L,Obj,MemFunT,A1,A2,A3,A4,A5,A6> > {
    lref<L> l;
	lref<Obj> obj_;
    MemFunT mf;
	A1 a1_; A2 a2_; A3 a3_; A4 a4_; A5 a5_; A6 a6_;
    bool changed;
public:
    Eq_mf_r6(const lref<L>& l, const lref<Obj>& obj_, MemFunT mf, const A1& a1_, const A2& a2_, const A3& a3_, const A4& a4_, const A5& a5_, const A6& a6_) : l(l), obj_(obj_), mf(mf), a1_(a1_), a2_(a2_), a3_(a3_), a4_(a4_), a5_(a5_), a6_(a6_), changed(false)
	{ }

    bool apply (void) {
        if(l.defined())
            return *l==( ((*obj_).*mf)(effective_value(a1_),effective_value(a2_),effective_value(a3_),effective_value(a4_),effective_value(a5_),effective_value(a6_)) );
        changed=true;
        l=( ((*obj_).*mf)(effective_value(a1_),effective_value(a2_),effective_value(a3_),effective_value(a4_),effective_value(a5_),effective_value(a6_)) );
        return true;
	}

    void revert(void) {
        if(changed)
            l.reset();
    }
};


template<typename L, typename Obj, typename MemFunT> inline
Eq_mf_r0<L, Obj, MemFunT> eq_mf(lref<L> l, lref<Obj>& obj_, MemFunT mf) {
    return Eq_mf_r0<L, Obj, MemFunT>(l, obj_, mf);
}

template<typename L, typename Obj, typename MemFunT, typename A1> inline
Eq_mf_r1<L, Obj, MemFunT, A1> eq_mf(lref<L> l, lref<Obj>& obj_, MemFunT mf, const A1& a1_) {
    return Eq_mf_r1<L, Obj, MemFunT, A1>(l, obj_, mf, a1_);
}

template<typename L, typename Obj, typename MemFunT, typename A1, typename A2> inline
Eq_mf_r2<L, Obj, MemFunT, A1, A2> eq_mf(lref<L> l, lref<Obj>& obj_, MemFunT mf, const A1& a1_, const A2& a2_) {
    return Eq_mf_r2<L, Obj, MemFunT, A1, A2>(l, obj_, mf, a1_, a2_);
}

template<typename L, typename Obj, typename MemFunT, typename A1, typename A2, typename A3> inline
Eq_mf_r3<L, Obj, MemFunT, A1, A2, A3> eq_mf(lref<L> l, lref<Obj>& obj_, MemFunT mf, const A1& a1_, const A2& a2_, const A3& a3_) {
    return Eq_mf_r3<L, Obj, MemFunT, A1, A2, A3>(l, obj_, mf, a1_, a2_, a3_);
}

template<typename L, typename Obj, typename MemFunT, typename A1, typename A2, typename A3, typename A4> inline
Eq_mf_r4<L, Obj, MemFunT, A1, A2, A3, A4> eq_mf(lref<L> l, lref<Obj>& obj_, MemFunT mf, const A1& a1_, const A2& a2_, const A3& a3_, const A4& a4_) {
    return Eq_mf_r4<L, Obj, MemFunT, A1, A2, A3, A4>(l, obj_, mf, a1_, a2_, a3_, a4_);
}

template<typename L, typename Obj, typename MemFunT, typename A1, typename A2, typename A3, typename A4, typename A5> inline
Eq_mf_r5<L, Obj, MemFunT, A1, A2, A3, A4, A5> eq_mf(lref<L> l, lref<Obj>& obj_, MemFunT mf, const A1& a1_, const A2& a2_, const A3& a3_, const A4& a4_, const A5& a5_) {
    return Eq_mf_r5<L, Obj, MemFunT, A1, A2, A3, A4, A5>(l, obj_, mf, a1_, a2_, a3_, a4_, a5_);
}

template<typename L, typename Obj, typename MemFunT, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6> inline
Eq_mf_r6<L, Obj, MemFunT, A1, A2, A3, A4, A5, A6> eq_mf(lref<L> l, lref<Obj>& obj_, MemFunT mf, const A1& a1_, const A2& a2_, const A3& a3_, const A4& a4_, const A5& a5_, const A6& a6_) {
    return Eq_mf_r6<L, Obj, MemFunT, A1, A2, A3, A4, A5, A6>(l, obj_, mf, a1_, a2_, a3_, a4_, a5_, a6_);
}

} // namespace castor

#endif
