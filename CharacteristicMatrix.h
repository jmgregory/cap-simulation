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
  double thickness() const;
  double wavelength() const;
  Matrix GetMatrix() const;

  CharacteristicMatrix & operator *= (const CharacteristicMatrix & rhs);

  double ReflectivityInEnvironment(const complex <double> & index_before = air_index, const complex <double> & index_after = air_index) const;
  double TransmissionInEnvironment(const complex <double> & index_before = air_index, const complex <double> & index_after = air_index) const;

 private:
  static double MagnitudeSquared(const complex <double> & number);
  Matrix CalculateFullMatrixInEnvironment(const complex <double> & index_before, const complex <double> & index_after) const;
  complex <double> Wavenumber(const complex <double> & index) const;

 protected:
  CharacteristicMatrix();

  double _thickness;
  double _wavelength;
  Matrix _matrix;
};

CharacteristicMatrix operator * (const CharacteristicMatrix & lhs, const CharacteristicMatrix & rhs);

#endif
