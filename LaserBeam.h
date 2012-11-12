#ifndef JMG_LASER_BEAM_H
#define JMG_LASER_BEAM_H

static const double pi = 3.1415926535897932384626;

class LaserBeam
{
 public:

  LaserBeam() :
    _probe_wavelength(400e-9),
    _pump_diameter(100e-6),
    _pump_power(300e-3),
    _rep_rate(76e6),
    _time_width(150e-15) {}

  LaserBeam(double probe_wavelength,
	    double pump_diameter,
	    double pump_power,
	    double rep_rate,
	    double time_width) :
    _probe_wavelength(probe_wavelength),
    _pump_diameter(pump_diameter),
    _pump_power(pump_power),
    _rep_rate(rep_rate),
    _time_width(time_width) {}

  double probe_wavelength() const { return _probe_wavelength; }
  double pump_diameter()    const { return _pump_diameter; }
  double pump_power()       const { return _pump_power; }
  double rep_rate()         const { return _rep_rate; }
  double time_width()       const { return _time_width; }

  void set_probe_wavelength(double wavelength) { _probe_wavelength = wavelength; }

  double EnergyPerPulse() const { return _pump_power / _rep_rate; }
  double PumpSpotArea() const { return pi * 0.25 * _pump_diameter * _pump_diameter; }

  void PrintParameters(std::ostream & out = std::cout, std::string tag = "") const;

 private:
  double _probe_wavelength; // m
  double _pump_diameter;   // m
  double _pump_power;      // W
  double _rep_rate;   // Hz
  double _time_width; // s
};

#endif
