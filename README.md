CAP Simulation
==============

A tool for simulating coherent acoustic phonon (CAP) interferometry experiments (also known as picosecond ultrasonics) in multilayered specimens.  Originally developed by Justin Gregory at Vanderbilt University.


Usage
-----

To simulate a given specimen, sub-class *CapMaterialInterface* and fill in the appropriate functions.  See **CapMaterialInterface.h** and **DefaultCapMaterial.h** for instructions and an example.

Next, modify **main.cpp** to use your class instead of *DefaultCapMaterial*, and modify the transducing layer parameters as needed.  See **TransducingLayer.h** for a description of each parameter, and the *Thomsen et al.* reference below for further detail.


Command Line Options
--------------------

All have sensible defaults and are therefore optional.

    -s <time_delay>, --start <time_delay>   Start time delay in seconds
    -e <time_delay>, --stop <time_delay>    Stop time delay in seconds
    -i <time_delay>, --step <time_delay>    Time delay step size in seconds
    -R <value>,      --reflectivity <value> Force transducing layer reflectivity (useful for data fitting)
    -q,              --quiet                Do not print parameters to stderr (will still be in stdout)
    -t <number>,     --threads <number>     Number of simulation threads to run


Command Line Example
--------------------

     $ ./cap-sim --start 0 --stop 150e-12 --step 0.25e-12 -t 8 > output.dat


Output
------

Simulation parameters are printed to stdout, with a '#' character at the beginning of each line.  After that, there are two columns:

1. Time Delay (s)
2. Differential Reflectivity [(R - R0) / R0]

Where R0 is the base reflectivity of the specimen in the absence of the strain pulse.


Scientific References
---------------------

- **Picosecond ultrasonics**

    http://en.wikipedia.org/wiki/Picosecond_ultrasonics

- **Strain wave model**

    C. Thomsen, H. Grahn, H. J. Maris and J. Tauc, "Surface Generation and Detection of Phonons by Picosecond Light Pulses", Phys. Rev. B. 34, 4129 (1986)).

- **Modeling approach (characteristic matrices)**

    O. Matsuda and O. B. Wright, "Reflection and transmission of light in multilayers perturbed by picosecond strain pulse propagation", J. Opt. Soc. Am. B 19, 3028-3041 (2002).


Potential Enhancements
----------------------

- Calculation of reflectivity using Green's functions instead of characteristic matrices (see the *Matsuda and Wright* reference, above).
- Use the speed of sound information from the *CapMaterialInterface* subclass to calculate the acoustic impedence of the interfaces in the specimen, and model the resulting reflection and transmission of the acoustic pulse.
