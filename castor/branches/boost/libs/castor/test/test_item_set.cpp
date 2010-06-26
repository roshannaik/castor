#include <boost/castor.h>

#include <boost/test/minimal.hpp>


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

#ifdef __GNUG__
    BOOST_CHECK(!item_set<std::multiset<int> >(-1,mi)());
#else
    BOOST_CHECK(!item_set(-1,mi)());
#endif
        
    }
    { // test mode - std::set
        lref<const int> i=2;
        relation r = item_set(i,si);
        int j=0;
        while(r())
            ++j;
        BOOST_CHECK(j==1);

#ifdef __GNUG__
    BOOST_CHECK(!item_set<std::set<int> >(-1,si)());
#else
    BOOST_CHECK(!item_set(-1,si)());
#endif

    }
    {  // gen mode - std::multiset
        BOOST_CHECK(mi->size()>si->size());  // require duplicates in mi
        lref<const int> i;
        relation r = item_set(i,mi);
        size_t j=0;
        while(r())
            ++j;
        BOOST_CHECK(j==mi->size());
    }
    {  // gen mode - std::set
        lref<int> i;
        relation r = item_set(i,si);
        std::size_t j=0;
        while(r())
            ++j;
        BOOST_CHECK(j==si->size());
    }
    return 0;
}
