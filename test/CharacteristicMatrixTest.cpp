#include <UnitTest++.h>
#include "../CharacteristicMatrix.h"
#include "../HomogeneousCharacteristicMatrix.h"

class CharacteristicMatrixTestFixture
{
public:
  CharacteristicMatrixTestFixture()
  {

  }

  ~CharacteristicMatrixTestFixture()
  {

  }  
};

TEST_FIXTURE(CharacteristicMatrixTestFixture, AirTransmissionAndReflection)
{
  double wavelength = 400e-9;
  HomogeneousCharacteristicMatrix air(air_index, 1e-3, wavelength);
  CHECK_EQUAL(1.0, air.TransmissionInEnvironment(wavelength));
  CHECK_EQUAL(0.0, air.ReflectivityInEnvironment(wavelength));
}
