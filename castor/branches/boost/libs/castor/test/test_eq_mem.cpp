#include "test_eq.h"

#include <boost/castor.h>

#include <boost/test/minimal.hpp>

#include <vector>


using namespace castor;


struct employee {
    std::string name;
    int salary;
    employee(std::string name, int salary) : name(name), salary(salary) {}
    bool operator== (const employee& rhs) const {
        return name == rhs.name && salary == rhs.salary;
    }
};

int test_main(int, char * [])
{
    { // compare
        lref<std::pair<int, std::string> > p = std::pair<int, std::string>(1, "Roshan");
        lref<int> one = 1;
        relation r = eq_mem(one, p, &std::pair<int, std::string>::first);

	int i = 0;
	for (; r(); ++i) {
            BOOST_CHECK(i <= 0 && *one == 1);
        }

	BOOST_CHECK(i == 1 && one.defined());
    }

    { // generate
        lref<std::pair<int, std::string> > p = std::pair<int, std::string>(1, "Roshan");
        lref<std::string> name;
        relation r = eq_mem(name, p, &std::pair<int, std::string>::second);

	int i = 0;
	for (; r(); ++i) {
            BOOST_CHECK(i <= 0 && *name == "Roshan");
        }

	BOOST_CHECK(i == 1 && !name.defined());
    }

    {
        std::list<employee> employees; employees.push_back(employee("Roshan", 50)); employees.push_back(employee("Roshan", 90));
        lref<int> salary;
        lref<employee> e;
        relation salaries = item(e, employees.begin(), employees.end()) && eq_mem(salary, e, &employee::salary);

        int total = 0;
        while (salaries())
            total += *salary;

        BOOST_CHECK(total == 140);
    }
    { // with const Obj- explicit template args
        lref<const pair<int,string> > p = pair<int,string>(1,"Roshan");
        relation r = eq_mem<int,const pair<int,string> >(1, p,&pair<int,string>::first);
	    int i=0;
	    for(; r(); ++i);
	    BOOST_CHECK(i==1);
    }

    return 0;
}
