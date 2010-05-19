// Castor : Logic Programming Library
// Copyright © 2008 Roshan Naik (roshan@mpprogramming.com).
// This software is goverened by the MIT license (http://www.opensource.org/licenses/mit-license.php).

#if !defined CASTOR_ILE_H
#define CASTOR_ILE_H 1

#include "relation.h"
#include "lref.h"
#include "ops.h"
#include "cut.h"
#include "workaround.h"
#include "helpers.h"

namespace castor {
//-------------------------------------------------------------------------
// Creation of 'Expression templates' from expressions involving lref<>
//-------------------------------------------------------------------------
template <typename Expr>
class Ile {
    Expr expr;
public:
    typedef typename Expr::result_type result_type;
    Ile(Expr expr) : expr(expr)
    { }

    result_type operator()(void) {
        return expr();
    }

    result_type operator()(void) const {
        return expr();
    }
};

namespace detail {
    template<typename Exp> inline
    Ile<Exp> make_IleExp( const Exp& e) {
        return (e);
    }

    // evaluate() : used to recursively evaluate an Ile(as its composed from other Iles, lrefs, values)
    template <typename T> inline
    T& evaluate(T& t) {
        return t;
    }

    template <typename T> inline
    const T& evaluate(const T& t) {
        return t;
    }

    template <typename T> inline
    typename lref<T>::result_type& evaluate(lref<T>& lr) {
        return *lr;
    }

    template <typename T> inline
        const typename lref<T>::result_type & evaluate(const lref<T>& lref) {
        return *lref;
    }

    template <typename Expr> inline 
        typename Ile<Expr>::result_type evaluate(Ile<Expr>& expr) {
        return expr();
    }

    template <typename Expr> inline 
    typename Ile<Expr>::result_type evaluate(const Ile<Expr>& expr) {
        return expr();
    }
} //namespace detail


#define LREF_BINARYOP_CLASS_DECL(OPERATOR, OP_CLASS, RESULT)              \
template <typename L, typename R>                                         \
struct OP_CLASS {                                                         \
    typedef RESULT result_type;                                            \
    L left;                                                               \
    R right;                                                              \
                                                                          \
    OP_CLASS(const L& left, const R& right) : left(left), right(right)    \
    { }                                                                   \
                                                                          \
    result_type operator()(void) {                                        \
    return ::castor::detail::evaluate(left) OPERATOR ::castor::detail::evaluate(right);       \
    }                                                                     \
                                                                          \
    result_type operator()(void) const {                                  \
        return ::castor::detail::evaluate(left) OPERATOR ::castor::detail::evaluate(right);   \
    }                                                                     \
};

#define LREF_BINARYOP_FUNC_DECL(OPERATOR, OP_CLASS)                       \
template <typename L, typename R> inline                                  \
Ile<OP_CLASS<lref<L>, R> >                                                \
operator OPERATOR (lref<L> & left, const R & right)  {                    \
    return ::castor::detail::make_IleExp(OP_CLASS<lref<L>, R>(left,right) );        \
}                                                                         \
                                                                          \
template <typename L, typename R> inline                                  \
Ile<OP_CLASS<L, lref<R> > >                                               \
operator OPERATOR (const L & left, lref<R> & right)  {                    \
    return ::castor::detail::make_IleExp(OP_CLASS<L, lref<R> >(left,right) );       \
}                                                                         \
                                                                          \
template <typename L, typename R> inline                                  \
Ile<OP_CLASS<lref<L>, lref<R> > >                                         \
operator OPERATOR (lref<L> & left, lref<R> & right)  {                    \
    return ::castor::detail::make_IleExp(OP_CLASS<lref<L>, lref<R> >(left,right) ); \
}                                                                         \
                                                                          \
template <typename L, typename R> inline                                  \
Ile<OP_CLASS<Ile<L>, R> >                                                 \
operator OPERATOR (const Ile<L> & left, const R & right)  {               \
	return ::castor::detail::make_IleExp(OP_CLASS<Ile<L>, R>(left,right) );         \
}                                                                         \
																		  \
template <typename L, typename R> inline                                  \
Ile<OP_CLASS<L, Ile<R> > >                                                \
operator OPERATOR (const L & left, const Ile<R> & right)  {               \
	return ::castor::detail::make_IleExp(OP_CLASS<L, Ile<R> >(left,right) );        \
}                                                                         \
																		  \
template <typename L, typename R> inline                                  \
Ile<OP_CLASS<Ile<L>, Ile<R> > >                                           \
operator OPERATOR (const Ile<L> & left, const Ile<R> & right)  {          \
    return ::castor::detail::make_IleExp(OP_CLASS<Ile<L>, Ile<R> >(left,right) );   \
}                                                                         \
                                                                          \
template <typename L, typename R> inline                                  \
Ile<OP_CLASS<Ile<L>, lref<R> > >                                          \
operator OPERATOR (const Ile<L> & left, lref<R> & right)  {               \
	return ::castor::detail::make_IleExp(OP_CLASS<Ile<L>, lref<R> >(left,right) );  \
}                                                                         \
																		  \
template <typename L, typename R> inline                                  \
Ile<OP_CLASS<lref<L>, Ile<R> > >                                          \
operator OPERATOR (lref<L> & left, const Ile<R> & right)  {               \
    return ::castor::detail::make_IleExp(OP_CLASS<lref<L>, Ile<R> >(left,right) );  \
}


#define LREF_BINARYOP_DECL(OPERATOR, OP_CLASS, RESULT) \
LREF_BINARYOP_CLASS_DECL(OPERATOR, OP_CLASS, RESULT)   \
LREF_BINARYOP_FUNC_DECL(OPERATOR, OP_CLASS) 

namespace detail {
    template<typename T>
    struct ComputeResultTyp {
        typedef typename ::castor::detail::NonRefType<T>::result_type result_type;
    };

