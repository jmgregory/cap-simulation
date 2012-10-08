#ifndef JMG_CHARACTERISTIC_MATRIX_H
#define JMG_CHARACTERISTIC_MATRIX_H

#include <complex>
#include <iostream>
#include <string>

using namespace std;

#ifndef pi
#define pi 3.1415926535897932384626
#endif

class matrix
{
 public:
  complex <double> a, b, c, d;

  matrix();
  matrix(complex <double> a, complex <double> b, complex <double> c, complex <double> d);

  matrix invert() const;
  complex <double> determinant() const;

  string to_string() const;
};

const matrix identity_matrix = matrix(1, 0, 0, 1);

matrix operator + (const matrix & lhs, const matrix & rhs);
matrix operator - (const matrix & lhs, const matrix & rhs);
matrix operator * (const matrix & lhs, const matrix & rhs);
matrix operator * (const matrix & lhs, complex <double> rhs);
matrix operator * (complex <double> lhs, const matrix & rhs);
matrix operator / (const matrix & lhs, complex <double> rhs);
matrix operator - (const matrix & rhs);
ostream & operator << (ostream & lhs, const matrix & rhs);

const complex <double> air_index = complex <double> (1, 0);

class characteristic_matrix
{
 public:
  characteristic_matrix();
  characteristic_matrix(double n, double k, double d, double lambda);
  characteristic_matrix(complex <double> index, double D, double lambda);

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
  matrix get_matrix() const;
  complex <double> get_index() const;
  
  characteristic_matrix & operator *= (const characteristic_matrix & rhs);

  double reflectivity(complex <double> index_before = air_index, complex <double> index_after = air_index) const;
  double transmission(complex <double> index_before = air_index, complex <double> index_after = air_index) const;
  
 private:
  matrix myMatrix;
  double thickness;
  double n, k;
  bool isHomogeneous;
  double wavelength;

  void evaluate_homogeneous();
};

characteristic_matrix operator * (const characteristic_matrix & lhs, const characteristic_matrix & rhs);

#endif
