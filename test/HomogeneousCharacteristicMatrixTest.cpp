#include <complex>
#include "../HomogeneousCharacteristicMatrix.h"

using std::complex;

class HomogeneousCharacteristicMatrixTestFixture
{
public:
  /*
  HomogeneousCharacteristicMatrixTestFixture()
  {

  }

  ~HomogeneousCharacteristicMatrixTestFixture()
  {

  } 
  */ 
};

TEST_FIXTURE(HomogeneousCharacteristicMatrixTestFixture, ConstructorSetsValues)
{
  HomogeneousCharacteristicMatrix hcm1(1.5, 0.1, 1.5e-6, 400e-9);
  CHECK_EQUAL(1.5, hcm1.n());
  CHECK_EQUAL(0.1, hcm1.k());
  CHECK_EQUAL(1.5e-6, hcm1.thickness());
  CHECK_EQUAL(400e-9, hcm1.wavelength());
  CHECK_EQUAL(complex <double> (1.5, 0.1), hcm1.index());

  HomogeneousCharacteristicMatrix hcm2(complex <double> (1.5, 0.1), 1.5e-6, 400e-9);
  CHECK_EQUAL(1.5, hcm2.n());
  CHECK_EQUAL(0.1, hcm2.k());
  CHECK_EQUAL(1.5e-6, hcm2.thickness());
  CHECK_EQUAL(400e-9, hcm2.wavelength());
  CHECK_EQUAL(complex <double> (1.5, 0.1), hcm2.index());
}

TEST_FIXTURE(HomogeneousCharacteristicMatrixTestFixture, CheckMatrixCalculation)
{
  double pi = 3.1415926535897932384626;
  double wavelength = 550e-9;
  complex <double> index(2.4, 0.1);
  double thickness = 1.2e-6;
  complex <double> k = 2.0 * pi * index / wavelength;
  complex <double> I(0.0, 1.0);

  Matrix expected(1.0 / 2.0 *       (exp(-I * k * thickness) + exp(I * k * thickness)),
		  1.0 / (2.0 * k) * (exp(-I * k * thickness) - exp(I * k * thickness)),
		  (1.0 / 2.0) * k * (exp(-I * k * thickness) - exp(I * k * thickness)),
		  1.0 / 2.0 *       (exp(-I * k * thickness) + exp(I * k * thickness)));
  
  HomogeneousCharacteristicMatrix hcm(index, thickness, wavelength);

  double tolerance = 1e-7;

  CHECK_CLOSE(expected.a.real(), hcm.GetMatrix().a.real(), tolerance);
  CHECK_CLOSE(expected.b.real(), hcm.GetMatrix().b.real(), tolerance);
  CHECK_CLOSE(expected.c.real(), hcm.GetMatrix().c.real(), tolerance);
  CHECK_CLOSE(expected.d.real(), hcm.GetMatrix().d.real(), tolerance);

  CHECK_CLOSE(expected.a.imag(), hcm.GetMatrix().a.imag(), tolerance);
  CHECK_CLOSE(expected.b.imag(), hcm.GetMatrix().b.imag(), tolerance);
  CHECK_CLOSE(expected.c.imag(), hcm.GetMatrix().c.imag(), tolerance);
  CHECK_CLOSE(expected.d.imag(), hcm.GetMatrix().d.imag(), tolerance);
}

TEST_FIXTURE(HomogeneousCharacteristicMatrixTestFixture, CheckSetters)
{
  HomogeneousCharacteristicMatrix hcm;
  hcm.set_n(2.4);
  CHECK_EQUAL(2.4, hcm.n());
  hcm.set_k(0.234);
  CHECK_EQUAL(0.234, hcm.k());
  CHECK_EQUAL(complex <double> (2.4, 0.234), hcm.index());
  hcm.set_index(1.0);
  CHECK_EQUAL(complex <double> (1.0, 0.0), hcm.index());
  hcm.set_index(2.4, 0.5);  
  CHECK_EQUAL(complex <double> (2.4, 0.5), hcm.index());
  hcm.set_index(complex <double> (1.1, 0.2));
  CHECK_EQUAL(complex <double> (1.1, 0.2), hcm.index());
  hcm.set_wavelength(880e-9);
  CHECK_EQUAL(880e-9, hcm.wavelength());
  hcm.set_thickness(1.5e-3);
  CHECK_EQUAL(1.5e-3, hcm.thickness());
}

TEST_FIXTURE(HomogeneousCharacteristicMatrixTestFixture, ChangingPropertiesChangesMatrix)
{
  HomogeneousCharacteristicMatrix hcm1(1.5, 0.1, 1.5e-6, 400e-9);
  HomogeneousCharacteristicMatrix hcm2;

  hcm2 = hcm1;
  CHECK_EQUAL(hcm2.GetMatrix(), hcm1.GetMatrix());
  hcm2.set_n(2.4);
  CHECK(hcm2.GetMatrix() != hcm1.GetMatrix());

  hcm2 = hcm1;
  CHECK_EQUAL(hcm2.GetMatrix(), hcm1.GetMatrix());
  hcm2.set_k(0.5);
  CHECK(hcm2.GetMatrix() != hcm1.GetMatrix());

  hcm2 = hcm1;
  CHECK_EQUAL(hcm2.GetMatrix(), hcm1.GetMatrix());
  hcm2.set_thickness(1e-6);
  CHECK(hcm2.GetMatrix() != hcm1.GetMatrix());

  hcm2 = hcm1;
  CHECK_EQUAL(hcm2.GetMatrix(), hcm1.GetMatrix());
  hcm2.set_wavelength(633e-9);
  CHECK(hcm2.GetMatrix() != hcm1.GetMatrix());
}
