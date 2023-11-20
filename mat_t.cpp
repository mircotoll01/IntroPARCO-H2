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

  cout << "Serial matrix transposition executed:        " << stop - start << " seconds elapsed" << endl;
  return B;
}

Matrix matBlockT(Matrix A, int block_size){
  Matrix B = Matrix(A.rows, A.cols); 
  double start = omp_get_wtime();

  for(int c = 0; c < A.cols/block_size; c++){
  	for(int r = 0; r < A.rows/block_size; r++){
    	for(int i = 0; i < block_size; i++){
      	for(int j = 0; j < block_size; j++){
        	B.elements[block_size*r+j][block_size*c+i] = A.elements[block_size*c+i][block_size*r+j];
      	}
   		}
  	}
  }
  for(int i = 0; i < A.cols/2; i++){
    for(int j = 0; j < A.rows/2; j++){
      B.elements[j][i] = A.elements[i][j];
    }
  } 

	double stop = omp_get_wtime();

  cout << "Serial matrix block transposition executed:  " << stop - start << " seconds elapsed" << endl; 
  return B;
}


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

  cout << "Serial matrix transposition executed:        " << stop - start << " seconds elapsed" << endl;
  return B;
}

Matrix matBlockTpar(Matrix A,int block_size){
  Matrix B = Matrix(A.rows, A.cols); 
  double start = omp_get_wtime();
#pragma omp parallel for
  for(int c = 0; c < A.cols/block_size; c++){
  	for(int r = 0; r < A.rows/block_size; r++){
    	for(int i = 0; i < block_size; i++){
      	for(int j = 0; j < block_size; j++){
        	B.elements[block_size*r+j][block_size*c+i] = A.elements[block_size*c+i][block_size*r+j];
      	}
   		}
  	}
  }
  for(int i = 0; i < A.cols/2; i++){
    for(int j = 0; j < A.rows/2; j++){
      B.elements[j][i] = A.elements[i][j];
    }
  } 

	double stop = omp_get_wtime();

  cout << "Serial matrix block transposition executed:  " << stop - start << " seconds elapsed" << endl; 
  return B;
}
//---------------------------------------------------------
//----------------------MAIN LOOP--------------------------
//---------------------------------------------------------


int main(){
  for(int i = 1; i <= 15; i++){
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
