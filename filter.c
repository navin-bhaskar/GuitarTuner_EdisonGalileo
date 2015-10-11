

/**
 * THIS IS AN AUTOMATICALLY GENERATED FILE.
 * Implements the difference equation given the input signal.
 * The a and b of the DE are defined here locally. Which means that
 * you will have to regenrate this file every time the sampling or 
 * the cut off frequency changes.
*/

static float a[] = {
+1.000000f, -0.508718f, +0.217698f, 
};

static float b[] = { 
+0.177245f, +0.354490f, +0.177245f, 
}; 

/**
 * This function filters the signal.
 * This function also takes care of the initial conditions 
 */
 
void iirDirect2(
    float *x,                   /**< Input signal */
    float *y,                   /**< Output signal */
    int n                       /**< Length of the input samples */
    ) 
{
    int i;
    y[0] = b[0] * x[0] + b[1] * 0  + b[2] * 0   - (a[1] * 0)  - (a[2] * 0)  ;
    y[1] = b[0] * x[1] + b[1] * x[0] + b[2] * 0   - (a[1] * y[0]) - (a[2] * 0)  ;

    for (i=2; i<n; i++)
    {
        y[i] = b[0] * x[i-0] + b[1] * x[i-1] + b[2] * x[i-2]  - (a[1] * y[i-1]) - (a[2] * y[i-2]) ;

    }
}