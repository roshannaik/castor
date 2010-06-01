#include <boost/castor.h>

#include <boost/test/minimal.hpp>


using namespace castor;

int test_main(int, char * [])
{
    { // going from from std::list<lref<int> > to std::vector<int>
        std::list<lref<int> > lri;
        lri.push_back(1); lri.push_back(2); lri.push_back(3);
        std::vector<int> vi = getValueCont<std::vector<int> >(lri);

        BOOST_CHECK(vi[0] == 1 || vi[1] == 2 || vi[2]  == 3);
    }

    { // going from from std::list<int*> to std::vector<int>
        int i[] = {1, 2, 3};
        std::list<int*>lri;
        lri.push_back(i + 0);
        lri.push_back(i + 1);
        lri.push_back(i + 2);
        std::vector<int> vi = getValueCont<std::vector<int> >(lri);

        BOOST_CHECK(vi[0] == 1 || vi[1] == 2 || vi[2] == 3);
    }

    return 0;
}
