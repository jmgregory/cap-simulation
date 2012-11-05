#include "CharacteristicMatrix.h"
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <assert.h>

static const double pi = 3.1415926535897932384626;

CharacteristicMatrix::CharacteristicMatrix()
  : _thickness(1e-9), _wavelength(400e-9)
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

double CharacteristicMatrix::wavelength() const
{
  return _wavelength;
}

CharacteristicMatrix & CharacteristicMatrix::operator *= (const CharacteristicMatrix & rhs)
{
  assert(_wavelength == rhs.wavelength());
  _matrix = (_matrix * rhs.GetMatrix());
  _thickness += rhs.thickness();
  return *this;
}

double CharacteristicMatrix::MagnitudeSquared(const complex <double> & number)
{
  return real(number) * real(number) + imag(number) * imag(number);
}

complex <double> CharacteristicMatrix::Wavenumber(const complex <double> & index) const
{
  return 2.0 * pi * index / _wavelength;
}

Matrix CharacteristicMatrix::CalculateFullMatrixInEnvironment(const complex <double> & index_before, const complex <double> & index_after) const
{
  Matrix M0(1.0, 1.0, Wavenumber(index_before), -Wavenumber(index_before));
  Matrix Ma(1.0, 1.0, Wavenumber(index_after),  -Wavenumber(index_after));
  return M0.Inverted() * _matrix * Ma;
}

double CharacteristicMatrix::ReflectivityInEnvironment(const complex <double> & index_before, const complex <double> & index_after) const
{
  Matrix M = CalculateFullMatrixInEnvironment(index_before, index_after);
  complex <double> b0 = M.c / M.a;
  return MagnitudeSquared(b0);
}

double CharacteristicMatrix::TransmissionInEnvironment(const complex <double> & index_before, const complex <double> & index_after) const
{
  Matrix M = CalculateFullMatrixInEnvironment(index_before, index_after);
  complex <double> a1 = 1.0 / M.a;
  return MagnitudeSquared(a1);
}
