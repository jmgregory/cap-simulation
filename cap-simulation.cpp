#include "cap-simulation.h"
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
  out << tag << "  Simulation" << endl;
  out << tag << "  -------------------------------" << endl;
  out << tag << "    Smallest interesting feature: " << material->smallest_feature() * 1e9 << " nm" << endl;
  out << tag << "       Maximum interesting depth: " << material->max_interesting_depth() * 1e9 << " nm" << endl;
  current_sim = (cap_simulation *)this;
  e_field_propagator efp(material, &laser, cap_index);
  out << tag << "                       Time step: " << efp.getTimeStep() * 1e15 << " fs" << endl;
  out << tag << "              Number of z-slices: " << efp.getSliceCount() << endl;
}

vector <cap_point> cap_simulation::run(double td_stop, double td_step)
{
  return run(0.0, td_stop, td_step);
}

vector <cap_point> cap_simulation::run(double td_start, double td_stop, double td_step)
{
  if (!quiet) cerr << "Calculating R_0...";
  int sec1 = time(NULL);
  double R0 = RR(-1);
  if (R0 == 0.0)
    {
      cerr << "Warning: R0 was calculated to be zero!  All CAP points will be INF." << endl;
    }
  int sec2 = time(NULL);
  if (!quiet) cerr << "done." << endl;
  if (!quiet) cerr << "Estimated calculating time: " << int(((sec2 - sec1) * (td_stop - td_start) / td_step) / 60) << " minute(s)" << endl;

  vector <cap_point> out;
  
  for (double t = td_start; t <= td_stop; t += td_step)
    {
      if (!quiet) cerr << "\r" << int((t - td_start)/td_stop*100) << "%";
      out.push_back(cap_point(t, (RR(t) - R0)/R0));
    }
  if (!quiet) cerr << "\rDone!" << endl;

  return out;
}

double cap_simulation::RR(double t)
{
  current_sim = this;
  current_time = t;
  e_field_propagator efp(material, &laser, cap_index);
  double out = efp.run();
  if (isinf(out) || isnan(out))
    {
      cerr << "Warning: e_field_propagator returned INF or NAN.  Consider increasing resolution." << endl;
    }
  return out;
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
  //cerr << "-- " << z << '\t' << material->n(z) << endl;
  return material->n(z, laser.probe_wavelength) + strain(td, z) * material->dndeta(z, laser.probe_wavelength);
}

double cap_simulation::k(double td, double z) const
{
  return material->kappa(z, laser.probe_wavelength) + strain(td, z) * material->dkappadeta(z, laser.probe_wavelength);
}

void cap_simulation::set_material(cap_material *mat)
{
  material = mat;
}

void cap_simulation::init_defaults()
{
  quiet = false;
}

cap_simulation::cap_simulation()
{ 
  init_defaults();
  set_material(new cap_material());
  material_needs_destroyed = true;
}

cap_simulation::cap_simulation(cap_material *mat)
{
  init_defaults();
  set_material(mat);
  material_needs_destroyed = false;
}

cap_simulation::cap_simulation(cap_material *mat, laser_beam *l)
{
  init_defaults();
  laser = *l;
  set_material(mat);
  material_needs_destroyed = false;
}

cap_simulation::~cap_simulation()
{
  if (material_needs_destroyed) delete material;
}

complex cap_index(double z)
{
  //cerr << "---- " << z << '\t' << current_sim->n(current_sim->current_time, z) << endl;
  return complex(current_sim->n(current_sim->current_time, z), current_sim->k(current_sim->current_time, z));
}
