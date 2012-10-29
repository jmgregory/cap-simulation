#include "CharacteristicMatrix.h"
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <assert.h>

static const double pi = 3.1415926535897932384626;

CharacteristicMatrix::CharacteristicMatrix()
  : _thickness(1e-9), _wavelength(400e-9)
{ }

CharacteristicMatrix::CharacteristicMatrix(double thickness, double wavelength)
  : _thickness(thickness), _wavelength(wavelength)
{ }

CharacteristicMatrix operator * (const CharacteristicMatrix & lhs, const CharacteristicMatrix & rhs)
{
  CharacteristicMatrix result = lhs;
  result *= rhs;
  return result;
}

Matrix CharacteristicMatrix::GetMatrix() const
{
  return _matrix;
}

double CharacteristicMatrix::thickness() const
{
  return _thickness;
}

void CharacteristicMatrix::set_thickness(double thickness)
{
  _thickness = thickness;
}

double CharacteristicMatrix::wavelength() const
{
  return _wavelength;
}

CharacteristicMatrix & CharacteristicMatrix::operator *= (const CharacteristicMatrix & rhs)
{
  _matrix = (_matrix * rhs.GetMatrix());
  _thickness += rhs.thickness();
  return *this;
}

double CharacteristicMatrix::MagnitudeSquared(complex <double> number)
{
  return real(number) * real(number) + imag(number) * imag(number);
}

double CharacteristicMatrix::ReflectivityInEnvironment(complex <double> index_before, complex <double> index_after) const
{
  Matrix M0(1.0, 1.0, 2.0 * pi * index_before / _wavelength, -2.0 * pi * index_before / _wavelength);
  Matrix Ma(1.0, 1.0, 2.0 * pi * index_after  / _wavelength, -2.0 * pi * index_after  / _wavelength);
  Matrix M = M0.Inverted() * _matrix * Ma;

  complex <double> b0 = M.c / M.a;
  return MagnitudeSquared(b0);
}

double CharacteristicMatrix::TransmissionInEnvironment(complex <double> index_before, complex <double> index_after) const
{
  Matrix M0(1.0, 1.0, 2.0 * pi * index_before / _wavelength, -2.0 * pi * index_before / _wavelength);
  Matrix Ma(1.0, 1.0, 2.0 * pi * index_after  / _wavelength, -2.0 * pi * index_after  / _wavelength);
  Matrix M = M0.Inverted() * _matrix * Ma;

  complex <double> a1 = 1.0 / M.a;
  return MagnitudeSquared(a1);
}

/*
CharacteristicMatrix CharacteristicMatrix::MultiplyMatrices(const std::vector <CharacteristicMatrix> & matrices)
{
  CharacteristicMatrix result = matrices[0];
  for(unsigned int i = 1; i < matrices.size(); i++)
    {
      result *= matrices[i];
    }
  return result;
}
*/

CharacteristicMatrix CharacteristicMatrix::operator = (const CharacteristicMatrix & rhs)
{
  _thickness = rhs.thickness();
  _wavelength = rhs.wavelength();
  _matrix = rhs.GetMatrix();
  return *this;
}
