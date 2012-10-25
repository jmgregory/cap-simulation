#ifndef JMG_TRANSDUCING_LAYER_H
#define JMG_TRANSDUCING_LAYER_H

#include <iostream>
#include <string>

class TransducingLayer
{
 public:
  TransducingLayer() :
    _reflectivity(0.0), 
    _absorption_length(7.6e-9),
    _mass_specific_heat_capacity(0.91),
    _mass_density(2.70),
    _poissons_ratio(0.334),
    _linear_expansion_coefficient(23e-6) {}

  TransducingLayer(double reflectivity, 
		   double absorption_length, 
		   double mass_specific_heat_capacity, 
		   double mass_density, 
		   double poissons_ratio, 
		   double linear_expansion_coefficient) :
    _reflectivity(reflectivity),
    _absorption_length(absorption_length),
    _mass_specific_heat_capacity(mass_specific_heat_capacity),
    _mass_density(mass_density),
    _poissons_ratio(poissons_ratio),
    _linear_expansion_coefficient(linear_expansion_coefficient) {}

  double absorption_length() const { return _absorption_length; }

  double CalcStrainFactor() const;
  void PrintParameters(std::ostream & out, std::string tag) const;

 private:
  double _reflectivity; // R
  double _absorption_length; // zeta (m)
  double _mass_specific_heat_capacity; // C (J/g/K)
  double _mass_density; // rho (g/cm3)
  double _poissons_ratio; // nu
  double _linear_expansion_coefficient; // beta
};

#endif
