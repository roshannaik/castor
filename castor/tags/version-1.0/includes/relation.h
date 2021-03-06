// Castor : Logic Programming Library
// Copyright � 2008 Roshan Naik (roshan@mpprogramming.com).
// This software is goverened by the MIT license (http://www.opensource.org/licenses/mit-license.php).

#if !defined CASTOR_RELATION_H
#define CASTOR_RELATION_H 1
#include <memory>
#include "workaround.h"
namespace castor {

 struct InvalidState{}; // Exception type

//---------------------------------------------------------------
// TestOnlyRelation : Helper for creating custom relations that
//                    do not produce any side effects
//---------------------------------------------------------------
template<typename Derived>
class TestOnlyRelation {
    bool done;
public:
    TestOnlyRelation() : done(false) 
    { }

    bool operator()(void) {
        if(done)
            return false;
        done=true;
        return (static_cast<Derived*>(this))->apply();
    }
};


//---------------------------------------------------------------
// OneSolutionRelation : Helper for creating custom relations that
//                       succeed only once
//---------------------------------------------------------------
template<typename Derived>
class OneSolutionRelation {
 enum { START, SUCCESS, END } state;
public:
    OneSolutionRelation() : state(START)
    { }

    bool operator() (void) {
        switch(state) {
        case START:
            if(static_cast<Derived*>(this)->apply()) {
                state=SUCCESS;
                return true;
            }
            state=END;
            return false;
        case SUCCESS:
            static_cast<Derived*>(this)->revert();
            state=END;
            return false;
        default: // case END
            return false;
        }
    }
};

//---------------------------------------------------------------
//    False Relation : Always generates 'false'
//---------------------------------------------------------------

struct False : TestOnlyRelation<False> {
    bool apply(void) const {
        return false;
    }
};

//---------------------------------------------------------------
//    True Relation : Generates 'true' once
//---------------------------------------------------------------
struct True : TestOnlyRelation<True> {
    bool apply(void) const {
        return true;
    }
};

//---------------------------------------------------------------
//    Boolean Relation : Produces the boolean value with which it was initialized
//---------------------------------------------------------------
struct Boolean : public TestOnlyRelation<Boolean> {
private : 
    bool result;
public  :
    Boolean(bool value) : result(value)
    { }

    bool apply (void) const {
        return result;
    }
};

//---------------------------------------------------------------
//    relation 
//---------------------------------------------------------------
template <typename Expr>
class Ile;

class relation {
    struct impl {
        virtual ~impl(){}
        virtual impl* clone() const=0;
        virtual bool operator()(void)=0;
    };

    template<class F>
    struct wrapper : public impl {
        explicit wrapper(const F& f_) : f(f_)
        { }
        virtual impl* clone() const {
            return new wrapper<F>(this->f);
        }
        virtual bool operator()(void) {
            return this->f();
        }
    private:
        F f;
    };

    std::auto_ptr<impl> pimpl;

    template<class Expr>  // disable instantiating relation from ILE
    relation(const Ile<Expr>& rhs);
public:
    typedef bool result_type;

    // Concept : F supports method... bool F::operator()(void)
    template<class F> 
    relation(F f) : pimpl(new wrapper<F>(f)) { 
        typedef bool (F::* boolMethod)(void);  // Compiler Error : f must suport method bool F::operator()(void)
    }

    relation(const relation& rhs) : pimpl(rhs.pimpl->clone())
    { }

    relation& operator=(const relation& rhs) {
        this->pimpl.reset(rhs.pimpl->clone());
        return *this;
    }

    bool operator()(void) const {
        return (*this->pimpl)();
	}
};


//---------------------------------------------------------------
//    Dyanmic relation support classes : Conjunctions, Disjunctions, ExDisjunctions
//---------------------------------------------------------------

class Conjunctions {
    relation clauses;
    unsigned int size;
public:
    Conjunctions() : clauses(False()), size(0)
    { }

	template <typename Rel>
	void push_back(const Rel& r) {
		if(size==0)
			clauses = r;
		else
			clauses = (clauses && r);
		++size;
	}

	template <typename Rel>
	void push_front(const Rel& r) {
		if(size==0)
			clauses = r;
		else
			clauses = (r && clauses);
		++size;
	}

	bool operator ()(void) {
		return clauses();
	}
};

class Disjunctions {
	relation clauses;
	unsigned int size;
public:
	Disjunctions() : clauses(False()), size(0)
	{ }

	template <typename Rel>
	void push_back(const Rel& r) {
		if(size==0)
			clauses = r;
		else
			clauses = (clauses || r);
		++size;
	}

	template <typename Rel>
	void push_front(const Rel& r) {
		if(size==0)
			clauses = r;
		else
			clauses = (r || clauses);
		++size;
	}

	bool operator ()(void) {
		return clauses();
	}
};

class ExDisjunctions {
	relation clauses;
	unsigned int size;
public:
	ExDisjunctions() : clauses(False()), size(0)
	{ }

	template <typename Rel>
	void push_back(const Rel& r) {
		if(size==0)
			clauses = r;
		else
			clauses = (clauses ^ r);
		++size;
	}

	template <typename Rel>
	void push_front(const Rel& r) {
		if(size==0)
			clauses = r;
		else
			clauses = (r ^ clauses);
		++size;
	}

    bool operator ()(void) {
        return clauses();
    }
};

} // namespace castor
#endif
