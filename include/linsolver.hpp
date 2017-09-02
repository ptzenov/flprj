/*
 * linsolver.h
 *
 *  Created on: 14 Jun 2014
 *      Author: kenny
 */

#ifndef LINSOLVER_HPP_
#define LINSOLVER_HPP_

#include <stdio.h> // for printing operations
#include <stdlib.h> // for malloc
//implement the gaussian elimination to solve systems of linear equations,
//in parcticular of the form A*x = b, where A is a square matrix

//implement gaussian elimination with pivoting!
int gaussianElimination (float* a,float* p_b,float* x,const int n );

//solves the equation Ax=b, using the LU decomposition method;
//performs the LU decomposition of a matrix A (if possible) and stores the 
//results in the lower and upper triangular
int _nxn_LUdecomposition(float* A,float*b, float* L, float* U ,const int n,float* x);
//performs the LU decomposition of a matrix A (if possible) and stores
//the results in the lower and upper triangular
// matrices L and U, respectively;
int LUdecompose(float* A,float* L, float* U,const int n);


//utility functions. Internal...

//the forward and backward substitutions, respectively.
int forwardSubstitute(float* a, float* b, float* y,const int n);
int backwardSubstitute(float* a,float* b,float* x, const int n);

//print the matrix in a nice form. n is the number of columns, and m the number of rows
int printmatrix(float* a,const int n, const int m);

//returns the absolute value of x
float absolute(float x);

//swaps row i and row "index_largest" of the extended matrix a|b
int swap(float* a,float* p_b,int index_largest,int i,const int n );
int multiply_add(float* a,float * p_b, const int n,int i,int j);

#endif /* LINSOLVER_H_ */
