#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>
#define BILLION 1000000000

void main(int argc,char *argv)
{
    int i,j,k,i1,j1,k1,B,n;
    double *a,*b,*c1,*c2,*c3,*c4,*c5,*c6,*cc1,*cc2,*cc3,*cc4,*cc5,*cc6;
    struct timespec start, end;
    double simple_ijk, simple_ikj , simple_jik , simple_jki , simple_kij , simple_kji,
    blocked_ijk, blocked_ikj , blocked_jik , blocked_jki , blocked_kij , blocked_kji;
    
    n=2048;
    B=10;    
    
    a=(double*)malloc(sizeof(double)*n*n);
    b=(double*)malloc(sizeof(double)*n*n);
    c1=(double*)malloc(sizeof(double)*n*n);
    c2=(double*)malloc(sizeof(double)*n*n);
    c3=(double*)malloc(sizeof(double)*n*n);
    c4=(double*)malloc(sizeof(double)*n*n);
    c5=(double*)malloc(sizeof(double)*n*n);
    c6=(double*)malloc(sizeof(double)*n*n);
    cc1=(double*)malloc(sizeof(double)*n*n);
    cc2=(double*)malloc(sizeof(double)*n*n);
    cc3=(double*)malloc(sizeof(double)*n*n);
    cc4=(double*)malloc(sizeof(double)*n*n);
    cc5=(double*)malloc(sizeof(double)*n*n);
    cc6=(double*)malloc(sizeof(double)*n*n);
    
    
    for(i=0;i<n;i++)
        for(j=0;j<n;j++)
        {
            a[i*n+j]=rand();
            b[i*n+j]=rand();
        }

    
//ijk algorithm
    clock_gettime(CLOCK_MONOTONIC, &start);
    for(i=0;i<n;i++)
        for(j=0;j<n;j++)
        {
            register double r=c1[i*n+j];
            for (k=0; k<n; k++) 
                r+= a[i*n+k] * b[k*n+j];
            c1[i*n+j]=r;
        }
    clock_gettime(CLOCK_MONOTONIC, &end);
    simple_ijk= (end.tv_sec - start.tv_sec)*BILLION + end.tv_nsec - start.tv_nsec;
    printf("$\n");

//ikj algorithm
    clock_gettime(CLOCK_MONOTONIC, &start);
     for(i=0;i<n;i++)
        for(k=0;k<n;k++)
        {
            register double r=a[i*n+k];
            for (j=0; j<n; j++) 
                c2[i*n+j]+= r * b[k*n+j];
        }
    clock_gettime(CLOCK_MONOTONIC, &end);
    simple_ikj= (end.tv_sec - start.tv_sec)*BILLION + end.tv_nsec - start.tv_nsec;
printf("$\n");
//jik algorithm
    clock_gettime(CLOCK_MONOTONIC, &start);
    for(j=0;j<n;j++)
        for(i=0;i<n;i++)
        {
                register double r=0;
                for(k=0;k<n;k++)
                    r+= a[i*n+k]*b[k*n+j];
                c3[i*n+j]=r;
        }
    clock_gettime(CLOCK_MONOTONIC, &end);
    simple_jik= (end.tv_sec - start.tv_sec)*BILLION + end.tv_nsec - start.tv_nsec;
printf("$\n");
//jki algorithm
    clock_gettime(CLOCK_MONOTONIC, &start);
    for(j=0;j<n;j++)
        for(k=0;k<n;k++)
        {
            register double r=b[k*n+j];
            for(i=0;i<n;i++)
                c4[i*n+j]+=a[i*n+k]*r;
        }
    clock_gettime(CLOCK_MONOTONIC, &end);
    simple_jki= (end.tv_sec - start.tv_sec)*BILLION + end.tv_nsec - start.tv_nsec;
printf("$\n");
//kij algorithm
    clock_gettime(CLOCK_MONOTONIC, &start);
    for(k=0;k<n;k++)
        for(i=0;i<n;i++)
        {
            register double r=a[i*n+k];
            for(j=0;j<n;j++)
                c5[i*n+j]+=r*b[k*n+j];
        }
    clock_gettime(CLOCK_MONOTONIC, &end);
    simple_kij= (end.tv_sec - start.tv_sec)*BILLION + end.tv_nsec - start.tv_nsec;
printf("$\n");
//kji algorithm
    clock_gettime(CLOCK_MONOTONIC, &start);
    for(k=0;k<n;k++)
        for(j=0;j<n;j++)
        {
            register double r=b[k*n+j];
            for(i=0;i<n;i++)
                c6[i*n+j]+=a[i*n+k]*r;
        }
    clock_gettime(CLOCK_MONOTONIC, &end);
    simple_kji= (end.tv_sec - start.tv_sec)*BILLION + end.tv_nsec - start.tv_nsec;
printf("$1\n");

/***********************************BLOCKED CACHE ALGORITHMS******************************************************/

//ijk blocked algorithm
    clock_gettime(CLOCK_MONOTONIC, &start);
    for (i = 0; i < n; i+=B)
      for (j = 0; j < n; j+=B)
         for (k = 0; k < n; k+=B)
    
            for (i1 = i; i1 < i+B; i1++)
                for (j1 = j; j1 < j+B; j1++)
                {
                    register double r=cc1[i1*n+j1]; 
                    for (k1 = k; k1 < k+B; k1++)
                        r+= a[i1*n + k1]*b[k1*n + j1];
                    cc1[i1*n+j1]=r;
                }
    clock_gettime(CLOCK_MONOTONIC, &end);
    blocked_kji= (end.tv_sec - start.tv_sec)*BILLION + end.tv_nsec - start.tv_nsec;
//ikj blocked algorithm
printf("$\n");
    clock_gettime(CLOCK_MONOTONIC, &start);
    for (i = 0; i < n; i+=B)
        for (k = 0; k < n; k+=B)
            for (j = 0; j < n; j+=B)

                for (i1 = i; i1 < i+B; i1++)
                    for (k1 = k; k1 < k+B; k1++)
                    {
                        register double r=a[i1*n+ k1]; 
                        for (j1 = j; j1 < j+B; j1++)
                            cc2[i1*n+ j1] += r *b[k1*n + j1];
                    }
    clock_gettime(CLOCK_MONOTONIC, &end);
    blocked_kji= (end.tv_sec - start.tv_sec)*BILLION + end.tv_nsec - start.tv_nsec;
printf("$\n");
//jik blocked algorithm
    clock_gettime(CLOCK_MONOTONIC, &start);
    for (j = 0; j < n; j+=B)
        for (i = 0; i < n; i+=B)
            for (k = 0; k < n; k+=B)

                for(j1=j;j1<j+B;j1++)
                    for(i1=i;i1<i+B;i1++)
                    {
                        register double r=0;
                        for (k1 = k; k1 < k+B; k1++)
                            r+=a[i1*n+ k1]*b[k1*n+j1];
                        cc3[i1*n+j1]=r;
                    }
    clock_gettime(CLOCK_MONOTONIC, &end);
    blocked_kji= (end.tv_sec - start.tv_sec)*BILLION + end.tv_nsec - start.tv_nsec;
printf("$\n");
//jki blocked algorithm
    clock_gettime(CLOCK_MONOTONIC, &start);                    
    for (j = 0; j < n; j+=B)
        for (k = 0; k < n; k+=B)
            for (i = 0; i < n; i+=B)
                
                for (j1 = j; j1 < j+B; j1++)
                    for (k1 = k; k1 < k+B; k1++)
                    {
                        register double r=b[k1*n+j1];
                        for (i1 = i; i1 < i+B; i1++)
                            cc4[i1*n+ j1]+=a[i1*n+ k1]*r;
                    }
    clock_gettime(CLOCK_MONOTONIC, &end);
    blocked_kji= (end.tv_sec - start.tv_sec)*BILLION + end.tv_nsec - start.tv_nsec;

printf("$\n");
//kij blocked algorithm
    clock_gettime(CLOCK_MONOTONIC, &start);                    
    for (k = 0; k < n; k+=B)
        for (i = 0; i < n; i+=B)
            for (j = 0; j < n; j+=B)
            
                for (k1 = k; k1 < k+B; k1++)
                    for (i1 = i; i1 < i+B; i1++)
                    {
                        register double r=a[i1*n+k1];
                        for (j1 = j; j1 < j+B; j1++)
                            cc5[i1*n+j1] += r*b[k1*n+j1];
                    }
    clock_gettime(CLOCK_MONOTONIC, &end);
    blocked_kji= (end.tv_sec - start.tv_sec)*BILLION + end.tv_nsec - start.tv_nsec;

printf("$\n");

//kji blocked algorithm
    clock_gettime(CLOCK_MONOTONIC, &start);                
    for (k = 0; k < n; k+=B)
        for (j = 0; j < n; j+=B)
            for (i = 0; i < n; i+=B)

                for (k1 = k; k1 < k+B; k1++)
                    for (j1 = j; j1 < j+B; j1++)
                    {
                        register double r=b[k1*n+j1];
                        for (i1 = i; i1 < i+B; i1++)
                            cc6[i1*n+j1]=a[i1*n+ k1]*r;
                    }
    clock_gettime(CLOCK_MONOTONIC, &end);
    blocked_kji= (end.tv_sec - start.tv_sec)*BILLION + end.tv_nsec - start.tv_nsec;

    printf("*****************************Results**********************************\n");
    printf("ITERATION TYPE      SIMPLE ALGORITHM              BLOCKED ALGORITHM  *\n");
    printf("* ijk                   %llu                        %llu             *\n",(long long unsigned int)simple_ijk,(long long unsigned int)blocked_ijk );
    printf("* ikj                   %llu                        %llu             *\n",(long long unsigned int)simple_ikj,(long long unsigned int)blocked_ikj );
    printf("* jik                   %llu                        %llu             *\n",(long long unsigned int)simple_jik,(long long unsigned int)blocked_jik );
    printf("* jki                   %llu                        %llu             *\n",(long long unsigned int)simple_jki,(long long unsigned int)blocked_jki );
    printf("* kij                   %llu                        %llu             *\n",(long long unsigned int)simple_kij,(long long unsigned int)blocked_kij );
    printf("* kji                   %llu                        %llu             *\n",(long long unsigned int)simple_kji,(long long unsigned int)blocked_kji );
    printf("**********************************************************************\n");
    
}