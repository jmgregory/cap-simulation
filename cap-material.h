#ifndef JMG_CAP_MATERIAL
#define JMG_CAP_MATERIAL

const double pi = 3.1415926535897932384626;

struct laser_beam
{
  laser_beam()
    : probe_wavelength(400e-9), pump_diameter(100e-6), pump_power(300e-3), rep_rate(76e6), time_width(150e-15) {}
  laser_beam(double lambda, double d, double P, double Hz, double tw)
    : probe_wavelength(lambda), pump_diameter(d), pump_power(P), rep_rate(Hz), time_width(tw) {}

  double probe_wavelength; // m
  double pump_diameter;   // m
  double pump_power;      // W
  double rep_rate;   // Hz
  double time_width; // s

  double Q() { return pump_power / rep_rate; }
  double A() { return pi*0.25*pump_diameter*pump_diameter; }
};

struct capping_layer
{
  capping_layer()
    :R(0.0), zeta(7.6e-9), C(0.91), rho(2.70), nu(0.334), beta(23e-6) {}
  capping_layer(double reflectivity, double abs_length, double heat_cap, double density, double poisson, double expansion_coeff)
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

  double strain_factor() { return (1-R)*beta/(zeta*C*rho*1e6)*(1+nu)/(1-nu); }
};

// Create a subclass of this class for specific systems
class cap_material
{
public:
  // The smallest interesting feature (determines simulation resolution)
  virtual double smallest_feature() { return 100e-9; }

  // The deepest interesting feature
  virtual double max_interesting_depth() { return 1e-6; }

  // Real index of refraction
  virtual double n(double z) { return 4.0; }

  // Imaginary index of refraction
  virtual double kappa(double z) { return 0.05; }

  // Speed of sound (m/s)
  virtual double vs(double z) { return 17520.0; }

  // Photoelastic constants
  virtual double dndeta(double z) { return 100; }
  virtual double dkappadeta(double z) { return 0; }

  capping_layer cap_layer;
};

#endif
