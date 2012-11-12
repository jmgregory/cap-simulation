#include <iostream>
#include <pthread.h>
#include <cstdlib>
#include "CapSimulation.h"
#include "ThreadedCapSimulationRunner.h"

#define NUM_THREADS 2

void print_data(const std::vector <CapPoint> & data, std::ostream & out = std::cout);

int main()
{
  double start_time = 0.0;
  double stop_time = 100e-12;
  double time_step = 0.25e-12;

  CapSimulation simulation;
  ThreadedCapSimulationRunner runner(&simulation);
  runner.set_time_delays(start_time, stop_time, time_step);
  runner.set_number_of_threads(NUM_THREADS);

  simulation.PrintParameters(std::cout, "# ");
  print_data(runner.Run());

  return 0;
}

void print_data(const std::vector <CapPoint> & data, std::ostream & out)
{
  out << "# Column 1: Time Delay (s)" << std::endl;
  out << "# Column 2: Reflectivity [(R - R0) / R0]" << std::endl;
  for (unsigned int i = 0; i < data.size(); i++)
    {
      out << data[i].time_delay << '\t' << data[i].reflectivity << std::endl;
    }
}
