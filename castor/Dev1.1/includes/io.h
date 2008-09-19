// Castor : Logic Programming Library for C++
// Copyright � 2007 Roshan Naik (naikrosh@gmail.com)

#if !defined CASTOR_IO_H
#define CASTOR_IO_H 1

#include <iostream>
#include <string>

namespace castor {

//--------------------------------------------------------
//  Write relation : Prints lrefs and POTs to std::cout
//--------------------------------------------------------
template<typename T>
class Write_r : public TestOnlyRelation<Write_r<T> > {
    T val;
    std::ostream* out_;
public:
    explicit Write_r(const T& obj_, std::ostream& outputStream=std::cout) : val(obj_), out_(&outputStream)
    { }
    
    bool apply(void) {
        *out_ << ::castor::effective_value(val);
        return true;
    }
};

// Concepts : T is lref or POT
template <typename T> inline
Write_r<T> write(const T& obj_) {
    return Write_r<T>(obj_, std::cout);
}

inline
Write_r<std::string> write(const char* obj_) {
    return Write_r<std::string>(obj_, std::cout);
}

//--------------------------------------------------------
//  WriteAll relation : Prints lrefs and POTs to std::cout
//--------------------------------------------------------
template<typename Itr>
class WriteAll_r : public TestOnlyRelation<WriteAll_r<Itr> > {
    Itr itr, end_;
    std::string sep;
    std::ostream* out;
public:
    explicit WriteAll_r(Itr beg_, Itr end_, const std::string& sep, std::ostream& outputStream=std::cout) : itr(beg_), end_(end_), sep(sep), out(&outputStream)
    { }
    
    bool apply(void) {
      for(Itr first = effective_value(itr); effective_value(itr)!=effective_value(end_); ++effective_value(itr) ) {
          if( effective_value(itr)!=effective_value(first) )
            *out << sep;
          *out << *effective_value(itr);
      }
      return true;
    }
};

// Concepts : Itr is an iterator, pointer, or lref<Iterator> or lref<pointer>
template<typename Itr> inline
WriteAll_r<Itr> writeAll(Itr begin_, Itr end_, const std::string& separator=" ") {
    return WriteAll_r<Itr>(begin_, end_, separator);
}

template<typename Cont> inline
relation writeAll(lref<Cont>& cont_, const std::string& separator=" ") {
  typedef typename Cont::iterator Itr;
  lref<Itr> b, e;
  return begin(cont_,b) && end(cont_,e) && WriteAll_r<lref<Itr> >(b, e, separator);
}

template<typename Itr> inline
WriteAll_r<Itr> writeAllTo(Itr begin_, Itr end_, std::ostream& outputStrm, const std::string& separator=" ") {
    return WriteAll_r<Itr>(begin_, end_, separator, outputStrm);
}

template<typename Cont> inline
relation writeAllTo(lref<Cont>& cont_, std::ostream& outputStrm, const std::string& separator=" ") {
  typedef typename Cont::iterator Itr;
  lref<Itr> b, e;
  return begin(cont_,b) && end(cont_,e) && WriteAll_r<lref<Itr> >(b, e, separator, outputStrm);
}


// -----------------------------------------------------------------
// writeTo relation : Prints lrefs and POTs to specified outputStream
// -----------------------------------------------------------------
template <typename T> inline
Write_r<T> writeTo(std::ostream& outputStrm, const T& obj_) {
    return Write_r<T>(obj_, outputStrm);
}

inline
Write_r<std::string> writeTo(std::ostream& outputStrm, const char* obj_) {
    return Write_r<std::string>(obj_, outputStrm);
}


// -----------------------------------------------------------------
// write_f relation
// -----------------------------------------------------------------

template<typename OP>
class WriteF_r : public TestOnlyRelation<WriteF_r<OP> > {
    OP func;
    std::ostream* outputStrm;
public:
    WriteF_r (const OP& func, std::ostream& outputStrm=std::cout) : func(func), outputStrm(&outputStrm)
    { }

