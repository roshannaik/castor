#include <boost/castor.h>

#include <boost/test/minimal.hpp>


using namespace castor;

int test_main(int, char * [])
{
    { // 1 .. input list is simple std::list<int>
        std::list<int> li; li.push_back(1); li.push_back(2);
        std::list<int> values; values.push_back(7); values.push_back(8);
        lref<std::list<int> > insertedList;
#if  defined(__BCPLUSPLUS__) || defined(__GNUG__)
        relation r =
            insert_seq<std::list<int> >(values.begin(), values.end(), li.begin(), li.end(), insertedList);
#else
        relation r = insert_seq(values.begin(), values.end(), li.begin(), li.end(), insertedList);
#endif
        int expected[][4] = { {7, 8, 1, 2}, {1, 7, 8, 2}, {1, 2, 7, 8} };
        int i = 0;

        for (; r(); ++i) {
            BOOST_CHECK(equal(expected[i], expected[i] + 3, insertedList->begin()));
        }

        BOOST_CHECK(i == 3);
    }

    { // 2 .. input list is simple lref<std::list<int> >
        lref<std::list<int> > li = std::list<int>(); li->push_back(1); li->push_back(2);
        std::list<int> values; values.push_back(7); values.push_back(8);
        lref<std::list<int> > insertedList;
#if  defined(__BCPLUSPLUS__) || defined(__GNUG__)
        relation r =
            insert_seq<std::list<int> >(values.begin(), values.end(), li->begin(), li->end(), insertedList);
#else
        relation r = insert_seq(values.begin(), values.end(), li->begin(), li->end(), insertedList);
#endif
        int expected[][4] = { {7, 8, 1, 2}, {1, 7, 8, 2}, {1, 2, 7, 8} };
        int i = 0;

        for (; r(); ++i) {
            BOOST_CHECK(equal(expected[i], expected[i] + 3, insertedList->begin()));
        }

        BOOST_CHECK(i == 3);
    }

    { // 3 .. test ...success
        lref<std::list<int> > li = std::list<int>(); li->push_back(1); li->push_back(2);
        std::list<int> values; values.push_back(7); values.push_back(8);
        int insValues[] = {1, 7, 8, 2};
        lref<std::list<int> > insertedList = std::list<int>(insValues + 0, insValues + 4);
        relation r =
            insert_seq<std::list<int> >(values.begin(), values.end(), li->begin(), li->end(), insertedList);
        int i = 0;
        for (; r(); ++i);

        BOOST_CHECK(i == 1);
    }

    { // 4 .. test.. failure
        lref<std::list<int> > li = std::list<int>(); li->push_back(1); li->push_back(2);
        std::list<int> values; values.push_back(7); values.push_back(8);
        int insValues[] = {1, 2, 3, 4};
        lref<std::list<int> > insertedList = std::list<int>(insValues + 0, insValues + 4);
        relation r =
            insert_seq<std::list<int> >(values.begin(), values.end(), li->begin(), li->end(), insertedList);
        int i = 0;
        for (; r(); ++i);

        BOOST_CHECK(i == 0);
    }

    return 0;
}
