#ifndef JMG_CHARACTERISTIC_MATRIX_H
#define JMG_CHARACTERISTIC_MATRIX_H

#include <complex>
#include <vector>
#include "Matrix.h"

using std::complex;

const complex <double> air_index = complex <double> (1.0, 0.0);

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

  Matrix GetMatrix() const;
  bool IsHomogeneous() const;

  CharacteristicMatrix & operator *= (const CharacteristicMatrix & rhs);

  double ReflectivityInEnvironment(complex <double> index_before = air_index, complex <double> index_after = air_index) const;
  double TransmissionInEnvironment(complex <double> index_before = air_index, complex <double> index_after = air_index) const;

  static CharacteristicMatrix MultiplyMatrices(const std::vector <CharacteristicMatrix> & matrices);
  
 private:
  bool _is_homogeneous;
  double _n, _k;
  double _thickness;
  double _wavelength;
  Matrix _matrix;

  void CalculateHomogeneousMatrix();
};

CharacteristicMatrix operator * (const CharacteristicMatrix & lhs, const CharacteristicMatrix & rhs);

#endif
