#include <boost/castor.h>

#include <boost/test/minimal.hpp>


using namespace castor;

int test_main(int, char * [])
{
    { // 1 .. input list is simple list<int>
        std::list<int> li; li.push_back(1); li.push_back(2);
        lref<std::list<int> > insertedList;
#if  defined(__BCPLUSPLUS__) || defined(__GNUG__)
        relation r = insert<std::list<int> >(9, li.begin(), li.end(), insertedList);
#else
        relation r = insert(9, li.begin(), li.end(), insertedList);
#endif    
        int expected[][3] = { {9, 1, 2}, {1, 9, 2}, {1, 2, 9} };
        int i = 0;

        for (; r(); ++i) {
            BOOST_CHECK(equal(expected[i], expected[i] + 3, insertedList->begin()));
        }

        BOOST_CHECK(i == 3);
    }

    { // 2 .. input list is simple lref<std::list<int> >
        lref<std::list<int> > li = std::list<int>(); li->push_back(1); li->push_back(2);
        lref<std::list<int> > insertedList;
#if  defined(__BCPLUSPLUS__) || defined(__GNUG__)
        relation r = insert<std::list<int> >(9, li->begin(), li->end(), insertedList);
#else
        relation r = insert(9, li->begin(), li->end(), insertedList);
#endif
        int expected[][3] = { {9, 1, 2}, {1, 9, 2}, {1, 2, 9} };
        int i = 0;

        for (; r(); ++i) {
            BOOST_CHECK(equal(expected[i], expected[i] + 3, insertedList->begin()));
        }

        BOOST_CHECK(i == 3);
    }

    { // 3 .. test ...success
        lref<std::list<int> > li = std::list<int>(); li->push_back(1); li->push_back(2);
        int values[] = {1, 9, 2};
        lref<std::list<int> >insertedList = std::list<int>(values + 0, values + 3);
        relation r = insert<std::list<int> >(9, li->begin(), li->end(), insertedList);
        int i = 0;
        for (; r(); ++i);

        BOOST_CHECK(i == 1);
    }

    { // 4 .. test.. failure
        lref<std::list<int> > li = std::list<int>(); li->push_back(1); li->push_back(2);
        int values[] = {1, 2, 3};
        lref<std::list<int> > insertedList = std::list<int>(values + 0, values + 3);
#if  defined(__BCPLUSPLUS__) || defined(__GNUG__)
        relation r = insert<std::list<int> >(9, li->begin(), li->end(), insertedList);
#else
        relation r = insert(9, li->begin(), li->end(), insertedList);
#endif
        int i = 0;
        for (; r(); ++i);

        BOOST_CHECK(i == 0);
    }

    return 0;
}
