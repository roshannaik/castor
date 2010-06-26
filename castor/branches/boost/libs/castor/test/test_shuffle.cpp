#include <boost/castor.h>

#include <boost/test/minimal.hpp>


using namespace castor;

int test_main(int, char * [])
{
    { // generate mode, lref<Itr>
        lref<int> c;
        lref<std::string> s = "hello", ss;
        lref<std::string::iterator> sb = s->begin(), se = s->end();
        relation r = zip(True(20), shuffle(sb, se, ss)) >>= count(c);

        BOOST_CHECK(r());

        BOOST_CHECK(*c == 20);

        BOOST_CHECK(!r() && !c.defined() && ss.defined());
    }

    { // generate mode, Itr
        lref<int> c;
        lref<std::string> s = "hello", ss;
        std::string::iterator sb = s->begin(), se = s->end();
        relation r = zip(True(20), shuffle(sb, se, ss)) >>= count(c);

        BOOST_CHECK(r());

        BOOST_CHECK(*c == 20);

        BOOST_CHECK(!r() && !c.defined() && ss.defined());
    }	

    { // test mode
        lref<int> c;
        lref<std::string> s = "hello", ps;
        lref<std::string::iterator> sb = s->begin(), se = s->end();
        relation r = (permute(s, ps) && shuffle(ps, s)) >>= count(c);

        BOOST_CHECK(r());

        BOOST_CHECK(*c == 60);

        BOOST_CHECK(!r() && !c.defined() && !ps.defined());
    }
    { // gen mode - const container, const_iterator
        lref<int> c;
        const std::string s = "hello";
        lref<const std::string> ls(&s,false);
        lref<std::string> sh;
        relation r = shuffle(s.begin(), s.end(), sh); //const_iterator
        BOOST_CHECK(r());
        sh.reset();
        r = shuffle(ls,sh);  // const container
        BOOST_CHECK(r());
    }
    return 0;
}
