#include <boost/castor.h>

#include <boost/test/minimal.hpp>


using namespace castor;

int test_main(int, char * [])
{
    { // 1 - lookup mode - with duplicates
        int ai[] = {1, 2, 3, 4, 2};
        lref<std::vector<int> > vi = std::vector<int>(ai + 0, ai + 5);
#ifdef __GNUG__
        relation r = ritem<std::vector<int> >(2, vi);
#else
        relation r = ritem(2, vi);
#endif
        int j = 0;
        while (r())
            ++j;

        BOOST_CHECK(j == 2);
    }

    { // 2 - iterate over std::collection
        lref<std::list<int> > li = std::list<int>();
	li->push_back(1); li->push_back(2); li->push_back(3); li->push_back(4); li->push_back(5);
	lref<int> obj;
	relation r = ritem(obj, li);
        int j = 5;

        while (r()) {
            BOOST_CHECK(*obj == j--);
        }

        BOOST_CHECK(!obj.defined() || j == 0);
    }

    return 0;
}
