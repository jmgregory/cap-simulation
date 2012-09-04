#include "e-field-propagator.h"

const complex <double> I = complex <double> (0.0, 1.0);

double magsquared(complex <double> z)
{
  return z.real() * z.real() + z.imag() * z.imag();
}

void slice::setWavenumber(complex <double> n, double wavelength)
{
  k = complex <double> (2 * pi * real(n) / wavelength, 2 * pi * imag(n) / wavelength);
  lambda = wavelength;
}

double slice::n() const
{
  return real(k) * lambda / (2 * pi);
}

double slice::kappa() const
{
  return imag(k) * lambda / (2 * pi);
}

e_field_propagator::e_field_propagator()
{
  slices = newslices = NULL;
  initialize(NULL, NULL, this->default_index);
}

e_field_propagator::e_field_propagator(const cap_material *cm, const laser_beam *laser, complex <double> (*index)(double))
{
  slices = newslices = NULL;
  initialize(cm, laser, index);
}

e_field_propagator::~e_field_propagator()
{
  if (slices != NULL) free(slices);
  if (newslices != NULL) free(newslices);
}

complex <double> e_field_propagator::r(complex <double> k_in, complex <double> k_out) const
{
  return (k_in - k_out) / (k_in + k_out);
}

complex <double> e_field_propagator::t(complex <double> k_from, complex <double> k_to) const
{
  return 2.0 * k_from / (k_from + k_to);
}

void e_field_propagator::initialize(const cap_material *cm, const laser_beam *laser, complex <double> (*index)(double))
{
  if (laser == NULL)
    {
      wavelength = 200e-9;
      pulse_fwhm = 50e-15;
    }
  else
    {
      wavelength = laser->probe_wavelength;
      pulse_fwhm = laser->time_width;
    }
  frequency = c / wavelength; // Hz
  omega = 2 * pi * frequency;

  if (cm == NULL)
    {
      resolution = 0.5e-9;
      zstart = -200e-9;
      zstop = 1e-6;
    }
  else
    {
      zstart = -10e-9;
      zstop = cm->max_interesting_depth();
      resolution = cm->smallest_feature();
      // Need at least 1nm resolution to properly sample the strain wave
      if (resolution > 1e-9) resolution = 1e-9;
    }
  resolution = 5e-9;

  if (slices != NULL)
    {
      free(slices);
      slices = NULL;
    }
  if (newslices != NULL)
    {
      free(newslices);
      newslices = NULL;
    }

  slicecount = 0;
  double zcount = zstart;
  //double width = zstop - zstart;
  double zstep = resolution;
  timestep = zstep / c;
  while (zcount <= zstop)
    {
      zcount += zstep / real(index(zcount));
      //cerr << slicecount << '\t' << zcount * 1e9 << '\t' << real(index(zcount)) << endl;
      slicecount++;
    }
  //cerr << "width " << width << endl;
  //cerr << timestep << endl;

  slices    = (slice *)malloc(sizeof(slice) * slicecount);
  newslices = (slice *)malloc(sizeof(slice) * slicecount);
  if (slices == NULL || newslices == NULL)
    {
      cerr << "Error: could not allocate memory." << endl;
      exit(1);
    }
  slices[0].El = slices[0].Er = 0.0;
  slices[0].setWavenumber(index(zstart), wavelength);
  slices[0].z = zstart;
  newslices[0] = slices[0];
  for (int i = 1; i < slicecount; i++)
    {
      slices[i].El = slices[i].Er = 0.0;
      slices[i].z = (zstep / slices[i-1].n()) + slices[i-1].z;
      slices[i].setWavenumber(index(slices[i].z), wavelength);
      newslices[i] = slices[i];
    }
  complex <double> preterm = exp(-I*omega*timestep);
  for (int i = 1; i < slicecount - 1; i++)
    {
      slices[i].tr = t(slices[i-1].k, slices[i].k);
      slices[i].tl = t(slices[i+1].k, slices[i].k);
      slices[i].rr = r(slices[i].k, slices[i-1].k);
      slices[i].rl = r(slices[i].k, slices[i+1].k);
      slices[i].propagator = preterm * exp(I * slices[i].k * (slices[i+1].z - slices[i].z));
      newslices[i] = slices[i];
    }
}

void e_field_propagator::clear_fields()
{
  for (int i = 0; i < slicecount; i++)
    {
      slices[i].El = slices[i].Er = 0.0;
      newslices[i] = slices[i];      
    }
}

double e_field_propagator::get_total_flux() const
{
  double sum = 0.0;
  for (int i = 0; i < slicecount - 1; i++)
    {
      sum += (magsquared(slices[i].Er) + magsquared(slices[i].El)) / (slices[i+1].z - slices[i].z);
    }
  return sum;
}

double e_field_propagator::run()
{
  double max_total_flux = 0.0;
  double total_flux = 1.0;
  double time = 0.0;
  double pulse_center = pulse_fwhm * 3;
  complex <double> propagator;
  complex <double> preterm = exp(-I*omega*timestep);
  slice * temp;
  i_flux = r_flux = 0.0;
  while (total_flux >= max_total_flux / 1000)
    {
      //cerr << "\r" << int(time / runtime * 100) << "%";
      for (int i = 0; i < slicecount; i++)
	{
	  if (i == 0)
	    {
	      newslices[i].Er = exp(-I*omega*time) * exp(-(time-pulse_center)*(time-pulse_center)/(0.3607*pulse_fwhm*pulse_fwhm));
	      newslices[i].El = slices[i+1].El * t(slices[i+1].k, slices[i].k) + slices[i].Er * r(slices[i].k, slices[i+1].k);
	      
	      propagator = preterm * exp(I * slices[i].k * (slices[i+1].z - slices[i].z));
	      newslices[i].Er = newslices[i].Er * propagator;
	      newslices[i].El = newslices[i].El * propagator;
	    }
	  else if (i == slicecount - 1)
	    {
	      newslices[i].Er = slices[i-1].Er * t(slices[i-1].k, slices[i].k) + slices[i].El * r(slices[i].k, slices[i-1].k);
	      newslices[i].El = 0;
	      
	      propagator = preterm * exp(I * slices[i].k * (slices[i].z - slices[i-1].z));
	      newslices[i].Er = newslices[i].Er * propagator;
	      newslices[i].El = newslices[i].El * propagator;
	    }
	  else
	    {
	      newslices[i].Er = slices[i-1].Er * slices[i].tr + slices[i].El * slices[i].rr;
	      newslices[i].El = slices[i+1].El * slices[i].tl + slices[i].Er * slices[i].rl;
	      
	      newslices[i].Er = newslices[i].Er * slices[i].propagator;
	      newslices[i].El = newslices[i].El * slices[i].propagator;
	    }
	}

      temp = slices;
      slices = newslices;
      newslices = temp;


      total_flux = get_total_flux();
      if (total_flux > max_total_flux)
	{
	  max_total_flux = total_flux;
	}

      i_flux += magsquared(slices[0].Er) / (slices[1].z - slices[0].z);
      r_flux += magsquared(slices[0].El) / (slices[1].z - slices[0].z);

      time += timestep;
    }

  return r_flux / i_flux;
}

complex <double> e_field_propagator::default_index(double z)
{
  if (z < 0.0) return 1.0;
  return 2.0;
}

int e_field_propagator::getSliceCount() const
{
  return slicecount;
}

double e_field_propagator::getTimeStep() const
{
  return timestep;
}

double e_field_propagator::getResolution() const
{
  return resolution;
}

