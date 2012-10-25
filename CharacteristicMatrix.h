#ifndef JMG_CHARACTERISTIC_MATRIX_H
#define JMG_CHARACTERISTIC_MATRIX_H

#include <complex>
#include <iostream>
#include <string>

using namespace std;

#ifndef pi
#define pi 3.1415926535897932384626
#endif

class Matrix
{
 public:

  static Matrix identity_matrix;

  complex <double> a, b, c, d;

  Matrix();
  Matrix(complex <double> a, complex <double> b, complex <double> c, complex <double> d);

  Matrix invert() const;
  complex <double> calc_determinant() const;

  string to_string() const;
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
  CharacteristicMatrix(double n, double k, double d, double lambda);
  CharacteristicMatrix(complex <double> index, double D, double lambda);

  void set_thickness(double D);
  void set_n(double N);
  void set_k(double K);
  void set_index(complex <double> index);
  void set_index(double N, double K = 0.0);

  bool is_homogeneous() const;
  double get_n() const;
  double get_k() const;
  double get_thickness() const;
  double get_wavelength() const;
  Matrix get_matrix() const;
  complex <double> get_index() const;
  
  CharacteristicMatrix & operator *= (const CharacteristicMatrix & rhs);

  double reflectivity(complex <double> index_before = air_index, complex <double> index_after = air_index) const;
  double transmission(complex <double> index_before = air_index, complex <double> index_after = air_index) const;
  
 private:
  Matrix myMatrix;
  double thickness;
  double n, k;
  bool isHomogeneous;
  double wavelength;

  void evaluate_homogeneous();
};

CharacteristicMatrix operator * (const CharacteristicMatrix & lhs, const CharacteristicMatrix & rhs);

#endif
