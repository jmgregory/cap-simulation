#include "CharacteristicMatrix.h"
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <assert.h>

using namespace std;

const Matrix Matrix::identity_matrix = Matrix(1, 0, 0, 1);

Matrix Matrix::invert() const
{
  Matrix temp(d, -b, -c, a);
  return temp / calc_determinant();
}

complex <double> Matrix::calc_determinant() const
{
  return (a * d) - (b * c);
}

string Matrix::to_string() const
{
  stringstream out;
  out << a;
  string sa = out.str();
  out.str("");
  out.clear();
  out << b;
  string sb = out.str();
  out.str("");
  out.clear();
  out << c;
  string sc = out.str();
  out.str("");
  out.clear();
  out << d;
  string sd = out.str();

  unsigned int maxWidthLeft  = ((sa.length() > sc.length()) ? sa.length() : sc.length());
  unsigned int maxWidthRight = ((sb.length() > sd.length()) ? sb.length() : sd.length());

  while (sa.length() < maxWidthLeft)  sa += " ";
  while (sb.length() < maxWidthRight) sb += " ";
  while (sc.length() < maxWidthLeft)  sc += " ";
  while (sd.length() < maxWidthRight) sd += " ";

  out.str("");
  out.clear();
  out << endl;
  out << "| " << sa << " " << sb << " |" << endl;
  out << "| " << sc << " " << sd << " |" << endl;

  return out.str();
}

Matrix operator + (const Matrix & lhs, const Matrix & rhs)
{
  return Matrix(lhs.a + rhs.a, lhs.b + rhs.b, lhs.c + rhs.c, lhs.d + rhs.d);
}

Matrix operator - (const Matrix & lhs, const Matrix & rhs)
{
  return lhs + (rhs * -1.0);
}

Matrix operator * (const Matrix & lhs, const Matrix & rhs)
{
  return Matrix(lhs.a * rhs.a + lhs.b * rhs.c, 
		lhs.a * rhs.b + lhs.b * rhs.d, 
		lhs.c * rhs.a + lhs.d * rhs.c, 
		lhs.c * rhs.b + lhs.d * rhs.d);
}

Matrix operator * (const Matrix & lhs, complex <double> rhs)
{
  return Matrix(lhs.a * rhs, lhs.b * rhs, lhs.c * rhs, lhs.d * rhs);
}

Matrix operator / (const Matrix & lhs, complex <double> rhs)
{
  return Matrix(lhs.a / rhs, lhs.b / rhs, lhs.c / rhs, lhs.d / rhs);
}

Matrix operator * (complex <double> lhs, const Matrix & rhs)
{
  return Matrix(rhs.a * lhs, rhs.b * lhs, rhs.c * lhs, rhs.d * lhs);
}

Matrix operator - (const Matrix & rhs)
{
  return rhs * -1.0;
}

ostream & operator << (ostream & lhs, const Matrix & rhs)
{
  lhs << rhs.to_string();
  return lhs;
}

CharacteristicMatrix::CharacteristicMatrix()
{
  _is_homogeneous = true;
  _n = 1.0;
  _k = 0.0;
  _thickness = 1e-9;
  _wavelength = 400e-9;
  evaluate_homogeneous();
}

CharacteristicMatrix::CharacteristicMatrix(double n, double k, double thickness, double wavelength)
{
  _is_homogeneous = true;
  _n = n;
  _k = k;
  _thickness = thickness;
  _wavelength = wavelength;
  evaluate_homogeneous();
}

CharacteristicMatrix::CharacteristicMatrix(complex <double> index, double thickness, double wavelength)
{
  _is_homogeneous = true;
  _n = real(index);
  _k = imag(index);
  _thickness = thickness;
  _wavelength = wavelength;
  evaluate_homogeneous();
}

CharacteristicMatrix operator * (const CharacteristicMatrix & lhs, const CharacteristicMatrix & rhs)
{
  CharacteristicMatrix temp = lhs;
  temp *= rhs;
  return temp;
}

Matrix CharacteristicMatrix::matrix() const
{
  return _matrix;
}

bool CharacteristicMatrix::IsHomogeneous() const
{
  return _is_homogeneous;
}

complex <double> CharacteristicMatrix::index() const
{
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
}

void CharacteristicMatrix::evaluate_homogeneous()
{
  assert(_is_homogeneous);
  complex <double> I(0, 1);
  complex <double> k_n = 2 * pi * index() / _wavelength;
  Matrix Mn(1.0, 1.0, k_n, -k_n);
  Matrix Qn(exp(I * k_n * _thickness), 0.0, 0.0, exp(-I * k_n * _thickness));
  _matrix = Mn * Qn.invert() * Mn.invert();
}

CharacteristicMatrix & CharacteristicMatrix::operator *= (const CharacteristicMatrix & rhs)
{
  assert(_wavelength == rhs.wavelength());
  _matrix = (_matrix * rhs.matrix());
  _is_homogeneous = false;
  _n = -1;
  _k = -1;
  _thickness += rhs.thickness();
  return *this;
}

double CharacteristicMatrix::ReflectivityInEnvironment(complex <double> index_before, complex <double> index_after) const
{
  Matrix M0(1.0, 1.0, 2.0 * pi * index_before / _wavelength, -2.0 * pi * index_before / _wavelength);
  Matrix Ma(1.0, 1.0, 2.0 * pi * index_after  / _wavelength, -2.0 * pi * index_after  / _wavelength);
  Matrix M = M0.invert() * _matrix * Ma;

  complex <double> b0 = M.c / M.a;
  return real(b0) * real(b0) + imag(b0) * imag(b0);
}

double CharacteristicMatrix::TransmissionInEnvironment(complex <double> index_before, complex <double> index_after) const
{
  Matrix M0(1.0, 1.0, 2.0 * pi * index_before / _wavelength, -2.0 * pi * index_before / _wavelength);
  Matrix Ma(1.0, 1.0, 2.0 * pi * index_after  / _wavelength, -2.0 * pi * index_after  / _wavelength);
  Matrix M = M0.invert() * _matrix * Ma;

  complex <double> a1 = 1.0 / M.a;
  return real(a1) * real(a1) + imag(a1) * imag(a1);
}
