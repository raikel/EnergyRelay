/*
 * Channel gain from source to destination nodes
 *
 * Calling syntax:
 *
 *   channels = getchannels(params)
 *
 * It will diagnose the MAT-file named <matfile>.
 *
 */

#include <math.h>
#include "mex.h"
#include "utils.h"

void mexFunction( int nlhs, mxArray *plhs[], 
		  int nrhs, const mxArray*prhs[] )
{ 
  /* Input parameters */
	mwSize	ns,   /* Number of source nodes */
					nd;		/* Number of destination nodes */
  double *spos, /* Source nodes positions   */ 
         *dpos;	/* Destination nodes positions      */
  double alpha, beta; /* Path loss exponent */  
	/* Output paremeters */
  double *channels; /* Channel between source and destination nodes */	
  /* Auxiliary variables */
  mwIndex i, j, n;
  double d1, d2;
  double *pos1, *pos2;
  mxArray *array;
  
 /* Check prper number of input and output arguments*/
 if (nrhs != 1) { 
    mexErrMsgTxt("One input arguments required."); 
  } else if (nlhs > 1) {
    mexErrMsgTxt("Too many output arguments.");
  }	
  // Input
  array = checkParam(prhs[0], "spos", "double", 0, true);
	ns = mxGetNumberOfElements(array)/2;
  spos = mxGetPr(array);  
  array = checkParam(prhs[0], "dpos", "double", 0, true);
  nd = mxGetNumberOfElements(array)/2;
	dpos = mxGetPr(array);  
	alpha = mxGetScalar( checkParam(prhs[0], "alpha", "double", 1, true) );
  beta = mxGetScalar( checkParam(prhs[0], "beta", "double", 1, true) );
	// Output
  plhs[0] = mxCreateDoubleMatrix(ns, nd, mxREAL);
	channels = mxGetPr(plhs[0] );	
  /* Main loop */
	n = 0;
	for(i = 0; i < nd; i++) {
    for(j = 0; j < ns; j++) {        
      // jth source position
      pos1 = &spos[j*2];
      // ith destination position
      pos2 = &dpos[i*2];
      //
      d1 = pos1[0] - pos2[0];
      d2 = pos1[1] - pos2[1];
      channels[n] = beta*pow(d1*d1 + d2*d2, -alpha/2);
      n++;       
    }        
  }	
  return;    
}
