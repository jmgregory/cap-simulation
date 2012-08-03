cap-sim: main.cpp cap-simulation.o e-field-propagator.o complex.o
	g++ -O3 -Wall main.cpp cap-simulation.o e-field-propagator.o complex.o -o cap-sim

cap-simulation.o: cap-simulation.cpp cap-simulation.h e-field-propagator.h cap-material.h complex.h
	g++ -O3 -Wall -c cap-simulation.cpp -o cap-simulation.o

e-field-propagator.o: e-field-propagator.cpp e-field-propagator.h cap-material.h complex.h
	g++ -O3 -Wall -c e-field-propagator.cpp -o e-field-propagator.o

complex.o: complex.cpp complex.h
	g++ -O3 -Wall -c complex.cpp -o complex.o

.PHONY: clean
clean:
	-rm *~
	-rm *.o
