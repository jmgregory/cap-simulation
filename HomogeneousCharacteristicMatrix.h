#ifndef JMG_HOMOGENEOUS_CHARACTERISTIC_MATRIX_H
#define JMG_HOMOGENEOUS_CHARACTERISTIC_MATRIX_H

#include "CharacteristicMatrix.h"

class HomogeneousCharacteristicMatrix : public CharacteristicMatrix
{
 public:
  HomogeneousCharacteristicMatrix();
  HomogeneousCharacteristicMatrix(double n, double k, double thickness, double wavelength);
  HomogeneousCharacteristicMatrix(complex <double> index, double thickness, double wavelength);

  void set_n(double n);
  void set_k(double k);
  void set_index(complex <double> index);
  void set_index(double n, double k = 0.0);
  void set_wavelength(double wavelength);
  void set_thickness(double thickness);

  double n() const;
  double k() const;
  double wavelength() const;
  complex <double> index() const;

 private:
  double _n, _k;
  double _wavelength;
  static const double pi = 3.1415926535897932384626;

  // It doesn't make sense to *= a homogeneous matrix, because then it isn't
  // homogeneous anymore.
  // Thus, we need to hide the inherited *= operator for the homogeneous case
  HomogeneousCharacteristicMatrix & operator *= (const HomogeneousCharacteristicMatrix & rhs);

  void CalculateHomogeneousMatrix();
};

#endif
