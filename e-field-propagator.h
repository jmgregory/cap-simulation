#ifndef JMG_E_FIELD_PROPAGATOR
#define JMG_E_FIELD_PROPAGATOR

#include <cstdlib>
#include <complex>
#include "cap-material.h"

using namespace std;

const double c = 2.998e8; // m/s

struct slice
{
  complex <double> El;
  complex <double> Er;
  complex <double> k;
  complex <double> tl;
  complex <double> tr;
  complex <double> rl;
  complex <double> rr;
  complex <double> propagator;
  double z;
  double lambda;

  void setWavenumber(complex <double> n, double wavelength);
  double n() const;
  double kappa() const;
};

class e_field_propagator
{
 public:
  e_field_propagator();
  e_field_propagator(const cap_material *cm, const laser_beam *laser, complex <double> (*index)(double));
  ~e_field_propagator();

  void initialize(const cap_material *cm, const laser_beam *laser, complex <double> (*index)(double));
  void clear_fields();
  double run(); // Return reflectivity of sample

  int getSliceCount() const;
  double getTimeStep() const;
  double getResolution() const;

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

  complex <double> t(complex <double> k_from, complex <double> k_to) const;
  complex <double> r(complex <double> k_in, complex <double> k_out) const;
  static complex <double> default_index(double z);
  double get_total_flux() const;

};

#endif
