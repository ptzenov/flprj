/*
 * linsolver.cpp
 *
 *  Created on: 14 Jun 2014
 *      Author: kenny
 */
#include "linsolver.hpp"


int gaussianElimination (float* a,float* p_b, float *x, const int n ){

	int i,j;
	for(i = 0; i<n;i++){
		// check for the largest element from column i...
			int index_largest = i;
			float largest = absolute(a[index_largest*n+i]);
			int k;
			for(k=i+1;k<n;k++){
				if(absolute(a[k*n+i])>largest){
						index_largest = k;
						largest = absolute(a[k*n+i]);
						}
				}
		//swap row i and row index_largest if i != index_largest
			if(index_largest != i)
				swap(a,p_b,index_largest, i,n);



		//so here we have setup our matrix such that the largest element of column i is in row i.

		// now multiply the elements in row i by a constant and add it to the elements of row j
			for(j = i+1;j<n;j++)
				multiply_add(a,p_b,n,i,j);
		}
		backwardSubstitute(a,p_b,x,n);
		return 0;

	}


//Uses the L-U decomposition method to solve the SLE Ax=b
 int _nxn_LUdecomposition(float* A,float*b, float* L , float* U, const int n, float* x){

	int additionalMemAlloc = 0;
	//in case the function is called without specifying the parameters L and U declare those.
	if(U==NULL || L ==NULL){
		additionalMemAlloc = 1;
		L = (float*) malloc(sizeof(float)*n*n);
		U = (float*) malloc(sizeof(float)*n*n);

		}

	LUdecompose(A,L,U,n);
	float y[n*n];
	forwardSubstitute(L,b,y,n);
	backwardSubstitute(U,y,x,n);


	if(additionalMemAlloc!=0) {
		free(L);
		free(U);
	}
	return 0;

}

int LUdecompose(float* A,float* L,float* U,const int n){

	int k,i,j;
	//initialize the diagonal elements of L to 1 !
	for(k=0;k<n;k++) L[k*n+k]=1;


	for(j = 0 ;j<n;j++){

		for(i=0;i<n;i++){

		//compute U
			if(i<=j){

				U[i*n+j] = A[i*n+j];
				for(k=0;k<i;k++) 	U[i*n+j]-=L[i*n+k]*U[k*n+j];

				}
		//compute L
			if(i>j){
				L[i*n+j] = A[i*n+j];

				for(k=0;k<j;k++) L[i*n+j]-=(L[i*n+k]*U[k*n+j]);
				L[i*n+j]/=U[j*n+j];
				}

		}
		}


		return 0;
	}

int swap(float* a,float* p_b,int index_largest,int i,const int n ){

	//swap the ith row and the index_largest row

	int j;
	for(j=0;j<n;j++){

		float temp = a[index_largest*n+j];
		a[index_largest*n+j] = a[i*n+j];
		a[i*n+j] = temp;

		}

	//swat the respective elements in the rhs of the equation

		float temp = p_b[index_largest];

		p_b[index_largest] = p_b[i];

		p_b[i] = temp;

		return 0;

	}

int multiply_add(float* a ,float * p_b, const int n,int i,int j){
	int k;
	float coeff = (a[j*n+i]/a[i*n+i]);
	//first modify p_b and then the row of the matrix;
	p_b[j] -=p_b[i]*coeff;
	for(k = i;k<n;k++)
		a[j*n+k] -= a[i*n+k]*coeff;

	return 0;
}

int backwardSubstitute(float* a,float* b,float* x,const int n){

	int j,k;

	for (j=n-1;j>=0;j--){

		x[j] = b[j];

		for(k =j+1;k<n;k++) x[j]-= a[j*n+k]*x[k];

		x[j]/=a[j*n+j];


		}

	return 0;
	}

int forwardSubstitute(float* a, float* b, float* y,const int n){
	int j,k;

	for(j=0;j<n;j++){

		y[j]=b[j];

		for(k=0;k<j;k++) y[j]-=a[j*n+k]*y[k];

		y[j]/=a[j*n+j];
	}



	return 0;
	}

float absolute(float x){

		return x>= 0 ? x : -x;

		}

int printmatrix(float* a,const int n,const int m){

	int i,j;
	if(m==1) {

		for(i=0;i<n;i++){
		printf("\n");

		for(j = 0;j<m;j++){
			printf("%4.2f ",a[j*n+i]);
			}

		}

		printf("\n");

		return 0;

		}


	for(i=0;i<n;i++){
		printf("\n");

		for(j = 0;j<m;j++){
			printf("%4.2f ",a[i*n+j]);
			}

		}


		printf("\n");
		return 0;
	}




