/*
Filename: twed.c
source code for the Time Warp Edit Distance in ANSI C.
Author: Pierre-Francois Marteau
Version: V1.2.a du 25/08/2014, radix addition line 101, thanks to Benjamin Herwig from University of Kassel, Germany
Licence: GPL
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

/* Fixed (in the most important parts) by me */

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
double DTWEDL1d(int dim, double *ta, int la, double *tsa, double *tb, int lb, double *tsb, double nu, double lambda, int degree) {
    // 
    //    TWED PAMI
    //    
    if(la<0||lb<0){
        fprintf(stderr, "twed: the lengths of the input timeseries should be greater or equal to 0\n");
        exit(-1);
    }
    int r = la;
    int c = lb;
    double dist, disti1, distj1;
    int i,j,k;

    // allocations
    double *D = (double *)calloc((r+1)*(c+1), sizeof(double));
    double *Di1 = (double *)calloc(r+1, sizeof(double));
    double *Dj1 = (double *)calloc(c+1, sizeof(double));

    // local costs initializations
    for(j=1; j<=c; j++) {
        distj1=0;
        for(k=0; k<dim; k++) {
            if(j>1){
                distj1+=pow(fabs(tb[(j-2) * dim + k]-tb[(j-1) * dim + k]),degree);	
            }
            else {
                distj1+=pow(fabs(tb[(j-1) * dim + k]),degree);
            }
        }

        Dj1[j]=(distj1);
    }

    for(i=1; i<=r; i++) { 
        disti1=0;
        for(k=0; k<dim; k++)
            if(i>1)
                disti1+=pow(fabs(ta[(i-2) * dim + k]-ta[(i-1) * dim + k]),degree);
            else disti1+=pow(fabs(ta[(i-1) * dim + k]),degree);

        Di1[i]=(disti1);

        for(j=1; j<=c; j++) {
            dist=0;
            for(k=0; k<dim; k++){
                dist+=pow(fabs(ta[(i-1) * dim + k]-tb[(j-1) * dim + k]),degree);	
                if(i>1&&j>1)
                    dist+=pow(fabs(ta[(i-2) * dim + k]-tb[(j-2) * dim + k]),degree);
            }
            D[i * (c+1) + j]=pow(dist,1.0/degree);
        }
    }// for i

    // border of the cost matrix initialization
    D[0]=0;
    for(i=1; i<=r; i++)
        D[i * (c + 1) + 0]=D[(i-1) * (c + 1) + 0]+Di1[i];
    for(j=1; j<=c; j++)
        D[j]=D[j-1]+Dj1[j];

    double dmin, htrans, dist0;
    int iback;

    for (i=1; i<=r; i++){ 
        for (j=1; j<=c; j++){
            htrans=fabs((double)(tsa[i-1]-tsb[j-1]));
            if(j>1&&i>1)
                htrans+=fabs((double)(tsa[i-2]-tsb[j-2]));
            dist0=D[(i-1) * (c + 1) + j-1]+nu*htrans+D[i * (c + 1) + j];
            dmin=dist0;
            if(i>1)
                htrans=((double)(tsa[i-1]-tsa[i-2]));
            else htrans=(double)tsa[i-1];
            dist=Di1[i]+D[(i-1) * (c + 1) + j]+lambda+nu*htrans;
            if(dmin>dist){
                dmin=dist;
            }
            if(j>1)
                htrans=((double)(tsb[j-1]-tsb[j-2]));
            else htrans=(double)tsb[j-1]; 
            dist=Dj1[j]+D[i * (c + 1) + j-1]+lambda+nu*htrans; 
            if(dmin>dist){
                dmin=dist;
            } 
            D[i * (c + 1) + j] = dmin;
        }
    }

    dist = D[r * (c + 1)];
    // freeing
    free(D);
    free(Di1);
    free(Dj1);
    return dist;
}

