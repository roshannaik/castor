#include <boost/castor.h>

#include <boost/test/minimal.hpp>


using namespace castor;

int test_main(int, char * [])
{
    { // basic compilation checks only
        pause("hello");
        std::string s("hi");
        pause(s);
        lref<std::string> ls = "blah";
        pause(ls);
    }

    return 0;
}
