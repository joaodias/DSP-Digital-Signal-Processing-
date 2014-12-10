/************************************************************/
/*  Function fft(complex *X, int M)                         */
/*                                                          */
/*   This is an elementary, complex, radix 2, decimation in */
/*  time FFT.  The computations are performed "in place" and */
/*  the output overwrites the input array.                  */
/************************************************************/

#include "conf.h"/* Definition of complex variable structure */
#include <math.h>   /* Definitions for math library             */

void fft(complex *X, int M)
         /* X is an array of N = 2**M complex points. */
	   /* M = log2(N), N is the number of points */
  {
  complex temp1;          /* temporary storage complex variable */
  complex W;              /* e**(-j 2 pi/ N)                    */
  complex U;              /* Twiddle factor W**k                */
  int i,j,k;              /* loop indexes                       */
  int id;                 /* Index of lower point in butterfly  */
  int N = 1 << M;         /* Number of points for FFT           */
  int N2 = N/2;
  int L;                  /* FFT stage                          */
  int LE;           /* Number of points in sub DFT at stage L,  */
                    /* and offset to next DFT in stage          */
  int LE1;                /* Number of butterflys in one DFT at */
                          /*  stage L. Also is offset to lower  */
                          /*  point in butterfly at stage L     */
  float pi = 3.1415926535897;

/*==============================================================*/
/*  Rearrange input array in bit-reversed order                 */
/*                                                              */
/*    The index j is the bit reversed value of i.  Since 0 -> 0 */
/*  and N-1 -> N-1 under bit-reversal, these two reversals are  */
/*  skipped.                                                    */

  j = 0;
  for(i=1; i<(N-1); i++)
    {

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*  Increment bit-reversed counter for j by adding 1 to msb and */
/*   propagating carries from left to right.                    */
                                            
    k = N2;               /* k is 1 in msb, 0 elsewhere         */

/*--------------------------------------------------------------*/
/*  Propagate carry from left to right                          */

    while(k<=j)           /* Propagate carry if bit is 1        */
      {
      j = j - k;          /* Bit tested is 1, so clear it.      */
      k = k/2;            /* Set up 1 for next bit to right.    */
      }
    j = j+k;              /* Change 1st 0 from left to 1        */
/*--------------------------------------------------------------*/
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

/*  Swap samples at locations i and j if not previously swapped.*/

    if(i<j)               /* Test if previously swapped.        */
      {
      temp1.real = (X[j]).real;
      temp1.imag = (X[j]).imag;
      (X[j]).real = (X[i]).real;
      (X[j]).imag = (X[i]).imag;
      (X[i]).real = temp1.real;
      (X[i]).imag = temp1.imag;
      }
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

    }

/*==============================================================*/
/*  Do M stages of butterflys                                   */

  for(L=1; L<= M; L++)
    {
    LE = 1 << L;            /*  LE = 2**L = points is sub DFT   */
    LE1 = LE/2;             /* Number of butterflys in sub-DFT  */
    U.real = 1.0;
    U.imag = 0.0;           /* U = 1 + j 0                      */
    W.real = cos(pi/LE1);
    W.imag = - sin(pi/LE1); /* W = e**(-j 2 pi/LE)              */  
/*--------------------------------------------------------------*/
/*   Do butterflys for L-th stage                               */

    for(j=0; j<LE1; j++)    /* Do the LE1 butterflys per sub DFT*/
      {
/*..............................................................*/
/*    Compute butterflys that use same W**k                     */
 
      for(i=j; i<N; i += LE)
        {
        id = i + LE1;      /* Index of lower point in butterfly */
        temp1.real = (X[id]).real*U.real - (X[id]).imag*U.imag;
        temp1.imag = (X[id]).imag*U.real + (X[id]).real*U.imag;

        (X[id]).real = (X[i]).real - temp1.real;
        (X[id]).imag = (X[i]).imag - temp1.imag;

        (X[i]).real  = (X[i]).real + temp1.real;
        (X[i]).imag  = (X[i]).imag + temp1.imag;
        }
/*..............................................................*/
/*   Recursively compute W**k as W*W**(k-1) = W*U               */

      temp1.real = U.real*W.real - U.imag*W.imag;
      U.imag = U.real*W.imag + U.imag*W.real;
      U.real = temp1.real;
/*..............................................................*/
      }
/*--------------------------------------------------------------*/
    }
  return;
  }

