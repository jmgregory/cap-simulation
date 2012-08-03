#include "e-field-propagator.h"

void slice::setWavenumber(complex n, double wavelength)
{
  k = complex(2 * pi * real(n) / wavelength, 2 * pi * imag(n) / wavelength);
  lambda = wavelength;
}

double slice::n()
{
  return real(k) * lambda / (2 * pi);
}

double slice::kappa()
{
  return imag(k) * lambda / (2 * pi);
}

e_field_propagator::e_field_propagator()
{
  slices = newslices = NULL;
  initialize(NULL, NULL, this->default_index);
}

e_field_propagator::e_field_propagator(cap_material *cm, laser_beam *laser, complex (*index)(double))
{
  slices = newslices = NULL;
  initialize(cm, laser, index);
}

e_field_propagator::~e_field_propagator()
{
  if (slices != NULL) free(slices);
  if (newslices != NULL) free(newslices);
}

complex e_field_propagator::r(complex k_in, complex k_out)
{
  return (k_in - k_out) / (k_in + k_out);
}

complex e_field_propagator::t(complex k_from, complex k_to)
{
  return 2 * k_from / (k_from + k_to);
}

void e_field_propagator::initialize(cap_material *cm, laser_beam *laser, complex (*index)(double))
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
      resolution = 100e-9;
      zstart = -200e-9;
      zstop = 1e6;
    }
  else
    {
      resolution = cm->smallest_feature();
      zstart = -wavelength / resolution * 2.0;
      zstop = cm->max_interesting_depth();
    }

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
  double width = zstop - zstart;
  double zstep = width / resolution;
  timestep = zstep / c;
  while (zcount <= zstop)
    {
      zcount += zstep / real(index(zcount));
      slicecount++;
    }
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
}

void e_field_propagator::clear_fields()
{
  for (int i = 0; i < slicecount; i++)
    {
      slices[i].El = slices[i].Er = 0.0;
      newslices[i] = slices[i];      
    }
}

double e_field_propagator::get_total_flux()
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
  complex propagator;
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
	      
	      propagator = exp(-I * omega * timestep) * exp(I * slices[i].k * (slices[i+1].z - slices[i].z));
	      newslices[i].Er = newslices[i].Er * propagator;
	      newslices[i].El = newslices[i].El * propagator;
	    }
	  else if (i == slicecount - 1)
	    {
	      newslices[i].Er = slices[i-1].Er * t(slices[i-1].k, slices[i].k) + slices[i].El * r(slices[i].k, slices[i-1].k);
	      newslices[i].El = 0;
	      
	      propagator = exp(-I * omega * timestep) * exp(I * slices[i].k * (slices[i].z - slices[i-1].z));
	      newslices[i].Er = newslices[i].Er * propagator;
	      newslices[i].El = newslices[i].El * propagator;
	    }
	  else
	    {
	      newslices[i].Er = slices[i-1].Er * t(slices[i-1].k, slices[i].k) + slices[i].El * r(slices[i].k, slices[i-1].k);
	      newslices[i].El = slices[i+1].El * t(slices[i+1].k, slices[i].k) + slices[i].Er * r(slices[i].k, slices[i+1].k);
	      
	      propagator = exp(-I * omega * timestep) * exp(I * slices[i].k * (slices[i+1].z - slices[i].z));
	      newslices[i].Er = newslices[i].Er * propagator;
	      newslices[i].El = newslices[i].El * propagator;
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

complex e_field_propagator::default_index(double z)
{
  return 1.0;
}
