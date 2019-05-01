#include "utils.h"

double xMax(double a, double b)
{
	if(a > b) {
    return a;
  }	else {
    return b;
  }
}

double xMin(double a, double b)
{
	if(a < b) {
    return a;
  }	else {
    return b;
  }
}

// Uniform integer random number generator
int randui(int xMin, int xMax)
{
    return ( rand() % (xMax + 1) + xMin );
}

// Uniform double random number generator
double randud(double xMin, double xMax)
{
	double u = ( (double)( rand() ) )/(RAND_MAX + 1);
    return ( u*(xMax - xMin) + xMin );
}

// Exponential double random number generator
double randexp(double mu)
{
  return -mu*log( randud(0,1) );
}

// Normal double random number generator
double randnd(double xMean, double xStd)
{
  static double V1, V2, S;
  double X;
	
  do {
    double U1 = (double)rand() / RAND_MAX;
    double U2 = (double)rand() / RAND_MAX;

    V1 = 2 * U1 - 1;
    V2 = 2 * U2 - 1;
    S = V1 * V1 + V2 * V2;
  } while(S >= 1 || S == 0);
  
  X = V1 * sqrt(-2 * log(S) / S);
  
  return (X*xStd + xMean);
}


// Uniform double random number generator
void fill(double *y, double *x, mwSize n)
{
  mwIndex i;
  
	for(i = 0; i < n; i++) 
  {
    y[i] = x[i];
  }
}

double * eyeDouble(mwSize N)
{
  mwIndex i, j;
  double *x = mxCalloc(N*N, sizeof(double) );
  
  for(i = 0; i < N; i++) {
    for(j = 0; j < N; j++) {
      if (j == i) {
        x[i*N + j] = 1;
      } else {
        x[i*N + j] = 0;
      }
    }
  }
  return x;
}

unsigned int * eyeUint(mwSize N)
{
  mwIndex i, j;
  unsigned int *x = mxCalloc(N*N, sizeof(unsigned int) );
  
  for(i = 0; i < N; i++) {
    for(j = 0; j < N; j++) {
      if (j == i) {
        x[i*N + j] = 1;
      } else {
        x[i*N + j] = 0;
      }
    }
  }
  return x;
}

bool * eyeBool(mwSize N)
{
  mwIndex i, j;
  bool *x = mxCalloc(N*N, sizeof(double) );
  
  for(i = 0; i < N; i++) {
    for(j = 0; j < N; j++) {
      if (j == i) {
        x[i*N + j] = true;
      } else {
        x[i*N + j] = false;
      }
    }
  }
  return x;
}

int * zerosInt(mwSize M, mwSize N)
{
  mwIndex i, j;
  int *x = mxCalloc(N*M, sizeof(int) );
  
  for(i = 0; i < M; i++) {
    for(j = 0; j < N; j++) {
      x[i*N + j] = 0;
    }
  }
  
  return x;
}

unsigned int * zerosUint(mwSize M, mwSize N)
{
  mwIndex i, j;
  unsigned int *x = mxCalloc(N*M, sizeof(unsigned int) );
  
  for(i = 0; i < M; i++) {
    for(j = 0; j < N; j++) {
      x[i*N + j] = 0;
    }
  }
  
  return x;
}

bool * zerosBool(mwSize M, mwSize N)
{
  mwIndex i, j;
  bool *x = mxCalloc(N*M, sizeof(bool) );
  
  for(i = 0; i < M; i++) {
    for(j = 0; j < N; j++) {
      x[i*N + j] = false;
    }
  }
  
  return x;
}

double * zerosDouble(mwSize M, mwSize N)
{
  mwIndex i, j;
  double *x = mxCalloc(N*M, sizeof(double) );
  
  for(i = 0; i < M; i++) {
    for(j = 0; j < N; j++) {
      x[i*N + j] = 0;
    }
  }
  
  return x;
}

double * onesDouble(mwSize M, mwSize N)
{
  mwIndex i, j;
  double *x = mxCalloc(N*M, sizeof(double) );
  
  for(i = 0; i < M; i++) {
    for(j = 0; j < N; j++) {
      x[i*N + j] = 1;
    }
  }  
  return x;
}

