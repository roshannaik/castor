#include <boost/castor.h>

#include <list>
#include <utility>

#include <boost/test/minimal.hpp>


using namespace castor;

int test_main(int, char * [])
{
    std::list<std::pair<int, int> > expected, actual;
    expected.push_back(std::make_pair(1, 1));
    expected.push_back(std::make_pair(1, 2));
    expected.push_back(std::make_pair(2, 1));
    expected.push_back(std::make_pair(2, 2));

    lref<int> a, b;
    relation r = (eq(a, 1) || eq(a, 2))
        && (eq(b, 1) || eq(b, 2));

    while (r())
        actual.push_back(std::make_pair(*a, *b));

    BOOST_CHECK(actual == expected);

    return 0;
}
