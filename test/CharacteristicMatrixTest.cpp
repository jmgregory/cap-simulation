#include <cpptest.h>
#include "../CharacteristicMatrix.h"

class CharacteristicMatrixTestSuite : public Test::Suite {
public:
  CharacteristicMatrixTestSuite() {
    
  }

  
}

int main()
{
  CharacteristicMatrixTestSuite ts;
  return ts.run();
}
