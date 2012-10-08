#ifndef JMG_CAP_SIMULATION_H
#define JMG_CAP_SIMULATION_H

#include <iostream>
#include <vector>
#include <string>
#include "cap-material.h"
#include "characteristic_matrix.h"

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
  cap_simulation(double zres = 1e-10);
  cap_simulation(cap_material *mat, double zres = 1e-10);
  cap_simulation(cap_material *mat, laser_beam *l, double zres = 1e-10);
  ~cap_simulation();

  vector <cap_point> run(double td_stop, double td_step);
  vector <cap_point> run(double td_start, double td_stop, double td_step);

  void print_parameters(ostream & out = cout, string tag = "") const;
  void set_material(cap_material *mat);

private:
  bool material_needs_destroyed;
  double RR(double td);

  cap_material *material;
  laser_beam laser;
  double resolution;

  double n(double td, double z) const;
  double k(double td, double z) const;
  double strain(double td, double z) const;
};

double abs(double x);

#endif