    bool apply (void) {
        return ( *outputStrm << func() ) ? true : false;
    }
};

template<typename OP1, typename A1>
class WriteF1_r : public TestOnlyRelation<WriteF1_r<OP1, A1> > {
    A1 arg1;
    OP1 func;
    std::ostream* outputStrm;
public:
    WriteF1_r (const OP1& func, const A1& arg1, std::ostream* outputStrm=std::cout) : func(func), arg1(arg1), outputStrm(&outputStrm)
    { }

    bool apply (void) {
        return ( *outputStrm << func(effective_value(arg1)) ) ? true : false;
    }
};

template<typename OP2, typename A1, typename A2>
class WriteF2_r : public TestOnlyRelation<WriteF2_r<OP2, A1, A2> > {
    A1 arg1; A2 arg2;
    OP2 func;
    std::ostream* outputStrm;
public:
    WriteF2_r (const OP2& func, const A1& arg1, const A2& arg2, std::ostream& outputStrm=std::cout) : func(func), arg1(arg1), arg2(arg2), outputStrm(&outputStrm)
    { }

    bool apply (void) {
        return ( *outputStrm << func(effective_value(arg1), effective_value(arg2)) ) ? true : false;
    }
};


template<typename OP3, typename A1, typename A2, typename A3>
class WriteF3_r : public TestOnlyRelation<WriteF3_r<OP3, A1, A2, A3> > {
    A1 arg1; A2 arg2; A3 arg3;
    OP3 func;
    std::ostream* outputStrm;
public:
    WriteF3_r (const OP3& func, const A1& arg1, const A2& arg2, const A3& arg3, std::ostream& outputStrm=std::cout) : func(func), arg1(arg1), arg2(arg2), arg3(arg3), outputStrm(&outputStrm)
    { }

    bool apply (void) {
        return ( *outputStrm << func(effective_value(arg1), effective_value(arg2), effective_value(arg3)) ) ? true : false;
    }
};

template<typename OP4, typename A1, typename A2, typename A3, typename A4>
class WriteF4_r : public TestOnlyRelation<WriteF4_r<OP4, A1, A2, A3, A4> > {
    A1 arg1; A2 arg2; A3 arg3; A4 arg4;
    OP4 func;
    std::ostream* outputStrm;
public:
    WriteF4_r (const OP4& func, const A1& arg1, const A2& arg2, const A3& arg3, const A4& arg4, std::ostream& outputStrm=std::cout) : func(func), arg1(arg1), arg2(arg2), arg3(arg3), arg4(arg4), outputStrm(&outputStrm)
    { }

    bool apply (void) {
        return ( *outputStrm << func(effective_value(arg1), effective_value(arg2), effective_value(arg3), effective_value(arg4)) ) ? true : false;
    }
};


template<typename OP5, typename A1, typename A2, typename A3, typename A4, typename A5>
class WriteF5_r : public TestOnlyRelation<WriteF5_r<OP5, A1, A2, A3, A4, A5> > {
    A1 arg1; A2 arg2; A3 arg3; A4 arg4; A5 arg5;
    OP5 func;
    std::ostream* outputStrm;
public:
    WriteF5_r (const OP5& func, const A1& arg1, const A2& arg2, const A3& arg3, const A4& arg4, const A5& arg5, std::ostream& outputStrm=std::cout) : func(func), arg1(arg1), arg2(arg2), arg3(arg3), arg4(arg4), arg5(arg5), outputStrm(&outputStrm)
    { }

