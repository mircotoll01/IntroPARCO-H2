#include <iostream>
#include <stdlib.h>
#include <cmath>
#include <omp.h>
#include "matrix.h"

using namespace std;

//-------------------OPERATOR OVERLOAD---------------------
//will be used for debug purposes, to be removed later since it's of no use outside the homework
ostream& operator<<(ostream& os, const Matrix& A){
  os << endl;
  for(int i = 0; i < A.cols; i++){;
    for(int j = 0; j < A.rows; j++){ os << A.elements[i][j] << " "; }
    os << endl;
  }
  return os;
}

//-----------------------FUNCTIONS--------------------------

void matPopulate(Matrix A){
  //matrix population with random numbers
  for(int i = 0; i < A.cols; i++){
    for(int j = 0; j < A.rows; j++){
      A.elements[i][j] = (float)(rand() % 100/(float)(rand()%100));
    }
  }
}

void isEqual(const Matrix& A, const Matrix& B){
  for(int i = 0; i < A.rows; i++){
    for(int j = 0; j < A.cols; j++){
      if(A.elements[i][j] != B.elements[i][j]){cout<<"matrices not equal"<<endl; return;}
    }
  }
}

Matrix matMul(const Matrix& A, const Matrix& B){
  // check if matrix multiplication is possible, otherwise return an empty matrix with the standard constructor
  if (A.rows != B.cols) {cout<<"invalid arguments"<<endl; return Matrix();}
  Matrix C = Matrix(A.cols, B.rows); 
  //matrix multiplication
	double start = omp_get_wtime();

  for(int i = 0; i < A.cols; i++){
    for(int k = 0; k < B.cols; k++){
      for(int j = 0; j < A.rows; j++){
        C.elements[i][j] += A.elements[i][k] * B.elements[k][j];      
      }
    }
  }

  double stop = omp_get_wtime();
  cout << "Serial matrix multiplication executed:  \t" << stop - start << " seconds elapsed" << endl;
  return C;
}

Matrix matMulPar(const Matrix& A, const Matrix& B){
  // check if matrix multiplication is possible, otherwise return an empty matrix with the standard constructor
  if (A.rows != B.cols) {cout<<"invalid arguments"<<endl; return Matrix();}
  Matrix C = Matrix(A.cols, B.rows);
  //matrix multiplication
  double start = omp_get_wtime();
#pragma omp parallel for schedule(static, A.cols/omp_get_max_threads()) collapse(2)
  for (int i = 0; i < A.cols; i++) { 
    for (int k = 0; k < B.cols; k++) {
      for (int j = 0; j < A.rows; j++) {//variables declared inside of loops are implicitly private
        #pragma omp atomic update// since multiple threads can work in the same position at the same time, an atomic directive is necessary
        C.elements[i][j] += A.elements[i][k] * B.elements[k][j];       
      }
    }
  }

  double stop = omp_get_wtime();
  cout << "Parallel matrix multiplication executed:\t"  << stop - start << " seconds elapsed" << endl;
  return C;
}

//-------------------------------------------------------------
//-----------------------MAIN LOOP-----------------------------
//-------------------------------------------------------------


int main(){
  cout << "Number of threads in use: " << omp_get_max_threads() << endl << endl;

  Matrix A,B,C,D;
  for(int i = 5; i <= 14; i++){    
    cout << "Testing with matrix size " << pow(2,i) << "x" << pow(2,i) << endl;

    A = Matrix(pow(2,i),pow(2,i));
    B = Matrix(pow(2,i),pow(2,i));
    matPopulate(A);
    matPopulate(B);
		
    C = matMul(A,B);
    D = matMulPar(A,B); //isEqual(C,D); //generate matrices of integers to verify the correct execution with isEqual
    cout<<endl<<endl;
  }

  return 0;
}
