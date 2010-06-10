#include <boost/castor.h>
#include <boost/castor.h>

using namespace castor;

int test_main(int, char * [])
{
    lref<std::multimap<char,int> > mm = std::multimap<char,int>();
    mm->insert ( make_pair('a',100) );
    mm->insert ( make_pair('z',150) ); 
    mm->insert ( make_pair('b',75) );
    mm->insert ( make_pair('c',100) );
    mm->insert ( make_pair('z',400) );
    mm->insert ( make_pair('c',100) );

    lref<std::map<char,int> > m = std::map<char,int>(mm->begin(), mm->end());

    { // lookup key & obj - multimap
        relation r = item_map('c',100,mm);
        int count=0;
        while(r())
            ++count;
        BOOST_CHECK(count==2);
        
        r = item_map('z',400,mm);
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
        int count=0;
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
        relation r = item_map(k,100,mm);
        BOOST_CHECK(r());
        BOOST_CHECK(*k=='a')
        BOOST_CHECK(r());
        BOOST_CHECK(*k=='c')
        BOOST_CHECK(r());
        BOOST_CHECK(*k=='c');
        BOOST_CHECK(!r());
    }
    return 0;
}
