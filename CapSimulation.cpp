#include "CapSimulation.h"
#include <cmath>

using namespace std;

cap_simulation *current_sim;

double my_abs(double x)
{
  return (x > 0.0 ? x : -x);
}

void cap_simulation::print_parameters(ostream & out, string tag) const
{
  out << tag << "CAP Simulation: Parameters" << endl;
  out << tag << "==========================" << endl;
  out << tag << "  Depth sampling resolution: " << resolution * 1e9 << " nm" << endl;
  out << tag << endl;
  out << tag << "  Laser Beam" << endl;
  out << tag << "  ---------------------" << endl;
  out << tag << "       Repetition rate: " << laser.rep_rate << " Hz" << endl;
  out << tag << "            Pump power: " << laser.pump_power << " W" << endl;
  out << tag << "    Pump spot diameter: " << laser.pump_diameter * 1e6 << " um" << endl;
  out << tag << "      Probe wavelength: " << laser.probe_wavelength * 1e9 << " nm" << endl;
  out << tag << "      Probe time width: " << laser.time_width * 1e15 << " fs (FWHM)" << endl;
  out << tag << endl;
  out << tag << "  Capping Layer" << endl;
  out << tag << "  -------------------------------" << endl;
  out << tag << "                    Reflectivity: " << material->cap_layer.R << endl;
  out << tag << "               Absorption length: " << material->cap_layer.zeta * 1e6 << " um" << endl;
  out << tag << "          Specific heat capacity: " << material->cap_layer.C << " J/g/K" << endl;
  out << tag << "                         Density: " << material->cap_layer.rho << " g/cm^3" << endl;
  out << tag << "                 Poisson's ratio: " << material->cap_layer.nu << endl;
  out << tag << "    Linear expansion coefficient: " << material->cap_layer.beta << endl;
  out << tag << endl;
  out << tag << "  Material" << endl;
  out << tag << "  ------------------------------- " << endl;
  out << tag << "       Maximum interesting depth: " << material->max_interesting_depth() * 1e9 << " nm" << endl;
  out << tag << "    Smallest interesting feature: " << material->smallest_feature() * 1e9 << " nm" << endl;
  material->print_parameters(out, tag);
}

vector <cap_point> cap_simulation::run(double td_stop, double td_step)
{
  return run(0.0, td_stop, td_step);
}

vector <cap_point> cap_simulation::run(double td_start, double td_stop, double td_step)
{
  double R0 = RR(-1);
  if (R0 == 0.0)
    {
      cerr << "Warning: R0 was calculated to be zero!  All CAP points will be INF." << endl;
    }
  vector <cap_point> out;
  for (double t = td_start; t <= td_stop; t += td_step)
    {
      out.push_back(cap_point(t, (RR(t) - R0)/R0));
    }
  return out;
}

double cap_simulation::RR(double td)
{
  vector <CharacteristicMatrix> matrices;
  complex <double> thisIndex;
  int identical_layers = 0;
  matrices.push_back(CharacteristicMatrix(n(-1, -resolution), k(-1, -resolution), resolution, laser.probe_wavelength));

  for (double z = 0.0; z < material->max_interesting_depth(); z += resolution)
    {
      thisIndex = complex <double> (n(td, z), k(td, z));
      if (thisIndex == matrices[matrices.size()-1].get_index())
	{
	  identical_layers++;
	}
      else
	{
	  if (identical_layers != 0)
	    {
	      matrices[matrices.size()-1].set_thickness(matrices[matrices.size()-1].get_thickness() + (resolution * identical_layers));
	      identical_layers = 0;
	    }
	  // TODO: Multiply thickness by strain in each layer
	  matrices.push_back(CharacteristicMatrix(thisIndex, resolution, laser.probe_wavelength));
	}
    }
  if (identical_layers != 0)
    {
      matrices[matrices.size()-1].set_thickness(matrices[matrices.size()-1].get_thickness() + (resolution * identical_layers));
    }
  CharacteristicMatrix myCM = matrices[0];
  //double z = -resolution;
  for(unsigned int i = 0; i < matrices.size(); i++)
    {
      //if (td > 0) cout << z << '\t' << matrices[i].get_n() << '\t' << matrices[i].get_k() << endl;
      //z += matrices[i].get_thickness();
      myCM *= matrices[i];
    }
  //if (td > 0) cout << z << '\t' << matrices[matrices.size()-1].get_n() << '\t' << matrices[matrices.size()-1].get_k() << endl;
  //if (td > 0) cout << endl << endl;
  return myCM.reflectivity(complex <double> (n(-1, resolution), k(-1, -resolution)),
			   complex <double> (n(-1, material->max_interesting_depth()), k(-1, material->max_interesting_depth())));
}

double sgn(double d)
{
  if (d > 0) return 1.0;
  else if (d < 0) return -1.0;
  return 0.0;
}

double cap_simulation::strain(double time_delay, double z) const
{
  if (time_delay < 0) return 0;
  if (z < 0) return 0;
  double vs = material->vs(z);
  double zeta = material->cap_layer.zeta;
  double depth = z - vs*time_delay;
  return material->cap_layer.strain_factor() * laser.Q() / laser.A() * (exp(-z/zeta) - 0.5*(exp(-(z+vs*time_delay)/zeta)) - 0.5*exp(-my_abs(depth)/zeta)*sgn(depth));
}

double cap_simulation::n(double td, double z) const
{
  double thisStrain = strain(td, z);
  if (thisStrain == 0.0)
    {
      return material->n(z, laser.probe_wavelength);
    }
  else
    {
      return material->n(z, laser.probe_wavelength) + thisStrain * material->dndeta(z, laser.probe_wavelength);
    }
}

double cap_simulation::k(double td, double z) const
{
  double thisStrain = strain(td, z);
  if (thisStrain == 0.0)
    {
      return material->kappa(z, laser.probe_wavelength);
    }
  else
    {
      return material->kappa(z, laser.probe_wavelength) + thisStrain * material->dndeta(z, laser.probe_wavelength);
    }
}

void cap_simulation::set_material(CapMaterial *mat)
{
  material = mat;
  if (resolution >= mat->smallest_feature()) resolution = mat->smallest_feature();
}

cap_simulation::cap_simulation(double zres)
{
  resolution = zres;
  set_material(new CapMaterial());
  material_needs_destroyed = true;
}

cap_simulation::cap_simulation(CapMaterial *mat, double zres)
{
  resolution = zres;
  set_material(mat);
  material_needs_destroyed = false;
}

cap_simulation::cap_simulation(CapMaterial *mat, LaserBeam *l, double zres)
{
  resolution = zres;
  laser = *l;
  set_material(mat);
  material_needs_destroyed = false;
}

cap_simulation::~cap_simulation()
{
  if (material_needs_destroyed) delete material;
}
