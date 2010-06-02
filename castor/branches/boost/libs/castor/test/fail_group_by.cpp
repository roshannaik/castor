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

int test_main(int, char * [])
{
    { // nested grouping (3 levels)
        lref<std::string> num;
        lref<group<std::size_t, group<bool, std::string> > > g1;
        lref<group<bool, std::string> > g2;

        // the following line must emit compiler error - "then() is not a member of .. "
        group_by(num, &firstChar, g1).then(slength).then(palind);  
    }

    return 0;
}
