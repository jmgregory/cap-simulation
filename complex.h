#ifndef JMG_COMPLEX
#define JMG_COMPLEX

#include <math.h>
#include <iostream>

using namespace std;

struct complex
{
  double real, imag;
  complex();
  complex(double);
  complex(double, double);
  complex operator = (complex rhs);
  complex operator = (double rhs);
};

#define I complex(0, 1)

complex operator + (complex lhs, complex rhs);
complex operator - (complex lhs, complex rhs);
complex operator * (complex lhs, complex rhs);
complex operator / (complex lhs, complex rhs);
complex operator + (complex lhs, double rhs);
complex operator - (complex lhs, double rhs);
complex operator * (complex lhs, double rhs);
complex operator / (complex lhs, double rhs);
complex operator + (double lhs, complex rhs);
complex operator - (double lhs, complex rhs);
complex operator * (double lhs, complex rhs);
complex operator / (double lhs, complex rhs);
double real(complex z);
double imag(complex z);
complex conj(complex z);
double magsquared(complex z);
double arg(complex z);
double mag(complex z);
complex sqrt(complex z);
bool operator == (complex lhs, complex rhs);
bool operator == (double lhs, complex rhs);
bool operator == (complex lhs, double rhs);
complex operator - (complex rhs);
complex sin(complex z);
complex cos(complex z);
complex exp(complex pow);
ostream & operator << (ostream & out, complex z);
bool isReal(complex z);
bool isImaginary(complex z);

#endif
