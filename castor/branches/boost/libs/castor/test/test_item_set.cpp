#include <boost/castor.h>
#include <boost/castor.h>

using namespace castor;

int test_main(int, char * [])
{
    int ai[] = {1,2,3,4,2};
    lref<std::multiset<int> > mi = std::multiset<int>(ai+0, ai+5);
    lref<std::set<int> > si = std::set<int>(ai+0, ai+5);

    { // test mode - std::multiset
#ifdef __GNUG__
        relation r = item_set<std::multiset<int> >(2, mi);
#else
        relation r = item_set(2,mi);
#endif
        int j=0;
        while(r())
            ++j;
        BOOST_CHECK(j==2);

        BOOST_CHECK(!item_set(-1,mi)());
    }
    { // test mode - std::set
        lref<int> i=2;
        relation r = item_set(i,si);
        int j=0;
        while(r())
            ++j;
        BOOST_CHECK(j==1);

        BOOST_CHECK(!item_set(-1,si)());
    }
    {  // gen mode - std::multiset
        BOOST_CHECK(mi->size()>si->size());  // require duplicates in mi
        lref<int> i;
        relation r = item_set(i,mi);
        int j=0;
        while(r())
            ++j;
        BOOST_CHECK(j==mi->size());
    }
    {  // gen mode - std::set
        lref<int> i;
        relation r = item_set(i,si);
        int j=0;
        while(r())
            ++j;
        BOOST_CHECK(j==si->size());
    }
    return 0;
}
