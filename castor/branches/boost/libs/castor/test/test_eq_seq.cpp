#include "test_eq.h"

#include <boost/castor.h>

#include <boost/test/minimal.hpp>

#include <vector>


using namespace castor;

int test_main(int, char * [])
{
    { // generate container with elements
        int ai[] = {1, 2, 3};
        lref<std::vector<int> > vi;
        relation r = eq_seq(vi, ai, ai + 3);
        int i = 0;

        for (; r(); ++i) {
            BOOST_CHECK(i <= 0 && (*vi).at(0) == 1 && (*vi).at(1) == 2 && (*vi).at(2) == 3);
        }

        BOOST_CHECK(i == 1 && !vi.defined());
    }

    { // compare using custom comparator
        const int ai[] = {1, 2, 3};
        lref<std::vector<int> > vi = std::vector<int>(ai + 0, ai + 3);

        relation r = eq_seq(vi, ai, ai + 3, &cmpi);

	BOOST_CHECK(r());
    }

    {
        const int bi[] = {1, 2, 3};
        lref<std::vector<int> > vj = std::vector<int>(bi, bi + 3);
        int ci[] = {1, 2, 3};
        relation r2 = eq_seq(vj, ci, ci + 3);
	int j = 0;

	for (; r2(); ++j);

	BOOST_CHECK(j == 1 && vj.defined());
    }

    {
        const int di[] = {1, 2, 3};
        lref<std::vector<int> > vk = std::vector<int>(di, di + 3);
        int ei[] = {3, 2, 3};
        relation r3 = eq_seq(vk, ei, ei + 3);

	int k = 0;
	for (; r3(); ++k);

	BOOST_CHECK(k == 0 && vk.defined());
    }

    return 0;
}
