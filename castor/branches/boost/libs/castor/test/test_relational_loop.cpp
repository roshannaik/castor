#include <boost/castor.h>

#include <boost/test/minimal.hpp>

#include <stack>

#include "test_family.h"


using namespace castor;

bool push(std::stack<std::string>& coll, std::string val) {
    coll.push(val);
    return true;
}

int test_main(int, char * [])
{
    {
        lref<std::string> ch, f;
        lref<std::stack<std::string> > lst = std::stack<std::string>();
        relation r = child(ch, "mother1", f) && predicate(&push, lst, ch) && False();

        BOOST_CHECK(!r());

        BOOST_CHECK(lst->size() == 2);
        BOOST_CHECK(lst->top() == "daughter1");

        lst->pop();

        BOOST_CHECK(lst->top() == "son1");
    }

    return 0;
}
