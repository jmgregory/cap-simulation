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
  Matrix(complex <double> a, complex <double> b, complex <double> c, complex <double> d)
    : a(a), b(b), c(c), d(d) {}
  
  Matrix Invert() const;
  complex <double> CalculateDeterminant() const;
  std::string ToString() const;

  complex <double> a, b, c, d;
};

Matrix operator + (const Matrix & lhs, const Matrix & rhs);
Matrix operator - (const Matrix & lhs, const Matrix & rhs);
Matrix operator * (const Matrix & lhs, const Matrix & rhs);
Matrix operator * (const Matrix & lhs, complex <double> rhs);
Matrix operator * (complex <double> lhs, const Matrix & rhs);
Matrix operator / (const Matrix & lhs, complex <double> rhs);
Matrix operator - (const Matrix & rhs);
std::ostream & operator << (std::ostream & lhs, const Matrix & rhs);

#endif
