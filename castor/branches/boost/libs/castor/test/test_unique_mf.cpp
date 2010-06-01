#include <boost/castor.h>

#include <boost/test/minimal.hpp>


using namespace castor;

int test_main(int, char * [])
{
    {
        std::string words[] = {"mary", "had", "a", "little", "lamb"};
        lref<std::string> w;
        lref<int> c;
	relation r =
            (item(w, words, words + 5) && unique_mf(w, &std::string::length) >>= count(c)) &&
            predicate(c == 4);

	BOOST_CHECK(r());
    }

    return 0;
}
