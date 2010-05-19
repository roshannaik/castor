// Castor : Logic Programming Library
// Copyright © 2008 Roshan Naik (roshan@mpprogramming.com).
// This software is goverened by the MIT license (http://www.opensource.org/licenses/mit-license.php).

#if !defined CASTOR_HELPERS_H
#define CASTOR_HELPERS_H 1

#include <iterator>
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

template<typename Itr>
struct Pointee {
    typedef typename Itr::value_type result_type;
};

template<typename T>
struct Pointee<T*> {
    typedef T result_type;
};

template<typename T>
struct Pointee<lref<T*> > {
    typedef T result_type;
};

template<typename Itr>
struct Pointee<lref<Itr> > {
    typedef typename Itr::value_type result_type;
};


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

} } // namespace castor::detail
#endif
