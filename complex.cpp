#include "complex.h"

complex operator + (complex lhs, complex rhs)
{
  return complex(lhs.real + rhs.real, lhs.imag + rhs.imag);
}

complex operator - (complex lhs, complex rhs)
{
  return complex(lhs.real - rhs.real, lhs.imag - rhs.imag);  
}

complex operator * (complex lhs, complex rhs)
{
  return complex(lhs.real * rhs.real - lhs.imag * rhs.imag, lhs.imag * rhs.real + lhs.real * rhs.imag);
}

complex operator / (complex lhs, complex rhs)
{
  double denom = rhs.real*rhs.real + rhs.imag*rhs.imag;
  complex numer = complex(lhs.real * rhs.real + lhs.imag * rhs.imag, lhs.imag * rhs.real - lhs.real * rhs.imag);
  return numer / denom;
}

complex operator + (complex lhs, double rhs)
{
  return complex(lhs.real + rhs, lhs.imag);
}

complex operator - (complex lhs, double rhs)
{
  return complex(lhs.real - rhs, lhs.imag);  
}

complex operator * (complex lhs, double rhs)
{
  return complex(lhs.real * rhs, lhs.imag * rhs);
}

complex operator / (complex lhs, double rhs)
{
  return complex(lhs.real / rhs, lhs.imag / rhs);  
}

complex operator + (double lhs, complex rhs)
{
  return complex(lhs + rhs.real, rhs.imag);
}

complex operator - (double lhs, complex rhs)
{
  return complex(lhs - rhs.real, -rhs.imag);  
}

complex operator * (double lhs, complex rhs)
{
  return complex(lhs * rhs.real, lhs*rhs.imag);
}

complex operator / (double lhs, complex rhs)
{
  double denom = rhs.real*rhs.real + rhs.imag*rhs.imag;
  complex numer = complex(lhs * rhs.real, -lhs * rhs.imag);
  return numer / denom;
}

double real(complex z)
{
  return z.real;
}

double imag(complex z)
{
  return z.imag;
}

complex conj(complex z)
{
  return complex(z.real, -z.imag);
}

double magsquared(complex z)
{
  return real(z*conj(z));
}

double sgn(double d)
{
  if (d > 0) return 1.0;
  else if (d < 0) return -1.0;
  return 0.0;
}

complex sqrt(complex z)
{
  double gam = sqrt((z.real + sqrt(z.real*z.real + z.imag*z.imag))/2);
  double del = sgn(z.imag)*sqrt((-z.real + sqrt(z.real*z.real + z.imag*z.imag))/2);
  return complex(gam, del);
}

double mag(complex z)
{
  return sqrt(z.real * z.real + z.imag * z.imag);
}

double arg(complex z)
{
  return atan2(z.imag, z.real);
}

complex::complex()
  :real(0.0), imag(0.0)
{ }

complex::complex(double r)
  :real(r), imag(0.0)
{ }

complex::complex(double r, double i)
  :real(r), imag(i)
{ }

bool operator == (complex lhs, complex rhs)
{
  return (lhs.real == rhs.real && lhs.imag == rhs.imag);
}

bool operator == (double lhs, complex rhs)
{
  return (lhs == rhs.real && rhs.imag == 0.0);
}

bool operator == (complex lhs, double rhs)
{
  return (lhs.real == rhs && lhs.imag == 0.0);
}

complex complex::operator = (complex rhs)
{
  real = rhs.real;
  imag = rhs.imag;
  return *this;
}

complex complex::operator = (double rhs)
{
  real = rhs;
  imag = 0.0;
  return *this;
}

complex operator - (complex rhs)
{
  return complex(-rhs.real, -rhs.imag);
}

complex sin(complex z)
{
  return complex(sin(z.real)*cosh(z.imag), cos(z.real)*sinh(z.imag));
}

complex cos(complex z)
{
  return complex(cos(z.real)*cosh(z.imag), -sin(z.real)*sinh(z.imag));
}

int factorial(int i)
{
  if (i == 1) return 1;
  if (i == 2) return 2;
  return factorial(i-1) * i;
}

complex exp(complex pow)
{
  return exp(pow.real) * (cos(pow.imag) + sin(pow.imag)*I);
}

ostream & operator << (ostream & out, complex z)
{
  out << z.real << " + " << z.imag << "i";
  return out;
}

bool isReal(complex z)
{
  return (z.imag == 0);
}

bool isImaginary(complex z)
{
  return (z.real == 0);
}

