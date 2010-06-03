#include <boost/castor.h>

#include <boost/test/minimal.hpp>


using namespace castor;

// (mother1, father1) => (son1 and daughter1)
// (mother2, father2) => (son2 and daughter2)
// (gmother2, gfather2) => (father2)
relation child(lref<std::string> progeny, lref<std::string> mother, lref<std::string> father) {
    return eq(progeny, "son1")      &&  eq(mother, "mother1")  && eq(father, "father1")
        || eq(progeny, "daughter1") &&  eq(mother, "mother1")  && eq(father, "father1")
        || eq(progeny, "son2")      &&  eq(mother, "mother2")  && eq(father, "father2")
        || eq(progeny, "daughter2") &&  eq(mother, "mother2")  && eq(father, "father2")
        || eq(progeny, "father2")   &&  eq(mother, "gmother2") && eq(father, "gfather2");
}

relation mother(lref<std::string> ch, lref<std::string> mom) {
    lref<std::string> x;
    return child(ch, mom, x);
}

relation father(lref<std::string> ch, lref<std::string> pop) {
    lref<std::string> x;
    return child(ch, x, pop);
}

relation parent(lref<std::string> par, lref<std::string> kid) {
    return father(kid, par) || mother(kid, par);
}

relation ancestor(lref<std::string> ans, lref<std::string> des) {
    lref<std::string> X;
    return parent(ans, des) || parent(X, des) && recurse(ancestor, ans, X);
}

relation spouse(lref<std::string> husband, lref<std::string> wife) {
    lref<std::string> child;
    return father(child, husband) && mother(child, wife);
}
