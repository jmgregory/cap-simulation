#include "HomogeneousCharacteristicMatrix.h"

HomogeneousCharacteristicMatrix::HomogeneousCharacteristicMatrix(double n, double k, double thickness, double wavelength)
  : CharacteristicMatrix(thickness), _n(n), _k(k), _wavelength(wavelength)
{
  CalculateHomogeneousMatrix();
}

HomogeneousCharacteristicMatrix::HomogeneousCharacteristicMatrix(complex <double> index, double thickness, double wavelength)
  : CharacteristicMatrix(thickness), _n(real(index)), _k(imag(index)), _wavelength(wavelength)
{
  CalculateHomogeneousMatrix();
}

HomogeneousCharacteristicMatrix::HomogeneousCharacteristicMatrix()
  : CharacteristicMatrix(), _n(1.0), _k(0.0), _wavelength(400e-9)
{
  CalculateHomogeneousMatrix();
}

double HomogeneousCharacteristicMatrix::n() const
{
  return _n;
}

double HomogeneousCharacteristicMatrix::k() const
{
  return _k;
}

void HomogeneousCharacteristicMatrix::set_n(double n)
{
  set_index(n, _k);
}

void HomogeneousCharacteristicMatrix::set_k(double k)
{
  set_index(_n, k);
}

complex <double> HomogeneousCharacteristicMatrix::index() const
{
  return complex <double> (_n, _k);
}

void HomogeneousCharacteristicMatrix::set_index(complex <double> index)
{
  set_index(real(index), imag(index));
}

void HomogeneousCharacteristicMatrix::set_index(double n, double k)
{
  _n = n;
  _k = k;
  CalculateHomogeneousMatrix();
}

void HomogeneousCharacteristicMatrix::CalculateHomogeneousMatrix()
{
  complex <double> I(0.0, 1.0);
  complex <double> k_n = 2.0 * pi * index() / _wavelength;
  Matrix Mn(1.0, 1.0, k_n, -k_n);
  Matrix Qn(exp(I * k_n * _thickness), 0.0, 0.0, exp(-I * k_n * _thickness));
  _matrix = Mn * Qn.Inverted() * Mn.Inverted();
}

void HomogeneousCharacteristicMatrix::set_thickness(double thickness)
{
  _thickness = thickness;
  CalculateHomogeneousMatrix();
}

double HomogeneousCharacteristicMatrix::wavelength() const
{
  return _wavelength;
}

void HomogeneousCharacteristicMatrix::set_wavelength(double wavelength)
{
  _wavelength = wavelength;
  CalculateHomogeneousMatrix();
}
