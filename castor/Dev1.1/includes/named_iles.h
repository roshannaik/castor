// Castor : Logic Programming Library
// Copyright © 2008 Roshan Naik (roshan@mpprogramming.com).
// This software is goverened by the MIT license (http://www.opensource.org/licenses/mit-license.php).

#if !defined CASTOR_NAMED_ILES_H
#define CASTOR_NAMED_ILES_H 1

#include "ile.h"
#include "helpers.h"

namespace castor {

//-------------------------------------------------
// create() and Create<>::with()
//-------------------------------------------------

template<typename T>
class CreateWith0 {
public:
    typedef T result_type;
    T operator()(void) {
        return T();
    }
};

template<typename T, typename A1>
class CreateWith1 {
    A1 a1;
public:
    typedef T result_type;
    CreateWith1(const A1& a1) : a1(a1) 
    { }

    T operator()(void) {
        return T(effective_value(a1));
    }
};

template<typename T, typename A1, typename A2>
class CreateWith2 {
    A1 a1; A2 a2;
public:
    typedef T result_type;
    CreateWith2(const A1& a1, const A2& a2) : a1(a1), a2(a2)
    { }

    T operator()(void) {
        return T(effective_value(a1), effective_value(a2));
    }
};

template<typename T, typename A1, typename A2, typename A3>
class CreateWith3 {
    A1 a1; A2 a2; A3 a3;
public:
    typedef T result_type;
    CreateWith3(const A1& a1, const A2& a2, const A3& a3) : a1(a1), a2(a2), a3(a3)
    { }

    T operator()(void) {
        return T(effective_value(a1), effective_value(a2), effective_value(a3));
    }
};

template<typename T, typename A1, typename A2, typename A3, typename A4>
class CreateWith4 {
    A1 a1; A2 a2; A3 a3; A4 a4;
public:
    typedef T result_type;
    CreateWith4(const A1& a1, const A2& a2, const A3& a3, const A4& a4) : a1(a1), a2(a2), a3(a3), a4(a4)
    { }

    T operator()(void) {
        return T(effective_value(a1), effective_value(a2), effective_value(a3), effective_value(a4));
    }
};

template<typename T, typename A1, typename A2, typename A3, typename A4, typename A5>
class CreateWith5 {
    A1 a1; A2 a2; A3 a3; A4 a4; A5 a5;
public:
    typedef T result_type;
    CreateWith5(const A1& a1, const A2& a2, const A3& a3, const A4& a4, const A5& a5) : a1(a1), a2(a2), a3(a3), a4(a4), a5(a5)
    { }

    T operator()(void) {
        return T(effective_value(a1), effective_value(a2), effective_value(a3), effective_value(a4), effective_value(a5));
    }
};

template<typename T, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6>
class CreateWith6 {
    A1 a1; A2 a2; A3 a3; A4 a4; A5 a5; A6 a6;
public:
    typedef T result_type;
    CreateWith6(const A1& a1, const A2& a2, const A3& a3, const A4& a4, const A5& a5, const A6& a6) : a1(a1), a2(a2), a3(a3), a4(a4), a5(a5), a6(a6)
    { }

    T operator()(void) {
        return T(effective_value(a1), effective_value(a2), effective_value(a3), effective_value(a4), effective_value(a5), effective_value(a6));
    }
};


#ifdef CASTOR_ENABLE_DEPRECATED

template<typename T>
class Create {
public:
    static Ile<CreateWith0<T> > with() {
        return CreateWith0<T>();
    }

    template<typename A1>
    static Ile<CreateWith1<T,A1> > with(const A1& a1) {
        return CreateWith1<T,A1>(a1);
    }

    template<typename A1, typename A2>
    static Ile<CreateWith2<T,A1,A2> > with(const A1& a1, const A2& a2) {
        return CreateWith2<T,A1,A2>(a1,a2);
    }

    template<typename A1, typename A2, typename A3>
    static Ile<CreateWith3<T,A1,A2,A3> > with(const A1& a1, const A2& a2, const A3& a3) {
        return CreateWith3<T,A1,A2,A3>(a1,a2,a3);
    }

    template<typename A1, typename A2, typename A3, typename A4>
    static Ile<CreateWith4<T,A1,A2,A3,A4> > with(const A1& a1, const A2& a2, const A3& a3, const A4& a4) {
        return CreateWith4<T,A1,A2,A3,A4>(a1,a2,a3,a4);
    }

