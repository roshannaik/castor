#include <boost/castor.h>
#include <boost/test/minimal.hpp>


using namespace castor;

int test_main(int, char * [])
{
    { // 1 - iterate over std::collection
        std::list<int> li;
        li.push_back(1); li.push_back(2); li.push_back(3); li.push_back(4); li.push_back(5);
        lref<int> obj;
        relation r = item(obj, li.begin(), li.end());
        int j = 1;

        while (r()) {
            BOOST_CHECK(*obj == j++);
        }

        BOOST_CHECK(!obj.defined());
    }

    { // 2 - lookup over std::collection (using lref)
        std::list<int> li;
        li.push_back(1); li.push_back(2); li.push_back(3); li.push_back(4); li.push_back(5); li.push_back(2);
        lref<int> obj = 2;
        relation r = item(obj, li.begin(), li.end());
        int j = 0;

        while (r()) {
            BOOST_CHECK(*obj == 2);
            ++j;
        }

        BOOST_CHECK(j == 2 && *obj == 2);
    }

    { // 3 - iterator with array type
        int li[] = {1, 2, 3, 4, 5};
        lref<int> obj;
        relation r = item(obj, li + 0, li + 5);
        int j = 1;

        while (r()) {
            BOOST_CHECK(*obj == j++);
        }

        BOOST_CHECK(!obj.defined());
    }

    { // 4 - lref<int*> over array type
        int ai[] = {1, 2, 3, 4, 5};
        lref<int> obj;
        lref<int*> b = ai + 0, e = ai + 5;
        relation r = item(obj, b, e);
        int j = 1;

        while (r()) {
            BOOST_CHECK(*obj == j++);
        }

        BOOST_CHECK(!obj.defined());
    }

    { // 5 - using lref<std::vector<int>::iterator>
        int ai[] = {1, 2, 3, 4, 5};
        std::vector<int> vi(ai + 0, ai + 5);
        lref<int> obj;
        lref<std::vector<int>::iterator> b = vi.begin(), e = vi.end();
        relation r = item(obj, b, e);
        int j = 1;

        while (r()) {
            BOOST_CHECK(*obj == j++);
        }

        BOOST_CHECK(!obj.defined());
    }

    { // 6 - find an value item that exist
        int ai[]  = { 1, 2, 3, 4 };
        std::vector<int> vi(ai + 0, ai + 4);

#if  defined(__BCPLUSPLUS__) || defined(__GNUG__)
        BOOST_CHECK(item<std::vector<int>::iterator>(4, vi.begin(), vi.end())());
#else
        BOOST_CHECK(item(4, vi.begin(), vi.end())());
#endif
    }

    { // 7 - find a value item that doesnt exist
        int ai[]  = { 1, 2, 3, 4 };
        std::vector<int> vi(ai + 0, ai + 4);

#if  defined(__BCPLUSPLUS__) || defined(__GNUG__)
        BOOST_CHECK(!item<std::vector<int>::iterator>(9, vi.begin(), vi.end())());
#else
        BOOST_CHECK(!item(9, vi.begin(), vi.end())());
#endif
    }

    { // 8 - intersection of array1 and array2
        int arr1[] = {1, 2, 3, 4};
        int arr2[] = {6, 3, 7, 1};
        lref<int> obj;
        relation r = item(obj, arr1 + 0, arr1 + 4) && item(obj, arr2 + 0, arr2 + 4);

        BOOST_CHECK(r() && *obj == 1);
        BOOST_CHECK(r() && *obj == 3);
        BOOST_CHECK(!r());
        BOOST_CHECK(!obj.defined());
    }

    { // 9 - iterate using

        int ai[] = {1, 2, 3, 4, 5};
        lref<std::vector<int> > vi = std::vector<int>(ai + 0, ai + 5);
        lref<int> obj;
        relation r = item(obj, vi);
        int j = 1;

        while (r()) {
            BOOST_CHECK(*obj == j++);
        }

        BOOST_CHECK(!obj.defined());
    }

    { // 10 - lookup mode - with duplicates - using item(cont)
        int ai[] = {1, 2, 3, 4, 2};
        lref<std::vector<int> > vi = std::vector<int>(ai + 0, ai + 5);
#ifdef __GNUG__
        relation r = item<std::vector<int> >(2, vi);
#else
        relation r = item(2, vi);
#endif
        int j = 0;
        while (r())
            ++j;

        BOOST_CHECK(j == 2);
    }

    { // 11 - lookup mode - non existent value - using item(cont)
        int ai[] = {1, 2, 3, 4, 2};
        lref<std::vector<int> > vi = std::vector<int>(ai + 0, ai + 5);
#ifdef __GNUG__
        relation r = item<std::vector<int> >(9, vi);
#else
        relation r = item(9, vi);
#endif
        int j = 0;
        while (r())
            ++j;

        BOOST_CHECK(j == 0);
    }

    { // 12 - compile time checks for overloads : item(i,std::set<>)  & item(i,std::multiset<>)
        lref<std::set<int> > s;
        lref<std::multiset<int> > ms;
        lref<int> i;
        ItemSet_r<std::set<int> > r1 = item(i,s);        // ensure the call is forwarded to item_set
        ItemSet_r<std::multiset<int> > r2 = item(i,ms);  // ensure the call is forwarded to item_set
    }
    {  //13 - basic tests for overload - item(i,std::set<>)  & item(i,std::multiset<>)
        lref<int> i;
        int ai[] = {1,2,3,4,2};
        lref<std::set<int> > si = std::set<int>(ai+0, ai+5);
        lref<std::multiset<int> > mi = std::multiset<int>(ai+0, ai+5);

        relation r = item(i,si);
        int j=0;
        while(r())
            ++j;
        BOOST_CHECK(j==si->size());

        r = item(i,mi);
        j=0;
        while(r())
            ++j;
        BOOST_CHECK(j==mi->size());
    }

    return 0;
}
