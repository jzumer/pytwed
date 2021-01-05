/*
Filename: twed.c
source code for the Time Warp Edit Distance in ANSI C.
Author: Pierre-Francois Marteau
Version: V1.2.a du 25/08/2014, radix addition line 101, thanks to Benjamin Herwig from University of Kassel, Germany
Licence: MIT
******************************************************************
This software and description is free delivered "AS IS" with no 
guaranties for work at all. Its up to you testing it modify it as 
you like, but no help could be expected from me due to lag of time 
at the moment. I will answer short relevant questions and help as 
my time allow it. I have tested it played with it and found no 
problems in stability or malfunctions so far. 
Have fun.
*****************************************************************
Please cite as:
@article{Marteau:2009:TWED,
 author = {Marteau, Pierre-Francois},
 title = {Time Warp Edit Distance with Stiffness Adjustment for Time Series Matching},
 journal = {IEEE Trans. Pattern Anal. Mach. Intell.},
 issue_date = {February 2009},
 volume = {31},
 number = {2},
 month = feb,
 year = {2009},
 issn = {0162-8828},
 pages = {306--318},
 numpages = {13},
 url = {http://dx.doi.org/10.1109/TPAMI.2008.76},
 doi = {10.1109/TPAMI.2008.76},
 acmid = {1496043},
 publisher = {IEEE Computer Society},
 address = {Washington, DC, USA},
 keywords = {Dynamic programming, Pattern recognition, Pattern recognition, time series, algorithms, similarity measures., Similarity measures, algorithms, similarity measures., time series},
} 
*/

/* Rewritten by JZ based on the python version */

/* 
INPUTS
int dim: dimension of the multivariate time series (dim=1 if scalar time series)
double *ta: contiguous array containing the first time series; ta[i] is an array of size dim containing the multidimensional i^th sample for i in {0, .., la-1}
int la: length of the first time series
double *tsa: array containing the time stamps for time series ta; tsa[i] is the time stamp for sample ta[i]. The length of tsb array is expected to be lb.
double *tb: contiguous array containing the second time series; tb[i] is an array of size dim containing the multidimensional j^th sample for j in {0, .., lb-1}
int lb: length of the second time series
double *tsb: array containing the time stamps for time series tb; tsb[j] is the time stamp for sample tb[j]. The length of tsb array is expected to be lb.
double nu: value for parameter nu
double lambda: value for parameter lambda
int degree: power degree for the evaluation of the local distance between samples: degree>0 required
OUTPUT
double: the TWED distance between time series ta and tb.
*/

#include "twed.h"

#define INDEX(x, y) ((y)*arr1_lgt + (x))

double dist(double* arr1, double* arr2, int lgt, int degree) {
    double ret = 0;

    for(unsigned int i = 0; i < lgt; i++) {
        ret += pow(fabs(arr1[i] - arr2[i]), degree);
    }

    return pow(ret, 1.0 / degree);
}

double DTWEDL1d(int n_feats, double* arr1, int arr1_lgt, double* arr1_spec, double* arr2, int arr2_lgt, double* arr2_spec, double nu, double lambda, int degree) {
    // Version based on the python version

    // INIT
    double* D = (double*)calloc((arr1_lgt)*(arr2_lgt), sizeof(double));
    for(unsigned int i = 1; i < arr1_lgt; i++) {
        D[INDEX(i, 0)] = INFINITY;
    }
    for(unsigned int i = 1; i < arr2_lgt; i++) {
        D[INDEX(0, i)] = INFINITY;
    }

    // Go!
    for(unsigned int j = 1; j < arr2_lgt; j++) {
        for(unsigned int i = 1; i < arr1_lgt; i++) {
            float del_a = D[INDEX(i-1, j)]
                            + dist(arr1+((i-1)*n_feats), arr1+(i*n_feats), n_feats, degree)
                            + nu * (arr1_spec[i] - arr1_spec[i-1])
                            + lambda;
            float del_b = D[INDEX(i, j-1)]
                            + dist(arr2+((j-1)*n_feats), arr2+(j*n_feats), n_feats, degree)
                            + nu * (arr2_spec[j] - arr2_spec[j-1])
                            + lambda;
            float match = D[INDEX(i-1, j-1)]
                            + dist(arr1+(i*n_feats), arr2+(j*n_feats), n_feats, degree)
                            + dist(arr1+((i-1)*n_feats), arr2+((j-1)*n_feats), n_feats, degree)
                            + nu * (fabs(arr1_spec[i] - arr2_spec[j]) + fabs(arr1_spec[i-1] - arr2_spec[j-1]));

            D[INDEX(i, j)] = fmin(match, fmin(del_a, del_b));
        }
    }

    double ret = D[INDEX(arr1_lgt-1, arr2_lgt-1)];
    free(D);
    return ret;
}
