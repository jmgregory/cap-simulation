#ifndef JMG_E_FIELD_PROPAGATOR
#define JMG_E_FIELD_PROPAGATOR

#include <cstdlib>
#include <map>
#include "complex.h"
#include "cap-material.h"

using namespace std;

const double c = 2.998e8; // m/s

struct slice
{
  complex El;
  complex Er;
  complex k;
  double z;
  double lambda;

  void setWavenumber(complex n, double wavelength);
  double n();
  double kappa();
};

class e_field_propagator
{
 public:
  e_field_propagator();
  e_field_propagator(cap_material *cm, laser_beam *laser, complex (*index)(double));
  ~e_field_propagator();

  void initialize(cap_material *cm, laser_beam *laser, complex (*index)(double));
  void clear_fields();
  double run(); // Return reflectivity of sample

 private:
  double resolution;
  double zstart, zstop; // m
  double wavelength; // m
  double pulse_fwhm;
  int slicecount;
  slice *slices;
  slice *newslices;
  double timestep;
  double frequency, omega;
  double r_flux, i_flux;

  complex t(complex k_from, complex k_to);
  complex r(complex k_in, complex k_out);
  static complex default_index(double z);
  double get_total_flux();

};

#endif
