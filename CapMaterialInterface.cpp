#include <complex>
#include <iostream>
#include <string>
#include "CapMaterialInterface.h"

void CapMaterialInterface::PrintParameters(std::ostream & out, std::string tag) const
{
  using std::endl;
  transducing_layer().PrintParameters(out, tag);
  out << tag << endl;
  out << tag << "  Material" << endl;
  out << tag << "  ------------------------------- " << endl;
  out << tag << "                     Description: " << description() << endl;
  out << tag << "       Maximum interesting depth: " << max_interesting_depth() * 1e9 << " nm" << endl;
  out << tag << "    Smallest interesting feature: " << smallest_feature() * 1e9 << " nm" << endl;
  PrintCustomParameters(out, tag);
}

double CapMaterialInterface::d_n_d_strain(double depth, double wavelength) const
{
  return real(d_index_d_strain(depth, wavelength));
}

double CapMaterialInterface::d_kappa_d_strain(double depth, double wavelength) const
{
  return imag(d_index_d_strain(depth, wavelength));
}

std::complex <double> CapMaterialInterface::d_index_d_strain(double depth, double wavelength) const
{
  // Equation from O. Matsuda and O. B. Wright, J. Opt. Soc. Am. B, vol. 19, pp. 3028-3041, Dec 2002.
  std::complex <double> index(n(depth, wavelength), kappa(depth, wavelength));
  return -(index * index * index) / 2.0 * p12(depth, wavelength);
}