    template<typename A1, typename A2, typename A3, typename A4, typename A5>
    static Ile<CreateWith5<T,A1,A2,A3,A4,A5> > with(const A1& a1, const A2& a2, const A3& a3, const A4& a4, const A5& a5) {
        return CreateWith5<T,A1,A2,A3,A4,A5>(a1,a2,a3,a4,a5);
    }

    template<typename A1, typename A2, typename A3, typename A4, typename A5, typename A6>
    static Ile<CreateWith6<T,A1,A2,A3,A4,A5,A6> > with(const A1& a1, const A2& a2, const A3& a3, const A4& a4, const A5& a5, const A6& a6) {
        return CreateWith6<T,A1,A2,A3,A4,A5,A6>(a1,a2,a3,a4,a5,a6);
    }
};

#endif


template<typename T> inline
Ile<CreateWith0<T> > create() {
    return CreateWith0<T>();
}

template<typename T, typename A1> inline
 Ile<CreateWith1<T,A1> > create(const A1& a1) {
    return CreateWith1<T,A1>(a1);
}

template<typename T, typename A1, typename A2> inline
 Ile<CreateWith2<T,A1,A2> > create(const A1& a1, const A2& a2) {
    return CreateWith2<T,A1,A2>(a1,a2);
}

template<typename T, typename A1, typename A2, typename A3> inline
 Ile<CreateWith3<T,A1,A2,A3> > create(const A1& a1, const A2& a2, const A3& a3) {
    return CreateWith3<T,A1,A2,A3>(a1,a2,a3);
}

template<typename T, typename A1, typename A2, typename A3, typename A4> inline
 Ile<CreateWith4<T,A1,A2,A3,A4> > create(const A1& a1, const A2& a2, const A3& a3, const A4& a4) {
    return CreateWith4<T,A1,A2,A3,A4>(a1,a2,a3,a4);
}

template<typename T, typename A1, typename A2, typename A3, typename A4, typename A5> inline
Ile<CreateWith5<T,A1,A2,A3,A4,A5> > create(const A1& a1, const A2& a2, const A3& a3, const A4& a4, const A5& a5) {
    return CreateWith5<T,A1,A2,A3,A4,A5>(a1,a2,a3,a4,a5);
}

template<typename T, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6> inline
Ile<CreateWith6<T,A1,A2,A3,A4,A5,A6> > create(const A1& a1, const A2& a2, const A3& a3, const A4& a4, const A5& a5, const A6& a6) {
    return CreateWith6<T,A1,A2,A3,A4,A5,A6>(a1,a2,a3,a4,a5,a6);
}
//-------------------------------------------------
// get() - get the value of data member
//-------------------------------------------------

template<typename Obj, typename MemberT>
class Get {
    lref<Obj> obj_;
	MemberT Obj::* mem;
public:
    typedef MemberT result_type;

    Get(const lref<Obj>& obj_, MemberT Obj::* mem) : obj_(obj_), mem(mem)
	{ }

    MemberT& operator()(void) {
        return (*obj_).*mem;
    }

    const MemberT& operator()(void) const {
        return (*obj_).*mem;
    }
};

template<typename Obj, typename MemberT> inline
Ile<Get<Obj,MemberT> > get(const lref<Obj>& obj_, MemberT Obj::* mem) {
	return Get<Obj,MemberT>(obj_,mem);
}


//-------------------------------------------------
// ref() - maintain a reference to an obj and return that reference on evaluation
//-------------------------------------------------
template<typename T>
class Ref {
    T& obj;
public:
    typedef T& result_type;

    Ref(T& obj) :obj(obj)
    { }

    T& operator()(void) {
        return obj;
    }
    const T& operator()(void) const {
        return obj;
    }
};

template<typename T> inline
Ile<Ref<T> > ref(T& obj) {
    return Ref<T>(obj);
}



//-------------------------------------------------
// call() - invoke a function
//-------------------------------------------------

template<typename FuncT>
class Call_0 {
	FuncT f;
public:
    typedef typename detail::return_type<FuncT>::result_type result_type;

	Call_0(FuncT f) : f(f)
	{ }

    result_type operator()(void) {
        return f();
    }

    result_type operator()(void)  const {
        return f();
    }
};


template<typename FuncT, typename A1>
class Call_1 {
	FuncT f;
	A1 a1;
public:
	typedef typename detail::return_type<FuncT>::result_type result_type;

