#include <boost/castor.h>

#include <boost/test/minimal.hpp>

#include <sstream>
#include <vector>


using namespace castor;

namespace {
struct Base { 
    int b; 
    Base() : b(0) { }
};
struct Derived : Base { 
    int d; 
    Derived() : d(1), Base() { }
};

} // namespace 

int getN(int, int, int, int, int, int) { return 6; }

struct num {
    int i;

    num(int i) : i(i) {}

    num(const num& n) : i(n.i) {}

    num operator+ (const num& r) const {
        return num(i + r.i);
    }

    static num static_compute(num i, num j, num k) {
        return num(i.i + j.i / k.i);
    }

    num nonstatic_compute(num i, num j, num k) {
        return num(i.i + j.i / k.i);
    }
};

num operator*(num l, num r) {
    return num(l.i * r.i);
}

std::ostream& operator<< (std::ostream &o, const num& n) {
    return o << n.i;
}

struct Functor6 {
    typedef int result_type;

    static int result;

    int operator() (void)                                               { return 0; }
    int operator() (int i1)                                             { return i1; }
    int operator() (int i1, int i2)                                     { return i1 + i2; }
    int operator() (int i1, int i2, int i3)                             { return i1 + i2 + i3; }
    int operator() (int i1, int i2, int i3, int i4)                     { return i1 + i2 + i3 + i4; }
    int operator() (int i1, int i2, int i3, int i4, int i5)             { return i1 + i2 + i3 + i4 + i5; }
    int operator() (int i1, int i2, int i3, int i4, int i5, int i6)     { return i1 + i2 + i3 + i4 + i5 + i6; }

    int method (void)                                                   { return 0; }
    int method (int i1)                                                 { return i1; }
    int method (int i1, int i2)                                         { return i1 + i2; }
    int method (int i1, int i2, int i3)                                 { return i1 + i2 + i3; }
    int method (int i1, int i2, int i3, int i4)                         { return i1 + i2 + i3 + i4; }
    int method (int i1, int i2, int i3, int i4, int i5)                 { return i1 + i2 + i3 + i4 + i5; }
    int method (int i1, int i2, int i3, int i4, int i5, int i6)         { return i1 + i2 + i3 + i4 + i5 + i6; }

    int method (int i1, int i2, int i3)   const                         { return i1 + i2 + i3; }

    int max(int i, int j)          { return (i > j) ? i : j; }
    double max(double i, double j) { return (i > j) ? i : j; }
};

int test_main(int, char * [])
{
    { // write value to stream
	std::stringstream sstrm;

	BOOST_CHECK(writeTo(sstrm, "Hello")());

        std::string s;
        sstrm >> s;

        BOOST_CHECK(s == "Hello");
    }

    std::string as[] = {"1", "2", "3", "4"};

    { // write to stream using pointers
	std::stringstream sstrm;

	BOOST_CHECK(writeAllTo(sstrm, as, as + 4, " ")());

        std::string s; int i=0;

        while (sstrm >> s) {
            BOOST_CHECK(s == as[i++]);
        }
    }

    { // write to stream using iterators
        std::stringstream sstrm;
        std::vector<std::string> vs (as, as + 4);

        BOOST_CHECK(writeAllTo(sstrm, vs.begin(), vs.end(), "")());

        std::string s;

        while (sstrm >> s) {
            BOOST_CHECK(s == "1234");
        }
    }	

    { // write container to stream
        std::stringstream sstrm;
        lref<const std::vector<std::string> > vls = std::vector<std::string>(as, as + 4);

        BOOST_CHECK(writeAllTo(sstrm, vls, "")());

        std::string s;

        while (sstrm >> s) {
            BOOST_CHECK(s == "1234");
        }
    }

    { // acquire lref<iterators> relationally and then use them with writeAllTo
        std::stringstream sstrm;
        lref<std::vector<std::string> > lvs = std::vector<std::string>(as, as + 4);
        lref<std::vector<std::string>::iterator> b, e;
        relation r = begin(lvs, b) && end(lvs, e) && writeAllTo(sstrm, b, e, "");

        while (r());

        std::string s;

        while (sstrm >> s) {
            BOOST_CHECK(s == "1234");
        }
    }

    { // write value of computing an ILE to stream
        std::stringstream sstrm;
        lref<num> i=3;
        writeTo_f(sstrm, i + i * i)();

        BOOST_CHECK(sstrm.str() == "12");
    }

    { // write value of computing a func
        std::stringstream sstrm;
        lref<num> i=3;
        writeTo_f(sstrm, &num::static_compute, i, 3, i)();

        BOOST_CHECK(sstrm.str() == "4");
    }

    {
	std::stringstream sstrm;
	lref<int> i=3;
	writeTo_f(sstrm, Functor6(), i, i, i)();

	BOOST_CHECK(sstrm.str() == "9");
    }

    {
	std::stringstream sstrm;
	lref<num> i=3;
	lref<num> n(5);
	writeTo_mf(sstrm, n, &num::nonstatic_compute, i, i, i)();

	BOOST_CHECK(sstrm.str() == "4");
    }

    {
        typedef std::pair<std::string, std::string> name;
	lref<name> me = name("Roshan", "Naik");
	std::stringstream sstrm;
        writeTo_mem(sstrm, me, &name::first)();

	BOOST_CHECK(sstrm.str() == "Roshan");
    }

    { // base class member
	lref<Derived> d = Derived();
        stringstream sstrm;
        writeTo_mem(sstrm, d, &Base::b)();
        writeTo_mem(sstrm, d, &Derived::b)();
        writeTo_mem(sstrm, d, &Derived::d)();

        BOOST_CHECK(sstrm.str()=="001");
    }

    { // write value to stream and then read it
        std::stringstream sstrm;
        relation r = writeTo(sstrm, "Hello") && readFrom(sstrm, "Hello");

        BOOST_CHECK(r());
    }

    { // copy words from one stream to another
        std::stringstream inputData, outputData;
        inputData << "Hello World.";
        lref<std::string> ls;
        relation copyWords = readFrom(inputData, ls) && writeTo(outputData, ls);
        int count=0;
        while (copyWords()) { ++count; }

        BOOST_CHECK(count == 2 && !ls.defined());
    }

    { // read value into undefined lref
	std::stringstream sstrm;
	sstrm << "Hello";
	lref<std::string> s;

	BOOST_CHECK(readFrom(sstrm, s)());
    }

    { // read value into defined lref
	std::stringstream sstrm;
	sstrm << "Hello";
	lref<std::string> s = "Hello";

	BOOST_CHECK(readFrom(sstrm, s)());
    }

    { // read value into defined lref
	std::stringstream sstrm;
	sstrm << "Hello";

	BOOST_CHECK(!readFrom(sstrm, "World")());
    }

    { // read all values from stream
	std::stringstream sstrm;
	sstrm << "Hello World";
	lref<std::string> s;
	relation r = readFrom(sstrm, s);

	BOOST_CHECK(r() && *s == "Hello");
	BOOST_CHECK(r() && *s == "World");
    }

    return 0;
}
