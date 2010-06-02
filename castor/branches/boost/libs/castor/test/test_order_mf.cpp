#include <boost/castor.h>

#include <boost/test/minimal.hpp>


using namespace castor;

int test_main(int, char * [])
{
    {
        std::string values[] = {"four", "one", "three"};
        std::string svalues[] = {"one", "four", "three"};
        std::vector<std::string> sorted_values(svalues, svalues + 3);

        lref<std::string> v;
        relation r = item(v, values, values + 3) >>= order_mf(v, &std::string::length);
        for (int i = 0; r(); ++i) {
            BOOST_CHECK(sorted_values.at(i) == *v);
        }

        BOOST_CHECK(!v.defined());
    }

    { // with comparator
        std::string values[] = {"four", "one", "three"};
        std::string svalues[] = {"three", "four", "one"};
        std::vector<std::string> sorted_values(svalues, svalues + 3);

        lref<std::string> v;
        relation r = item(v, values, values + 3) >>= order_mf(v, &std::string::length, std::greater<int>());
        for (int i = 0; r(); ++i) {
            BOOST_CHECK(sorted_values.at(i) == *v);
        }

        BOOST_CHECK(!v.defined());
    }

    return 0;
}
