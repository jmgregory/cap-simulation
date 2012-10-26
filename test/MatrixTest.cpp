#include <UnitTest++.h>
#include <complex>
#include <sstream>
#include "../Matrix.h"

struct MatrixTestFixture
{
  MatrixTestFixture() 
    : test_matrix_1(1.0, 2.0, 3.0, 4.0),
      test_matrix_2(0.5, 4.0, 1.0, 1.5) {}

  Matrix test_matrix_1;
  Matrix test_matrix_2;
};

TEST(MatrixAssignmentAndEquality)
{
  Matrix m;
  CHECK(m == m);
  CHECK_EQUAL(m, m);
  Matrix n(m);
  CHECK_EQUAL(m, n);
  Matrix o = m;
  CHECK_EQUAL(m, o);
}

TEST(MatrixDefaultConstructor)
{
  Matrix empty;
  CHECK_EQUAL(0.0, empty.a);
  CHECK_EQUAL(0.0, empty.b);
  CHECK_EQUAL(0.0, empty.c);
  CHECK_EQUAL(0.0, empty.d);
}

TEST_FIXTURE(MatrixTestFixture, MatrixConstructor)
{
  CHECK_EQUAL(1.0, test_matrix_1.a);
  CHECK_EQUAL(2.0, test_matrix_1.b);
  CHECK_EQUAL(3.0, test_matrix_1.c);
  CHECK_EQUAL(4.0, test_matrix_1.d);  
}

TEST_FIXTURE(MatrixTestFixture, InversionCalculation)
{
  complex <double> determinant = test_matrix_1.a * test_matrix_1.d - test_matrix_1.b * test_matrix_1.c;
  Matrix expected( test_matrix_1.d/determinant, 
		  -test_matrix_1.b/determinant, 
		  -test_matrix_1.c/determinant, 
		   test_matrix_1.a/determinant);
  CHECK_EQUAL(expected, test_matrix_1.Inverted());
}

TEST_FIXTURE(MatrixTestFixture, DeterminantCalculation)
{
  complex <double> expected = test_matrix_1.a * test_matrix_1.d - test_matrix_1.b * test_matrix_1.c;
  CHECK_EQUAL(expected, test_matrix_1.CalculateDeterminant());
}

TEST_FIXTURE(MatrixTestFixture, ToStringProducesText)
{
  CHECK(test_matrix_1.ToString().length() != 0);
}

TEST_FIXTURE(MatrixTestFixture, MatrixMathOperations)
{
  CHECK_EQUAL(Matrix(1.5, 6.0, 4.0, 5.5), test_matrix_1 + test_matrix_2);
  CHECK_EQUAL(Matrix(0.5, -2.0, 2.0, 2.5), test_matrix_1 - test_matrix_2);
  CHECK_EQUAL(Matrix(2.5, 7.0, 5.5, 18), test_matrix_1 * test_matrix_2);
  CHECK_EQUAL(Matrix(0.5, 1.0, 1.5, 2.0), test_matrix_1 * 0.5);
  CHECK_EQUAL(Matrix(0.5, 1.0, 1.5, 2.0), 0.5 * test_matrix_1);
  CHECK_EQUAL(Matrix(0.5, 1.0, 1.5, 2.0), test_matrix_1 / 2.0);
  CHECK_EQUAL(Matrix(-1.0, -2.0, -3.0, -4.0), -test_matrix_1);
}

TEST_FIXTURE(MatrixTestFixture, OutputOperatorProducesText)
{
  std::stringstream ss;
  ss << test_matrix_1;
  CHECK(ss.str().length() > 0);
}
