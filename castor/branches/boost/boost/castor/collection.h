// Castor : Logic Programming Library
// Copyright © 2009 Roshan Naik (roshan@mpprogramming.com).
// This software is goverened by the MIT license (http://www.opensource.org/licenses/mit-license.php).

#if !defined CASTOR_COLLECTION_H
#define CASTOR_COLLECTION_H 1

#include <boost/castor/coroutine.h>
#include <boost/castor/relation.h>
#include <boost/castor/lref.h>
#include <boost/castor/utils.h>

#include <algorithm>
#include <vector>
#include <functional>

namespace castor {

namespace detail {
template<typename InItr>
bool equal_heap(InItr first_b, InItr first_e, InItr second_b, InItr second_e) {
	if( std::distance(first_b, first_e) != std::distance(second_b, second_e) )
		return false;
	while(first_b!=first_e) {
		if(*first_b!=*second_b)
			return false;
		std::pop_heap(first_b,first_e);
		std::pop_heap(second_b,second_e);
		--first_e; 
		--second_e;
	}
	return true;
}

template<typename InItr, typename Pred>
bool equal_heap(InItr first_b, InItr first_e, InItr second_b, InItr second_e, Pred cmp) {
	if( std::distance(first_b, first_e) != std::distance(second_b, second_e) )
		return false;
	while(first_b!=first_e) {
		if(!cmp(*first_b,*second_b))
			return false;
		std::pop_heap(first_b,first_e,cmp);
		std::pop_heap(second_b,second_e,cmp);
		--first_e; 
		--second_e;
	}
	return true;
}

template<typename InItr> 
bool equal_bags(InItr first_b, InItr first_e, InItr second_b, InItr second_e) {
	typedef typename std::iterator_traits<typename effective_type<InItr>::result_type>::value_type value_type;
	std::vector<value_type>  tmp1(first_b, first_e), tmp2(second_b, second_e);
	if(tmp1.size()!=tmp2.size())
		return false;
	std::make_heap(tmp1.begin(),tmp1.end());
	std::make_heap(tmp2.begin(),tmp2.end());
	return equal_heap(tmp1.begin(),tmp1.end(),tmp2.begin(),tmp2.end());
}

template<typename InItr, typename Pred>
bool equal_bags(InItr first_b, InItr first_e, InItr second_b, InItr second_e, Pred cmp) {
	typedef typename std::iterator_traits<typename effective_type<InItr>::result_type>::value_type value_type;
	std::vector<value_type>  tmp1(first_b, first_e), tmp2(second_b, second_e);
	if(tmp1.size()!=tmp2.size())
		return false;
	std::make_heap(tmp1.begin(),tmp1.end(), cmp);
	std::make_heap(tmp2.begin(),tmp2.end(), cmp);
	return equal_heap(tmp1.begin(),tmp1.end(),tmp2.begin(),tmp2.end(), cmp);
}

} // namespace detail

//-------------------------------------------------
// shuffle(seq_i,shuf) - shuffling (randomize) sequence seq_i, yields shuf
//-------------------------------------------------
template<typename Cont, typename InItr>
class Shuffle_r : public Coroutine {
	typedef typename std::iterator_traits<typename effective_type<InItr>::result_type>::value_type value_type;
	InItr begin_i, end_i;
	lref<Cont> shuf;

public:
	Shuffle_r(InItr begin_i, InItr end_i, const lref<Cont>& shuf) : begin_i(begin_i), end_i(end_i), shuf(shuf) 
	{ }

	bool operator()(void) {
		co_begin();
		if(shuf.defined())
			co_return( detail::equal_bags(effective_value(begin_i), effective_value(end_i), shuf->begin(), shuf->end()) );
		if(effective_value(begin_i)==effective_value(end_i))
			co_return(false);
		shuf.set_ptr(new Cont(effective_value(begin_i),effective_value(end_i)), true);
		while (true) {
			std::random_shuffle(shuf->begin(), shuf->end());
			co_yield(true);
		}
		shuf.reset();
		co_end();
	}
};

template<typename Cont, typename InItr>
Shuffle_r<Cont,InItr> shuffle(const InItr& begin_i, const InItr& end_i, const lref<Cont>& shuf) {
	return Shuffle_r<Cont,InItr>(begin_i,end_i,shuf);
}


template<typename Seq>
relation shuffle(lref<Seq>& seq_i, const lref<Seq>& shuf) {
	lref<typename Seq::iterator> b,e;
	return begin(seq_i,b) && end(seq_i,e) && shuffle(b,e,shuf);
}


//-------------------------------------------------
// permute(seq_i, p_seq) - permuting seq_i, yields p_seq
//-------------------------------------------------
template<typename Seq>
struct Permute_r : public Coroutine {
	lref<Seq> seq_i, p_seq;
	Permute_r(const lref<Seq>& seq_i, const lref<Seq>& p_seq) : seq_i(seq_i), p_seq(p_seq)
	{ }	

	bool operator()(void) {
		co_begin();
		if(p_seq.defined()) {
			co_return( detail::equal_bags( seq_i->begin(), seq_i->end(), p_seq->begin(), p_seq->end() ) );
		}
		p_seq=seq_i;
		while ( std::next_permutation(p_seq->begin(), p_seq->end()) )
			co_yield(true);
		p_seq=seq_i;
		co_yield(true);
		while ( std::prev_permutation(p_seq->begin(), p_seq->end()) )
			co_yield(true);
		p_seq.reset();
		co_end();
	}
};

template<typename Seq, typename Pred = std::less<typename Seq::value_type> >
struct PermutePred_r : public Coroutine {
	lref<Seq> seq_i, p_seq;
	Pred cmp;

	PermutePred_r(const lref<Seq>& seq_i, const lref<Seq>& p_seq, Pred cmp) : seq_i(seq_i), p_seq(p_seq), cmp(cmp)
	{ }	

	bool operator()(void) {
		co_begin();
		if(p_seq.defined()) {
			co_return( detail::equal_bags( seq_i->begin(), seq_i->end(), p_seq->begin(), p_seq->end(), cmp ) );
		}
		p_seq=seq_i;
		while ( std::next_permutation(p_seq->begin(), p_seq->end(), cmp) )
			co_yield(true);
		p_seq=seq_i;
		co_yield(true);
		while ( std::prev_permutation(p_seq->begin(), p_seq->end(), cmp) )
			co_yield(true);
		p_seq.reset();
		co_end();
	}
};

template<typename Seq>
Permute_r<Seq> permute(lref<Seq>& seq_i, const lref<Seq>& p_seq) {
	return Permute_r<Seq>(seq_i,p_seq);
}

template<typename Seq, typename Pred>
PermutePred_r<Seq,Pred> permute(lref<Seq>& seq_i, const lref<Seq>& p_seq, Pred cmp) {
	return PermutePred_r<Seq,Pred>(seq_i,p_seq,cmp);
}


} // namespace castor

#endif
