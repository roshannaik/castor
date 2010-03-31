// Castor : Logic Programming Library
// Copyright � 2008 Roshan Naik (roshan@mpprogramming.com).
// This software is governed by the MIT license (http://www.opensource.org/licenses/mit-license.php).

#if !defined CASTOR_HELPERS_H
#define CASTOR_HELPERS_H 1

#include <iterator>
#include <algorithm>
#include <set>

namespace castor { namespace detail {

struct None
{ };

template<typename IteratorCategory>
struct ItemCounter {
    template<typename InputIterator> 
    static typename std::iterator_traits<InputIterator>::difference_type
    count(InputIterator first, InputIterator last) {
        typename std::iterator_traits<InputIterator>::difference_type result=0;
        for(;first!=last; ++first)
            ++result;
        return result;    
    }
};

template<>
struct ItemCounter<std::random_access_iterator_tag> {
    template<typename InputIterator> 
    static typename std::iterator_traits<InputIterator>::difference_type
    count(InputIterator first, InputIterator last) {
       return last-first;    
    }
};

// returns the number of elements between iterators first & last
template<typename InputIterator>
typename std::iterator_traits<InputIterator>::difference_type
countItems(InputIterator first, InputIterator last) {
    return ItemCounter<typename InputIterator::iterator_category>::count(first,last);
}

template<typename T>
size_t countItems(T* first, T* last) {
    return last-first;
}


template <typename T>
struct RefType {
    typedef T& result_type;
};

template <typename T>
struct RefType <T&> {
    typedef T& result_type;
};

template <typename T>
struct NonRefType {
    typedef T result_type;
};

template <typename T>
struct NonRefType <T&> {
    typedef T result_type;
};


template <typename T>
struct RemoveConst {
    typedef T result_type;
};

template <typename T>
struct RemoveConst<const T> {
    typedef T result_type;
};

#ifdef __BCPLUSPLUS__
template <typename T1, typename T2>
struct SameType { bool static const result = false; };

template <typename T>
struct SameType<T,T> { bool static const result = false; };
#else
template <typename T1, typename T2>
struct SameType { enum { result = false }; };

template <typename T>
struct SameType<T,T> { enum { result = true }; };
#endif


// ---  Static Assertions
template<bool> struct Assertion_ERROR;

template<> struct Assertion_ERROR<true> 
{ };

#define ASSERT_SAME_TYPE(T1, T2, ERROR_MSG) \
::castor::detail::Assertion_ERROR<::castor::detail::SameType<T1,T2>::result >();


//-------- compute return type of functions, function objects or  member functions ----------------

// supports function objects
template<typename FuncT>
struct return_type {
	typedef typename FuncT::result_type result_type;
};

// support for functions
template<typename T>
struct return_type<T(*)(void)> {
    typedef T result_type;
};

template<typename T, typename A1>
struct return_type<T(*)(A1)> {
    typedef T result_type;
};

template<typename T, typename A1, typename A2>
struct return_type<T(*)(A1,A2)> {
    typedef T result_type;
};

template<typename T, typename A1, typename A2, typename A3>
struct return_type<T(*)(A1,A2,A3)> {
    typedef T result_type;
};

template<typename T, typename A1, typename A2, typename A3, typename A4>
struct return_type<T(*)(A1,A2,A3,A4)> {
    typedef T result_type;
};

template<typename T, typename A1, typename A2, typename A3, typename A4, typename A5>
struct return_type<T(*)(A1,A2,A3,A4,A5)> {
    typedef T result_type;
};

template<typename T, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6>
struct return_type<T(*)(A1,A2,A3,A4,A5,A6)> {
    typedef T result_type;
};

// support for non-const methods
template<typename Obj, typename R>
struct return_type<R(Obj::*)(void)> {
    typedef R result_type;
};

template<typename Obj, typename R, typename A1>
struct return_type<R(Obj::*)(A1)> {
    typedef R result_type;
};

template<typename Obj, typename R, typename A1, typename A2>
struct return_type<R(Obj::*)(A1,A2)> {
    typedef R result_type;
};

template<typename Obj, typename R, typename A1, typename A2, typename A3>
struct return_type<R(Obj::*)(A1,A2,A3)> {
    typedef R result_type;
};

template<typename Obj, typename R, typename A1, typename A2, typename A3, typename A4>
struct return_type<R(Obj::*)(A1,A2,A3,A4)> {
    typedef R result_type;
};

template<typename Obj, typename R, typename A1, typename A2, typename A3, typename A4, typename A5>
struct return_type<R(Obj::*)(A1,A2,A3,A4,A5)> {
    typedef R result_type;
};

template<typename Obj, typename R, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6>
struct return_type<R(Obj::*)(A1,A2,A3,A4,A5,A6)> {
    typedef R result_type;
};


// support for const methods
template<typename Obj, typename R>
struct return_type<R(Obj::*)(void) const> {
    typedef R result_type;
};

template<typename Obj, typename R, typename A1>
struct return_type<R(Obj::*)(A1) const> {
    typedef R result_type;
};

template<typename Obj, typename R, typename A1, typename A2>
struct return_type<R(Obj::*)(A1,A2) const> {
    typedef R result_type;
};

template<typename Obj, typename R, typename A1, typename A2, typename A3>
struct return_type<R(Obj::*)(A1,A2,A3) const> {
    typedef R result_type;
};

template<typename Obj, typename R, typename A1, typename A2, typename A3, typename A4>
struct return_type<R(Obj::*)(A1,A2,A3,A4) const> {
    typedef R result_type;
};

template<typename Obj, typename R, typename A1, typename A2, typename A3, typename A4, typename A5>
struct return_type<R(Obj::*)(A1,A2,A3,A4,A5) const> {
    typedef R result_type;
};

template<typename Obj, typename R, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6>
struct return_type<R(Obj::*)(A1,A2,A3,A4,A5,A6) const> {
    typedef R result_type;
};


// Compile time If
template<bool b, typename T, typename F>
struct SelectIf { 
	typedef T type;
};

template<typename T, typename F>
struct SelectIf<false, T, F>  { 
	typedef F type;
};

//  Meta functions to detect is Type is a class, function_object, relation
template<typename T>
class is_class {
	typedef char one;
	typedef struct {char c[2];} two;

