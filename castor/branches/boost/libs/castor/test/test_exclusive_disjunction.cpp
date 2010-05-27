#include <boost/castor.h>

#include <string>

#include <boost/test/minimal.hpp>


using namespace castor;

int test_main(int, char * [])
{
    { // push_back
        ExDisjunctions males, males2;
        lref<std::string> p;
        males.push_back(eq(p, "roshan"));
        males.push_back(eq(p, "shivaram"));

        BOOST_CHECK(males());
        BOOST_CHECK(*p == "roshan");

	males2 = males;

	BOOST_CHECK(!males());
        BOOST_CHECK(!p.defined());
        BOOST_CHECK(!males2());
    }

    { // push_front
        ExDisjunctions males, males2;
        lref<std::string> p;
        males.push_back(eq(p, "shivaram"));
        males.push_front(eq(p, "roshan"));

        BOOST_CHECK(males());
        BOOST_CHECK(*p == "roshan");

	males2 = males;

	BOOST_CHECK(!males());
        BOOST_CHECK(!p.defined());
    	BOOST_CHECK(!males2());
    }

    {
        ExDisjunctions books;
        lref<std::string> t, a;
        books.push_back(eq(t, "Tuesdays with Morrie") && eq(a, "Mitch Albom"));
        books.push_back(eq(t, "Bertrand Russell in 90 minutes") && eq(a, "Paul Strathern"));

        BOOST_CHECK(books());
        BOOST_CHECK(*a == "Mitch Albom");
        BOOST_CHECK(!books());
    }

    return 0;
}
