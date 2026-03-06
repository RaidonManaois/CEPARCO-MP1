#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

// Assembly kernels
extern void kernel_x86(int n, float *A, float *B, float *C, int *idx);
extern void kernel_xmm(int n, float *A, float *B, float *C, int *idx);
extern void kernel_ymm(int n, float *A, float *B, float *C, int *idx);

// C kernel
void kernel_c(int n, float *A, float *B, float *C, int *idx);

int main(){

    int n = 1<<20;
    int i,r;

    float *A = malloc(n*sizeof(float));
    float *B = malloc(n*sizeof(float));

    float *C_c   = malloc(n*sizeof(float));
    float *C_x86 = malloc(n*sizeof(float));
    float *C_xmm = malloc(n*sizeof(float));
    float *C_ymm = malloc(n*sizeof(float));

    int *idx_c   = malloc(n*sizeof(int));
    int *idx_x86 = malloc(n*sizeof(int));
    int *idx_xmm = malloc(n*sizeof(int));
    int *idx_ymm = malloc(n*sizeof(int));

    // initialize vectors
    for(i=0;i<n;i++){
        A[i] = sin(i*0.0005f)*100.0f + 50.0f;
        B[i] = cos(i*0.0003f)*100.0f + 50.0f;
    }

    clock_t start,end;
    double total;
    int runs = 30;

    // ================= C =================
    total=0;
    for(r=0;r<runs;r++){
        start=clock();
        kernel_c(n,A,B,C_c,idx_c);
        end=clock();
        total+=(double)(end-start)/CLOCKS_PER_SEC;
    }
    printf("\nC kernel time: %f\n",total/runs);

    // ================= x86 =================
    total=0;
    for(r=0;r<runs;r++){
        start=clock();
        kernel_x86(n,A,B,C_x86,idx_x86);
        end=clock();
        total+=(double)(end-start)/CLOCKS_PER_SEC;
    }
    printf("x86 kernel time: %f\n",total/runs);

    // ================= XMM =================
    total=0;
    for(r=0;r<runs;r++){
        start=clock();
        kernel_xmm(n,A,B,C_xmm,idx_xmm);
        end=clock();
        total+=(double)(end-start)/CLOCKS_PER_SEC;
    }
    printf("XMM kernel time: %f\n",total/runs);

    // ================= YMM =================
    total=0;
    for(r=0;r<runs;r++){
        start=clock();
        kernel_ymm(n,A,B,C_ymm,idx_ymm);
        end=clock();
        total+=(double)(end-start)/CLOCKS_PER_SEC;
    }
    printf("YMM kernel time: %f\n",total/runs);

    // ================= PRINT RESULTS =================

    printf("\n===== FIRST 5 ELEMENTS =====\n");

    for(i=0;i<5;i++){
        printf("\nIndex %d\n",i);
        printf("C   : %f  idx=%d\n",C_c[i],idx_c[i]);
        printf("x86 : %f  idx=%d\n",C_x86[i],idx_x86[i]);
        printf("XMM : %f  idx=%d\n",C_xmm[i],idx_xmm[i]);
        printf("YMM : %f  idx=%d\n",C_ymm[i],idx_ymm[i]);
    }

    printf("\n===== LAST 5 ELEMENTS =====\n");

    for(i=n-5;i<n;i++){
        printf("\nIndex %d\n",i);
        printf("C   : %f  idx=%d\n",C_c[i],idx_c[i]);
        printf("x86 : %f  idx=%d\n",C_x86[i],idx_x86[i]);
        printf("XMM : %f  idx=%d\n",C_xmm[i],idx_xmm[i]);
        printf("YMM : %f  idx=%d\n",C_ymm[i],idx_ymm[i]);
    }

    free(A);
    free(B);
    free(C_c);
    free(C_x86);
    free(C_xmm);
    free(C_ymm);
    free(idx_c);
    free(idx_x86);
    free(idx_xmm);
    free(idx_ymm);

    return 0;
}


// ================= C KERNEL =================

void kernel_c(int n,float *A,float *B,float *C,int *idx){

    int i;

    for(i=0;i<n;i++){
        if(A[i]>=B[i]){
            C[i]=A[i];
            idx[i]=0;
        }else{
            C[i]=B[i];
            idx[i]=1;
        }
    }
}