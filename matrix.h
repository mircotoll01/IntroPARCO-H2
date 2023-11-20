#ifndef MATRIX_H
#define MATRIX_H

#include <iostream>

class Matrix{
  public:
  int cols, rows;
  float** elements;
  Matrix(){ cols = 0; rows = 0; elements = NULL;}
  Matrix(int c, int r){
    cols = c;
    rows = r;
    elements = new float*[c];
    for (int i = 0; i < c; i++){
			 elements[i] = new float[r];
       for (int j = 0; j < r; j++){ elements[i][j] = 0;}
    }
  }

  void matPopulate(Matrix A);

};

#endif