	Call_1(FuncT f, const A1& a1) : f(f), a1(a1)
	{ }

    result_type operator()(void)  {
        return f( effective_value(a1) );
    }

    result_type operator()(void)  const {
        return f( effective_value(a1) );
    }
};


template<typename FuncT, typename A1, typename A2>
class Call_2 {
	FuncT f;
	A1 a1; A2 a2;
public:
	typedef typename detail::return_type<FuncT>::result_type result_type;

	Call_2(FuncT f, const A1& a1, const A2& a2) : f(f), a1(a1), a2(a2)
	{ }

    result_type operator()(void) {
        return f( effective_value(a1),effective_value(a2) );
    }

    result_type operator()(void)  const {
        return f( effective_value(a1),effective_value(a2) );
    }
};

template<typename FuncT, typename A1, typename A2, typename A3>
class Call_3 {
	FuncT f;
	A1 a1; A2 a2; A3 a3;
public:
	typedef typename detail::return_type<FuncT>::result_type result_type;

	Call_3(FuncT f, const A1& a1, const A2& a2, const A3& a3) : f(f), a1(a1), a2(a2), a3(a3)
	{ }

    result_type operator()(void)  {
        return f( effective_value(a1),effective_value(a2),effective_value(a3) );
    }

    result_type operator()(void)  const {
        return f( effective_value(a1),effective_value(a2),effective_value(a3) );
    }
};


template<typename FuncT, typename A1, typename A2, typename A3, typename A4>
class Call_4 {
	FuncT f;
	A1 a1; A2 a2; A3 a3; A4 a4;
public:
	typedef typename detail::return_type<FuncT>::result_type result_type;

	Call_4(FuncT f, const A1& a1, const A2& a2, const A3& a3, const A4& a4) : f(f), a1(a1), a2(a2), a3(a3), a4(a4)
	{ }

    result_type operator()(void)  {
        return f(  effective_value(a1),effective_value(a2),effective_value(a3),effective_value(a4) );
    }

    result_type operator()(void)  const {
        return f(  effective_value(a1),effective_value(a2),effective_value(a3),effective_value(a4) );
    }
};


template<typename FuncT, typename A1, typename A2, typename A3, typename A4, typename A5>
class Call_5 {
	FuncT f;
	A1 a1; A2 a2; A3 a3; A4 a4; A5 a5;
public:
	typedef typename detail::return_type<FuncT>::result_type result_type;

	Call_5(FuncT f, const A1& a1, const A2& a2, const A3& a3, const A4& a4, const A5& a5) : f(f), a1(a1), a2(a2), a3(a3), a4(a4), a5(a5)
	{ }

    result_type operator()(void)  {
        return f( effective_value(a1),effective_value(a2),effective_value(a3),effective_value(a4),effective_value(a5) );
    }

    result_type operator()(void)  const {
        return f( effective_value(a1),effective_value(a2),effective_value(a3),effective_value(a4),effective_value(a5) );
    }
};

template<typename FuncT, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6>
class Call_6 {
	FuncT f;
	A1 a1; A2 a2; A3 a3; A4 a4; A5 a5; A6 a6;
public:
	typedef typename detail::return_type<FuncT>::result_type result_type;

	Call_6(FuncT f, const A1& a1, const A2& a2, const A3& a3, const A4& a4, const A5& a5, const A6& a6) : f(f), a1(a1), a2(a2), a3(a3), a4(a4), a5(a5), a6(a6)
	{ }

    result_type operator()(void)  {
        return f( effective_value(a1),effective_value(a2),effective_value(a3),effective_value(a4),effective_value(a5),effective_value(a6) );
    }

