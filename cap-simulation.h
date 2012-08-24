#ifndef JMG_CAP_SIMULATION_H
#define JMG_CAP_SIMULATION_H

#include <iostream>
#include <map>
#include <vector>
#include <string>
#include "cap-material.h"
#include "e-field-propagator.h"

using namespace std;

struct cap_point
{
  // time delay (s)
  double t;

  // (R - R0) / R
  double R;

  cap_point()
    : t(0.0), R(0.0) {}

  cap_point(double time, double refl)
    : t(time), R(refl) {}
};

class cap_simulation
{
public:
  cap_simulation();
  cap_simulation(cap_material *mat);
  cap_simulation(cap_material *mat, laser_beam *l);
  ~cap_simulation();

  vector <cap_point> run(double td_stop, double td_step);
  vector <cap_point> run(double td_start, double td_stop, double td_step);

  void print_parameters(ostream & out = cout, string tag = "") const;
  void set_material(cap_material *mat);

  // Set to true to disable progress output
  bool quiet;

  double n(double z, double td) const;
  double k(double z, double td) const;
  double current_time;

private:
  bool material_needs_destroyed;
  double RR(double t);
  void init_defaults();

  cap_material *material;
  laser_beam laser;

  double strain(double z, double td) const;
};

complex cap_index(double z);
double abs(double x);

#endif
