#include <boost/castor.h>

#include <boost/test/minimal.hpp>


using namespace castor;

int test_main(int, char * [])
{
    { // basic compilation checks only
        lref<std::string> s;
        lref<std::vector<std::string> > names;
        relation r = item(s, names) && pause_f(s + "\n");
    }

    return 0;
}
