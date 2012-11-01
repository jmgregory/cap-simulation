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

  void set_thickness(double thickness);

  double wavelength() const;
  double thickness() const;
  Matrix GetMatrix() const;

  CharacteristicMatrix & operator *= (const CharacteristicMatrix & rhs);

  double ReflectivityInEnvironment(double wavelength, complex <double> index_before = air_index, complex <double> index_after = air_index) const;
  double TransmissionInEnvironment(double wavelength, complex <double> index_before = air_index, complex <double> index_after = air_index) const;

  //static CharacteristicMatrix MultiplyMatrices(const std::vector <CharacteristicMatrix> & matrices);
  
  CharacteristicMatrix operator = (const CharacteristicMatrix & rhs);
 
 private:
  static double MagnitudeSquared(complex <double> number);
  Matrix CalculateFullMatrixInEnvironment(double wavelength, complex <double> index_before, complex <double> index_after) const;

 protected:
  CharacteristicMatrix();
  CharacteristicMatrix(double thickness);

  double _thickness;
  Matrix _matrix;
};

CharacteristicMatrix operator * (const CharacteristicMatrix & lhs, const CharacteristicMatrix & rhs);

#endif
