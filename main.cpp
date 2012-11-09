#include <iostream>
#include <pthread.h>
#include <cstdlib>
#include "CapSimulation.h"

#define NUM_THREADS 2

using std::vector;

void print_data(const vector <CapPoint> & data, std::ostream & out = std::cout);
void *proxy_function(void *);

struct sim_data
{
  vector <CapPoint> * output;
  int id;
  double start_time;
  double stop_time;
  double time_step;
};

int main()
{
  double start_time = 0.0;
  double stop_time = 100e-12;
  double time_step = 0.25e-12;

  pthread_t thread[NUM_THREADS];
  pthread_attr_t attr;
  pthread_attr_init(&attr);
  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
  void *status;
  int rc;
  sim_data t_params[NUM_THREADS];
  int time_step_count = (stop_time - start_time) / time_step;
  for (int th = 0; th < NUM_THREADS; th++)
    {
      t_params[th].start_time = start_time + (th * (time_step_count / NUM_THREADS)) * time_step;
      t_params[th].stop_time = start_time + (((th + 1) * (time_step_count / NUM_THREADS)) - 1) * time_step;
      if (th == NUM_THREADS - 1) t_params[th].stop_time += time_step;
      t_params[th].time_step = time_step;
      t_params[th].id = th;
      t_params[th].output = NULL;
      rc = pthread_create(&thread[th], &attr, proxy_function, &t_params[th]);
      if (rc)
	{
	  std::cerr << "ERROR; return code from pthread_create() is " << rc << std::endl;
	  exit(-1);
	}
    }
  pthread_attr_destroy(&attr);
  for (int th = 0; th < NUM_THREADS; th++)
    {
      rc = pthread_join(thread[th], &status);
      if (rc)
	{
	  std::cerr << "ERROR; return code from pthread_join() is " << rc << std::endl;
	  exit(-1);
	}
    }


  for (int th = 0; th < NUM_THREADS; th++)
    {
      print_data(*(t_params[th].output));
    }

  return 0;
}

void * proxy_function(void * t_params)
{
  sim_data * params = static_cast<sim_data *>(t_params);
  CapSimulation mySim;
  if (params->id == 0)
    {
      mySim.PrintParameters(std::cerr);
      mySim.PrintParameters(std::cout, "# ");
    }
  vector <CapPoint> data = mySim.Run(params->start_time, params->stop_time, params->time_step);
  params->output = new vector <CapPoint> (data);
  return NULL;
}

void print_data(const vector <CapPoint> & data, std::ostream & out)
{
  out << "# Column 1: Time Delay (s)" << std::endl;
  out << "# Column 2: Reflectivity [(R - R0) / R0]" << std::endl;
  for (unsigned int i = 0; i < data.size(); i++)
    {
      out << data[i].time_delay << '\t' << data[i].reflectivity << std::endl;
    }
}
