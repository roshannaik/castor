#include <boost/castor.h>

#include <boost/test/minimal.hpp>


using namespace castor;

int test_main(int, char * [])
{
    {
        std::string values[] = {"four", "one", "two", "three"};
        std::vector<std::string> rev(values, values + 4);
        std::reverse(rev.begin(), rev.end());

        lref<std::string> s;
        relation r = item(s, values, values + 4) >>= reverse(s);
        for (int i = 0; r(); ++i) {
            BOOST_CHECK(rev.at(i) == *s);
        }
    }

    return 0;
}
