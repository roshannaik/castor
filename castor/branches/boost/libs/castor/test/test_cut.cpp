#include <boost/castor.h>

#include <string>
#include <stack>

#include <boost/test/minimal.hpp>


using namespace castor;

namespace {
    bool push(std::stack<std::string>& coll, std::string val) {
        coll.push(val);
        return true;
    }
}

int test_main(int, char * [])
{
#ifndef __BCPLUSPLUS__   // cut() && cut() not supported in BCB
    { // cut() && cut()
	relation r = cutexpr(cut() && cut());

	BOOST_CHECK(!r());
    }
#endif // __BCPLUSPLUS__

    { // cut() && ...       TODO : test for all solns here
	lref<std::stack<std::string> > lss = std::stack<std::string>(); lref<std::string> ls = "blah";
	relation r = cutexpr(cut() && predicate(&push, lss, ls));

	BOOST_CHECK(r());
	BOOST_CHECK(!lss->empty());
    }

    { // ... && cut()
	lref<std::stack<std::string> > lss = std::stack<std::string>(); lref<std::string> s = "blah";
	relation r = cutexpr(cut() && predicate(&push, lss, s));

	BOOST_CHECK(r());
	BOOST_CHECK(!lss->empty());
	BOOST_CHECK(!r());
    }

#ifndef __BCPLUSPLUS__   // cut() && cut() not supported in BCB
    { // cut() || cut()
	relation r = cutexpr(cut() || cut());

	BOOST_CHECK(!r());
    }
#endif // __BCPLUSPLUS__

    { // cut() || ...
        lref<std::stack<std::string> > lss = std::stack<std::string>(); lref<std::string> s = "blah";
        relation r = cutexpr(cut() || predicate(&push, lss, s));

        BOOST_CHECK(!r());
        BOOST_CHECK(lss->empty());
    }

    { // .. || cut()
        lref<std::stack<std::string> > lss = std::stack<std::string>(); lref<std::string> s = "blah";
        relation r = cutexpr(predicate(&push, lss, s) || cut());

        BOOST_CHECK(r());
        BOOST_CHECK(lss->size() == 1);
    }

    { // full blown cut expression
        lref<std::stack<std::string> > lss = std::stack<std::string>(); lref<std::string> s1 = "one", s2 = "two", s3 = "three";
        lref<int> i = 2, j = 3;
        relation r = cutexpr(
            predicate(&push, lss, s1) &&  predicate(i < j) && cut()
            || predicate(&push, lss, s2) &&  predicate(i > j) && cut() && True()
            || predicate(&push, lss, s3) &&  predicate(i == j) && cut()
        );
        relation temp = r;
        while (r());

        BOOST_CHECK(lss->size() == 1);

        r = temp;
        i = 3; j = 2; lss = std::stack<std::string>();
        while (r());

        BOOST_CHECK(lss->size() == 2);

        r = temp; lss = std::stack<std::string>();
        i = 2; j = 2;
        while (r());

        BOOST_CHECK(lss->size() == 3);
    }

    return 0;
}
