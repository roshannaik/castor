#include <boost/castor.h>


using namespace castor;

int getNIntResult = 0;
double getNDblResult = 0;

int getN(void)                             { return getNIntResult = 0; }
int getN(int)                              { return getNIntResult = 1; }
int getN(int, int)                         { return getNIntResult = 2; }
int getN(int, int, int)                    { return getNIntResult = 3; }
int getN(int, int, int, int)               { return getNIntResult = 4; }
int getN(int, int, int, int, int)          { return getNIntResult = 5; }
int getN(int, int, int, int, int, int)     { return getNIntResult = 6; }

double getN(double)                                             { return getNDblResult = -1; }
double getN(double, double)                                     { return getNDblResult = -2; }
double getN(double, double, double)                             { return getNDblResult = -3; }
double getN(double, double, double, double)                     { return getNDblResult = -4; }
double getN(double, double, double, double, double)             { return getNDblResult = -5; }
double getN(double, double, double, double, double, double)     { return getNDblResult = -6; }


int getN2Result = 0;
double getN2(double)                                             { return getN2Result = -1; }
double getN2(double, lref<double>)                               { return getN2Result = -2; }
double getN2(lref<double>, double, double)                       { return getN2Result = -3; }
double getN2(double, double, double, double)                     { return getN2Result = -4; }
double getN2(double, double, double, double, double)             { return getN2Result = -5; }
double getN2(double, double, double, double, double, double)     { return getN2Result = -6; }

struct Functor6 {
    typedef int result_type;
    static int result;

    int operator() (void)                                           { return result = 0; }
    int operator() (int i1)                                         { return result = i1; }
    int operator() (int i1, int i2)                                 { return result = i1 + i2; }
    int operator() (int i1, int i2, int i3)                         { return result = i1 + i2 + i3; }
    int operator() (int i1, int i2, int i3, int i4)                 { return result = i1 + i2 + i3 + i4; }
    int operator() (int i1, int i2, int i3, int i4, int i5)         { return result = i1 + i2 + i3 + i4 + i5; }
    int operator() (int i1, int i2, int i3, int i4, int i5, int i6) { return result = i1 + i2 + i3 + i4 + i5 + i6; }

    int method (void)                                               { return result = 0; }
    int method (int i1)                                             { return result = i1; }
    int method (int i1, int i2)                                     { return result = i1 + i2; }
    int method (int i1, int i2, int i3)                             { return result = i1 + i2 + i3; }
    int method (int i1, int i2, int i3, int i4)                     { return result = i1 + i2 + i3 + i4; }
    int method (int i1, int i2, int i3, int i4, int i5)             { return result = i1 + i2 + i3 + i4 + i5; }
    int method (int i1, int i2, int i3, int i4, int i5, int i6)     { return result = i1 + i2 + i3 + i4 + i5 + i6; }

    int max(int i, int j)          { return (i > j) ? i : j; }
    double max(double i, double j) { return (i > j) ? i : j; }

    int cmax(int i, int j) const   { return (i > j) ? i : j; }

};

int Functor6::result = 6;
