#include <UnitTest++.h>
#include <vector>
#include <iostream>
#include <fstream>
#include "../CapSimulation.h"
#include "../Exception.h"
#include "../CapMaterialInterface.h"

struct DampedSineParameters
{
  double amplitude;
  double period;
  double phase;
  double decay;
  double offset;
};

double DampedSine(double x, DampedSineParameters parameters)
{
  return (parameters.offset +
	  (parameters.amplitude * 
	   sin(2 * pi * x / parameters.period + parameters.phase) *
	   exp(- x / parameters.decay)));
}

double CalculateAverageDeviationFromIdeal(std::vector <CapPoint> data_points, DampedSineParameters fitting_parameters)
{
  double sum = 0.0;
  double calculated_value, difference;
  for (unsigned int i = 0; i < data_points.size(); i++)
    {
      calculated_value = DampedSine(data_points[i].time_delay, fitting_parameters);
      difference = calculated_value - data_points[i].reflectivity;
      sum += (difference * difference);
    }
  return sum / data_points.size();
}

TEST(SimulationResultBehavesAsExpected)
{
  CapSimulation simulation;
  std::vector <CapPoint> simulation_output = simulation.Run(0, 100e-12, 0.5e-12);

  DampedSineParameters fitting_parameters;
  fitting_parameters.amplitude = -0.024000000000006;
  fitting_parameters.period = -4.75015303043906e-12;
  fitting_parameters.phase = 1.5707963267949;
  fitting_parameters.decay = 3.94321605576412e-11;
  fitting_parameters.offset = 0.025643009960261;
  
  double deviation = CalculateAverageDeviationFromIdeal(simulation_output, fitting_parameters);
  double max_deviation = 3e-7;
  CHECK(deviation < max_deviation);
  if (deviation >= max_deviation)
    {
      std::cerr << "Error: deviation " << deviation << " >= maximum allowable value of " << max_deviation << std::endl;
      std::ofstream outfile;
      outfile.open("cap-sim-output-dump.dat");
      for (unsigned int i = 0; i < simulation_output.size(); i++)
	{
	  outfile << simulation_output[i].time_delay << '\t' << simulation_output[i].reflectivity << std::endl;
	}
      outfile.close();
      std::cerr << "Output dumped to cap-sim-output-dump.dat." << std::endl;
    }
}

TEST(ZeroUnstrainedReflectivityThrowsException)
{
  class AirCapMaterial : public CapMaterialInterface
  {
    double smallest_feature() const {return 1e-9;}
    double max_interesting_depth() const {return 1e-9;}
    double n(double z, double lambda) const {return 1.0;}
    double kappa(double z, double lambda) const {return 0.0;}
    double speed_of_sound(double z) const {return 346.13;}
    double dndeta(double z, double lambda) const {return 0.0;}
    double dkappadeta(double z, double lambda) const {return 0.0;}
    TransducingLayer transducing_layer() const {return TransducingLayer();}
    void PrintCustomParameters(std::ostream & out = std::cout, std::string tag = "") const {}
  } air_material;

  CapSimulation mySim(&air_material);
  CHECK_THROW(mySim.Run(100e-12, 1e-12), Exception);
}

