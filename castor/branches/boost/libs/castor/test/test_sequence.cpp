#define CASTOR_ENABLE_DEPRECATED // TODO: Remove the need for this (it's due to reliance on deprecated size<>()).

#include <boost/castor.h>

#include <boost/test/minimal.hpp>


using namespace castor;

int test_main(int, char * [])
{
    { // copmare ... unequal number of elements .. L < R
        int a[] = {1, 2, 3};
        lref<std::list<int> > rli = std::list<int>(a + 0, a + 3);
        lref<int> i1 = 1;
        std::list<int> il2; il2.push_back(2);
        relation r = sequence(rli)(i1)(il2.begin(), il2.end())(3)(4);

        BOOST_CHECK(!r());
    }

    { // compare ... unequal number of elements .. L > R
        int b[] = {1, 2, 3, 4};
        lref<std::list<int> > rly = std::list<int>(b + 0, b + 4);
        lref<int> y1 = 1;
        std::list<int> yl2; yl2.push_back(2);
        relation rm = sequence(rly)(y1)(yl2.begin(), yl2.end())(3);

        BOOST_CHECK(!rm());
    }

    { // compare ...  equal number of identical elements
        int c[] = {1, 2, 3, 4};
        lref<std::list<int> > rlj = std::list<int>(c + 0, c + 4);
        lref<int> j2 = 2;
        std::list<int> jl3; jl3.push_back(3); jl3.push_back(4);
        relation rj = sequence(rlj)(1)(j2)(jl3.begin(), jl3.end());

        BOOST_CHECK(rj());
    }

    { // compare ...  differring elements
        int n[] = {1, 2, 3};
        lref<std::list<int> > rln = std::list<int>(n + 0, n + 3);
        lref<int> n2 = 2;
        relation rn = sequence(rln)(1)(n2)(4);

        BOOST_CHECK(!rn());
    }

    { // compare partial collection (using ignore())
        int d[] = {1, 2, 3, 4};
        lref<std::list<int> > rlk = std::list<int>(d + 0, d + 4);
        lref<int> k2 = 2; 
        std::list<int> l3; l3.push_back(d[2]);
        lref<std::list<int> > k3 = l3;
        relation rk = sequence(rlk)(1)(k2)(k3).ignore();

        BOOST_CHECK(!rk());
    }

    { // generate list
        int e[] = {1, 2, 3};
        lref<std::list<int> > rlx ;
        lref<int> x1 = 1;
        std::list<int> lx3; lx3.push_back(e[2]);
        relation rx = sequence(rlx)(x1)(2)(lx3.begin(), lx3.end());
        int x = 0;

        for (; rx(); ++x) {
            BOOST_CHECK(x <= 1 && *rlx == std::list<int>(e + 0, e + 3));
        }

        BOOST_CHECK(x == 1 && !rlx.defined());
    }

    { // generate list : using mix of values, lref, std::lists, lref<std::vector>
        lref<std::vector<std::string> > ln;
        std::string s = "One";
        lref<std::string> lrs = "Two";
        std::list<std::string> ls;
        ls.push_back("Three"); ls.push_back("Four");
        std::vector<std::string> lsTemp; 
        lsTemp.push_back("Five"); lsTemp.push_back("Six");
        lref<std::vector<std::string> > lrls = lsTemp;
        // create the sequence
        relation numbers = sequence(ln)("Zero")(s)(lrs)(ls.begin(), ls.end())(lrls);
        // see what it produces

        BOOST_CHECK(numbers());

        std::string expected[] = { "Zero", "One", "Two", "Three", "Four", "Five", "Six"};

        BOOST_CHECK(equal(expected, expected + 7, ln->begin()));
    }

    { // generate using lref<iterator> 
        std::list<int> li; li.push_back(1); li.push_back(2); li.push_back(3);
        lref<std::list<int> > lrli1, lrli2 = li;
        lref<std::list<int>::iterator> itrB, itrE;

        BOOST_CHECK((begin(lrli2, itrB) && end(lrli2, itrE) && sequence(lrli1)(itrB, itrE))());
        BOOST_CHECK(equal(li.begin(), li.end(), lrli1->begin()));
    }

    { // compare using lref<iterator>  
        std::list<int> li; li.push_back(1); li.push_back(2); li.push_back(3);
        lref<std::list<int> > lrli = li;
        lref<std::list<int>::iterator> itrB, itrE;

        BOOST_CHECK((begin(lrli, itrB) && end(lrli, itrE) && sequence(lrli)(itrB, itrE))());
        BOOST_CHECK(equal(li.begin(), li.end(), lrli->begin()));
    }

    { // generate using iterators
        std::list<int> li; li.push_back(1); li.push_back(2); li.push_back(3);
        lref<std::list<int>::iterator> itrB, itrE;
        lref<std::list<int> > lrli1, lrli2 = li;

        BOOST_CHECK((begin(lrli2, itrB) && end(lrli2, itrE) && sequence(lrli1)(li.begin(), li.end()))());
        BOOST_CHECK(equal(li.begin(), li.end(), lrli1->begin()));
    }

    { // test for empty sequence
        lref<std::list<int> > li = std::list<int>();

        BOOST_CHECK(sequence<std::list<int> >(li)());
    }

    { // generate for empty sequence
        lref<std::list<int> > lrli;

        BOOST_CHECK(sequence(lrli)());
        BOOST_CHECK(lrli->empty());
    }

    return 0;
}
