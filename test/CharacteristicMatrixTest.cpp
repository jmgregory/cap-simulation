#include <UnitTest++.h>
#include "../CharacteristicMatrix.h"
#include "../HomogeneousCharacteristicMatrix.h"

class CharacteristicMatrixTestFixture {};

TEST_FIXTURE(CharacteristicMatrixTestFixture, AirTransmissionAndReflection)
{
  double wavelength = 400e-9;
  HomogeneousCharacteristicMatrix air(air_index, 1e-3, wavelength);
  CHECK_EQUAL(1.0, air.TransmissionInEnvironment());
  CHECK_EQUAL(0.0, air.ReflectivityInEnvironment());
}

TEST_FIXTURE(CharacteristicMatrixTestFixture, ReflectionAndTransmissionAddToOne)
{
  complex <double> index(1.55, 0.0);
  double wavelength = 400e-9;
  double thickness = 913e-9;
  HomogeneousCharacteristicMatrix glass(index, thickness, wavelength);
  CHECK_CLOSE(1.0, glass.TransmissionInEnvironment() + glass.ReflectivityInEnvironment(), 1e-16);
}

TEST_FIXTURE(CharacteristicMatrixTestFixture, OneLayerTransmissionAndReflection)
{
  const double pi = 3.1415926535897932384626;
  const complex <double> i(0.0, 1.0);

  double wavelength = 400e-9;
  double thickness = 913e-9;
  complex <double> index(1.55, 0.1);
  complex <double> index_before(1.2, 0.3);
  complex <double> index_after(1.1, 0.05);
  complex <double> wavenumber = 2.0 * pi * index / wavelength;
  complex <double> wavenumber_before = 2.0 * pi * index_before / wavelength;
  complex <double> wavenumber_after = 2.0 * pi * index_after / wavelength;
  
  complex <double> a_term = 0.25 * (1.0                                  * (exp(-i * wavenumber * thickness) + exp(i * wavenumber * thickness)) +
                                    wavenumber / wavenumber_before       * (exp(-i * wavenumber * thickness) - exp(i * wavenumber * thickness)) +
				    wavenumber_after / wavenumber        * (exp(-i * wavenumber * thickness) - exp(i * wavenumber * thickness)) +
				    wavenumber_after / wavenumber_before * (exp(-i * wavenumber * thickness) + exp(i * wavenumber * thickness)));
  complex <double> c_term = 0.25 * (1.0                                  * (exp(-i * wavenumber * thickness) + exp(i * wavenumber * thickness)) -
                                    wavenumber / wavenumber_before       * (exp(-i * wavenumber * thickness) - exp(i * wavenumber * thickness)) +
				    wavenumber_after / wavenumber        * (exp(-i * wavenumber * thickness) - exp(i * wavenumber * thickness)) -
				    wavenumber_after / wavenumber_before * (exp(-i * wavenumber * thickness) + exp(i * wavenumber * thickness)));

  double transmission = real(1.0 / a_term) * real(1.0 / a_term) + imag(1.0 / a_term) * imag(1.0 / a_term);
  double reflectivity = real(c_term / a_term) * real(c_term / a_term) + imag(c_term / a_term) * imag(c_term / a_term);

  HomogeneousCharacteristicMatrix glass(index, thickness, wavelength);

  CHECK_CLOSE(transmission, glass.TransmissionInEnvironment(index_before, index_after), 1e-16);
  CHECK_CLOSE(reflectivity, glass.ReflectivityInEnvironment(index_before, index_after), 1e-16);
}

TEST_FIXTURE(CharacteristicMatrixTestFixture, CheckMultiplication)
{
  double thickness1 = 1.1e-6;
  double thickness2 = 0.746e-6;
  double wavelength = 800e-9;
  HomogeneousCharacteristicMatrix hcm1(complex <double> (2.55, 0.1), thickness1, wavelength);
  HomogeneousCharacteristicMatrix hcm2(complex <double> (1.1, 0.67), thickness2, wavelength);
  Matrix multiplied = hcm1.GetMatrix() * hcm2.GetMatrix();

  CHECK_EQUAL((hcm1 * hcm2).GetMatrix(), multiplied);
  CHECK_EQUAL((hcm1 * hcm2).thickness(), thickness1 + thickness2);

  CharacteristicMatrix cm = hcm1;
  cm *= hcm2;
  CHECK_EQUAL(cm.GetMatrix(), multiplied);
  CHECK_EQUAL(cm.thickness(), thickness1 + thickness2);
}
