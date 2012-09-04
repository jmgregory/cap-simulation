#include "e-field-propagator.h"
#include "cap-material.h"

class my_material : public cap_material
{
public:
  // The smallest interesting feature (determines simulation resolution)
  double smallest_feature() const { return 10e-9; }

  // The deepest interesting feature
  double max_interesting_depth() const { return 400e-9; }

  // Real index of refraction
  double n(double z, double lambda) const 
  { 
    if (z >= 0.0) 
      {
	if (z > 200e-9)
	  {
	    return 2.0;
	  }
	return 1.0;
      }
    return 1.0; 
  }

  // Imaginary index of refraction
  double kappa(double z, double lambda) const 
  { 
    if (z >= 0.0) 
      {
	if (z > 200e-9)
	  {
	    return 0.0;
	  }
	return 0.0;
      }
    return 0.0; 
  }
};


my_material *mm;
const double wl = 50e-9;
complex <double> index(double z)
{
  return complex <double> (mm->n(z, wl), mm->kappa(z, wl));
}

int main()
{
  mm = new my_material();
  laser_beam beam;
  beam.probe_wavelength = wl;
  beam.time_width = 20e-15;
  e_field_propagator myEFP(mm, &beam, index);
  myEFP.quiet = false;
  myEFP.run();
  delete mm;
  return 0;
}
