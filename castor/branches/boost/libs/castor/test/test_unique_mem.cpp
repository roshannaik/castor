#include <boost/castor.h>

#include <boost/test/minimal.hpp>


using namespace castor;

struct person {
    std::string firstName, lastName;
    person (std::string firstName, std::string lastName) : firstName(firstName), lastName(lastName)
	{ }
    bool operator==(const person& rhs) const {
        return (firstName == rhs.firstName) && (lastName == rhs.lastName);
    }
};

int test_main(int, char * [])
{
    {
        person people[] = { person("roshan", "naik"), person("runa", "naik"), person("harry", "potter") };
        lref<person> p;
	lref<int> c;
	relation r =
            (item(p, people, people + 3) && unique_mem(p, &person::lastName) >>= count(c)) &&
            predicate(c == 2);

	BOOST_CHECK(r());
    }

    return 0;
}
