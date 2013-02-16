#ifndef JMG_DEFAULT_CAP_MATERIAL_H
#define JMG_DEFAULT_CAP_MATERIAL_H

#include <iostream>
#include <string>
#include "CapMaterialInterface.h"

class TransducingLayer;

class DefaultCapMaterial : public CapMaterialInterface
{
 private:
  TransducingLayer _transducing_layer;
  
 public:
  double smallest_feature() const           { return 10e-9; }
  double max_interesting_depth() const      { return 2e-6; }
  double speed_of_sound(double depth) const     { return 17520.0; }
  double p12(double depth, double wavelength) const { return 0.1; }
  std::string description() const           { return "Default CAP Material (n = 2.4 + 0.05i, p12 = 0.1, vs = 17520 m/s)"; }

  double n(double depth, double wavelength) const 
  { 
    if (depth >= 0.0) return 2.4;
    return 1.0;
  }

  double kappa(double depth, double wavelength) const 
  {
    if (depth >= 0.0) return 0.05;
    return 0.0;
  }

  void PrintCustomParameters(std::ostream & out = std::cout, std::string tag = "") const
  { }

  TransducingLayer transducing_layer() const
  {
    return _transducing_layer;
  }

  CapMaterialInterface * clone() const
  {
    return new DefaultCapMaterial();
  }

  void set_transducing_layer(const TransducingLayer & tl)
  {
    _transducing_layer = tl;
  }
};

#endif