    bool apply (void) {
        return ( *outputStrm << func(effective_value(arg1), effective_value(arg2), effective_value(arg3), effective_value(arg4), effective_value(arg5)) ) ? true : false;
    }
};

template<typename OP6, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6>
class WriteF6_r : public TestOnlyRelation<WriteF6_r<OP6, A1, A2, A3, A4, A5, A6> > {
    A1 arg1; A2 arg2; A3 arg3; A4 arg4; A5 arg5; A6 arg6;
    OP6 func;
    std::ostream* outputStrm;
public:
    WriteF6_r (const OP6& func, const A1& arg1, const A2& arg2, const A3& arg3, const A4& arg4, const A5& arg5, const A6& arg6, std::ostream& outputStrm=std::cout) : func(func), arg1(arg1), arg2(arg2), arg3(arg3), arg4(arg4), arg5(arg5), arg6(arg6), outputStrm(&outputStrm)
    { }

    bool apply (void) {
        return ( *outputStrm << func(effective_value(arg1), effective_value(arg2), effective_value(arg3), effective_value(arg4), effective_value(arg5), effective_value(arg6)) ) ? true : false;
    }
};


template<typename Func> inline
WriteF_r<Func> write_f(Func f) {
    return WriteF_r<Func>(f,std::cout);
}

template<typename Func1, typename A1> inline
WriteF1_r<Func1, A1> write_f( Func1 f, const A1& a1_) {
    return WriteF1_r<Func1, A1>(f, a1_, std::cout);
}

template<typename Func2, typename A1, typename A2> inline
WriteF2_r<Func2, A1, A2> write_f( Func2 f, const A1& a1_, const A2& a2_) {
    return WriteF2_r<Func2, A1, A2>(f, a1_, a2_, std::cout);
}

template<typename Func3, typename A1, typename A2, typename A3> inline
WriteF3_r<Func3, A1, A2, A3> write_f( Func3 f, const A1& a1_, const A2& a2_, const A3& a3_) {
    return WriteF3_r<Func3, A1, A2, A3>(f, a1_, a2_, a3_, std::cout);
}

template<typename Func4, typename A1, typename A2, typename A3, typename A4> inline
WriteF4_r<Func4, A1, A2, A3, A4> write_f( Func4 f, const A1& a1_, const A2& a2_, const A3& a3_, const A4& a4_) {
    return WriteF4_r<Func4, A1, A2, A3, A4>(f, a1_, a2_, a3_, a4_,std::cout);
}

template<typename Func5, typename A1, typename A2, typename A3, typename A4, typename A5> inline
WriteF5_r<Func5, A1, A2, A3, A4, A5> write_f( Func5 f, const A1& a1_, const A2& a2_, const A3& a3_, const A4& a4_, const A5& a5_) {
    return WriteF5_r<Func5, A1, A2, A3, A4, A5>(f, a1_, a2_, a3_, a4_, a5_, std::cout);
}

template<typename Func6, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6> inline
WriteF6_r<Func6, A1, A2, A3, A4, A5, A6> write_f( Func6 f, const A1& a1_, const A2& a2_, const A3& a3_, const A4& a4_, const A5& a5_, const A6& a6_) {
    return WriteF6_r<Func6, A1, A2, A3, A4, A5, A6>(f, a1_, a2_, a3_, a4_, a5_, a6_, std::cout);
}

// -----------------------------------------------------------------
// writeTo_f relation
// -----------------------------------------------------------------
template<typename Func> inline
WriteF_r<Func> writeTo_f(std::ostream& outputStrm,Func f) {
    return WriteF_r<Func>(f,outputStrm);
}

template<typename Func1, typename A1> inline
WriteF1_r<Func1, A1> writeTo_f(std::ostream& outputStrm, Func1 f, const A1& a1_) {
    return WriteF1_r<Func1, A1>(f, a1_, outputStrm);
}

template<typename Func2, typename A1, typename A2> inline
WriteF2_r<Func2, A1, A2> writeTo_f(std::ostream& outputStrm, Func2 f, const A1& a1_, const A2& a2_) {
    return WriteF2_r<Func2, A1, A2>(f, a1_, a2_, outputStrm);
}

template<typename Func3, typename A1, typename A2, typename A3> inline
WriteF3_r<Func3, A1, A2, A3> writeTo_f(std::ostream& outputStrm, Func3 f, const A1& a1_, const A2& a2_, const A3& a3_) {
    return WriteF3_r<Func3, A1, A2, A3>(f, a1_, a2_, a3_, outputStrm);
}

template<typename Func4, typename A1, typename A2, typename A3, typename A4> inline
WriteF4_r<Func4, A1, A2, A3, A4> writeTo_f(std::ostream& outputStrm, Func4 f, const A1& a1_, const A2& a2_, const A3& a3_, const A4& a4_) {
    return WriteF4_r<Func4, A1, A2, A3, A4>(f, a1_, a2_, a3_, a4_,outputStrm);
}

template<typename Func5, typename A1, typename A2, typename A3, typename A4, typename A5> inline
WriteF5_r<Func5, A1, A2, A3, A4, A5> writeTo_f(std::ostream& outputStrm, Func5 f, const A1& a1_, const A2& a2_, const A3& a3_, const A4& a4_, const A5& a5_) {
    return WriteF5_r<Func5, A1, A2, A3, A4, A5>(f, a1_, a2_, a3_, a4_, a5_, outputStrm);
}

template<typename Func6, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6> inline
WriteF6_r<Func6, A1, A2, A3, A4, A5, A6> writeTo_f(std::ostream& outputStrm, Func6 f, const A1& a1_, const A2& a2_, const A3& a3_, const A4& a4_, const A5& a5_, const A6& a6_) {
    return WriteF6_r<Func6, A1, A2, A3, A4, A5, A6>(f, a1_, a2_, a3_, a4_, a5_, a6_, outputStrm);
}



// -----------------------------------------------------------------
// writeTo_mf relation
// -----------------------------------------------------------------

template<typename Obj, typename MemFunT>
class WriteMF_r : public TestOnlyRelation<WriteMF_r<Obj,MemFunT> > {
	lref<Obj> obj_;
    MemFunT mf;
    std::ostream* outputStrm;
public:
    WriteMF_r(const lref<Obj>& obj_, MemFunT mf, std::ostream& outputStrm=std::cout) : obj_(obj_), mf(mf), outputStrm(&outputStrm)
	{ }

