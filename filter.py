#!/usr/bin/env python

from scipy import signal

"""
This script generates the c function template for a given cuttof 
frequency. Note that this script will use butterworth filter to
generate the differnce equations.
"""

# Define your filter parameters here
Fs    = 44100    # Sampling frequency in Hz
Fc    = 8000     # Cut-off frequncy in Hz, set to 8k here
order = 2        # Design a second order filter 
outFile = "filter" # The output file to be created, without any extension. 
#This script will create .c and accompanying .h file for you to use

nyqusitRate = Fs/2.0

Wc   = Fc/nyqusitRate # Cutoff freq in normalized to nyquist pi rads/samples 

# Get the filter co-efficents for Wc as low pass filter using butterowrth design
[b, a] = signal.butter(order, Wc, 'low')

output= """

/**
 * THIS IS AN AUTOMATICALLY GENERATED FILE.
 * Implements the difference equation given the input signal.
 * The a and b of the DE are defined here locally. Which means that
 * you will have to regenrate this file every time the sampling or 
 * the cut off frequency changes.
*/

static float a[] = {
"""

outText = ""
nPos = 0
inFloat = a.astype(float)

for num in inFloat:
    outText = outText + "%+ff, " %num
    
output = output + outText
output = output + "\n};"


output = output + "\n\nstatic float b[] = { \n"

outText = ""

inFloat = b.astype(float)
for num in inFloat:
    outText = outText + "%+ff, " %num

output = output + outText
output = output + "\n};"


functionImp = """ 

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
"""
# Handle the initial condition
initialCond = ""
eq = ""
for i in range(0, order):
    eq = eq + "    y[%d] = " %(i)
    for j in range(0, len(b)):
        if i-j < 0:
            eq = eq + "b[%d] * 0 "  %(j) # Take care of negative indices 
        else:
            eq = eq + "b[%d] * x[%d]" %(j, i-j)
        eq = eq + " + "
        
    eq = eq[0:len(eq)-2]  # Get rid of extra '+'
    eq = eq + " - "
        
    for j in range(1, len(a)): # Ignore the unit gain of the coefficents generated
        if i-j < 0:
            eq = eq + "(a[%d] * 0) "  %(j) # Take care of negative indices 
        else:
            eq = eq + "(a[%d] * y[%d])" %(j, i-j)
        eq = eq + " - "
        
    eq = eq[0:len(eq)-2]  # Get rid of extra '-'
    
    eq = eq + ";\n"
            
functionImp = functionImp + eq

# Now for the rest of the equation
theForloop = """
    for (i=%d; i<n; i++)
    {
"""
eq = "        y[i] = "

for j in range(0, len(b)):
    eq = eq + "b[%d] * x[i-%d]" %(j, j)
    eq = eq + " + "
    
eq = eq[0:len(eq)-2]  # Get rid of extra '+'
eq = eq + " - "
    
for j in range(1, len(a)):
    eq = eq + "(a[%d] * y[i-%d])" %(j, j)
    eq = eq + " - "
    
eq = eq[0:len(eq)-2]  # Get rid of extra '-'

eq = eq + ";\n"

theForloop = theForloop %(order)
theForloop = theForloop + eq + "\n    }\n}"
functionImp = functionImp + theForloop

output = output + functionImp

#print output

# create the header file first 

headerContent = """

#ifndef __FILTER_H
#define __FILTER_H

void iirDirect2(float *, float *, int);

#endif

"""

f = file(outFile+".h", "w")
f.write(headerContent)
f.close()

f = file(outFile+".c", "w")
f.write(output)
f.close()




