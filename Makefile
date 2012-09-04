cap-sim: main.cpp cap-simulation.o e-field-propagator.o
	g++ -O3 -Wall main.cpp cap-simulation.o e-field-propagator.o -o cap-sim

cap-simulation.o: cap-simulation.cpp cap-simulation.h e-field-propagator.h cap-material.h
	g++ -O3 -Wall -c cap-simulation.cpp -o cap-simulation.o

e-field-propagator.o: e-field-propagator.cpp e-field-propagator.h cap-material.h
	g++ -O3 -Wall -c e-field-propagator.cpp -o e-field-propagator.o

efp-test: e-field-propagator-test.cpp e-field-propagator.o
	g++ -O3 -Wall e-field-propagator-test.cpp e-field-propagator.o -o efp-test

out-efp.dat: efp-test
	./efp-test > out-efp.dat

efp-test-plot: out-efp.dat
	gnuplot -e 'set xlabel "Time (fs)"; set ylabel "Depth (nm)"; set pm3d map; splot [50:60] "out-efp.dat" using ($$1*1e15):($$3*1e9):($$4+$$5) with pm3d'

.PHONY: clean
clean:
	-rm *~
	-rm *.o
	-rm cap-sim
	-rm efp-test
	-rm out-efp.dat