    template<typename T>
    struct ComputeResultTyp<Ile<T> > {
        typedef typename ::castor::detail::NonRefType<typename Ile<T>::result_type >::result_type result_type;
    };

    template<typename T>
    struct ComputeResultTyp<lref<T> > {
        typedef typename ::castor::detail::NonRefType<typename lref<T>::result_type >::result_type result_type;
    };
}
LREF_BINARYOP_DECL(+,  Plus_ILE, typename ::castor::detail::ComputeResultTyp<L>::result_type)
LREF_BINARYOP_DECL(-,  Minus_ILE, typename ::castor::detail::ComputeResultTyp<L>::result_type)
LREF_BINARYOP_DECL(*,  Mult_ILE, typename ::castor::detail::ComputeResultTyp<L>::result_type)
LREF_BINARYOP_DECL(/,  Div_ILE, typename ::castor::detail::ComputeResultTyp<L>::result_type)
LREF_BINARYOP_DECL(%,  Mod_ILE, typename ::castor::detail::ComputeResultTyp<L>::result_type)
LREF_BINARYOP_DECL(|,  BitOr_ILE, typename ::castor::detail::ComputeResultTyp<L>::result_type)
LREF_BINARYOP_DECL(^,  BitExOr_ILE, typename ::castor::detail::ComputeResultTyp<L>::result_type)
LREF_BINARYOP_DECL(&,  BitAnd_ILE, typename ::castor::detail::ComputeResultTyp<L>::result_type)
LREF_BINARYOP_DECL(<<, ShiftLeft_ILE, typename ::castor::detail::ComputeResultTyp<L>::result_type)
LREF_BINARYOP_DECL(>>, ShiftRight_ILE, typename ::castor::detail::ComputeResultTyp<L>::result_type)

LREF_BINARYOP_DECL(==, Equal_ILE, bool)
LREF_BINARYOP_DECL(!=, UnEqual_ILE, bool)
LREF_BINARYOP_DECL(<,  Less_ILE, bool)
LREF_BINARYOP_DECL(>,  Greater_ILE, bool)
LREF_BINARYOP_DECL(<=, LessEqual_ILE, bool)
LREF_BINARYOP_DECL(>=, GreaterEqual_ILE, bool)
LREF_BINARYOP_DECL(&&, And_ILE, bool)
LREF_BINARYOP_DECL(||, Or_ILE, bool)


// Prefix unary operators come in two varieties: either they return a value(e.g -, +, !)
//  or they return a reference (e.g. ++,--)
#define LREF_PREFIX_UNARYOP_CLASS_DECL_RETREF(OPERATOR, OP_CLASS)                               \
template <typename Expr>                                                                        \
class OP_CLASS {                                                                                \
    Expr expr;                                                                                  \
public:                                                                                         \
    typedef typename ::castor::detail::RefType<typename Expr::result_type>::result_type result_type;      \
                                                                                                \
    OP_CLASS(const Expr& expr) : expr(expr)                                                     \
    { }                                                                                         \
                                                                                                \
    result_type operator()(void) {                                                              \
        return OPERATOR (::castor::detail::evaluate(expr));                                               \
    }                                                                                           \
                                                                                                \
    result_type operator()(void) const  {                                                       \
        return OPERATOR (::castor::detail::evaluate(expr));                                               \
    }                                                                                           \
};

#define LREF_PREFIX_UNARYOP_CLASS_DECL_RETVAL(OPERATOR, OP_CLASS, RESULT)           \
template <typename Expr>                                                            \
class OP_CLASS {                                                                    \
    Expr expr;                                                                      \
public:                                                                             \
    typedef RESULT result_type;                                                     \
                                                                                    \
    OP_CLASS(const Expr& expr) : expr(expr)                                         \
    { }                                                                             \
                                                                                    \
    result_type operator()(void)  {                                                 \
        return OPERATOR (::castor::detail::evaluate(expr));                                   \
    }                                                                               \
    result_type operator()(void) const {                                            \
        return OPERATOR (::castor::detail::evaluate(expr));                                   \
    }                                                                               \
};


#define LREF_PREFIX_UNARYOP_FUNCS_DECL(OPERATOR, OP_CLASS)                          \
template <typename T> inline                                                        \
Ile<OP_CLASS<lref<T> > >                                                            \
operator OPERATOR (lref<T> const & obj) {                                           \
    return Ile<OP_CLASS<lref<T> > >(obj);                                           \
}                                                                                   \
                                                                                    \
template <typename T> inline                                                        \
Ile<OP_CLASS<Ile<T> > >                                                             \
operator OPERATOR (Ile<T> const & expr) {                                           \
    return Ile<OP_CLASS<Ile<T> > >(expr);                                           \
}


#define LREF_PREFIX_UNARYOP_DECL_RETREF(OPERATOR, OP_CLASS)                         \
LREF_PREFIX_UNARYOP_CLASS_DECL_RETREF(OPERATOR, OP_CLASS)                           \
LREF_PREFIX_UNARYOP_FUNCS_DECL(OPERATOR, OP_CLASS)

#define LREF_PREFIX_UNARYOP_DECL_RETVAL(OPERATOR, OP_CLASS, RESULT)                 \
LREF_PREFIX_UNARYOP_CLASS_DECL_RETVAL(OPERATOR, OP_CLASS, RESULT)                   \
LREF_PREFIX_UNARYOP_FUNCS_DECL(OPERATOR, OP_CLASS)



LREF_PREFIX_UNARYOP_DECL_RETVAL(+, Prefix_Plus_ILE, typename ::castor::detail::ComputeResultTyp<Expr>::result_type )
LREF_PREFIX_UNARYOP_DECL_RETVAL(-, Prefix_Minus_ILE, typename ::castor::detail::ComputeResultTyp<Expr>::result_type )
LREF_PREFIX_UNARYOP_DECL_RETVAL(~, Prefix_Complement_ILE, typename ::castor::detail::ComputeResultTyp<Expr>::result_type )
LREF_PREFIX_UNARYOP_DECL_RETVAL(!, Prefix_Not_ILE, bool)

LREF_PREFIX_UNARYOP_DECL_RETREF(++, Prefix_Inc_ILE)
LREF_PREFIX_UNARYOP_DECL_RETREF(--, Prefix_Dec_ILE)


#define LREF_POSTFIX_UNARYOP_CLASS_DECL(OPERATOR, OP_CLASS)                                      \
template <typename IleExpr>                                                                      \
class OP_CLASS {                                                                                 \
    IleExpr expr;                                                                                \
public:                                                                                          \
    typedef typename ::castor::detail::NonRefType<typename IleExpr::result_type>::result_type result_type; \
                                                                                                 \
    OP_CLASS(const IleExpr& expr) : expr(expr)                                                   \
    { }                                                                                          \
                                                                                                 \
    result_type operator()(void) {                                                               \
        return (::castor::detail::evaluate(expr)) OPERATOR;                                                \
    }                                                                                            \
    result_type  operator()(void) const {                                                        \
        return (::castor::detail::evaluate(expr)) OPERATOR;                                                \
    }                                                                                            \
};

#define LREF_POSTFIX_UNARYOP_FUNCS_DECL(OPERATOR, OP_CLASS)                         \
template <typename T> inline                                                        \
Ile<OP_CLASS<lref<T> > >                                                            \
operator OPERATOR (lref<T> const & obj, int) {                                      \
    return Ile<OP_CLASS<lref<T> > >(obj);                                           \
}                                                                                   \
                                                                                    \
template <typename T> inline                                                        \
Ile<OP_CLASS<Ile<T> > >                                                             \
operator OPERATOR (Ile<T> const & expr, int) {                                      \
    return Ile<OP_CLASS<Ile<T> > >(expr);                                           \
}

#define LREF_POSTFIX_UNARYOP_DECL(OPERATOR, OP_CLASS)                               \
LREF_POSTFIX_UNARYOP_CLASS_DECL(OPERATOR, OP_CLASS)                                 \
LREF_POSTFIX_UNARYOP_FUNCS_DECL(OPERATOR, OP_CLASS)


LREF_POSTFIX_UNARYOP_DECL(++, Postfix_Inc_ILE)
LREF_POSTFIX_UNARYOP_DECL(--, Postfix_Dec_ILE)


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


} // namespace castor
#endif
