#include <boost/castor.h>

#include <string>

#include <boost/test/minimal.hpp>


using namespace castor;

int test_main(int, char * [])
{
    { // push_back
        Conjunctions gender, gender2;
        lref<std::string> p, m;
        gender.push_back(eq(p, "roshan"));
        gender.push_back(eq(m, "male"));

        BOOST_CHECK(gender());
        BOOST_CHECK(*p == "roshan");

        gender2 = gender;

	BOOST_CHECK(!gender());
        BOOST_CHECK(!gender2());
    }

    { // push_front
        Conjunctions gender, gender2;
        lref<std::string> p, m;
        gender.push_front(eq(m, "male"));
        gender.push_front(eq(p, "roshan"));

        BOOST_CHECK(gender());
        BOOST_CHECK(*p == "roshan");

        gender2 = gender;

	BOOST_CHECK(!gender());
        BOOST_CHECK(!gender2());
    }

    {
        Conjunctions books;
        lref<std::string> t, a;
        books.push_back(eq(t, "Tuesdays with Morrie") && eq(a, "Mitch Albom"));

        BOOST_CHECK(books());
        BOOST_CHECK(*a == "Mitch Albom");
        BOOST_CHECK(!books());
    }

    return 0;
}
