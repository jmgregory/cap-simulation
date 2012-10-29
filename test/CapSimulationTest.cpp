#include <UnitTest++.h>
#include <vector>
#include <iostream>
#include "../CapSimulation.h"

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

TEST(TestOutput)
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
  CHECK(deviation < 3e-7);
}
