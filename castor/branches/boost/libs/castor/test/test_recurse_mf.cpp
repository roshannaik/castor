#include <boost/castor.h>

#include <boost/test/minimal.hpp>


using namespace castor;

struct MyRel {
    relation decrementToUnity(lref<int> j) {
        return eq(j, 1)
            || predicate(j != 1) && dec(j) && recurse_mf(this, &MyRel::decrementToUnity, j);
    }
    
    relation decrementToUnity_c(lref<int> j) const {
        return eq(j, 1)
            || predicate(j != 1) && dec(j) && recurse_mf(this, &MyRel::decrementToUnity_c, j);
    }

    relation decrementToUnity_c2(lref<int> j, lref<int>& blah)  const {
        return eq(j, 1)
            || predicate(j != 1) && dec(j) && recurse_mf(this, &MyRel::decrementToUnity_c2, j, blah);
    }
};

int test_main(int, char * [])
{
    { // non const obj, non const member relation
        MyRel mr;
        lref<int> value = 4;
        mr.decrementToUnity(value)();

        BOOST_CHECK(*value == 1);
    }

    { // non const obj, const member relation
        MyRel mr;
        lref<int> value = 4;
        mr.decrementToUnity_c(value)();

        BOOST_CHECK(*value == 1);
    }

    { // const obj, const member relation
#if defined(__GNUG__)
        const MyRel mr = MyRel();
#else
        const MyRel mr;
#endif
        lref<int> value = 4;
        mr.decrementToUnity_c(value)();

        BOOST_CHECK(*value == 1);
    }

    return 0;
}
