#include <iostream>
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

Matrix matMul(const Matrix& A, const Matrix& B){
  // check if matrix multiplication is possible, otherwise return an empty matrix with the standard constructor
  if (A.rows != B.cols) {cout<<"invalid arguments"<<endl; return Matrix();}
  Matrix C = Matrix(A.cols, B.rows); 
  //matrix multiplication
	double start = omp_get_wtime();

  for(int i = 0; i < A.cols; i++){
    for(int k = 0; k < B.cols; k++){
      for(int j = 0; j < A.rows; j++){
        C.elements[i][j] += A.elements[i][k] * B.elements[k][j];//visiting data row by row increases cache hit since the cache will load entire rows in its lines, but the calculation of a specific posizion will require more loop cycles
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
  int i,j,k;
  //matrix multiplication
  double start = omp_get_wtime();
#pragma omp parallel for collapse(2) private(i,j,k)
  for (i = 0; i < A.cols/2; i++) {
    for (k = 0; k < B.cols; k++) {
      for (j = 0; j < A.rows; j++) {
#pragma omp atomic update
        C.elements[i][j] += A.elements[i][k] * B.elements[k][j];
      }
    }
  }
#pragma omp parallel for collapse(2) private(i,j,k)
  for (i =A.cols/2; i < A.cols; i++) {
    for (k = 0; k < B.cols; k++) {
      for (j = 0; j < A.rows; j++) {
#pragma omp atomic update
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
  cout << "Matrix multiplication testing two 2x2 matrices with a 2 in every position. Verify the correct operations by looking the result below:" << endl;
  Matrix E = Matrix(4,4);
  Matrix F = Matrix (4,4);
  for(int i = 0; i < 4; i++){
    for(int j = 0; j <4; j++){
      E.elements[i][j] = 2;
      F.elements[i][j] = 2;
    }
  }

  cout << E << endl;
  cout << F << endl;
  Matrix G = matMul(E,F);
  cout << G << endl;
  G = matMulPar(E,F);
  cout << G << endl;
  cout << "test finished" << endl;

  Matrix A,B,C,D;
  for(int i = 1; i <= 15; i++){    
    cout << "Testing with matrix size " << pow(2,i) << "x" << pow(2,i) << endl;

    A = Matrix(pow(2,i),pow(2,i));
    B = Matrix(pow(2,i),pow(2,i));
    matPopulate(A);
    matPopulate(B);
		
    C = matMul(A,B);
    D = matMulPar(A,B);
    cout<<endl;
  }

  return 0;
}
