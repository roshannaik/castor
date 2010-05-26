#if !defined TEST_CASTOR_EQ_H
#define TEST_CASTOR_EQ_H 1

#include <string>


// structs A and B are inter convertible
struct A; 
struct B;

struct A {
    A() {}

    A(const B&) {}

    void operator=(const B&) {}

    bool operator==(const B&) const{
        return true;
    }
};

struct B {
    B() {}

    B(const A&) {}

    void operator=(const A&) {}

    bool operator==(const A&) const{
        return true;
    }
};

bool myStrCmp(const std::string& l, const std::string& r) {
    return l == r;
}

bool cmpAB(const A&, const B&) {
    return true;
}

inline 
bool cmpi(int i, int j) {
    return i == j;
}


int compute(int j, int k) {
    return j / k - 1;
}

struct Compute {
    int operator()(int j, int k) {
        return j / k - 1;
    }
};

struct Compute2 {
    int j; 

    Compute2(int j) : j(j) {}

    int apply(int k) const {
        return j / k - 1;
    }
};

struct Functor6 {
    typedef int result_type;

    int operator() (void)                                           { return 0; }
    int operator() (int i1)                                         { return i1; }
    int operator() (int i1, int i2)                                 { return i1+i2; }
    int operator() (int i1, int i2, int i3)                         { return i1+i2+i3; }
    int operator() (int i1, int i2, int i3, int i4)                 { return i1+i2+i3+i4; }
    int operator() (int i1, int i2, int i3, int i4, int i5)         { return i1+i2+i3+i4+i5; }
    int operator() (int i1, int i2, int i3, int i4, int i5, int i6) { return i1+i2+i3+i4+i5+i6; }

    int zero (void)                                                 { return 0; }

    int method (void)                                               { return 0; }
    int method (int i1)                                             { return i1; }
    int method (int i1, int i2)                                     { return i1+i2; }
    int method (int i1, int i2, int i3)                             { return i1+i2+i3; }
    int method (int i1, int i2, int i3, int i4)                     { return i1+i2+i3+i4; }
    int method (int i1, int i2, int i3, int i4, int i5)             { return i1+i2+i3+i4+i5; }
    int method (int i1, int i2, int i3, int i4, int i5, int i6)     { return i1+i2+i3+i4+i5+i6; }

    int cmethod (int i1, int i2) const                              { return i1+i2; }

    int max(int i, int j) { return (i > j) ? i : j; }
    double max(double i, double j) { return (i > j) ? i : j; }
};

int getN(void)                                                                       { return 0; }
int getN(int /*i1*/)                                                                 { return 1; }
int getN(int /*i1*/, int /*i2*/)                                                     { return 2; }
int getN(int /*i1*/, int /*i2*/, int /*i3*/)                                         { return 3; }
int getN(int /*i1*/, int /*i2*/, int /*i3*/, int /*i4*/)                             { return 4; }
int getN(int /*i1*/, int /*i2*/, int /*i3*/, int /*i4*/, int /*i5*/)                 { return 5; }
int getN(int /*i1*/, int /*i2*/, int /*i3*/, int /*i4*/, int /*i5*/, int /*i6*/)     { return 6; }


int getN2(void)                                                                                            { return 0; }
int getN2(int /*i1*/)                                                                                      { return 1; }
int getN2(int /*i1*/, int /*i2*/)                                                                          { return 2; }
int getN2(int /*i1*/, int /*i2*/, int /*i3*/)                                                              { return 3; }
int getN2(int /*i1*/, int /*i2*/, int /*i3*/, int /*i4*/)                                                  { return 4; }
int getN2(int /*i1*/, int /*i2*/, int /*i3*/, int /*i4*/, int /*i5*/)                                      { return 5; }
int getN2(int /*i1*/, int /*i2*/, int /*i3*/, int /*i4*/, int /*i5*/, int /*i6*/)                          { return 6; }

double getN2(double /*i1*/)                                                                                { return -1; }
double getN2(double /*i1*/, double /*i2*/)                                                                 { return -2; }
double getN2(double /*i1*/, double /*i2*/, double /*i3*/)                                                  { return -3; }
double getN2(double /*i1*/, double /*i2*/, double /*i3*/, double /*i4*/)                                   { return -4; }
double getN2(double /*i1*/, double /*i2*/, double /*i3*/, double /*i4*/, double /*i5*/)                    { return -5; }
double getN2(double /*i1*/, double /*i2*/, double /*i3*/, double /*i4*/, double /*i5*/, double /*i6*/)     { return -6; }

#endif
