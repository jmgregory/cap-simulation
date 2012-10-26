#include <iostream>
#include <pthread.h>
#include <cstdlib>
#include "CapSimulation.h"

using namespace std;

#define NUM_THREADS 2

void print_data(const vector <CapPoint> & data, ostream & out = cout);
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
	  cerr << "ERROR; return code from pthread_create() is " << rc << endl;
	  exit(-1);
	}
    }
  pthread_attr_destroy(&attr);
  for (int th = 0; th < NUM_THREADS; th++)
    {
      rc = pthread_join(thread[th], &status);
      if (rc)
	{
	  cerr << "ERROR; return code from pthread_join() is " << rc << endl;
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
  sim_data * params = (sim_data *)t_params;
  CapSimulation mySim;
  if (params->id == 0)
    {
      mySim.PrintParameters(cerr);
      mySim.PrintParameters(cout, "# ");
    }
  vector <CapPoint> data = mySim.Run(params->start_time, params->stop_time, params->time_step);
  params->output = new vector <CapPoint> (data);
  return NULL;
}

void print_data(const vector <CapPoint> & data, ostream & out)
{
  for (unsigned int i = 0; i < data.size(); i++)
    {
      out << data[i].time_delay << '\t' << data[i].reflectivity << endl;
    }
}
