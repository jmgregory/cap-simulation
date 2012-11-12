#include <vector>
#include <assert.h>
#include "ThreadedCapSimulationRunner.h"
#include "CapSimulation.h"

ThreadedCapSimulationRunner::ThreadedCapSimulationRunner(CapSimulation *seed)
  :_simulation_seed(seed), _thread_count(1), _start_time_delay(0.0), _stop_time_delay(100e-12), _time_delay_step(1e-12)
{ }

void ThreadedCapSimulationRunner::set_number_of_threads(unsigned int thread_count)
{
  assert(thread_count > 0);
  _thread_count = thread_count;
}

void ThreadedCapSimulationRunner::set_time_delays(double start_time_delay, double stop_time_delay, double time_delay_step)
{
  _start_time_delay = start_time_delay;
  _stop_time_delay = stop_time_delay;
  _time_delay_step = time_delay_step;
}

std::vector <CapPoint> ThreadedCapSimulationRunner::Run()
{
  std::vector <CapPoint> result;

  pthread_t thread[_thread_count];
  pthread_attr_t attr;
  pthread_attr_init(&attr);
  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
  void *status;
  int rc;
  sim_data t_params[_thread_count];
  int time_step_count = (_stop_time_delay - _start_time_delay) / _time_delay_step;
  for (unsigned int th = 0; th < _thread_count; th++)
    {
      t_params[th].start_time_delay = _start_time_delay + (th * (time_step_count / _thread_count)) * _time_delay_step;
      t_params[th].stop_time_delay = _start_time_delay + (((th + 1) * (time_step_count / _thread_count)) - 1) * _time_delay_step;
      if (th == _thread_count - 1) t_params[th].stop_time_delay += _time_delay_step;
      t_params[th].time_delay_step = _time_delay_step;
      t_params[th].id = th;
      t_params[th].output = NULL;
      t_params[th].simulation = _simulation_seed;
      rc = pthread_create(&thread[th], &attr, proxy_function, &t_params[th]);
      if (rc)
	{
	  std::cerr << "ERROR; return code from pthread_create() is " << rc << std::endl;
	  exit(-1);
	}
    }
  pthread_attr_destroy(&attr);
  for (unsigned int th = 0; th < _thread_count; th++)
    {
      rc = pthread_join(thread[th], &status);
      if (rc)
	{
	  std::cerr << "ERROR; return code from pthread_join() is " << rc << std::endl;
	  exit(-1);
	}
      for (unsigned int i = 0; i < t_params[th].output->size(); i++)
	{
	  result.push_back(t_params[th].output->at(i));
	}
    }
  
  return result;
}

void * ThreadedCapSimulationRunner::proxy_function(void * t_params)
{
  sim_data * params = static_cast<sim_data *>(t_params);
  std::vector <CapPoint> data = params->simulation->Run(params->start_time_delay, params->stop_time_delay, params->time_delay_step);
  params->output = new std::vector <CapPoint> (data);
  return NULL;
}
