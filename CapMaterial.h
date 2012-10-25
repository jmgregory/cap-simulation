#ifndef JMG_CAP_MATERIAL
#define JMG_CAP_MATERIAL

#include <iostream>
#include <string>
#include "TransducingLayer.h"

using std::string;

// Create a subclass of this class for specific systems
class CapMaterial
{
public:
  // The smallest interesting feature (determines simulation resolution)
  virtual double smallest_feature() const { return 10e-9; }

  // The deepest interesting feature
  virtual double max_interesting_depth() const { return 1e-6; }

  // Real index of refraction
  virtual double n(double z, double lambda) const { if (z >= 0.0) return 2.4; return 1.0; }

  // Imaginary index of refraction
  virtual double kappa(double z, double lambda) const { if (z >= 0.0) return 0.05; return 0.0; }

  // Speed of sound (m/s)
  virtual double vs(double z) const { return 17520.0; }

  // Photoelastic constants
  virtual double dndeta(double z, double lambda) const { return 100; }
  virtual double dkappadeta(double z, double lambda) const { return 0; }

  virtual void print_parameters(std::ostream & out = std::cout, string tag = "") const { }

  virtual ~CapMaterial() {}

  TransducingLayer cap_layer;
};

#endif
