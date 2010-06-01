#include <boost/castor.h>

#include <boost/test/minimal.hpp>


using namespace castor;

int test_main(int, char * [])
{
    { // generate
        std::vector<int> v;
        v.push_back(2);
        lref<std::vector<int> > rvi = v;
    
        lref<int> h;
        relation r = head(rvi, h);
        int i = 0;

        for (; r(); ++i) {
            BOOST_CHECK(*h == 2);
        }

        BOOST_CHECK(i == 1 && !h.defined());
    }

    { // compare
        std::vector<int> vj;
        vj.push_back(2);
        lref<std::vector<int> > rvj = vj;
        lref<int> h2 = 2;
        relation r2 = head(rvj, h2);
        int j = 0;

        for (; r2(); ++j) {
            BOOST_CHECK(*h2 == 2);
        }

        BOOST_CHECK(j == 1 && h2.defined());
    }

    { // compare - failing
        std::vector<int> vk;
        vk.push_back(9);
        lref<std::vector<int> > rvk = vk;
        lref<int> h3=2;
        relation r3 = head(rvk, h3);
        int k = 0;
        for (; r3(); ++k);

        BOOST_CHECK(k == 0 && h3.defined());
    }

    { // head of empty list
        lref<int> hl;
        lref<std::vector<int> >vl = std::vector<int>();
        relation rl =  head(vl, hl);

        BOOST_CHECK(!rl());
    }

    { // head of undefiend list
        lref<int> hm;
        lref<std::vector<int> >vm;
        relation rm =  head(vm, hm);
        int passed = false;
        try { rm(); }
        catch (...) { passed = true; }

        BOOST_CHECK(passed);
    }

    return 0;
}
