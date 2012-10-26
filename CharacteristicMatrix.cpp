#include "CharacteristicMatrix.h"
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <assert.h>

static const double pi = 3.1415926535897932384626;

CharacteristicMatrix::CharacteristicMatrix()
  : _is_homogeneous(true), _n(1.0), _k(0.0), _thickness(1e-9), _wavelength(400e-9)
{
  CalculateHomogeneousMatrix();
}

CharacteristicMatrix::CharacteristicMatrix(double n, double k, double thickness, double wavelength)
  : _is_homogeneous(true), _n(n), _k(k), _thickness(thickness), _wavelength(wavelength)
{
  CalculateHomogeneousMatrix();
}

CharacteristicMatrix::CharacteristicMatrix(complex <double> index, double thickness, double wavelength)
  : _is_homogeneous(true), _n(real(index)), _k(imag(index)), _thickness(thickness), _wavelength(wavelength)
{
  CalculateHomogeneousMatrix();
}

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

bool CharacteristicMatrix::IsHomogeneous() const
{
  return _is_homogeneous;
}

complex <double> CharacteristicMatrix::index() const
{
  assert(_is_homogeneous);
  return complex <double> (_n, _k);
}

double CharacteristicMatrix::n() const
{
  assert(_is_homogeneous);
  return _n;
}

double CharacteristicMatrix::k() const
{
  assert(_is_homogeneous);
  return _k;
}

double CharacteristicMatrix::thickness() const
{
  return _thickness;
}

double CharacteristicMatrix::wavelength() const
{
  return _wavelength;
}

void CharacteristicMatrix::set_n(double n)
{
  set_index(n, _k);
}

void CharacteristicMatrix::set_k(double k)
{
  set_index(_n, k);
}

void CharacteristicMatrix::set_thickness(double thickness)
{
  assert(_is_homogeneous);
  _thickness = thickness;
  CalculateHomogeneousMatrix();
}

void CharacteristicMatrix::set_index(complex <double> index)
{
  set_index(real(index), imag(index));
}

void CharacteristicMatrix::set_index(double n, double k)
{
  assert(_is_homogeneous);
  _n = n;
  _k = k;
  CalculateHomogeneousMatrix();
}

void CharacteristicMatrix::CalculateHomogeneousMatrix()
{
  assert(_is_homogeneous);
  complex <double> I(0.0, 1.0);
  complex <double> k_n = 2.0 * pi * index() / _wavelength;
  Matrix Mn(1.0, 1.0, k_n, -k_n);
  Matrix Qn(exp(I * k_n * _thickness), 0.0, 0.0, exp(-I * k_n * _thickness));
  _matrix = Mn * Qn.Inverted() * Mn.Inverted();
}

CharacteristicMatrix & CharacteristicMatrix::operator *= (const CharacteristicMatrix & rhs)
{
  assert(_wavelength == rhs.wavelength());
  _matrix = (_matrix * rhs.GetMatrix());
  _is_homogeneous = false;
  _n = -1;
  _k = -1;
  _thickness += rhs.thickness();
  return *this;
}

double MagnitudeSquared(complex <double> number)
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

CharacteristicMatrix CharacteristicMatrix::MultiplyMatrices(const std::vector <CharacteristicMatrix> & matrices)
{
  assert(matrices.size() != 0);
  CharacteristicMatrix result = matrices[0];
  for(unsigned int i = 1; i < matrices.size(); i++)
    {
      result *= matrices[i];
    }
  return result;
}
