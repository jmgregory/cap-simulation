#ifndef JMG_THREADED_CAP_SIMULATION_RUNNER_H
#define JMG_THREADED_CAP_SIMULATION_RUNNER_H

#include <vector>
#include "CapSimulation.h"
#include "LaserBeam.h"
#include "CapMaterialInterface.h"

struct sim_data;

class ThreadedCapSimulationRunner
{
 public:
  ThreadedCapSimulationRunner(const CapSimulation * seed);
  ~ThreadedCapSimulationRunner();
  
  void set_number_of_threads(unsigned int thread_count);
  void set_time_delays(double start_time_delay, double stop_time_delay, double time_delay_step);

  std::vector <CapPoint> Run();

  void PrintParameters(std::ostream & out = std::cout, std::string tag = "") const;

 private:
  static void * RunSimulationThread(void *t_params);
  void StartThreads();
  void WaitForThreadsToFinish();
  std::vector <CapPoint> ConcatenateThreadOutputs();

  CapSimulation * _simulation_seed;
  unsigned int _thread_count;
  double _start_time_delay;
  double _stop_time_delay;
  double _time_delay_step;

  pthread_t * _threads;
  sim_data * _t_params;
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
