#ifndef JMG_DEFAULT_CAP_MATERIAL_H
#define JMG_DEFAULT_CAP_MATERIAL_H

#include <iostream>
#include <string>
#include "CapMaterialInterface.h"

class TransducingLayer;

class DefaultCapMaterial : public CapMaterialInterface
{
 public:  
  double smallest_feature() const                  { return 10e-9; }
  double max_interesting_depth() const             { return 2e-6; }
  double speed_of_sound(double z) const            { return 17520.0; }
  double dndeta(double z, double lambda) const     { if (z < 0) return 0; else return 100.0; }
  double dkappadeta(double z, double lambda) const { return 0.0; }

  double n(double z, double lambda) const 
  { 
    if (z >= 0.0) return 2.4;
    return 1.0;
  }

  double kappa(double z, double lambda) const 
  {
    if (z >= 0.0) return 0.05;
    return 0.0;
  }

  void PrintCustomParameters(std::ostream & out = std::cout, std::string tag = "") const
  { }

  TransducingLayer transducing_layer() const
  {
    return TransducingLayer();
  }
};

#endif
