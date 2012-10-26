#include <UnitTest++.h>
#include <iostream>
#include <sstream>
#include "../TransducingLayer.h"

class TransducingLayerTestFixture
{
public:
  TransducingLayerTestFixture()
  {
    customLayer = TransducingLayer(0.5, 1e-6, 0.22, 1.0, 0.5, 1e-7);
  }

  TransducingLayer defaultLayer;
  TransducingLayer customLayer;
  std::stringstream string_stream;
};

TEST_FIXTURE(TransducingLayerTestFixture, StrainFactorCalculationCorrect)
{
  CHECK_CLOSE(0.002467, defaultLayer.CalculateStrainFactor(), 0.00001);
  CHECK_CLOSE(6.818e-7, customLayer.CalculateStrainFactor(), 0.001e-7);
}

TEST_FIXTURE(TransducingLayerTestFixture, ParameterOutputProducesText)
{
  defaultLayer.PrintParameters(string_stream);
  CHECK(string_stream.str().length() > 0);
}

TEST_FIXTURE(TransducingLayerTestFixture, ParameterOutputUsesCustomTag)
{
  defaultLayer.PrintParameters(string_stream, "myTAG");
  CHECK_EQUAL("myTAG", string_stream.str().substr(0, 5));
}
