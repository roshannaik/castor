#include <boost/castor.h>

#include <boost/test/minimal.hpp>


using namespace castor;

struct Nfa {
    // all tranistions in the NFA => ((ab) * ba) | b
    static relation transition(lref<int> state1, lref<char> input, lref<int> state2) {
        return eq(state1, 0) && eq(input, 'a') && eq(state2, 1)
            || eq(state1, 0) && eq(input, 'b') && eq(state2, 2)
            || eq(state1, 0) && eq(input, 'b') && eq(state2, 4)
            || eq(state1, 1) && eq(input, 'b') && eq(state2, 0)
            || eq(state1, 2) && eq(input, 'a') && eq(state2, 3);
    }

    // all final states of the NFA
    static relation final(lref<int> state) {
        return eq(state, 3) || eq(state, 4);
    }
};

// rule determining successful exuecution of a FA
relation runNfa(lref<std::string> input, lref<int> startState = 0)
{
    lref<char> firstChar;
    lref<std::string> rest;
    lref<int> nextState;
#ifdef __BCPLUSPLUS__
    relation (*self)(lref<std::string>, lref<int>) = &runNfa;
    return eq(input, "") && Nfa::final(startState)
        || head(input, firstChar) && Nfa::transition(startState, firstChar, nextState) && tail(input, rest) && recurse(self, rest, nextState);
#else
    return eq(input, "") && Nfa::final(startState)
        || head(input, firstChar) && Nfa::transition(startState, firstChar, nextState) && tail(input, rest) && recurse(runNfa, rest, nextState);
#endif
}

int test_main(int, char * [])
{
    BOOST_CHECK(runNfa("abba")());
    BOOST_CHECK(!runNfa("aabba")());

    return 0;
}
