#include <boost/castor.h>

#include <boost/test/minimal.hpp>

#include "test_family.h"


using namespace castor;

int test_main(int, char * [])
{
    {
        lref<std::string> h, w;
        relation r = spouse(h, w);
        std::string expectedH[] = {"father1", "father1", "father2", "father2", "gfather2"};
        std::string expectedW[] = {"mother1", "mother1", "mother2", "mother2", "gmother2"};
        int i = 0;
        for(; r(); ++i) {
            BOOST_CHECK(i <= 4 && *h == expectedH[i] && *w == expectedW[i]);
        }

        BOOST_CHECK(i != 0 && !h.defined() && !w.defined());
    }

    {
        lref<std::string> w, ch;
        relation r = mother(ch, w);
        std::string expected[] = {"mother1", "mother1", "mother2", "mother2", "gmother2"};
        int i = 0;
        for(; r(); ++i) {
            BOOST_CHECK(i <= 4 && expected[i] == *w);
        }

        BOOST_CHECK(i != 0 && !w.defined() && !ch.defined());
    }

    {
        lref<std::string> w;
        relation r = spouse("father1", w);
        std::string expected[] = {"mother1", "mother1"};
        int i = 0;
        for(; r(); ++i) {
            BOOST_CHECK(i <= 1 && expected[i] == *w);
        }

        BOOST_CHECK(i != 0 && !w.defined());
    }

    {
        lref<std::string> h;
        relation r = spouse(h, "mother2");
        std::string expected[] = {"father2", "father2"};
        int i = 0;
        for(; r(); ++i) {
            BOOST_CHECK(i <= 1 && expected[i] == *h);
        }

        BOOST_CHECK(i != 0 && !h.defined());
    }

    {
        lref<std::string> ch;
        relation r = child(ch, "mother1", "father1");

        std::string expected[] = { "son1", "daughter1"};
        int i = 0;
        for(; r(); ++i) {
            BOOST_CHECK(i <= 1 && expected[i] == *ch);
        }

        BOOST_CHECK(i != 0 && !ch.defined());
    }

    {
        relation r = child("daughter1", "mother1", "father1");

        BOOST_CHECK(r());
    }

    {
        lref<std::string> p;
        relation r = parent(p, "son1");
        std::string expected[] = { "father1", "mother1"};
        int i = 0;
        for(; r(); ++i) {
            BOOST_CHECK(i <= 1 && expected[i] == *p);
        }

        BOOST_CHECK(i != 0 && !p.defined());
    }

    {
        lref<std::string> a;
        relation r = ancestor(a, "son2");
        std::string expected[] = { "father2", "mother2", "gfather2", "gmother2" };
        int i = 0;
        for(; r(); ++i) {
            BOOST_CHECK(i <= 3 && *a == expected[i]);
        }

        BOOST_CHECK(i != 0 && !a.defined());
    }

    {
        lref<std::string> a;
        relation r = ancestor("gfather2", "son2");

        BOOST_CHECK(r());
    }

    return 0;
}
