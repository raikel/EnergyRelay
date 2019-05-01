#ifndef utils_h
#define utils_h

#include <math.h>
#include "mex.h"
#include "matrix.h"
#include <string.h>
#include <limits.h>

#define DOUBLE_TYPE 0
#define INT_TYPE    1
#define UINT_TYPE   2


double xMin(double, double);
double xMax(double, double);
int randui(int, int);
double randud(double, double);
double randexp(double);
double randnd(double, double);
void fill(double *, double *, mwSize);
double *eyeDouble(mwSize);
bool *eyeBool(mwSize);
unsigned int *eyeUint(mwSize);
double * zerosDouble(mwSize, mwSize);
bool* zerosBool(mwSize, mwSize);
int * zerosInt(mwSize, mwSize);
unsigned int * zerosUint(mwSize, mwSize);
double *onesDouble(mwSize, mwSize);
int *double2int(double *, mwSize );
unsigned int *double2uint(double *, mwSize );
void mxPrintDoubleMatrix(const char *, double *, mwSize , mwSize, const char * );
void mxPrintUIntMatrix(const char *, unsigned int *, mwSize , mwSize, const char * );
mxArray *checkParam(mxArray *, const char *, const char *, const mwIndex, bool);
mwSize nchoosek(mwSize, mwSize);
void combnk(mwSize, mwSize, mwIndex *);

#endif
