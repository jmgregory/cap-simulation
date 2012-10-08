#include "characteristic_matrix.h"
#include <iostream>
#include <sstream>
#include <cstdlib>

using namespace std;

matrix::matrix()
{
  a = b = c = d = 0.0;
}

matrix::matrix(complex <double> A, complex <double> B, complex <double> C, complex <double> D)
  : a(A), b(B), c(C), d(D)
{ }

matrix matrix::invert() const
{
  matrix temp(d, -b, -c, a);
  temp = temp / this->determinant();
  return temp;
}

complex <double> matrix::determinant() const
{
  return (a * d) - (b * c);
}

string matrix::to_string() const
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

matrix operator + (const matrix & lhs, const matrix & rhs)
{
  return matrix(lhs.a + rhs.a, lhs.b + rhs.b, lhs.c + rhs.c, lhs.d + rhs.d);
}

matrix operator - (const matrix & lhs, const matrix & rhs)
{
  return lhs + (rhs * -1.0);
}

matrix operator * (const matrix & lhs, const matrix & rhs)
{
  return matrix(lhs.a * rhs.a + lhs.b * rhs.c, 
		lhs.a * rhs.b + lhs.b * rhs.d, 
		lhs.c * rhs.a + lhs.d * rhs.c, 
		lhs.c * rhs.b + lhs.d * rhs.d);
}

matrix operator * (const matrix & lhs, complex <double> rhs)
{
  return matrix(lhs.a * rhs, lhs.b * rhs, lhs.c * rhs, lhs.d * rhs);
}

matrix operator / (const matrix & lhs, complex <double> rhs)
{
  return matrix(lhs.a / rhs, lhs.b / rhs, lhs.c / rhs, lhs.d / rhs);
}

matrix operator * (complex <double> lhs, const matrix & rhs)
{
  return matrix(rhs.a * lhs, rhs.b * lhs, rhs.c * lhs, rhs.d * lhs);
}

matrix operator - (const matrix & rhs)
{
  return rhs * -1.0;
}

ostream & operator << (ostream & lhs, const matrix & rhs)
{
  lhs << rhs.to_string();
  return lhs;
}

characteristic_matrix::characteristic_matrix()
{
  isHomogeneous = true;
  n = 1.0;
  k = 0.0;
  thickness = 1e-9;
  wavelength = 400e-9;
  evaluate_homogeneous();
}

characteristic_matrix::characteristic_matrix(double N, double K, double D, double lambda)
{
  isHomogeneous = true;
  n = N;
  k = K;
  thickness = D;
  wavelength = lambda;
  evaluate_homogeneous();
}

characteristic_matrix::characteristic_matrix(complex <double> index, double D, double lambda)
{
  isHomogeneous = true;
  n = real(index);
  k = imag(index);
  thickness = D;
  wavelength = lambda;
  evaluate_homogeneous();
}

characteristic_matrix operator * (const characteristic_matrix & lhs, const characteristic_matrix & rhs)
{
  characteristic_matrix temp = lhs;
  temp *= rhs;
  return temp;
}

matrix characteristic_matrix::get_matrix() const
{
  return myMatrix;
}

bool characteristic_matrix::is_homogeneous() const
{
  return isHomogeneous;
}

complex <double> characteristic_matrix::get_index() const
{
  return complex <double> (n, k);
}

double characteristic_matrix::get_n() const
{
  if (isHomogeneous)
    {
      return n;
    }
  else
    {
      cerr << "Error: tried get_n() on a non-homogeneous characteristic matrix." << endl;
      exit(1);
    }
}

double characteristic_matrix::get_k() const
{
  if (isHomogeneous)
    {
      return k;
    }
  else
    {
      cerr << "Error: tried get_k() on a non-homogeneous characteristic matrix." << endl;
      exit(1);
    }  
}

double characteristic_matrix::get_thickness() const
{
  return thickness;
}

double characteristic_matrix::get_wavelength() const
{
  return wavelength;
}

void characteristic_matrix::set_n(double N)
{
  set_index(N, k);
}

void characteristic_matrix::set_k(double K)
{
  set_index(n, K);
}

void characteristic_matrix::set_thickness(double D)
{
  if (isHomogeneous)
    {
      thickness = D;
      evaluate_homogeneous();
    }
  else
    {
      cerr << "Error: tried set_thickness() on a non-homogeneous characteristic matrix." << endl;
      exit(1);
    }  
}

void characteristic_matrix::set_index(complex <double> index)
{
  set_index(real(index), imag(index));
}

void characteristic_matrix::set_index(double N, double K)
{
  if (isHomogeneous)
    {
      n = N;
      k = K;
      evaluate_homogeneous();
    }
  else
    {
      cerr << "Error: tried set_index() on a non-homogeneous characteristic matrix." << endl;
      exit(1);
    }  
}

void characteristic_matrix::evaluate_homogeneous()
{
  if (!isHomogeneous)
    {
      cerr << "Error: tried to evaluate the members of a non-homogenous characteristic matrix." << endl;
      exit(1);
    }
  complex <double> I(0, 1);
  complex <double> k_n = 2 * pi * get_index() / wavelength;
  matrix Mn(1, 1, k_n, -k_n);
  matrix Qn(exp(I*k_n*thickness), 0, 0, exp(-I*k_n*thickness));
  myMatrix = Mn * Qn.invert() * Mn.invert();
}

characteristic_matrix & characteristic_matrix::operator *= (const characteristic_matrix & rhs)
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

double characteristic_matrix::reflectivity(complex <double> index_before, complex <double> index_after) const
{
  matrix M0(1, 1, 2*pi*index_before/wavelength, -2*pi*index_before/wavelength);
  matrix Ma(1, 1, 2*pi*index_after/wavelength, -2*pi*index_after/wavelength);
  matrix M = M0.invert() * myMatrix * Ma;

  complex <double> b0 = M.c / M.a;
  return real(b0) * real(b0) + imag(b0) * imag(b0);
}

double characteristic_matrix::transmission(complex <double> index_before, complex <double> index_after) const
{
  matrix M0(1, 1, 2*pi*index_before/wavelength, -2*pi*index_before/wavelength);
  matrix Ma(1, 1, 2*pi*index_after/wavelength, -2*pi*index_after/wavelength);
  matrix M = M0.invert() * myMatrix * Ma;

  complex <double> a1 = 1.0 / M.a;
  return real(a1) * real(a1) + imag(a1) * imag(a1);
}