int *double2int(double *X, mwSize N) 
{
  mwIndex i;
  int *Y = mxCalloc(N, sizeof(int) );
  for(i = 0; i < N; i++) {
    Y[i] = (int)(X[i]);
  }
  return Y;
}

unsigned int *double2uint(double *X, mwSize N) 
{
  mwIndex i;
  unsigned int *Y = mxCalloc(N, sizeof(unsigned int) );
  for(i = 0; i < N; i++) {
    Y[i] = (unsigned int)(X[i]);
  }
  return Y;
}

mxArray *checkParam( mxArray       *options,
                     const char    *paramName, 
                     const char    *paramClass,
                     const mwIndex  paramLength,
                     bool           required) {
  // Get pointer to param
  mxArray *param = mxGetField(options, 0, paramName);
  if( param != NULL ) {
    // Check param class
    if ( !mxIsClass(param, paramClass) ){
      mexErrMsgIdAndTxt("checkparam:error", "Param %s must be %s type", 
                        strupr(paramName), 
                        strupr(paramClass) );                
    }
    // Check param length is necessary
    if ( (paramLength != 0) && 
      (mxGetNumberOfElements(param) != paramLength) ) {
        mexErrMsgIdAndTxt("checkparam:error", "Param %s must have %d elements", 
                          strupr(paramName),
                          paramLength );
    }
  } else if ( param == NULL && required ) {
    mexErrMsgIdAndTxt("checkparam:error", "Param %s required", strupr(paramName));
  }
  return param;
}

// Print matrix
void mxPrintDoubleMatrix(const char *name, double *x, mwSize M, mwSize N, const char *format) {
  mwIndex i, j;
  mexPrintf("\n ");
  mexPrintf(name);
  mexPrintf(" = \n");
  for (i =0; i<M; i++) {
    for (j =0; j<N; j++) {
      mexPrintf(format, x[i*N + j]); // ej. format: "%e ", "%d "
    }
    mexPrintf("\n");
  }
  mexPrintf("\n");
}

// Print matrix
void mxPrintUIntMatrix(const char *name, unsigned int *x, mwSize M, mwSize N, const char *format) {
  mwIndex i, j;
  mexPrintf("\n ");
  mexPrintf(name);
  mexPrintf(" = \n");
  for (i =0; i<M; i++) {
    for (j =0; j<N; j++) {
      mexPrintf(format, x[i*N + j]); // ej. format: "%e ", "%d "
    }
    mexPrintf("\n");
  }
  mexPrintf("\n");
}

// Print matrix
void mxPrintIntMatrix(const char *name, int *x, mwSize M, mwSize N, const char *format) {
  mwIndex i, j;
  mexPrintf("\n ");
  mexPrintf(name);
  mexPrintf(" = \n");
  for (i =0; i<M; i++) {
    for (j =0; j<N; j++) {
      mexPrintf(format, x[i*N + j]); // ej. format: "%e ", "%d "
    }
    mexPrintf("\n");
  }
  mexPrintf("\n");
}

mwSize nchoosek(mwSize n, mwSize k) {
  mwSize d, r = 1;
  if (k == 0) return 1;
  if (k == 1) return n;
  if (k >= n) return (k == n);
  if (k > n/2) k = n-k;
  for (d = 1; d <= k; d++) {
    if (r >= INT_MAX/n) {
      mexErrMsgIdAndTxt("nchoosek:error", "Overflow");
    } else {
      r *= n--;
      r /= d;
    }
  }
  return r;
}

void combnk(mwSize m, mwSize n, mwIndex *x)
{
	int i, k;
  mwIndex *c;
  
  c = mxCalloc(n, sizeof(mwIndex) );  
	k = 0;
  for (i = 0; i < n; i++) {c[i] = n - i;}
 
	while (1) {
		for (i = n; i--;) {
      x[k] = c[i] - 1;
      k++;
			printf("%d%c", c[i]-1, i ? ' ': '\n');
    }
		/* this check is not strictly necessary, but if m is not close to n,
		   it makes the whole thing quite a bit faster */
		if (c[i]++ < m) continue;
 
		for (i = 0; c[i] >= m - i;) if (++i >= n) return;
		for (c[i]++; i; i--) c[i-1] = c[i] + 1;
	}
  
  mxFree(c);
}