    result_type operator()(void)  const {
        return f( effective_value(a1),effective_value(a2),effective_value(a3),effective_value(a4),effective_value(a5),effective_value(a6) );
    }
};


// For Nullary through sestiary(6-ary) functions
template<typename FuncT> inline
Ile<Call_0<FuncT> > call(FuncT f) {
	return Call_0<FuncT>(f);
}

template<typename FuncT, typename A1> inline
Ile<Call_1<FuncT,A1> > call(FuncT f, const A1& a1) {
	return Call_1<FuncT,A1>(f, a1);
}

template<typename FuncT, typename A1, typename A2> inline
Ile<Call_2<FuncT,A1,A2> > call(FuncT f, const A1& a1, const A2& a2) {
	return Call_2<FuncT,A1,A2>(f,a1,a2);
}

template<typename FuncT, typename A1, typename A2, typename A3> inline
Ile<Call_3<FuncT,A1,A2,A3> > call(FuncT f, const A1& a1, const A2& a2, const A3& a3) {
	return Call_3<FuncT,A1,A2,A3>(f,a1,a2,a3);
}

template<typename FuncT, typename A1, typename A2, typename A3, typename A4> inline
Ile<Call_4<FuncT,A1,A2,A3,A4> > call(FuncT f, const A1& a1, const A2& a2, const A3& a3, const A4& a4) {
	return Call_4<FuncT,A1,A2,A3,A4>(f,a1,a2,a3,a4);
}

template<typename FuncT, typename A1, typename A2, typename A3, typename A4, typename A5> inline
Ile<Call_5<FuncT,A1,A2,A3,A4,A5> > call(FuncT f, const A1& a1, const A2& a2, const A3& a3, const A4& a4, const A5& a5) {
	return Call_5<FuncT,A1,A2,A3,A4,A5>(f,a1,a2,a3,a4,a5);
}

template<typename FuncT, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6> inline
Ile<Call_6<FuncT,A1,A2,A3,A4,A5,A6> > call(FuncT f, const A1& a1, const A2& a2, const A3& a3, const A4& a4, const A5& a5, const A6& a6) {
	return Call_6<FuncT,A1,A2,A3,A4,A5,A6>(f,a1,a2,a3,a4,a5,a6);
}


// For nullary through sestiary(6-ary) function objects
template<typename Ret> inline
Ile<Call_0<Ret(*)(void)> > call(Ret(* f)(void)) {
	return Call_0<Ret(*)(void)>(f);
}

template<typename Ret, typename A1, typename P1> inline
Ile<Call_1<Ret(*)(P1),A1> > call(Ret(* f)(P1), const A1& a1) {
	return Call_1<Ret(*)(P1),A1>(f,a1);
}

template<typename Ret, typename A1, typename A2, typename P1, typename P2> inline
Ile<Call_2<Ret(*)(P1,P2),A1,A2> > call(Ret(* f)(P1,P2), const A1& a1, const A2& a2) {
	return Call_2<Ret(*)(P1,P2),A1,A2>(f,a1,a2);
}

template<typename Ret, typename A1, typename A2, typename A3, typename P1, typename P2, typename P3> inline
Ile<Call_3<Ret(*)(P1,P2,P3),A1,A2,A3> > call(Ret(* f)(P1,P2,P3), const A1& a1, const A2& a2, const A3& a3) {
	return Call_3<Ret(*)(P1,P2,P3),A1,A2,A3>(f,a1,a2,a3);
}

template<typename Ret, typename A1, typename A2, typename A3, typename A4, typename P1, typename P2, typename P3, typename P4> inline
Ile<Call_4<Ret(*)(P1,P2,P3,P4),A1,A2,A3,A4> > call(Ret(* f)(P1,P2,P3,P4), const A1& a1, const A2& a2, const A3& a3, const A4& a4) {
	return Call_4<Ret(*)(P1,P2,P3,P4),A1,A2,A3,A4>(f,a1,a2,a3,a4);
}

template<typename Ret, typename A1, typename A2, typename A3, typename A4, typename A5, typename P1, typename P2, typename P3, typename P4, typename P5> inline
Ile<Call_5<Ret(*)(P1,P2,P3,P4,P5),A1,A2,A3,A4,A5> > call(Ret(* f)(P1,P2,P3,P4,P5), const A1& a1, const A2& a2, const A3& a3, const A4& a4, const A5& a5) {
	return Call_5<Ret(*)(P1,P2,P3,P4,P5),A1,A2,A3,A4,A5>(f,a1,a2,a3,a4,a5);
}

template<typename Ret, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6> inline
Ile<Call_6<Ret(*)(P1,P2,P3,P4,P5,P6),A1,A2,A3,A4,A5,A6> > call(Ret(* f)(P1,P2,P3,P4,P5,P6), const A1& a1, const A2& a2, const A3& a3, const A4& a4, const A5& a5, const A6& a6) {
	return Call_6<Ret(*)(P1,P2,P3,P4,P5,P6),A1,A2,A3,A4,A5,A6>(f,a1,a2,a3,a4,a5,a6);
}

} // namespace castor
#endif
