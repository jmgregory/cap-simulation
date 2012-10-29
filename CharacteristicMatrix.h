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

  double ReflectivityInEnvironment(complex <double> index_before = air_index, complex <double> index_after = air_index) const;
  double TransmissionInEnvironment(complex <double> index_before = air_index, complex <double> index_after = air_index) const;

  //static CharacteristicMatrix MultiplyMatrices(const std::vector <CharacteristicMatrix> & matrices);
  
  CharacteristicMatrix operator = (const CharacteristicMatrix & rhs);
 
 private:
  static double MagnitudeSquared(complex <double> number);

 protected:
  CharacteristicMatrix();
  CharacteristicMatrix(double thickness, double wavelength);

  double _thickness;
  double _wavelength;
  Matrix _matrix;
};

CharacteristicMatrix operator * (const CharacteristicMatrix & lhs, const CharacteristicMatrix & rhs);

#endif
