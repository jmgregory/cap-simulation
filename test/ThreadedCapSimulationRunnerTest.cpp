#include <vector>
#include "../ThreadedCapSimulationRunner.h"
#include "../DefaultCapMaterial.h"
#include "../LaserBeam.h"

struct ThreadedCapSimulationTestFixture
{

};

TEST_FIXTURE(ThreadedCapSimulationTestFixture, ThreadedCapSimulationRunnerGivesSameResultAsCapSimulation)
{
  double start_time_delay = 0.0e-12;
  double stop_time_delay = 100e-12;
  double time_delay_step = 0.5e-12;

  DefaultCapMaterial material;
  LaserBeam laser;
  CapSimulation simulation;
  simulation.set_material(&material);
  simulation.set_laser(laser);

  ThreadedCapSimulationRunner runner(&simulation);
  runner.set_number_of_threads(2);
  runner.set_time_delays(start_time_delay, stop_time_delay, time_delay_step);

  std::vector <CapPoint> threaded_points = runner.Run();

  std::vector <CapPoint> unthreaded_points = simulation.Run(start_time_delay, stop_time_delay, time_delay_step);

  CHECK_EQUAL(unthreaded_points.size(), threaded_points.size());
  for(unsigned int i = 0; i < unthreaded_points.size(); i++)
    {
      CHECK_CLOSE(unthreaded_points[i].time_delay, threaded_points[i].time_delay, 1e-25);
      CHECK_CLOSE(unthreaded_points[i].reflectivity, threaded_points[i].reflectivity, 1e-14);
    }
}