    bool apply (void) {
        return ( *outputStrm << ((*obj_).*mf)() ) ? true : false;
	}
};


template<typename Obj, typename MemFunT, typename A1>
class WriteMF1_r : public TestOnlyRelation<WriteMF1_r<Obj,MemFunT,A1> > {
	lref<Obj> obj_;
    MemFunT mf;
	A1 a1_;
    std::ostream* outputStrm;
public:
    WriteMF1_r(const lref<Obj>& obj_, MemFunT mf, const A1& a1_, std::ostream& outputStrm=std::cout) : obj_(obj_), mf(mf), a1_(a1_), outputStrm(&outputStrm)
	{ }

    bool apply (void) {
        return ( *outputStrm << ((*obj_).*mf)(effective_value(a1_)) ) ? true : false;
	}
};


template<typename Obj, typename MemFunT, typename A1, typename A2>
class WriteMF2_r : public TestOnlyRelation<WriteMF2_r<Obj,MemFunT,A1,A2> > {
	lref<Obj> obj_;
    MemFunT mf;
	A1 a1_; A2 a2_;
    std::ostream* outputStrm;
public:
    WriteMF2_r(const lref<Obj>& obj_, MemFunT mf, const A1& a1_, const A2& a2_, std::ostream& outputStrm=std::cout) : obj_(obj_), mf(mf), a1_(a1_), a2_(a2_), outputStrm(&outputStrm)
	{ }

