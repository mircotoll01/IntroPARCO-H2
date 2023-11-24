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
  for(int i = 0; i < A.rows; i++){
    for(int j = 0; j < A.cols; j++){
      A.elements[i][j] = (float)(rand() % 100)/(float)(rand()%100);
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

Matrix matT(const Matrix& A){
  Matrix B = Matrix(A.rows, A.cols);
	
  double start = omp_get_wtime();
  for(int i = 0; i < A.rows; i++){
    for(int j = 0; j < A.cols; j++){
      B.elements[j][i] = A.elements[i][j];
    }
  }
  double stop = omp_get_wtime();

  cout << "Serial matrix transposition executed:          " << stop - start << "  \tseconds elapsed" << endl;
  return B;
}

Matrix matBlockT(const Matrix& A, int block_size){
  Matrix B = Matrix(A.rows, A.cols);
  double start = omp_get_wtime();
  //first loop is for selecting the chunk
  for(int r = 0; r < A.rows; r += block_size){
  	for(int c = 0; c < A.cols; c += block_size){
    	//transposition of the elements of the chunk
      for(int i = 0; i < block_size; i++){
      	for(int j = 0; j < block_size; j++){
        	B.elements[c+j][r+i] = A.elements[r+i][c+j];
      	}
   		}
    }
  }

	double stop = omp_get_wtime();

  cout << "Serial matrix block transposition executed:    " << stop - start << "  \tseconds elapsed (block dimension:" << block_size << "x" << block_size << ")" <<endl; 
  return B;
}



//------------------PARALLEL FUNCTIONS---------------------



Matrix matTpar(const Matrix& A){
  Matrix B = Matrix(A.rows, A.cols);
  double start = omp_get_wtime();
#pragma omp parallel for schedule(static) collapse(2)
  for(int i = 0; i < A.rows; i++){
    for(int j = 0; j < A.cols; j++){
      B.elements[j][i] = A.elements[i][j];
    }
  }
  double stop = omp_get_wtime();

  cout << "Parallel matrix transposition executed:        " << stop - start << "  \tseconds elapsed" << endl;
  return B;
}

Matrix matBlockTpar(const Matrix& A,int block_size){
  Matrix B = Matrix(A.rows, A.cols);
 
  double start = omp_get_wtime();
#pragma omp parallel for schedule(static) collapse(4)
  for(int r = 0; r < A.rows; r += block_size){
  	for(int c = 0; c < A.cols; c += block_size){
    	//transposition of the elements of the chunk
      for(int i = 0; i < block_size; i++){
      	for(int j = 0; j < block_size; j++){
        	B.elements[c+j][r+i] = A.elements[r+i][c+j];
      	}
   		}
    }
  }
	double stop = omp_get_wtime();

  cout << "Parallel matrix block transposition executed:  " << stop - start << "  \tseconds elapsed (block dimension:" << block_size << "x" << block_size << ")"  << endl; 
  return B;
}
//---------------------------------------------------------
//----------------------MAIN LOOP--------------------------
//---------------------------------------------------------


int main(){
  for(int i = 5; i <= 14; i++){
    cout << "Testing with " << omp_get_max_threads() << " threads" << endl << endl; 
    cout << "Testing with matrix size " << pow(2,i) << "x" << pow(2,i) <<endl;
    Matrix A = Matrix(pow(2,i),pow(2,i));
    matPopulate(A);
    Matrix B = Matrix(pow(2,i),pow(2,i));
    Matrix C = Matrix(pow(2,i),pow(2,i));

    C = matT(A);
    B = matTpar(A); //isEqual(C,B);//generate matrices with integers to verify the correct execution with isEqual
    for(int j = 1; j < i; j++){
      B = matBlockT(A, pow(2,j)); //isEqual(C,B);
    }
    for(int k = 1; k < i; k++){
      B = matBlockTpar(A, pow(2,k)); //isEqual(C,B);
    }
    cout << endl;
  }
  return 0;
}
