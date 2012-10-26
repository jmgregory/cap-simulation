#include <string>
#include <iostream>
#include "LaserBeam.h"

void LaserBeam::PrintParameters(std::ostream & out, std::string tag) const
{
  using std::endl;
  out << tag << "  Laser Beam" << endl;
  out << tag << "  ---------------------" << endl;
  out << tag << "       Repetition rate: " << _rep_rate                 << " Hz" << endl;
  out << tag << "            Pump power: " << _pump_power               << " W" << endl;
  out << tag << "    Pump spot diameter: " << _pump_diameter     * 1e6  << " um" << endl;
  out << tag << "      Probe wavelength: " << _probe_wavelength  * 1e9  << " nm" << endl;
  out << tag << "      Probe time width: " << _time_width        * 1e15 << " fs (FWHM)" << endl;
}
