#include <vector>
#include <assert.h>
#include <pthread.h>
#include "ThreadedCapSimulationRunner.h"
#include "CapSimulation.h"

ThreadedCapSimulationRunner::ThreadedCapSimulationRunner(const CapSimulation *seed)
  :_simulation_seed(seed->clone()), _thread_count(1), _start_time_delay(0.0), _stop_time_delay(100e-12), _time_delay_step(1e-12), _threads(NULL), _t_params(NULL)
{ }

ThreadedCapSimulationRunner::~ThreadedCapSimulationRunner()
{
  delete _simulation_seed;
}

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

void ThreadedCapSimulationRunner::StartThreads()
{
  pthread_attr_t attr;
  pthread_attr_init(&attr);
  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
  int rc;
  int time_step_count = ((_stop_time_delay - _start_time_delay) / _time_delay_step) + 1;
  for (unsigned int th = 0; th < _thread_count; th++)
    {
      _t_params[th].start_time_delay = _start_time_delay + (th * (time_step_count / _thread_count)) * _time_delay_step;
      _t_params[th].stop_time_delay = _start_time_delay + (((th + 1) * (time_step_count / _thread_count)) - 1) * _time_delay_step;
      if (th == _thread_count - 1) _t_params[th].stop_time_delay = _stop_time_delay;
      _t_params[th].stop_time_delay += _time_delay_step * 0.1; // Fix for floating point rounding errors, which can drop simulation points
      _t_params[th].time_delay_step = _time_delay_step;
      _t_params[th].id = th;
      _t_params[th].output = NULL;
      _t_params[th].simulation = _simulation_seed->clone();

      rc = pthread_create(&_threads[th], &attr, RunSimulationThread, &_t_params[th]);
      if (rc)
	{
	  std::cerr << "ERROR; return code from pthread_create() is " << rc << std::endl;
	  exit(-1);
	}
    }
  pthread_attr_destroy(&attr);
}

void ThreadedCapSimulationRunner::WaitForThreadsToFinish()
{
  int rc;
  void *status;
  for (unsigned int th = 0; th < _thread_count; th++)
    {
      rc = pthread_join(_threads[th], &status);
      if (rc)
	{
	  std::cerr << "ERROR; return code from pthread_join() is " << rc << std::endl;
	  exit(-1);
	}
    }
}

std::vector <CapPoint> ThreadedCapSimulationRunner::ConcatenateThreadOutputs()
{
  std::vector <CapPoint> result;
  for (unsigned int th = 0; th < _thread_count; th++)
    {
      for (unsigned int i = 0; i < _t_params[th].output->size(); i++)
	{
	  result.push_back(_t_params[th].output->at(i));
	}
    }
  return result;
}

std::vector <CapPoint> ThreadedCapSimulationRunner::Run()
{
  _threads = static_cast<pthread_t *>(malloc(sizeof(pthread_t) * _thread_count));
  _t_params = static_cast<sim_data *>(malloc(sizeof(sim_data) * _thread_count));
  StartThreads();
  WaitForThreadsToFinish();
  std::vector <CapPoint> result = ConcatenateThreadOutputs();
  for (unsigned int i = 0; i < _thread_count; i++)
    {
      delete _t_params[i].simulation;
    }
  free(_threads);
  free(_t_params);
  return result;
}

void * ThreadedCapSimulationRunner::RunSimulationThread(void * t_params)
{
  sim_data * params = static_cast<sim_data *>(t_params);
  std::vector <CapPoint> data = params->simulation->Run(params->start_time_delay, params->stop_time_delay, params->time_delay_step);
  params->output = new std::vector <CapPoint> (data);
  return NULL;
}

void ThreadedCapSimulationRunner::PrintParameters(std::ostream & out, std::string tag) const
{
  out << tag << "Threaded CAP Simulation Runner" << std::endl;
  out << tag << "------------------------------" << std::endl;
  out << tag << "  Number of threads: " << _thread_count << std::endl;
  out << tag << "   Start time delay: " << _start_time_delay * 1e12 << " ps" << std::endl;
  out << tag << "    Stop time delay: " << _stop_time_delay * 1e12 << " ps" << std::endl;
  out << tag << "    Time delay step: " << _time_delay_step * 1e12 << " ps" << std::endl;
  out << tag << std::endl;
  _simulation_seed->PrintParameters(out, tag);
}
