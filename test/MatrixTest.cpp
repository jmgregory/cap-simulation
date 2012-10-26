#include <UnitTest++.h>
#include "../Matrix.h"

struct MatrixTestFixture
{
  
};

TEST(MatrixEquality)
{
  Matrix m;
  CHECK_EQUAL(m, m);
}
