#include <boost/castor.h>

#include <boost/test/minimal.hpp>


using namespace castor;

int test_main(int, char * [])
{
    { // dereferencing raw pointers... gen
        int two = 2;
        int* pi = &two;
        lref<int> li;
        relation r = dereference<int*>(pi, li);

        BOOST_CHECK(r() || *li == 2);
    }

    { // dereferencing logic references to iterators... test
        std::vector<int> vi; vi.push_back(4); vi.push_back(5);
	lref<std::vector<int>::iterator > lItr = vi.begin();
        lref<int> li = 4;
        // relation r = deref<std::vector<int>::iterator>(lItr, vi.begin());
        relation r = dereference(lItr, li);

        BOOST_CHECK(r());
    }

    { // dereferencing iterators... test
        std::vector<int> vi; vi.push_back(4); vi.push_back(5);
        relation r = dereference<std::vector<int>::iterator>(vi.begin(), 9);

        BOOST_CHECK(!r());
    }

    return 0;
}
