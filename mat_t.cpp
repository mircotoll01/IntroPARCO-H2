#include <iostream>
#include <math.h>
#include <omp.h>
#include "matrix.h"

using namespace std;

//-------------------OPERATOR OVERLOAD---------------------
//will be used for debug purposes, to be removed later since it's of no use outside the homework
ostream& operator<<(ostream& os, const Matrix& A){
  os << endl;
  for(int i = 0; i < A.cols; i++){;
    for(int j = 0; j < A.rows; j++){ os << A.elements[i][j] << "\t"; }
    os << endl;
  }
  return os;
}

//-----------------------FUNCTIONS--------------------------

void matPopulate(Matrix A){
  //matrix population with random numbers
  for(int i = 0; i < A.cols; i++){
    for(int j = 0; j < A.rows; j++){
      A.elements[i][j] = (float)(rand() % 100)/(float)(rand()%100);
    }
  }
}

Matrix matT(Matrix A){
  Matrix B = Matrix(A.rows, A.cols);
	
  double start = omp_get_wtime();
  for(int i = 0; i < A.cols; i++){
    for(int j = 0; j < A.rows; j++){
      B.elements[j][i] = A.elements[i][j];
    }
  }
  double stop = omp_get_wtime();

  cout << "Serial matrix transposition executed:          " << stop - start << " \tseconds elapsed" << endl;
  return B;
}

Matrix matBlockT(Matrix A, int block_size){
  Matrix B = Matrix(A.rows, A.cols);
  Matrix C = Matrix(A.rows, A.cols);
  double start = omp_get_wtime();

  for(int c = 0; c < A.cols; c += block_size){
  	for(int r = 0; r < A.rows; r += block_size){
    	//transposition of the elements of the block
      for(int i = 0; i < block_size; i++){
        for(int j = 0; j < block_size; j++){
          B.elements[c+j][r+i] = A.elements[c+i][r+j];
        }
      }
      //transposition of the block
      for(int i = 0; i < block_size; i++){
      	for(int j = 0; j < block_size; j++){
        	C.elements[r+i][c+j] = B.elements[c+i][r+j];
      	}
   		}
    }
  }

	double stop = omp_get_wtime();

  cout << "Serial matrix block transposition executed:    " << stop - start << " \tseconds elapsed" << endl; 
  return C;
}



//------------------PARALLEL FUNCTIONS---------------------



Matrix matTpar(Matrix A){
  Matrix B = Matrix(A.rows, A.cols);
  double start = omp_get_wtime();
#pragma omp parallel for
  for(int i = 0; i < A.cols; i++){
    for(int j = 0; j < A.rows; j++){
      B.elements[j][i] = A.elements[i][j];
    }
  }
  double stop = omp_get_wtime();

  cout << "Parallel matrix transposition executed:        " << stop - start << " \tseconds elapsed" << endl;
  return B;
}

Matrix matBlockTpar(Matrix A,int block_size){
  Matrix B = Matrix(A.rows, A.cols);
  Matrix C = Matrix(A.rows, A.cols);
  int r,c,i,j;

  double start = omp_get_wtime();
#pragma omp parallel for collapse(2) private(r,c,i,j)
  for(c = 0; c < A.cols; c += block_size){
  	for(r = 0; r < A.rows; r += block_size){
      //transposition of the elements of the block
      #pragma omp sections
      {
      for(i = 0; i < block_size; i++){
        for(j = 0; j < block_size; j++){
          #pragma omp atomic write
          B.elements[c+j][r+i] = A.elements[c+i][r+j];
        }
      }
      //transposition of the block
      for(i = 0; i < block_size; i++){
      	for(j = 0; j < block_size; j++){
          #pragma omp atomic write
        	C.elements[r+i][c+j] = B.elements[c+i][r+j];
      	}
   		}
    }
    }
  } 
	double stop = omp_get_wtime();

  cout << "Parallel matrix block transposition executed:  " << stop - start << "\tseconds elapsed" << endl; 
  return C;
}
//---------------------------------------------------------
//----------------------MAIN LOOP--------------------------
//---------------------------------------------------------


int main(){
  int index_test = 0;
  cout << "Matrix transposition testing two 4x4 matrices. Verify the correct operations by looking the result below:" << endl;
  Matrix E = Matrix(4,4);
  for(int i = 0; i < 4; i++){
    for(int j = 0; j <4; j++){
      E.elements[i][j] = index_test++;
    }
  }

  Matrix F;

  cout << E << endl;
  F = matT(E);
  cout << F << endl;
  F = matBlockT(E,2);
  cout << F << endl;
  F = matTpar(E);
  cout << F << endl;
  F = matBlockTpar(E,2);
  cout << F << endl;
  cout << "test finished" << endl;



  for(int i = 1; i <= 12; i++){
    cout << "Testing with matrix size " << pow(2,i) << "x" << pow(2,i) <<endl;
    Matrix A = Matrix(pow(2,i),pow(2,i));
    matPopulate(A);

    matT(A);
    matBlockT(A,2);
    matTpar(A);
    matBlockTpar(A,2);

    cout << endl;
  }
  return 0;
}
