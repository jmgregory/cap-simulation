#include "CharacteristicMatrix.h"
#include <iostream>
#include <sstream>
#include <cstdlib>

using namespace std;

Matrix Matrix::identity_matrix = Matrix(1, 0, 0, 1);

Matrix::Matrix()
{
  a = b = c = d = 0.0;
}

Matrix::Matrix(complex <double> A, complex <double> B, complex <double> C, complex <double> D)
  : a(A), b(B), c(C), d(D)
{ }

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
  isHomogeneous = true;
  n = 1.0;
  k = 0.0;
  thickness = 1e-9;
  wavelength = 400e-9;
  evaluate_homogeneous();
}

CharacteristicMatrix::CharacteristicMatrix(double N, double K, double D, double lambda)
{
  isHomogeneous = true;
  n = N;
  k = K;
  thickness = D;
  wavelength = lambda;
  evaluate_homogeneous();
}

CharacteristicMatrix::CharacteristicMatrix(complex <double> index, double D, double lambda)
{
  isHomogeneous = true;
  n = real(index);
  k = imag(index);
  thickness = D;
  wavelength = lambda;
  evaluate_homogeneous();
}

CharacteristicMatrix operator * (const CharacteristicMatrix & lhs, const CharacteristicMatrix & rhs)
{
  CharacteristicMatrix temp = lhs;
  temp *= rhs;
  return temp;
}

Matrix CharacteristicMatrix::get_matrix() const
{
  return myMatrix;
}

bool CharacteristicMatrix::is_homogeneous() const
{
  return isHomogeneous;
}

complex <double> CharacteristicMatrix::get_index() const
{
  return complex <double> (n, k);
}

double CharacteristicMatrix::get_n() const
{
  if (isHomogeneous)
    {
      return n;
    }
  else
    {
      cerr << "Error: tried get_n() on a non-homogeneous characteristic Matrix." << endl;
      exit(1);
    }
}

double CharacteristicMatrix::get_k() const
{
  if (isHomogeneous)
    {
      return k;
    }
  else
    {
      cerr << "Error: tried get_k() on a non-homogeneous characteristic Matrix." << endl;
      exit(1);
    }  
}

double CharacteristicMatrix::get_thickness() const
{
  return thickness;
}

double CharacteristicMatrix::get_wavelength() const
{
  return wavelength;
}

void CharacteristicMatrix::set_n(double N)
{
  set_index(N, k);
}

void CharacteristicMatrix::set_k(double K)
{
  set_index(n, K);
}

void CharacteristicMatrix::set_thickness(double D)
{
  if (isHomogeneous)
    {
      thickness = D;
      evaluate_homogeneous();
    }
  else
    {
      cerr << "Error: tried set_thickness() on a non-homogeneous characteristic Matrix." << endl;
      exit(1);
    }  
}

void CharacteristicMatrix::set_index(complex <double> index)
{
  set_index(real(index), imag(index));
}

void CharacteristicMatrix::set_index(double N, double K)
{
  if (isHomogeneous)
    {
      n = N;
      k = K;
      evaluate_homogeneous();
    }
  else
    {
      cerr << "Error: tried set_index() on a non-homogeneous characteristic Matrix." << endl;
      exit(1);
    }  
}

void CharacteristicMatrix::evaluate_homogeneous()
{
  if (!isHomogeneous)
    {
      cerr << "Error: tried to evaluate the members of a non-homogenous characteristic Matrix." << endl;
      exit(1);
    }
  complex <double> I(0, 1);
  complex <double> k_n = 2 * pi * get_index() / wavelength;
  Matrix Mn(1, 1, k_n, -k_n);
  Matrix Qn(exp(I*k_n*thickness), 0, 0, exp(-I*k_n*thickness));
  myMatrix = Mn * Qn.invert() * Mn.invert();
}

CharacteristicMatrix & CharacteristicMatrix::operator *= (const CharacteristicMatrix & rhs)
{
  if (wavelength != rhs.get_wavelength())
    {
      cerr << "Warning: multiplying two characteristic matrices with different wavelengths: ";
      cerr << wavelength << " != " << rhs.get_wavelength() << endl;
    }
  myMatrix = (myMatrix * rhs.get_matrix());
  isHomogeneous = false;
  n = -1;
  k = -1;
  thickness += rhs.get_thickness();
  return *this;
}

double CharacteristicMatrix::reflectivity(complex <double> index_before, complex <double> index_after) const
{
  Matrix M0(1, 1, 2*pi*index_before/wavelength, -2*pi*index_before/wavelength);
  Matrix Ma(1, 1, 2*pi*index_after/wavelength, -2*pi*index_after/wavelength);
  Matrix M = M0.invert() * myMatrix * Ma;

  complex <double> b0 = M.c / M.a;
  return real(b0) * real(b0) + imag(b0) * imag(b0);
}

double CharacteristicMatrix::transmission(complex <double> index_before, complex <double> index_after) const
{
  Matrix M0(1, 1, 2*pi*index_before/wavelength, -2*pi*index_before/wavelength);
  Matrix Ma(1, 1, 2*pi*index_after/wavelength, -2*pi*index_after/wavelength);
  Matrix M = M0.invert() * myMatrix * Ma;

  complex <double> a1 = 1.0 / M.a;
  return real(a1) * real(a1) + imag(a1) * imag(a1);
}