    bool apply (void) {
        return ( *outputStrm << ((*obj_).*mf)(effective_value(a1_),effective_value(a2_)) ) ? true : false;
	}
};


template<typename Obj, typename MemFunT, typename A1, typename A2, typename A3>
class WriteMF3_r : public TestOnlyRelation<WriteMF3_r<Obj,MemFunT,A1,A2,A3> > {
	lref<Obj> obj_;
    MemFunT mf;
	A1 a1_; A2 a2_; A3 a3_;
    std::ostream* outputStrm;
public:
    WriteMF3_r(const lref<Obj>& obj_, MemFunT mf, const A1& a1_, const A2& a2_, const A3& a3_, std::ostream& outputStrm=std::cout) : obj_(obj_), mf(mf), a1_(a1_), a2_(a2_), a3_(a3_), outputStrm(&outputStrm)
	{ }

    bool apply (void) {
        return ( *outputStrm << ((*obj_).*mf)(effective_value(a1_),effective_value(a2_),effective_value(a3_)) ) ? true : false;
	}
};


template<typename Obj, typename MemFunT, typename A1, typename A2, typename A3, typename A4>
class WriteMF4_r : public TestOnlyRelation<WriteMF4_r<Obj,MemFunT,A1,A2,A3,A4> > {
	lref<Obj> obj_;
    MemFunT mf;
	A1 a1_; A2 a2_; A3 a3_; A4 a4_;
    std::ostream* outputStrm;
public:
    WriteMF4_r(const lref<Obj>& obj_, MemFunT mf, const A1& a1_, const A2& a2_, const A3& a3_, const A4& a4_, std::ostream& outputStrm=std::cout) : obj_(obj_), mf(mf), a1_(a1_), a2_(a2_), a3_(a3_), a4_(a4_), outputStrm(&outputStrm)
	{ }

    bool apply (void) {
        return ( *outputStrm << ((*obj_).*mf)(effective_value(a1_),effective_value(a2_),effective_value(a3_),effective_value(a4_)) ) ? true : false;
	}
};


template<typename Obj, typename MemFunT, typename A1, typename A2, typename A3, typename A4, typename A5>
class WriteMF5_r : public TestOnlyRelation<WriteMF5_r<Obj,MemFunT,A1,A2,A3,A4,A5> > {
	lref<Obj> obj_;
    MemFunT mf;
	A1 a1_; A2 a2_; A3 a3_; A4 a4_; A5 a5_;
    std::ostream* outputStrm;
public:
    WriteMF5_r(const lref<Obj>& obj_, MemFunT mf, const A1& a1_, const A2& a2_, const A3& a3_, const A4& a4_, const A5& a5_, std::ostream& outputStrm=std::cout) : obj_(obj_), mf(mf), a1_(a1_), a2_(a2_), a3_(a3_), a4_(a4_), a5_(a5_), outputStrm(&outputStrm)
	{ }

    bool apply (void) {
        return ( *outputStrm << ((*obj_).*mf)(effective_value(a1_),effective_value(a2_),effective_value(a3_),effective_value(a4_),effective_value(a5_)) ) ? true : false;
	}
};


template<typename Obj, typename MemFunT, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6>
class WriteMF6_r : public TestOnlyRelation<WriteMF6_r<Obj,MemFunT,A1,A2,A3,A4,A5,A6> > {
	lref<Obj> obj_;
    MemFunT mf;
	A1 a1_; A2 a2_; A3 a3_; A4 a4_; A5 a5_; A6 a6_;
    std::ostream* outputStrm;
public:
    WriteMF6_r(const lref<Obj>& obj_, MemFunT mf, const A1& a1_, const A2& a2_, const A3& a3_, const A4& a4_, const A5& a5_, const A6& a6_, std::ostream& outputStrm=std::cout) : obj_(obj_), mf(mf), a1_(a1_), a2_(a2_), a3_(a3_), a4_(a4_), a5_(a5_), a6_(a6_), outputStrm(&outputStrm)
	{ }

