// Castor : Logic Programming Library
// Copyright © 2009 Roshan Naik (roshan@mpprogramming.com).
// This software is goverened by the MIT license (http://www.opensource.org/licenses/mit-license.php).

#if !defined CASTOR_COLLECTION_H
#define CASTOR_COLLECTION_H 1

//#include "coroutine.h"
#include "relation.h"
#include "lref.h"
#include "utils.h"
#include <algorithm>

namespace castor {


//-------------------------------------------------
// shuffle(seq_i,shuf) - shuffling (randomize) sequence seq_i, yields shuf
//-------------------------------------------------
template<typename Cont, typename InItr>
class Shuffle_r : public Coroutine {
	typedef typename std::iterator_traits<typename effective_type<InItr>::result_type>::value_type value_type;
	lref<Cont> shuf;
	InItr begin_i, end_i;

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
