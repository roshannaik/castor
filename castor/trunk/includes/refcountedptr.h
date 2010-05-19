// Castor : Logic Programming Library
// Copyright © 2008 Roshan Naik (roshan@mpprogramming.com).
// This software is goverened by the MIT license (http://www.opensource.org/licenses/mit-license.php).

#ifndef CASTOR_REFCOUNTEDPTR_H
#define CASTOR_REFCOUNTEDPTR_H 1

namespace castor { namespace detail {

struct InvalidDeref{};
struct Underflow{};


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
// IsConvertibleStrict<T1,T2>::result==true  if T1 is convertible to T2, and T1!=T2
#ifdef __BCPLUSPLUS__
namespace ICS_detail {
// BCB workaround: sizeof trick doesnt directly work in BCB. call to overloaded test() 
// inside sizeof()produces compiler error 
// These members actually belong inside IsConvertibleStrict.
    class Big { char dummy[2]; };
    template<typename T1> T1 MakeT1();
    typedef char Small;
    template<typename T2> Small test(T2);
    template<typename T2> Big test(...);
};

// IsConvertibleStrict<T1,T2>::result==true  if T1 is convertible to T2, and T1!=T2
template <typename T1, typename T2>
class IsConvertibleStrict {
public:
	bool static const result = (sizeof(ICS_detail::Small)==sizeof( ICS_detail::test<T2>(ICS_detail::MakeT1<T1>()) ) )
				 && !SameType<T1,T2>::result ;
};
#else // __BCPLUSPLUS__
template <typename T1, typename T2>
class IsConvertibleStrict {
	static T1 MakeT1();
	typedef char Small;
    class Big { char dummy[2]; };
	static Small test(T2);
	static Big test(...);
public:
	enum { result = (sizeof(Small)==sizeof( test(MakeT1()) ) )
                 && !SameType<T1,T2>::result 
         };
};
#endif // __BCPLUSPLUS__

// IsDerivedStrict<T1,T2>::result==true  if T1 is derived from T2 and T1!=T2
template <typename D, typename B>
struct IsDerivedStrict {
	enum { result = IsConvertibleStrict<const D*, const B*>::result
                && !SameType<const D*, const void*>::result 
        };
};


// Class RefCountedPtr
//-------------------------------------------------------

// alloc_ should have been (as originally implemented in v0.8) a nested template inside RefCountedPtr<T>
// such that T in alloc<T>_ is same as T in RefCountedPtr. But BCB compiler couldnt handle it
template<typename T, bool allocateT2> struct alloc_;
template<typename T>
struct alloc_<T, true> {
    template<typename T2>
    static T* exec( const T2& value ) { return new typename RemoveConst<T2>::result_type(value); }
};

template<typename T>
struct alloc_<T, false> {
    template<typename T2>
    static T* exec( const T2& value ) {
//        typedef typename RemoveConst<T>::result_type NonConstT;
        return new T(value);
    }
};


template<typename T>
class RefCountedPtr {
protected:
    T* ptr_;
    unsigned long count_;
    bool reset_, exactType_;

    RefCountedPtr(const RefCountedPtr&);
    RefCountedPtr& operator =(const RefCountedPtr&);

    template<typename T2>
    static T* initPtr(const T2& value) {
        return ::castor::detail::alloc_<T, IsDerivedStrict<T2,T>::result> ::exec(value);
        //return new T2(value);
    }
public:
    typedef T pointee_type;
    RefCountedPtr() : ptr_(0), count_(1), reset_(true), exactType_(false)
    { }

    explicit RefCountedPtr(const T& value) : ptr_(new T(value)), count_(1), reset_(false), exactType_(true)
    { }

    template<typename T2>
    explicit RefCountedPtr(const T2& value) : ptr_(initPtr(value)), count_(1), reset_(false), exactType_(!IsDerivedStrict<T2,T>::result)
    { }

    ~RefCountedPtr() {
       if(/*count_==0 && */ptr_) delete ptr_; // we need to take count_ into account if set_ptr calls inc()
    }

    // Here we avoid invoking 'new' if possible for efficiency reasons
    // The new value is assigned to an existing T object pointed to by ptr_
    // If ptr_==0 then we invoke new.
    // TODO: how well do set(T) and set(X) mix
    void set(const T& value) {
        if(exactType_)
            *ptr_ = value;
        else {
            if(ptr_) delete ptr_;
            ptr_= new T(value);
        }            
        reset_=false;
        exactType_=true;
    }

    template<typename T2>
    void set(const T2& value) {
        if(ptr_)
            delete ptr_;
        ptr_= initPtr(value);
        reset_=false;
        exactType_=!IsDerivedStrict<T2,T>::result;
    }

    void reset() { // nothrow
        reset_=true;
    }

    T& operator*() const {
        if(reset_) throw InvalidDeref();
        return *ptr_;
    }

    T* operator->() const {
        if(reset_) throw InvalidDeref();
        return ptr_;
    }

    T* get() const {
        return ptr_;
    }

    void inc() { // nothrow
        ++count_; 
    }

    unsigned long dec() {
        if(count_==0)
            throw Underflow();
        return --count_;
    }

    unsigned long use_count() const { // nothrow
        return count_;
    }

    bool defined() const {  // nothrow
        return !reset_;
    }

    void swap(RefCountedPtr& other) { // no throw
        ::std::swap(ptr_,other.ptr_);
        ::std::swap(reset_,other.reset_);
        ::std::swap(exactType_,other.exactType_);
    }
}; // class RefCountedPtr


} } // namespace castor::detail

#endif
