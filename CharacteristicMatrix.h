#ifndef JMG_CHARACTERISTIC_MATRIX_H
#define JMG_CHARACTERISTIC_MATRIX_H

#include <complex>
#include <iostream>
#include <string>
#include <vector>

using std::complex;
using std::vector;
using std::string;
using std::ostream;

#ifndef pi
#define pi 3.1415926535897932384626
#endif

struct Matrix {
 public:
  static const Matrix identity_matrix;
  
 Matrix()
   : a(0.0), b(0.0), c(0.0), d(0.0) {}
 Matrix(complex <double> a, complex <double> b, complex <double> c, complex <double> d)
   : a(a), b(b), c(c), d(d) {}
  
  Matrix invert() const;
  complex <double> calc_determinant() const;
  string to_string() const;

  complex <double> a, b, c, d;
};

Matrix operator + (const Matrix & lhs, const Matrix & rhs);
Matrix operator - (const Matrix & lhs, const Matrix & rhs);
Matrix operator * (const Matrix & lhs, const Matrix & rhs);
Matrix operator * (const Matrix & lhs, complex <double> rhs);
Matrix operator * (complex <double> lhs, const Matrix & rhs);
Matrix operator / (const Matrix & lhs, complex <double> rhs);
Matrix operator - (const Matrix & rhs);
ostream & operator << (ostream & lhs, const Matrix & rhs);

const complex <double> air_index = complex <double> (1, 0);

class CharacteristicMatrix
{
 public:
  CharacteristicMatrix();
  CharacteristicMatrix(double n, double k, double thickness, double wavelength);
  CharacteristicMatrix(complex <double> index, double thickness, double wavelength);

  void set_thickness(double thickness);
  void set_n(double n);
  void set_k(double k);
  void set_index(complex <double> index);
  void set_index(double n, double k = 0.0);

  double n() const;
  double k() const;
  double thickness() const;
  double wavelength() const;
  complex <double> index() const;
  Matrix matrix() const;

  bool IsHomogeneous() const;

  CharacteristicMatrix & operator *= (const CharacteristicMatrix & rhs);

  double ReflectivityInEnvironment(complex <double> index_before = air_index, complex <double> index_after = air_index) const;
  double TransmissionInEnvironment(complex <double> index_before = air_index, complex <double> index_after = air_index) const;
  
 private:
  Matrix _matrix;
  double _thickness;
  double _n, _k;
  bool _is_homogeneous;
  double _wavelength;

  void evaluate_homogeneous();
};

CharacteristicMatrix operator * (const CharacteristicMatrix & lhs, const CharacteristicMatrix & rhs);

#endif
