#include "TransducingLayer.h"

#include <iostream>
#include <string>

double TransducingLayer::CalcStrainFactor() const
{
  return (1.0 - _reflectivity) * _linear_expansion_coefficient / 
    (_absorption_length * _mass_specific_heat_capacity * _mass_density * 1e6) * 
    (1.0 + _poissons_ratio) / (1.0 - _poissons_ratio); 
}

void TransducingLayer::PrintParameters(std::ostream & out, std::string tag) const
{
  using std::endl;
  out << tag << "  Capping Layer" << endl;
  out << tag << "  -------------------------------" << endl;
  out << tag << "                    Reflectivity: " << _reflectivity << endl;
  out << tag << "               Absorption length: " << _absorption_length * 1e6 << " um" << endl;
  out << tag << "          Specific heat capacity: " << _mass_specific_heat_capacity << " J/g/K" << endl;
  out << tag << "                         Density: " << _mass_density << " g/cm^3" << endl;
  out << tag << "                 Poisson's ratio: " << _poissons_ratio << endl;
  out << tag << "    Linear expansion coefficient: " << _linear_expansion_coefficient << endl;
}
