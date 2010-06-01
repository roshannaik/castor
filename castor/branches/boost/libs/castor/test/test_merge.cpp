#include <boost/castor.h>

#include <boost/test/minimal.hpp>


using namespace castor;

int test_main(int, char * [])
{
    { // generate : left is empty std::vector
        int ai[] = {1, 2, 3};
        lref<std::vector<int> >  vi, vj = std::vector<int>(ai, ai + 0);
        lref<std::vector<int> > m;
        relation r = castor::merge<std::vector<int> >(vi, vj, m);
        int i = 0;

        for (; r(); ++i) {
            BOOST_CHECK((*m) == (*vj));
        }

        BOOST_CHECK(i == 1);
    }

    { // generate : left&right are non-empty std::vector
        int aj[]  = {1, 3};
        int bj[] = {2, 4};
        lref<std::vector<int> > rlj = std::vector<int>(aj + 0, aj + 2), rrj = std::vector<int>(bj + 0, bj + 2); 
        lref<std::vector<int> > m2; 
        relation rj = merge<std::vector<int> >(rlj, rrj, m2);
        int j = 0;
        for (; rj(); ++j) {
            int expect[] = {1, 2, 3, 4};
            std::vector<int> expected(expect + 0, expect + 4);
    
            BOOST_CHECK(*m2 == expected);
        }

        BOOST_CHECK(j == 1);
    }

    { // compare
        int ak[] = {1, 3}, bk[]={2, 4};
        int expect[] = {1, 2, 3, 4};
        std::vector<int> expected3(expect + 0, expect + 4);

        lref<std::vector<int> > rlk = std::vector<int>(ak + 0, ak + 2), rrk= std::vector<int>(bk + 0, bk + 2);
        lref<std::vector<int> > m3; 
        relation rk = merge<std::vector<int> >(rlk, rrk, m3);
        int k = 0;

        for (; rk(); ++k) {
            BOOST_CHECK(*m3 == expected3);
        }

        BOOST_CHECK(k == 1);
    }

    { // generate first seq given a seq and a
        int ak[] = {1, 3}, bk[] = {2, 4};
        int expect[] = {1, 2, 3, 4};
        std::vector<int> expected3(expect + 0, expect + 4);

        lref<std::vector<int> > rlk = std::vector<int>(ak + 0, ak + 2), rrk = std::vector<int>(bk + 0, bk + 2);
        lref<std::vector<int> > m3; 
        relation rk = merge<std::vector<int> >(rlk, rrk, m3);
        int k = 0;

        for (; rk(); ++k) {
            BOOST_CHECK(*m3 == expected3);
        }

        BOOST_CHECK(k == 1);
    }

    return 0;
}
