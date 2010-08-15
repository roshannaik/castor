#include "test_eq.h"

#include <boost/castor.h>

#include <boost/test/minimal.hpp>

#include <vector>


using namespace castor;

namespace {
struct Base {
    int b;
    Base():b(0) {}
    int foo(){ return 1; }
    int foo_base(){ return 2; }
};
struct Derived : Base {
    int d;
    Derived() : d(10), Base() {}
    int foo() { return 3; }
    int foo_derived(){ return 4; }
};


struct employee {
    std::string name;
    int salary;
    employee(std::string name, int salary) : name(name), salary(salary) {}
    bool operator== (const employee& rhs) const {
        return name == rhs.name && salary == rhs.salary;
    }
};

} // namespace 

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
        lref<const std::pair<int,std::string> > p =std:: pair<int,std::string>(1,"Roshan");
        relation r = eq_mem<int,const std::pair<int,std::string> >(1, p,&std::pair<int,std::string>::first);
        int i=0;
        for(; r(); ++i);
        BOOST_CHECK(i==1);
    }
    {// members of base type
        lref<Derived> d = Derived();
        BOOST_CHECK(eq_mem<int>(0,d, &Base::b)());

        BOOST_CHECK(eq_mem<int>(0,d, &Derived::b)());

        BOOST_CHECK(eq_mem<int>(10,d, &Derived::d)());
    }
    { // on "string" literals
        lref<pair<string,string> > p = make_pair("Castor","C++");
        BOOST_CHECK( eq_mem<string>("Castor",p, &pair<string,string>::first)() );
        BOOST_CHECK( eq_mem("Castor",p, &pair<string,string>::first)() );
    }
    return 0;
}
