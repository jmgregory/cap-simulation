#ifndef JMG_THREADED_CAP_SIMULATION_RUNNER_H
#define JMG_THREADED_CAP_SIMULATION_RUNNER_H

#include <vector>
#include "CapSimulation.h"
#include "LaserBeam.h"
#include "CapMaterialInterface.h"

class ThreadedCapSimulationRunner
{
 public:
  ThreadedCapSimulationRunner(CapSimulation * seed);
  
  void set_number_of_threads(unsigned int thread_count);
  void set_time_delays(double start_time_delay, double stop_time_delay, double time_delay_step);

  std::vector <CapPoint> Run();

 private:
  static void * proxy_function(void * t_params);

  CapSimulation * _simulation_seed;
  unsigned int _thread_count;
  double _start_time_delay;
  double _stop_time_delay;
  double _time_delay_step;
};

struct sim_data
{
  std::vector <CapPoint> * output;
  int id;
  double start_time_delay;
  double stop_time_delay;
  double time_delay_step;
  CapSimulation * simulation;
};


#endif
