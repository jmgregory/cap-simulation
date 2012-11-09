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

double CapMaterialInterface::dndeta(double z, double lambda) const
{
  return real(d_index_d_eta(z, lambda));
}

double CapMaterialInterface::dkappadeta(double z, double lambda) const
{
  return imag(d_index_d_eta(z, lambda));
}

std::complex <double> CapMaterialInterface::d_index_d_eta(double z, double lambda) const
{
  // Equation from O. Matsuda and O. B. Wright, J. Opt. Soc. Am. B, vol. 19, pp. 3028-3041, Dec 2002.
  std::complex <double> index(n(z, lambda), kappa(z, lambda));
  return -(index * index * index) / 2.0 * p12(z, lambda);
}
