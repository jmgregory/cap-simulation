#ifndef JMG_CAP_MATERIAL
#define JMG_CAP_MATERIAL

#include <iostream>
#include <string>

using namespace std;

#ifndef pi
#define pi 3.1415926535897932384626
#endif

struct LaserBeam
{
  LaserBeam()
    : probe_wavelength(400e-9), pump_diameter(100e-6), pump_power(300e-3), rep_rate(76e6), time_width(150e-15) {}
  LaserBeam(double lambda, double d, double P, double Hz, double tw)
    : probe_wavelength(lambda), pump_diameter(d), pump_power(P), rep_rate(Hz), time_width(tw) {}

  double probe_wavelength; // m
  double pump_diameter;   // m
  double pump_power;      // W
  double rep_rate;   // Hz
  double time_width; // s

  double Q() const { return pump_power / rep_rate; }
  double A() const { return pi*0.25*pump_diameter*pump_diameter; }
};

struct TransducingLayer
{
  TransducingLayer()
    :R(0.0), zeta(7.6e-9), C(0.91), rho(2.70), nu(0.334), beta(23e-6) {}
  TransducingLayer(double reflectivity, double abs_length, double heat_cap, double density, double poisson, double expansion_coeff)
    :R(reflectivity), zeta(abs_length), C(heat_cap), rho(density), nu(poisson), beta(expansion_coeff) {}

  // Reflectivity
  double R;

  // Absorption length (m)
  double zeta;

  // Mass specific heat capacity (J/g-K)
  double C;
  
  // Density (g/cm3)
  double rho;

  // Poisson's ratio
  double nu;

  // Linear expansion coefficient
  double beta;

  double strain_factor() const { return (1-R)*beta/(zeta*C*rho*1e6)*(1+nu)/(1-nu); }
};

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

  virtual void print_parameters(ostream & out = cout, string tag = "") const { }

  TransducingLayer cap_layer;
};

#endif
