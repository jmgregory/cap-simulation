#include <cmath>
#include <assert.h>
#include "CapSimulation.h"
#include "DefaultCapMaterial.h"
#include "HomogeneousCharacteristicMatrix.h"
#include "Exception.h"

using namespace std;

CapSimulation *current_sim;

double my_abs(double x)
{
  return (x > 0.0 ? x : -x);
}

void CapSimulation::PrintParameters(ostream & out, string tag) const
{
  out << tag << "CAP Simulation: Parameters" << endl;
  out << tag << "==========================" << endl;
  out << tag << "  Depth sampling resolution: " << _depth_sampling_resolution * 1e9 << " nm" << endl;
  out << tag << endl;
  _laser.PrintParameters(out, tag);
  out << tag << endl;
  _material->PrintParameters(out, tag);
}

vector <CapPoint> CapSimulation::Run(double stop_time_delay, double time_delay_step)
{
  return Run(0.0, stop_time_delay, time_delay_step);
}

double CapSimulation::CalculateDifferentialReflectivity(double modulated_reflectivity, double baseline_reflectivity) const
{
  return (modulated_reflectivity - baseline_reflectivity) / baseline_reflectivity;
}

vector <CapPoint> CapSimulation::Run(double start_time_delay, double stop_time_delay, double time_delay_step)
{
  double unstrained_reflectivity = CalculateUnstrainedReflectivity();
  vector <CapPoint> result;
  for (double time_delay = start_time_delay; time_delay <= stop_time_delay; time_delay += time_delay_step)
    {
      result.push_back(CapPoint(time_delay,
				CalculateDifferentialReflectivity(CalculateReflectivityForTimeDelay(time_delay), 
								  unstrained_reflectivity)));
    }
  return result;
}

double CapSimulation::CalculateUnstrainedReflectivity() const
{
  double result = CalculateReflectivityForTimeDelay(-1.0);
  if (result == 0.0) throw Exception("Unstrained Reflectivity of the material was zero... All (R-R0)/R0 points will be infinity!");
  return result;
}

double CapSimulation::StrainPulseDepth(double time_delay) const
{
  double depth = 0.0;
  double time_increment = time_delay / 10000.0;
  for (double time = 0.0; time <= time_delay; time += time_increment)
    {
      depth += time_increment * _material->speed_of_sound(depth);
    }
  return depth;
}

vector <HomogeneousCharacteristicMatrix> CapSimulation::BuildLayerMatricesList(double time_delay) const
{
  vector <HomogeneousCharacteristicMatrix> matrices;
  complex <double> this_index;
  int identical_layer_count = 0;
  matrices.push_back(HomogeneousCharacteristicMatrix(IndexBeforeSpecimen(),
					  _depth_sampling_resolution, 
					  _laser.probe_wavelength()));

  double furthest_depth = StrainPulseDepth(time_delay) + 50e-9;
  if (_material->max_interesting_depth() > furthest_depth) furthest_depth = _material->max_interesting_depth();

  for (double depth = 0.0; depth < furthest_depth; depth += _depth_sampling_resolution)
    {
      this_index = CalculateIndexWithStrain(time_delay, depth);
      if (this_index == matrices[matrices.size()-1].index())
	{
	  identical_layer_count++;
	}
      else
	{
	  if (identical_layer_count != 0)
	    {
	      matrices[matrices.size()-1].set_thickness(matrices[matrices.size()-1].thickness() + (_depth_sampling_resolution * identical_layer_count));
	      identical_layer_count = 0;
	    }
	  // TODO: Multiply thickness by strain in each layer
	  matrices.push_back(HomogeneousCharacteristicMatrix(real(this_index), imag(this_index), _depth_sampling_resolution, _laser.probe_wavelength()));
	}
    }
  if (identical_layer_count != 0)
    {
      matrices[matrices.size()-1].set_thickness(matrices[matrices.size()-1].thickness() + (_depth_sampling_resolution * identical_layer_count));
    }
  return matrices;
}

