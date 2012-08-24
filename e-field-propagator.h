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
  double n() const;
  double kappa() const;
};

class e_field_propagator
{
 public:
  e_field_propagator();
  e_field_propagator(const cap_material *cm, const laser_beam *laser, complex (*index)(double));
  ~e_field_propagator();

  void initialize(const cap_material *cm, const laser_beam *laser, complex (*index)(double));
  void clear_fields();
  double run(); // Return reflectivity of sample

  int getSliceCount() const;
  double getTimeStep() const;

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

  complex t(complex k_from, complex k_to) const;
  complex r(complex k_in, complex k_out) const;
  static complex default_index(double z);
  double get_total_flux() const;

};

#endif
