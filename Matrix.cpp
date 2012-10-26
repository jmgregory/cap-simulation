#include <string>
#include <sstream>
#include <iostream>
#include <complex>
#include "Matrix.h"

using std::string;
using std::complex;

const Matrix Matrix::identity_matrix = Matrix(1.0, 0.0, 0.0, 1.0);

Matrix Matrix::Invert() const
{
  Matrix inverted(d, -b, -c, a);
  return inverted / CalculateDeterminant();
}

complex <double> Matrix::CalculateDeterminant() const
{
  return (a * d) - (b * c);
}

string Matrix::ComplexToString(const complex <double> & value)
{
  std::stringstream out;
  out << value;
  return out.str();
}

int MaxLength(const string & a, const string & b)
{
  return (a.length() > b.length()) ? a.length() : b.length();
}

string PadStringToLength(const string & input_string, unsigned int final_length)
{
  string result = input_string;
  while (result.length() < final_length) result += " ";
  return result;
}

string Matrix::ToString() const
{
  string string_a = ComplexToString(a);
  string string_b = ComplexToString(b);
  string string_c = ComplexToString(c);
  string string_d = ComplexToString(d);

  unsigned int max_width_left  = MaxLength(string_a, string_c);
  unsigned int max_width_right = MaxLength(string_b, string_d);

  std::stringstream result;
  result << "| " << PadStringToLength(string_a, max_width_left) << " " << PadStringToLength(string_b, max_width_right) << " |" << std::endl;
  result << "| " << PadStringToLength(string_c, max_width_left) << " " << PadStringToLength(string_d, max_width_right) << " |" << std::endl;
  return result.str();
}

bool operator == (const Matrix & lhs, const Matrix & rhs)
{
  return (lhs.a == rhs.a &&
	  lhs.b == rhs.b &&
	  lhs.c == rhs.c &&
	  lhs.d == rhs.d);
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

Matrix operator * (const Matrix & lhs, const complex <double> & rhs)
{
  return Matrix(lhs.a * rhs, lhs.b * rhs, lhs.c * rhs, lhs.d * rhs);
}

Matrix operator / (const Matrix & lhs, const complex <double> & rhs)
{
  return Matrix(lhs.a / rhs, lhs.b / rhs, lhs.c / rhs, lhs.d / rhs);
}

Matrix operator * (const complex <double> & lhs, const Matrix & rhs)
{
  return Matrix(rhs.a * lhs, rhs.b * lhs, rhs.c * lhs, rhs.d * lhs);
}

Matrix operator - (const Matrix & rhs)
{
  return rhs * -1.0;
}

std::ostream & operator << (std::ostream & lhs, const Matrix & rhs)
{
  lhs << rhs.ToString();
  return lhs;
}