complex <double> CapSimulation::UnstrainedIndex(double depth) const
{
  return CalculateIndexWithStrain(-1.0, depth);
}

complex <double> CapSimulation::IndexBeforeSpecimen() const
{
  return UnstrainedIndex(-_depth_sampling_resolution);
}

complex <double> CapSimulation::IndexAfterSpecimen() const
{
  return UnstrainedIndex(_material->max_interesting_depth());
}

CharacteristicMatrix CapSimulation::MultiplyMatrices(const std::vector <HomogeneousCharacteristicMatrix> & matrices) const
{
  CharacteristicMatrix result = matrices[0];
  for(unsigned int i = 1; i < matrices.size(); i++)
    {
      result *= matrices[i];
    }
  return result;
}

double CapSimulation::CalculateReflectivityForTimeDelay(double time_delay) const
{
  vector <HomogeneousCharacteristicMatrix> matrices = BuildLayerMatricesList(time_delay);
  CharacteristicMatrix full_specimen = MultiplyMatrices(matrices);
  return full_specimen.ReflectivityInEnvironment(IndexBeforeSpecimen(), IndexAfterSpecimen());
}

double sgn(double x)
{
  if (x > 0) return 1.0;
  else if (x < 0) return -1.0;
  return 0.0;
}

double CapSimulation::CalculateStrain(double time_delay, double depth) const
{
  if (time_delay < 0.0) return 0.0;
  if (depth < 0.0) return 0.0;
  double speed_of_sound = _material->speed_of_sound(depth);
  double absorption_length = _material->transducing_layer().absorption_length();
  double strain_center_depth = depth - speed_of_sound * time_delay;
  return _material->transducing_layer().CalculateStrainFactor() * _laser.EnergyPerPulse() / _laser.PumpSpotArea() * (exp(-depth/absorption_length) - 0.5*(exp(-(depth+speed_of_sound*time_delay)/absorption_length)) - 0.5*exp(-my_abs(strain_center_depth)/absorption_length)*sgn(strain_center_depth));
}

complex <double> CapSimulation::CalculateIndexWithStrain(double time_delay, double depth) const
{
  double strain = CalculateStrain(time_delay, depth);
  if (strain == 0.0)
    {
      return complex <double> (_material->n(depth, _laser.probe_wavelength()),
			       _material->kappa(depth, _laser.probe_wavelength()));
    }
  else
    {
      return complex <double> (_material->n(depth, _laser.probe_wavelength())     + strain * _material->d_n_d_strain(depth, _laser.probe_wavelength()),
			       _material->kappa(depth, _laser.probe_wavelength()) + strain * _material->d_kappa_d_strain(depth, _laser.probe_wavelength()));  
    }
}

void CapSimulation::CheckDepthSamplingResolutionAgainstMaterial()
{
  if (_depth_sampling_resolution >= _material->smallest_feature())
    {
      _depth_sampling_resolution = _material->smallest_feature();
    }
}

void CapSimulation::set_laser(const LaserBeam & laser)
{
  _laser = laser;
}

CapSimulation::CapSimulation()
  : _depth_sampling_resolution(1e-10), _laser(LaserBeam())
{
  set_material(NULL);
  CheckDepthSamplingResolutionAgainstMaterial();
}

void CapSimulation::DestroyMaterialIfNecessary()
{
  if (_material_needs_destroyed)
    {
      delete _material;
      _material_needs_destroyed = false;
    }
}

void CapSimulation::set_material(CapMaterialInterface * material)
{
  if (material == NULL)
    {
      _material = new DefaultCapMaterial();
      _material_needs_destroyed = true;
    }
  else
    {
      DestroyMaterialIfNecessary();
      _material = material;
    }
  CheckDepthSamplingResolutionAgainstMaterial();
}

CapSimulation::~CapSimulation()
{
  DestroyMaterialIfNecessary();
}
