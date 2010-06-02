#include <boost/castor.h>

#include <boost/test/minimal.hpp>


using namespace castor;

char firstChar(std::string& s) {
    return s.at(0);
}

std::size_t slength(std::string& s) {
    return s.length();
}

bool palind(std::string& s) {
    return std::equal(s.begin(), s.end(), s.rbegin());
}


bool myCmp(std::pair<std::string, std::pair<char, int> >& lhs, std::pair<std::string, std::pair<char, int> >& rhs) {
    if (lhs.second.first < rhs.second.first)
        return true;

    if (lhs.second.first == rhs.second.first)
        return lhs.second.second <= rhs.second.second;

    return false;
}

int test_main(int, char * [])
{
    // common data for all tests
    std::string numbers[] = { "One", "Three", "Two", "Four", "Five", "Six" , "million", "madam" };
    std::size_t len = sizeof(numbers) / sizeof(std::string);

    std::vector<std::string> nums(numbers, numbers + len);
    lref<std::vector<std::string> > lnums(&nums, false);

    { // nested grouping (3 levels)
        lref<std::string> num;
        std::string results[] = { "Four", "Five", "One", "Six", "Two", "Three", "madam", "million" };
        char keys1[] = { 'F', 'O', 'S', 'T', 'm' };
        std::size_t keys2[] = { 4, 3, 3, 3, 5, 5, 7 };
        bool keys3[] = { false, false, false, false, false, true, false };
        int v = 0, k1 = 0, k2 = 0, k3 = 0;

        lref<group<char, group<std::size_t, group<bool, std::string> > > > g1;
        lref<group<std::size_t, group<bool, std::string> > > g2;
        lref<group<bool, std::string> > g3;

        relation r1 = item(num, lnums) >>= group_by(num, &firstChar, g1).then(slength).then(palind);
        lref<std::string> s;

        while (r1()) {
            BOOST_CHECK(g1->key == keys1[k1++]);

            relation r2 = item(g2, g1);
            while (r2()) {
                BOOST_CHECK(g2->key == keys2[k2++]);

                relation r3 = item(g3, g2);
                while (r3()) {
                    BOOST_CHECK(g3->key == keys3[k3++]);

                    relation values = item(s, g3);
                    while (values())
                        BOOST_CHECK(*s == results[v++]);
                }
            }
        }

        BOOST_CHECK(!g1.defined() && !g2.defined() && !g3.defined() && !s.defined() && !num.defined());
    }

    { // nested grouping - 2 levels
        lref<std::string> num;
        std::string results[] = { "Four", "Five", "One", "Six", "Two", "Three", "madam", "million" };
        char keys1[] = { 'F', 'O', 'S', 'T', 'm' };
        std::size_t keys2[] = { 4, 3, 3, 3, 5, 5, 7 };
        int v = 0, k1 = 0, k2 = 0;

        lref<group<char, group<std::size_t, std::string> > > g1;
        lref<group<std::size_t, std::string> > g2;
        relation outer = item(num, lnums) >>= group_by(num, &firstChar, g1).then(slength);
        lref<std::string> s;

        while (outer()) {
            BOOST_CHECK((g1->key) == keys1[k1++]);

            relation inner = item(g2, g1);
            while (inner()) { 
                BOOST_CHECK(g2->key == keys2[k2++]);

                relation values = item(s, g2);
                while (values())
                    BOOST_CHECK(*s == results[v++]);
            }
        }

        BOOST_CHECK(!g1.defined() && !g2.defined() && !s.defined() && !num.defined());
    }

    { // basic grouping - 1 levels
        lref<std::string> num;
        std::string results[] = { "Four", "Five", "One", "Six", "Three", "Two", "million", "madam" };
        char keys[] = { 'F', 'O', 'S', 'T', 'm' };
        int v = 0, k = 0;
        lref<group<char, std::string> > g;
        relation r = item(num, lnums) >>= group_by(num, &firstChar, g);
        lref<std::string> s;

        while (r()) {
            BOOST_CHECK((g->key) == keys[k++]);

            relation values = item(s, g);
            while (values()) {
                BOOST_CHECK(*s == results[v++]);
            }
        }

        BOOST_CHECK(!g.defined() && !s.defined() && !num.defined());
    }

    { // basic grouping with keyCmp - 1 levels
        lref<std::string> num;
        std::string results[] = { "million", "madam", "Three", "Two", "Six", "One", "Four", "Five"};
        char keys[] = { 'm', 'T', 'S', 'O', 'F' };
        int v = 0, k = 0;
        lref<group<char, std::string> > g;
        relation r = item(num, lnums) >>= group_by(num, &firstChar, g, std::greater<char>());
        lref<std::string> s;

        while (r()) {
            BOOST_CHECK((g->key) == keys[k++]);

            relation values = item(s, g);
            while (values()) {
                BOOST_CHECK(*s == results[v++]);
            }
        }

        BOOST_CHECK(!g.defined() && !s.defined() && !num.defined());
    }

    { // nested grouping with keyCmp - 2 levels
        lref<std::string> num;
        std::string results[] = { "million", "madam", "Three", "Two", "Six", "One", "Four", "Five"};
        char keys1[] = { 'm', 'T', 'S', 'O', 'F' };
        std::size_t keys2[] = { 7, 5, 5, 3, 3, 3, 4 };
        int v = 0, k1 = 0, k2 = 0;

        lref<group<char, group<std::size_t, std::string> > > g1;
        lref<group<std::size_t, std::string> > g2;
        relation outer = item(num, lnums) >>= group_by(num, &firstChar, g1, std::greater<char>()).then(slength, std::greater<int>());
        lref<std::string> s;

        while (outer()) {
            BOOST_CHECK((g1->key) == keys1[k1++]);

            relation inner = item(g2, g1);
            while (inner()) { 
                BOOST_CHECK(g2->key == keys2[k2++]);

                relation values = item(s, g2);
                while (values()) {
                    BOOST_CHECK(*s == results[v++]);
                }
            }
        }

        BOOST_CHECK(!g1.defined() && !g2.defined() && !s.defined() && !num.defined());
    }

    { // nested grouping with keyCmp & values_by() - 2 levels
        lref<std::string> num;
        std::string results[] = { "million", "madam", "Three", "Two", "Six", "One", "Five", "Four" };
        char keys1[] = { 'm', 'T', 'S', 'O', 'F' };
        std::size_t keys2[] = { 7, 5, 5, 3, 3, 3, 4 };
        int v = 0, k1 = 0, k2 = 0;

        lref<group<char, group<std::size_t, std::string> > > g1;
        lref<group<std::size_t, std::string> > g2;
        relation outer = item(num, lnums) >>= group_by(num, &firstChar, g1, std::greater<char>()).then(slength, std::greater<int>()).item_order(std::less<std::string>());
        lref<std::string> s;

        while (outer()) {
            BOOST_CHECK((g1->key) == keys1[k1++]);

            relation inner = item(g2, g1);
            while (inner()) {
                BOOST_CHECK(g2->key == keys2[k2++]);

                relation values = item(s, g2);

                while (values()) {
                    BOOST_CHECK(*s == results[v++]);
                }
            }
        }

        BOOST_CHECK(!g1.defined() && !g2.defined() && !s.defined() && !num.defined());
    }

    { // empty input range
        lref<std::string> num;
        lref<group<char, group<std::size_t, std::string> > > g;
        relation r = item(num, numbers, numbers) >>= group_by(num, firstChar, g).then(slength);

        BOOST_CHECK(!r());
    }

    return 0;
}
