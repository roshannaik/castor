#include <boost/castor.h>

#include <boost/test/minimal.hpp>

#include <map>


using namespace castor;

int test_main(int, char * [])
{
    lref<std::multimap<char,int> > mm = std::multimap<char,int>();
    mm->insert ( std::make_pair('a',100) );
    mm->insert ( std::make_pair('z',150) ); 
    mm->insert ( std::make_pair('b',75) );
    mm->insert ( std::make_pair('c',100) );
    mm->insert ( std::make_pair('z',400) );
    mm->insert ( std::make_pair('c',100) );

    lref<std::map<char,int> > m = std::map<char,int>(mm->begin(), mm->end());

    { // lookup key & obj - multimap
#ifdef __GNUG__
        relation r = item_map<std::multimap<char,int> >('c',100,mm);
#else
        relation r = item_map('c',100,mm);
#endif
        int count=0;
        while(r())
            ++count;
        BOOST_CHECK(count==2);
        
#ifdef __GNUG__
        r = item_map<std::multimap<char,int> >('z',400,mm);
#else
        r = item_map('z',400,mm);
#endif
        count=0;
        while(r())
            ++count;
        BOOST_CHECK(count==1);
        throw "failed test_item_map 1";
    }
    { // gen key & obj - multimap
        lref<const char> k;
        lref<int> v;
        relation r = item_map(k,v,mm);
        std::size_t count=0;
        while(r())
            ++count;
        BOOST_CHECK(count==mm->size());
        throw "failed test_item_map 2";
    }
    { // lookup key, gen obj - multimap
        lref<const char> k='z';
        lref<int> v;
        relation r = item_map(k,v,mm);
        int count=0;
        while(r())
            ++count;
        BOOST_CHECK(count==2);
        throw "failed test_item_map 3";
    }
    { // gen key, lookup obj - multimap
        lref<const char> k;
#ifdef __GNUG__
        relation r = item_map<std::multimap<char,int> >(k,100,mm);
#else
        relation r = item_map(k,100,mm);
#endif
        BOOST_CHECK(r());
        BOOST_CHECK(*k=='a');
        BOOST_CHECK(r());
        BOOST_CHECK(*k=='c');
        BOOST_CHECK(r());
        BOOST_CHECK(*k=='c');
        BOOST_CHECK(!r());
    }
    { // gen key & obj - const multimap
        const std::multimap<char,int> cmm(mm->begin(), mm->end());
        lref<const std::multimap<char,int> > lcmm (&cmm, false);
        lref<const char> k;
        lref<int> v;
        relation r = item_map(k,v,mm);
        std::size_t count=0;
        while(r())
            ++count;
        BOOST_CHECK(count==cmm.size());
    }   
    return 0;
}