    bool apply (void) {
        return ( *outputStrm << ((*obj_).*mf)(effective_value(a1_),effective_value(a2_),effective_value(a3_),effective_value(a4_),effective_value(a5_),effective_value(a6_)) ) ? true : false;
	}
};


template<typename Obj, typename MemFunT> inline
WriteMF_r<Obj, MemFunT> writeTo_mf(std::ostream& outputStrm, lref<Obj>& obj_, MemFunT mf) {
    return WriteMF_r<Obj, MemFunT>(obj_, mf, outputStrm);
}

template<typename Obj, typename MemFunT, typename A1> inline
WriteMF1_r<Obj, MemFunT, A1> writeTo_mf(std::ostream& outputStrm, lref<Obj>& obj_, MemFunT mf, const A1& a1_) {
    return WriteMF1_r<Obj, MemFunT, A1>(obj_, mf, a1_, outputStrm);
}

template<typename Obj, typename MemFunT, typename A1, typename A2> inline
WriteMF2_r<Obj, MemFunT, A1, A2> writeTo_mf(std::ostream& outputStrm, lref<Obj>& obj_, MemFunT mf, const A1& a1_, const A2& a2_) {
    return WriteMF2_r<Obj, MemFunT, A1, A2>(obj_, mf, a1_, a2_, outputStrm);
}

template<typename Obj, typename MemFunT, typename A1, typename A2, typename A3> inline
WriteMF3_r<Obj, MemFunT, A1, A2, A3> writeTo_mf(std::ostream& outputStrm, lref<Obj> obj_, MemFunT mf, const A1& a1_, const A2& a2_, const A3& a3_) {
    return WriteMF3_r<Obj, MemFunT, A1, A2, A3>(obj_, mf, a1_, a2_, a3_, outputStrm);
}

template<typename Obj, typename MemFunT, typename A1, typename A2, typename A3, typename A4> inline
WriteMF4_r<Obj, MemFunT, A1, A2, A3, A4> writeTo_mf(std::ostream& outputStrm, lref<Obj>& obj_, MemFunT mf, const A1& a1_, const A2& a2_, const A3& a3_, const A4& a4_) {
    return WriteMF4_r<Obj, MemFunT, A1, A2, A3, A4>(obj_, mf, a1_, a2_, a3_, a4_, outputStrm);
}

template<typename Obj, typename MemFunT, typename A1, typename A2, typename A3, typename A4, typename A5> inline
WriteMF5_r<Obj, MemFunT, A1, A2, A3, A4, A5> writeTo_mf(std::ostream& outputStrm, lref<Obj>& obj_, MemFunT mf, const A1& a1_, const A2& a2_, const A3& a3_, const A4& a4_, const A5& a5_) {
    return WriteMF5_r<Obj, MemFunT, A1, A2, A3, A4, A5>(obj_, mf, a1_, a2_, a3_, a4_, a5_, outputStrm);
}

template<typename Obj, typename MemFunT, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6> inline
WriteMF6_r<Obj, MemFunT, A1, A2, A3, A4, A5, A6> writeTo_mf(std::ostream& outputStrm, lref<Obj>& obj_, MemFunT mf, const A1& a1_, const A2& a2_, const A3& a3_, const A4& a4_, const A5& a5_, const A6& a6_) {
    return WriteMF6_r<Obj, MemFunT, A1, A2, A3, A4, A5, A6>(obj_, mf, a1_, a2_, a3_, a4_, a5_, a6_, outputStrm);
}



//--------------------------------------------------------
// write_mf relation
//--------------------------------------------------------
template<typename Obj, typename MemFunT> inline
WriteMF_r<Obj, MemFunT> write_mf(lref<Obj>& obj_, MemFunT mf) {
    return WriteMF_r<Obj, MemFunT>(obj_, mf, std::cout);
}

template<typename Obj, typename MemFunT, typename A1> inline
WriteMF1_r<Obj, MemFunT, A1> write_mf(lref<Obj>& obj_, MemFunT mf, const A1& a1_) {
    return WriteMF1_r<Obj, MemFunT, A1>(obj_, mf, a1_, std::cout);
}

template<typename Obj, typename MemFunT, typename A1, typename A2> inline
WriteMF2_r<Obj, MemFunT, A1, A2> write_mf(lref<Obj>& obj_, MemFunT mf, const A1& a1_, const A2& a2_) {
    return WriteMF2_r<Obj, MemFunT, A1, A2>(obj_, mf, a1_, a2_, std::cout);
}

template<typename Obj, typename MemFunT, typename A1, typename A2, typename A3> inline
WriteMF3_r<Obj, MemFunT, A1, A2, A3> write_mf(lref<Obj> obj_, MemFunT mf, const A1& a1_, const A2& a2_, const A3& a3_) {
    return WriteMF3_r<Obj, MemFunT, A1, A2, A3>(obj_, mf, a1_, a2_, a3_, std::cout);
}

template<typename Obj, typename MemFunT, typename A1, typename A2, typename A3, typename A4> inline
WriteMF4_r<Obj, MemFunT, A1, A2, A3, A4> write_mf(lref<Obj>& obj_, MemFunT mf, const A1& a1_, const A2& a2_, const A3& a3_, const A4& a4_) {
    return WriteMF4_r<Obj, MemFunT, A1, A2, A3, A4>(obj_, mf, a1_, a2_, a3_, a4_, std::cout);
}

template<typename Obj, typename MemFunT, typename A1, typename A2, typename A3, typename A4, typename A5> inline
WriteMF5_r<Obj, MemFunT, A1, A2, A3, A4, A5> write_mf(lref<Obj>& obj_, MemFunT mf, const A1& a1_, const A2& a2_, const A3& a3_, const A4& a4_, const A5& a5_) {
    return WriteMF5_r<Obj, MemFunT, A1, A2, A3, A4, A5>(obj_, mf, a1_, a2_, a3_, a4_, a5_, std::cout);
}

template<typename Obj, typename MemFunT, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6> inline
WriteMF6_r<Obj, MemFunT, A1, A2, A3, A4, A5, A6> write_mf(lref<Obj>& obj_, MemFunT mf, const A1& a1_, const A2& a2_, const A3& a3_, const A4& a4_, A5& a5_, A6& a6_) {
    return WriteMF6_r<Obj, MemFunT, A1, A2, A3, A4, A5, A6>(obj_, mf, a1_, a2_, a3_, a4_, a5_, a6_, std::cout);
}



//--------------------------------------------------------
//  Read relation : reads from intputstream (default=cin)
//--------------------------------------------------------

// Concepts: T supports operator==(const T&, const T&), Copy Assignment and Default construction
template<typename T>
class Read_r : public custom_relation {
  lref<T> val;
  std::istream* in;
  T tmp;
public:
  Read_r(const lref<T>& obj, std::istream& inputStream=std::cin) : val(obj), in(&inputStream), tmp()
  { }

  bool operator()(void) {
    rel_begin();
    while(*in >> tmp) {
      if(!val.defined()) {
        val=tmp;
        rel_yield(true);
        val.reset();
      }
      else 
        rel_return(*val==tmp);
    }
    rel_end();
  }
};


//--------------------------------------------------------
//  read relation : reads values from an std::cin
//--------------------------------------------------------
template <typename T> inline
Read_r<T> read(lref<T> val) {
    return Read_r<T>(val,std::cin); 
}

inline
Read_r<std::string> read(const char* val) {
  lref<std::string> str(val);
  return Read_r<std::string>(str,std::cin);
}


//--------------------------------------------------------
//  readFrom relation : reads values from an istream
//--------------------------------------------------------
template <typename T> inline
Read_r<T> readFrom(std::istream& inputStream, lref<T> val) {
    return Read_r<T>(val,inputStream); 
}

inline
Read_r<std::string> readFrom(std::istream& inputStream, const char* val) {
    lref<std::string> str(val);
    return Read_r<std::string>(str,inputStream);
}


} // namespace castor
#endif