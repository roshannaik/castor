#include <boost/castor.h>

#include <boost/test/minimal.hpp>


using namespace castor;

int test_main(int, char * [])
{
    {
        lref<std::string> s="ab", ps;
        relation r = permutation_cmp(s,ps,std::less<char>() );
        BOOST_CHECK(r());
        BOOST_CHECK(*ps=="ba");
        BOOST_CHECK(r());
        BOOST_CHECK(*ps=="ab");
    }
    { // test mode 
        std::string s="ba";
        lref<std::vector<char> > ps;
        relation r = permutation_cmp(s.begin(), s.end(), ps, std::less<char>() ) ;
        BOOST_CHECK(r());
        BOOST_CHECK((*ps)[0]=='b');
        BOOST_CHECK((*ps)[1]=='a');
        BOOST_CHECK(r());
        BOOST_CHECK((*ps)[0]=='a');
        BOOST_CHECK((*ps)[1]=='b');
    }
    { // gen mode - greater<>
        lref<std::string> s="ab", ps;
        relation r = permutation_cmp(s, ps, std::greater<char>() ) ;
        BOOST_CHECK(r());
        BOOST_CHECK(*ps=="ab");
        BOOST_CHECK(r());
        BOOST_CHECK(*ps=="ba");
    }

    return 0;
}
