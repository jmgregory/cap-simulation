#ifndef JMG_MATRIX_H
#define JMG_MATRIX_H

#include <string>
#include <iostream>
#include <complex>

using std::complex;

struct Matrix 
{
public:
  static const Matrix identity_matrix;
  
  Matrix()
    : a(0.0), b(0.0), c(0.0), d(0.0) {}
  Matrix(const complex <double> & A, const complex <double> & B, const complex <double> & C, const complex <double> & D)
    : a(A), b(B), c(C), d(D) {}
  
  Matrix Invert() const;
  complex <double> CalculateDeterminant() const;
  std::string ToString() const;

  complex <double> a, b, c, d;

private:
  static std::string ComplexToString(const complex <double> & value); 
};

Matrix operator + (const Matrix & lhs, const Matrix & rhs);
Matrix operator - (const Matrix & lhs, const Matrix & rhs);
Matrix operator * (const Matrix & lhs, const Matrix & rhs);
Matrix operator * (const Matrix & lhs, const complex <double> & rhs);
Matrix operator * (const complex <double> &lhs, const Matrix & rhs);
Matrix operator / (const Matrix & lhs, const complex <double> & rhs);
Matrix operator - (const Matrix & rhs);
std::ostream & operator << (std::ostream & lhs, const Matrix & rhs);
bool operator == (const Matrix & lhs, const Matrix & rhs);

#endif