	template<typename C> static one test(int C::*);
	template<typename C> static two test(...);
public:
	enum { result = (sizeof(test<T>(0))==1) };
};


template <typename Type>
class is_function_obj  { 
	typedef char oneByte;
    class  twoBytes { char c[2]; }; 

   struct BaseMixin  { 
     bool operator()(void)
	 {} 
   }; 

   struct Base : public SelectIf<is_class<Type>::result,Type,None>::type, public BaseMixin 
   {}; 

   template <typename T, T t>  class Helper
   {}; 

   template <typename U> 
   static twoBytes  deduce(U*, Helper<bool (BaseMixin::*)(void),&U::operator()>*  = 0); 
   static oneByte   deduce(...);

public: 
	enum { result = sizeof( deduce((Base*)0) ) == 1 };

};

namespace misc {
	bool operator,(bool, None);
}


template <typename type>
class is_relation {
    class dummy { };
	typedef char one;
    typedef struct { char c[2]; } two; 

	struct derived : public type {
        using type::operator();
        dummy operator()(...) const ; 
    };
	
	static one  test(bool);
	static two  test(...);

    template <bool has, typename Dummy> // redundant type-parameter Dummy to ensure lazy instantiation of specialization
    struct impl {
        static const bool value = false;
    };
	
    template <typename Dummy> 
    struct impl<true, Dummy> {
		enum { value = sizeof( test( (((derived*)0)->operator()(), None()) ) ) == 1 };
    };

public:
	static const bool result = impl<is_function_obj<type>::result, dummy>::value;
};


template<typename L, typename R>
struct are_relations {
	enum { result = (is_relation<L>::result && is_relation<R>::result) };
};



// Any function with one argument

template<class R,class P>
struct Func1 {
	typedef R result_type;
	typedef P param_type;
private:
	struct callable {
        virtual ~callable(){}
        virtual callable* clone() const=0;
        virtual R operator()(P) =0;
    };

    template<class F>
    struct dispatcher : callable {
		F f;
        explicit dispatcher(const F& f_) : f(f_)
        { }
        virtual callable* clone() const {
            return new dispatcher<F>(this->f);
        }
        virtual R operator()(P p) {
            return this->f(p);
        }
    };

    std::auto_ptr<callable> fptr;

public:

    // Concept : F supports method... bool F::operator()(void)
    template<class F> 
    Func1(F f) : fptr(new dispatcher<F>(f)) { 
        typedef R (F::* boolMethod)(param_type);  // Static assertion to ensure F satisfies concept
    }

	Func1(const Func1& rhs) : fptr(rhs.fptr->clone())
    { }

    template<class F> 
	Func1& operator =(F f) {
		typedef R (F::* boolMethod)(param_type);  // Static assertion to ensure F satisfies concept
		fptr(new dispatcher<F>(f));
    }
	
	Func1& operator=(const Func1& rhs) {
        this->fptr.reset(rhs.fptr->clone());
        return *this;
    }

    R operator()(param_type p) {
      return (*this->fptr.get())(p); // we use get() instead of * on fptr since constructors guarantee fptr will be initialized
	}
};

} } // namespace castor::detail
#endif
