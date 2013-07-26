#include <UnitTest++.h>
#include <vector>
#include <iostream>
#include <fstream>
#include "../CapSimulation.h"
#include "../Exception.h"
#include "../CapMaterialInterface.h"
#include "../DefaultCapMaterial.h"

struct DampedSineParameters
{
  double amplitude;
  double period;
  double phase;
  double decay;
  double offset;
};

double DampedSine(double x, DampedSineParameters parameters);
double CalculateAverageDeviationFromIdeal(std::vector <CapPoint> data_points, DampedSineParameters fitting_parameters);
void DumpSimResultsToFile(const std::vector <CapPoint> & simulation_output, string filename);

TEST(SimulationTimeDelayLimits)
{
  CapSimulation simulation;
  std::vector <CapPoint> simulation_output;

  simulation_output = simulation.Run(2e-12, 0.5e-12);
  CHECK_EQUAL(5, (int)simulation_output.size());
  CHECK_EQUAL(0.0, simulation_output[0].time_delay);
  CHECK_CLOSE(2e-12, simulation_output[simulation_output.size()-1].time_delay, 1e-20);

  simulation_output = simulation.Run(0.0, 2e-12, 0.5e-12);
  CHECK_EQUAL(5, (int)simulation_output.size());
  CHECK_EQUAL(0.0, simulation_output[0].time_delay);
  CHECK_CLOSE(2e-12, simulation_output[simulation_output.size()-1].time_delay, 1e-20);
}

double abs(double x)
{
  return (x > 0.0) ? x : -x;
}

TEST(SimulationResultBehavesAsExpected)
{
  CapSimulation simulation;
  std::vector <CapPoint> simulation_output = simulation.Run(0, 100e-12, 0.5e-12);

  DampedSineParameters fitting_parameters;
  // Values taken from GNUPlot fit
  fitting_parameters.amplitude       =  0.000178706    ;
  fitting_parameters.offset          = -0.000186032    ;
  fitting_parameters.decay           =  3.62021e-11    ;
  fitting_parameters.phase           =  1.63316        ;
  fitting_parameters.period          =  4.75847e-12    ;

  double deviation = CalculateAverageDeviationFromIdeal(simulation_output, fitting_parameters);
  double max_deviation = 1e-12;
  CHECK(deviation < max_deviation);

  if (deviation >= max_deviation)
    {
      std::cerr << "Error: deviation " << deviation << " >= maximum allowable value of " << max_deviation << std::endl;
      DumpSimResultsToFile(simulation_output, "cap-sim-output-dump.dat");
    }
  
  double wavelength = LaserBeam().probe_wavelength();
  double index = DefaultCapMaterial().n(0.0, wavelength);
  double speed_of_sound = DefaultCapMaterial().speed_of_sound(0.0);

  CHECK_CLOSE(wavelength / (2.0 * index * speed_of_sound),
	      abs(fitting_parameters.period), 
	      3e-15);

  double kappa = DefaultCapMaterial().kappa(0.0, wavelength);
  const double pi = 3.1415926535897932384627;
  double alpha = 4.0 * pi * kappa / wavelength; // m-1

  // Note: oscillations should decay as exp(-alpha*z), NOT exp(-2*alpha*z)
  // This is due to the way the electric fields decay and recombine.
  // This may be verified by looking at Thomsen (1986), and dividing the
  // squared magnitude of Eqn. 30 by the squared magnitude of Eqn. 21 to get R.
  // The oscillating component of the resulting reflectivity decays by
  // exp(-4*pi*kappa*z/lambda), which is equal to exp(-alpha*z).
  CHECK_CLOSE(alpha,
	      1.0 / fitting_parameters.decay / speed_of_sound,
	      6e3);
}

void DumpSimResultsToFile(const std::vector <CapPoint> & simulation_output, string filename)
{
  std::ofstream outfile;
  outfile.open(filename.c_str());
  for (unsigned int i = 0; i < simulation_output.size(); i++)
    {
      outfile << simulation_output[i].time_delay << '\t' << simulation_output[i].reflectivity << std::endl;
    }
  outfile.close();
  std::cerr << "Output dumped to cap-sim-output-dump.dat." << std::endl;
}

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


class AirCapMaterial : public CapMaterialInterface
{
 private:
  TransducingLayer _transducing_layer;
  double p12(double z, double lambda) const {return 0.1;}
 public:
  AirCapMaterial() : _transducing_layer(TransducingLayer()) {};
  double smallest_feature() const {return 1e-9;}
  double max_interesting_depth() const {return 1e-9;}
  double n(double z, double lambda) const {return 1.0;}
  double kappa(double z, double lambda) const {return 0.0;}
  double speed_of_sound(double z) const {return 346.13;}
  TransducingLayer transducing_layer() const {return _transducing_layer;}
  void PrintCustomParameters(std::ostream & out = std::cout, std::string tag = "") const {}
  std::string description() const {return "Air";}
  CapMaterialInterface * clone() const {return new AirCapMaterial();}
  void set_transducing_layer(const TransducingLayer & tl) {_transducing_layer = tl;}
};

struct AirTestFixture
{
  AirTestFixture()
    : air_material()
  {
    my_simulation.set_material(&air_material);
  }

  AirCapMaterial air_material;
  CapSimulation my_simulation;
};

TEST_FIXTURE(AirTestFixture, ZeroUnstrainedReflectivityThrowsException)
{
  CHECK_THROW(my_simulation.Run(100e-12,1e-12), Exception);
}

