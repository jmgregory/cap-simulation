#ifndef JMG_CAP_MATERIAL_INTERFACE_H
#define JMG_CAP_MATERIAL_INTERFACE_H

#include <iostream>
#include <string>
#include "TransducingLayer.h"

// Create a subclass of this class for specific specimen structures
// See DefaultCapMaterial.h for an example
class CapMaterialInterface
{
public:
  // The smallest interesting feature (increases simulation's depth sampling resolution)
  virtual double smallest_feature() const = 0;

  // The deepest interesting feature
  virtual double max_interesting_depth() const = 0;

  // Real index of refraction
  virtual double n(double z, double lambda) const = 0;

  // Imaginary index of refraction
  virtual double kappa(double z, double lambda) const = 0;

  // Speed of sound (m/s)
  virtual double speed_of_sound(double z) const = 0;

  // Photoelastic coefficients
  virtual double dndeta(double z, double lambda) const = 0;
  virtual double dkappadeta(double z, double lambda) const = 0;

  // Transducing layer
  virtual TransducingLayer transducing_layer() const = 0;
  
  // Don't overload this function; overload PrintCustomParameters() instead
  void PrintParameters(std::ostream & out = std::cout, std::string tag = "") const
  {
    using std::endl;
    transducing_layer().PrintParameters(out, tag);
    out << tag << endl;
    out << tag << "  Material" << endl;
    out << tag << "  ------------------------------- " << endl;
    out << tag << "       Maximum interesting depth: " << max_interesting_depth() * 1e9 << " nm" << endl;
    out << tag << "    Smallest interesting feature: " << smallest_feature() * 1e9 << " nm" << endl;
    PrintCustomParameters(out, tag);
  }

  virtual void PrintCustomParameters(std::ostream & out = std::cout, std::string tag = "") const = 0;

  virtual ~CapMaterialInterface() {}
};

#endif
