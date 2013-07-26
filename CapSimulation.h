#ifndef JMG_CAP_SIMULATION_H
#define JMG_CAP_SIMULATION_H

#include <iostream>
#include <vector>
#include <string>
#include <complex>
#include "CapMaterialInterface.h"
#include "LaserBeam.h"
#include "TransducingLayer.h"
#include "HomogeneousCharacteristicMatrix.h"

using std::complex;

struct CapPoint
{
  double time_delay; // s

  // (R - R0) / R0
  double reflectivity;

  CapPoint()
    : time_delay(0.0), reflectivity(0.0) {}

  CapPoint(double Time_Delay, double Reflectivity)
    : time_delay(Time_Delay), reflectivity(Reflectivity) {}
};

class CapSimulation
{
public:
  CapSimulation();
  ~CapSimulation();

  CapSimulation * clone() const;
  
  std::vector <CapPoint> Run(double stop_time_delay, double time_delay_step);
  std::vector <CapPoint> Run(double start_time_delay, double stop_time_delay, double time_delay_step);

  void PrintParameters(std::ostream & out = std::cout, std::string tag = "") const;

  void set_material(const CapMaterialInterface * material);
  void set_laser(const LaserBeam & laser);

private:
  double _depth_sampling_resolution;
  CapMaterialInterface * _material;
  LaserBeam _laser;
  bool _material_needs_destroyed;

  void DestroyMaterialIfNecessary();
  void CheckDepthSamplingResolutionAgainstMaterial();
  double CalculateReflectivityForTimeDelay(double time_delay) const;
  double CalculateDifferentialReflectivity(double modulated_reflectivity, double baseline_reflectivity) const;
  double CalculateStrain(double time_delay, double depth) const;
  double CalculateUnstrainedReflectivity() const;
  std::vector <HomogeneousCharacteristicMatrix> BuildLayerMatricesList(double time_delay) const;
  CharacteristicMatrix MultiplyMatrices(const std::vector <HomogeneousCharacteristicMatrix> & matrices) const;
  complex <double> UnstrainedIndex(double depth) const;
  complex <double> IndexBeforeSpecimen() const;
  complex <double> IndexAfterSpecimen() const;
  complex <double> CalculateIndexWithStrain(double time_delay, double depth) const;
  double StrainPulseDepth(double time_delay) const;
};

#endif
