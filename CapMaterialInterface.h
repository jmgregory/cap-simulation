#ifndef JMG_CAP_MATERIAL_INTERFACE_H
#define JMG_CAP_MATERIAL_INTERFACE_H

#include <iostream>
#include <string>
#include <complex>
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

  // Transducing layer
  virtual TransducingLayer transducing_layer() const = 0;

  // A descriptive name for the material
  virtual std::string description() const = 0;

  virtual void PrintCustomParameters(std::ostream & out = std::cout, std::string tag = "") const = 0;
  virtual ~CapMaterialInterface() {}

 private:
  // Photoelastic tensor component p12
  virtual double p12(double z, double lambda) const = 0;



  // Begin no-overload members

 public:
  // Overload PrintCustomParameters() instead
  void PrintParameters(std::ostream & out = std::cout, std::string tag = "") const;

  // Photoelastic coefficients
  // Should be no need to overload these functions; instead, define p12()
  double dndeta(double z, double lambda) const;
  double dkappadeta(double z, double lambda) const;

 private:
  std::complex <double> d_index_d_eta(double z, double lambda) const;
};

#endif
