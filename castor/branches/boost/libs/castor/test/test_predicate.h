#include <boost/castor.h>

#include <boost/test/minimal.hpp>


using namespace castor;

bool isEven(int num) {
    return num % 2 == 0;
}

bool isEven_lref(lref<int> num) {
    return (*num) % 2 == 0;
}

struct Functor {
    int j;
    bool t, f;

    Functor() : j(4), t(true), f(false) {}

    bool isEven(int i) {
        return (i + j) % 2 == 0;
    }

    bool isEven_c(int i) const {
        return i % 2 == 0;
    }

    bool allEqual(int i, const int j, int k, int l, lref<int> m, const int n) const {
        return i == j == k == l == *m == n;
    }

    bool ternaryConstPred(int, const int, int) const {
        return true;
    }

    static bool staticMembPred(int) {
        return true;
    }
};

bool myPred (void)                                               { return true; }
bool myPred (int i1)                                             { return 0 < i1; }
bool myPred (int i1, int i2)                                     { return myPred(i1) && i1 < i2; }
bool myPred (int i1, lref<int> i2, int i3)                       { return myPred(i1, *i2) && myPred(*i2, i3); }
bool myPred (int i1, int i2, int i3, int i4)                     { return myPred(i1, i2, i3) && myPred(i3, i4); }
bool myPred (int i1, int i2, int i3, int i4, int i5)             { return myPred(i1, i2, i3, i4) && myPred(i4, i5); }
bool myPred (int i1, int i2, int i3, int i4, int i5, int i6)     { return myPred(i1, i2, i3, i4, i5) && myPred(i5, i6); }


struct MyPred {
    bool operator() (void)                                           { return myPred(); }
    bool operator() (int i1)                                         { return myPred(i1); }
    bool operator() (int i1, int i2)                                 { return myPred(i1, i2); }
    bool operator() (int i1, lref<int> i2, int i3)                   { return myPred(i1, *i2, i3); }
    bool operator() (int i1, int i2, int i3, int i4)                 { return myPred(i1, i2, i3, i4); }
    bool operator() (int i1, int i2, int i3, int i4, int i5)         { return myPred(i1, i2, i3, i4, i5); }
    bool operator() (int i1, int i2, int i3, int i4, int i5, int i6) { return myPred(i1, i2, i3, i4, i5, i6); }

    bool method (void)                                               { return myPred(); }
    bool method (int i1)                                             { return myPred(i1); }
    bool method (int i1, int i2)                                     { return myPred(i1, i2); }
    bool method (int i1, lref<int> i2, int i3)                       { return myPred(i1, *i2, i3); }
    bool method (int i1, int i2, int i3, int i4)                     { return myPred(i1, i2, i3, i4); }
    bool method (int i1, int i2, int i3, int i4, int i5)             { return myPred(i1, i2, i3, i4, i5); }
    bool method (int i1, int i2, int i3, int i4, int i5, int i6)     { return myPred(i1, i2, i3, i4, i5, i6); }
};
