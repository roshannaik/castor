#include <boost/castor.h>

#include <boost/test/minimal.hpp>


using namespace castor;

struct Number {
    int i;

    Number(int i) : i(i) {}

    bool operator==(const Number & rhs) {
        return i == rhs.i;
    }
}; 

int test_main(int, char * [])
{
    {
        Number values[] = {3, 1, 4, 2};
        Number svalues[] = {1, 2, 3, 4};
        std::vector<Number> sorted_values(svalues, svalues + 4);

        lref<Number> v;
        relation r = item(v, values, values + 4) >>= order_mem(v, &Number::i);
        for (int i = 0; r(); ++i) {
            BOOST_CHECK(sorted_values.at(i).i == v->i);
        }

        BOOST_CHECK(!v.defined());
    }

    {
        Number values[] = {3, 1, 4, 2};
        Number svalues[] = {4, 3, 2, 1};
        std::vector<Number> sorted_values(svalues, svalues + 4);

        lref<Number> v;
        relation r = item(v, values, values + 4) >>= order_mem(v, &Number::i, std::greater<int>());
        for (int i = 0; r(); ++i) {
            BOOST_CHECK(sorted_values.at(i).i == v->i);
        }

        BOOST_CHECK(!v.defined());
    }

    return 0;
}
