#include <cpptest.h>
#include "../CharacteristicMatrix.h"

class CharacteristicMatrixTestSuite : public Test::Suite {
public:
  CharacteristicMatrixTestSuite() {}
};

int main()
{
  Test::TextOutput output(Test::TextOutput::Verbose);
  CharacteristicMatrixTestSuite ts;
  return ts.run(output) ? 0 : 1;
}
